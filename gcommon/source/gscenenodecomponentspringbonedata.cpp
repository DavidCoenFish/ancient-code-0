//
//  GSceneNodeComponentSpringBoneData.cpp
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentSpringBoneData.h"

#include "GMath.h"
#include "GMatrix16Float.h"
#include "GMatrix9Float.h"
#include "GPhysicsComponentPosition.h"
#include "GSkeletonInstance.h"
#include "GSceneNode.h"
#include "GSceneNodeComponentSpringBoneDataLoad.h"
#include "GAssert.h"

#ifdef DSC_DEBUG
#include "GScene.h"
typedef std::vector<GMatrix16Float> TArrayMatrix;
#endif

static GVector3Float sWorldJiggle;

//static public methods
/*static*/ GSceneNodeComponentSpringBoneData::TPointerSceneNodeComponentSpringBoneData GSceneNodeComponentSpringBoneData::Factory(
	const GSceneNodeComponentSpringBoneDataLoad& in_data
#ifdef DSC_DEBUG
	, GScene* const in_scene
#endif
	)
{
	TPointerSceneNodeComponentSpringBoneData pointer;

	pointer.reset(new GSceneNodeComponentSpringBoneData(
		in_data.GetBoneName(),
		in_data.GetCenterMass(),
		in_data.GetSpringConstant(),
		in_data.GetSpringDampen(),
		in_data.GetSpringMax(),
		in_data.GetTargetDown()
#ifdef DSC_DEBUG
		, in_data.GetDebug()
		, in_scene
#endif
		));
	return pointer;
}

/*
argh crap. there is some numerical instablity when mLength != 1.0 (like, 0.5)
have spent too long on this, just make input bones length 1
*/
/*static*/ void GSceneNodeComponentSpringBoneData::BoneUpdateCallback(
	GVector3Float& inout_at,
	GVector3Float& inout_up,
	GVector3Float& inout_pos,
	float& inout_scale,
	GSceneNode& in_parentSceneNode,
	const float in_timeDelta,
	void* in_callbackData,
	GSkeletonInstance& inout_skeletonInstance
	)
{
	GSceneNodeComponentSpringBoneData* const pThis = (GSceneNodeComponentSpringBoneData*)in_callbackData;
	if (!pThis)
	{
		return;
	}

	const int parentIndex = inout_skeletonInstance.GetBoneParentIndex(pThis->mBoneIndex);
	const GMatrix16Float parentBoneWorldMatrix = inout_skeletonInstance.GetBoneObjectMatrix(parentIndex) * in_parentSceneNode.GetWorldTransform();

	const GMatrix16Float initialBoneMatrix = GMatrix16FloatConstructAtUp(
		inout_at * inout_scale,
		inout_up * inout_scale,
		GVector3Float::sUnitZ * inout_scale,
		GVector3Float::sUnitY * inout_scale,
		inout_pos
		);

	const GMatrix16Float initialBoneWorldMatrix = initialBoneMatrix * parentBoneWorldMatrix;


	const GVector3Float limitTargetWorld = pThis->mCenterMass * initialBoneWorldMatrix;
	GVector3Float targetWorld(limitTargetWorld);
	if (pThis->mTargetDown)
	{
		targetWorld = GVector3Float(0.0F, -(pThis->mLength), 0.0F) + initialBoneWorldMatrix.GetPosition();
	}

	//deal teleport case~ though this doesn't deal with target being outside limits...
	if (in_parentSceneNode.GetTeleportFlag())
	{
		GVector3Float earlylimitedPos(targetWorld);
		pThis->mComponentLimit.CalculateLimit(earlylimitedPos, limitTargetWorld); 

		pThis->mComponentPosition.ManualSetPosition(earlylimitedPos, GVector3Float::sZero);
		pThis->mOldTargetWorld = targetWorld;
		return;
	}


	const GVector3Float oldComponentPosition = pThis->mComponentPosition.GetPosition();
	pThis->mComponentPosition.ClearForceSum();

	//add world jiggle
	if (in_timeDelta)
	{
		pThis->mComponentPosition.AddForce(sWorldJiggle / in_timeDelta);
	}

	//add spring force
	pThis->mComponentPosition.AddForce(pThis->mComponentSpring.CalculateSpringForce(
		targetWorld,
		0.0F,
		oldComponentPosition
		));

	//add spring friction
	pThis->mComponentPosition.AddForce(pThis->mComponentSpring.CalculateSpringFriction(
		pThis->mComponentPosition.GetVelocity(),
		GPhysicsComponentPosition::CalculateGetVelocity(pThis->mOldTargetWorld, targetWorld, in_timeDelta)
		));

	pThis->mComponentPosition.Intergrate(in_timeDelta);

	//apply limit, and move limited pos to pThis->mLength of pivot
	{
		GVector3Float limitedPos = pThis->mComponentPosition.GetPosition();

		//apply limit if needed
		pThis->mComponentLimit.CalculateLimit(limitedPos, limitTargetWorld); 

		//project onto sphere
		const GVector3Float offset = NormaliseAprox(limitedPos - initialBoneWorldMatrix.GetPosition());
		limitedPos = initialBoneWorldMatrix.GetPosition() + (offset * pThis->mLength);

		pThis->mComponentPosition.ManualSetPosition(
			limitedPos,
			GPhysicsComponentPosition::CalculateGetVelocity(oldComponentPosition, limitedPos, in_timeDelta)
			);
	}

	pThis->mOldTargetWorld = targetWorld;

	//update bone direction
	{
#if 0
		const GMatrix16Float parentBoneMatrixInvert = parentBoneWorldMatrix.ReturnInverse();

		const GVector3Float toTarget = (limitTargetWorld * parentBoneMatrixInvert) - inout_pos;
		const GVector3Float toTargetNormal = Normalise(toTarget);
		const GVector3Float toComponentPosition = (pThis->mComponentPosition.GetPosition() * parentBoneMatrixInvert) - inout_pos;
		const GVector3Float toComponentPositionNormal = Normalise(toComponentPosition);
#else	
		//use Transpose, not invers, doesn't support scale

		const GVector3Float pivot = parentBoneWorldMatrix.GetPosition();
		GMatrix9Float rotMatrix = parentBoneWorldMatrix.GetRotation();
		rotMatrix.TransposeSelf();
		const GVector3Float toTarget = ((limitTargetWorld - pivot) * rotMatrix) - inout_pos;
		const GVector3Float toTargetNormal = Normalise(toTarget);
		const GVector3Float toComponentPosition = ((pThis->mComponentPosition.GetPosition() - pivot) * rotMatrix) - inout_pos;
		const GVector3Float toComponentPositionNormal = Normalise(toComponentPosition);

#endif
		float length = 0.0F;
		const GVector3Float cross = Normalise(CrossProduct(toComponentPositionNormal, toTargetNormal), length);
		//if (GMath::NotAlmostZero(length))
		if (!GMath::AlmostZero(length, 0.005F))
		{
			const GMatrix9Float deltaMatrix = GMatrix9FloatConstructAtUp(
				toComponentPosition,
				cross,
				toTarget,
				cross
				);

			inout_at = inout_at * deltaMatrix;
			inout_up = inout_up * deltaMatrix;
		}
	}

#ifdef DSC_DEBUG
	if (pThis->mScene && pThis->mDebug)
	{
		GMatrix16Float debugMatrix(initialBoneWorldMatrix);
		//pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);

		debugMatrix.SetPosition(pThis->mComponentPosition.GetPosition());
		pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);

		debugMatrix.SetPosition(targetWorld);
		//pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);

		const GMatrix16Float finalBoneMatrix = GMatrix16FloatConstructAtUp(
			inout_at * inout_scale,
			inout_up * inout_scale,
			GVector3Float::sUnitZ * inout_scale,
			GVector3Float::sUnitY * inout_scale,
			inout_pos
			);
		const GMatrix16Float finalBoneWorldMatrix = finalBoneMatrix * parentBoneWorldMatrix;
		debugMatrix = finalBoneWorldMatrix;

		debugMatrix.SetPosition(pThis->mCenterMass * finalBoneWorldMatrix);
		//pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);
	}
#endif

	return;
}

/*static*/ void GSceneNodeComponentSpringBoneData::SetWorldJiggle(
	const GVector3Float& in_worldJiggle
	)
{
	sWorldJiggle = in_worldJiggle;
	return;
}

//constructor
GSceneNodeComponentSpringBoneData::GSceneNodeComponentSpringBoneData(
	const std::string& in_boneName,
	const GVector3Float& in_centerMass,
	const float in_constant,
	const float in_dampen,
	const float in_max,
	const bool in_targetDown
#ifdef DSC_DEBUG
	, const bool in_debug
	, GScene* const in_scene
#endif
	)
: mBoneName(in_boneName)
, mCenterMass(in_centerMass)
, mOldTargetWorld(GVector3Float::sZero)
, mComponentPosition(1.0F)
, mComponentSpring(in_constant, in_dampen)
, mComponentLimit(in_max)
, mLength(Length(in_centerMass))
, mTargetDown(in_targetDown)
#ifdef DSC_DEBUG
, mDebug(in_debug)
, mScene(in_scene)
#endif
{
	return;
}

GSceneNodeComponentSpringBoneData::~GSceneNodeComponentSpringBoneData()
{
	return;
}
