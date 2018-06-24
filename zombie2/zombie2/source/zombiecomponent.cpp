//
//  ZombieComponent.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieComponent.h"

#include "ZombieComponentLoad.h"
#include "ZombiePart.h"
#include "ZombiePartLoad.h"
#include "ZombiePartDataLoad.h"
#include "ZombieParticleLoad.h"
#include "ZombiePartDetach.h"

#include <GCommon.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentCollision.h>
#include <GSceneNodeComponentParticle.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentFade.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentPhysics.h>
#include <GSceneNodeComponentPhysicsLoad.h>

typedef boost::shared_ptr<ZombiePart> TPointerZombiePart;
typedef std::vector<TPointerZombiePart> TArrayZombiePart;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;
typedef std::vector<TWeakSceneNodeComponentParticle> TArrayWeakSceneNodeComponentParticle;
typedef std::map<std::string, TWeakSceneNodeComponentParticle> TMapStringWeakSceneNodeComponentParticle;

typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;

typedef boost::shared_ptr<ZombiePartDetach> TPointerZombiePartDetach;
typedef std::vector<TPointerZombiePartDetach> TArrayPointerZombiePartDetach;

typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

typedef std::pair<TWeakSceneNodeComponentVisual, TWeakSceneNode> TPairWeakSceneNodeComponentVisualWeakSceneNode;
typedef std::vector<TPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayPairWeakSceneNodeComponentVisualWeakSceneNode;

//////////////////////////////////////////////////////////////
// static local methods
//////////////////////////////////////////////////////////////
static const bool LocalPartOk(const TArrayZombiePart& in_arrayZombiePart, const int in_index)
{
	const bool result = (0.0F < in_arrayZombiePart[in_index]->GetHealth());
	return result;
}

static const bool LocalCheckParentsOk(const ZombieComponentLoad& in_zombieComponentLoad, const int in_index, const TArrayZombiePart& in_arrayZombiePart)
{
	if (GCOMMON_INVALID_INDEX == in_index)
	{
		return false;
	}

	if (!LocalPartOk(in_arrayZombiePart, in_index))
	{
		return false;
	}

	const ZombiePartLoad& zombiePartLoad = in_zombieComponentLoad.GetArrayPart()[in_index];
	if (zombiePartLoad.GetArrayParentCount())
	{
		for (int index = 0; index < zombiePartLoad.GetArrayParentCount(); ++index)
		{
			if (LocalCheckParentsOk(in_zombieComponentLoad, zombiePartLoad.GetArrayParent()[index], in_arrayZombiePart))
			{
				return true;
			}
		}
		return false;
	}

	return true;
}

static void LocalUpdateParticleSystem(
	const ZombieComponentLoad& in_zombieComponentLoad,
	TMapStringWeakSceneNodeComponentParticle& inout_mapParticles, 
	const TArrayZombiePart& in_arrayZombiePart
	)
{
	for (int index = 0; index < in_zombieComponentLoad.GetArrayParticleCount(); ++index)
	{
		const ZombieParticleLoad& particleData = in_zombieComponentLoad.GetArrayParticle()[index];
		TMapStringWeakSceneNodeComponentParticle::iterator iterator = inout_mapParticles.find(particleData.GetParticleData());
		if (iterator == inout_mapParticles.end())
		{
			continue;
		}
		TPointerSceneNodeComponentParticle pointerComponentParticle = (*iterator).second.lock();
		if (!pointerComponentParticle)
		{
			continue;
		}

		GSceneNodeComponentParticle& componentParticle = *pointerComponentParticle;

		bool atLeastOnePresent = false;
		bool atLeastOneDead = false;

		for (int subIndex = 0; subIndex < particleData.GetArrayPartAliveCount(); ++subIndex)
		{
			const int partIndex = particleData.GetArrayPartAlive()[subIndex];
			if (0.0F < in_arrayZombiePart[partIndex]->GetHealth())
			{
				atLeastOnePresent = true;
				break;
			}
		}
		for (int subIndex = 0; subIndex < particleData.GetArrayPartDeadCount(); ++subIndex)
		{
			const int partIndex = particleData.GetArrayPartDead()[subIndex];
			if (in_arrayZombiePart[partIndex]->GetHealth() <= 0.0F)
			{
				atLeastOneDead = true;
				break;
			}
		}
//#define DEBUG_FORCE_ENABLE_PARTICLES
#ifdef DEBUG_FORCE_ENABLE_PARTICLES
		bool shouldEmit = true;
#else
		bool shouldEmit = atLeastOnePresent && atLeastOneDead;
#endif
		if (shouldEmit)
		{
			componentParticle.SetVisible(true);
			if (0 == componentParticle.GetActiveEmittorCount())
			{
				componentParticle.RequestEmittor(particleData.mParticleEmittor, 0.0F);
			}
		}
		else
		{
			componentParticle.ClearAllEmittor();
		}
	}

	return;
}
/**/

static void LocalCollectZombiePart(
	const ZombieComponentLoad& in_zombieComponentLoad,
	TArrayPointerZombiePartDetach& inout_arrayZombiePartDetach,
	const int in_index, 
	TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& in_arrayDetachedVisual,
	GScene& inout_scene,
	TPointerSceneNode& inout_sceneNodeRoot,
	TPointerSceneNode& inout_zombieParentSceneNode
	)
{
	//is there a detached zombie part already for us
	const ZombiePartLoad& partData = in_zombieComponentLoad.GetArrayPart()[in_index];
	for (TArrayPointerZombiePartDetach::iterator iterator = inout_arrayZombiePartDetach.begin(); iterator != inout_arrayZombiePartDetach.end(); ++iterator)
	{
		bool found = false;
		ZombiePartDetach& partDetach = *(*iterator);
		for (int subIndex = 0; subIndex < partData.GetArrayParentCount(); ++subIndex)
		{
			if (partDetach.SearchPartIndex(partData.GetArrayParent()[subIndex]))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			break;
		}

		partDetach.AddParts(
			in_index,
			in_arrayDetachedVisual
			);
		return;
	}

	TPointerZombiePartDetach pointerPartDetach;
	pointerPartDetach.reset(new ZombiePartDetach(
		inout_scene,
		inout_sceneNodeRoot,
		inout_zombieParentSceneNode
		));

	pointerPartDetach->AddParts(
		in_index,
		in_arrayDetachedVisual
		);

	//is there any exsisting zombie parts that should bellong to us
	//TODO: No? presume heirarchical order of parts?
	
	inout_arrayZombiePartDetach.push_back(pointerPartDetach);

	return;
}

static void LocalDetachPart(
	const ZombieComponentLoad& in_zombieComponentLoad,
	TArrayPointerZombiePartDetach& inout_arrayZombiePartDetach,
	TArrayZombiePart& inout_arrayZombiePart,
	const int in_index,
	GScene& inout_scene,
	TPointerSceneNode& inout_sceneNodeRoot,
	TPointerSceneNode& inout_zombieParentSceneNode
	)
{
	ZombiePart& zombiePart = *(inout_arrayZombiePart[in_index]);
	TArrayPairWeakSceneNodeComponentVisualWeakSceneNode arrayDetachedVisual;
	zombiePart.StealCurrentVisual(arrayDetachedVisual);

	LocalCollectZombiePart(
		in_zombieComponentLoad,
		inout_arrayZombiePartDetach,
		in_index, 
		arrayDetachedVisual,
		inout_scene,
		inout_sceneNodeRoot,
		inout_zombieParentSceneNode
		);

	zombiePart.SetHealth(0.0F);

	return;
}

/////////////////////////////////////////////////////////////////////
//static public methods
/////////////////////////////////////////////////////////////////////
/*static*/ ZombieComponent::TPointerZombieComponent ZombieComponent::Factory(
	const ZombieComponentLoad& in_zombieComponentLoad,
	TPointerSceneNode& in_sceneNodeParent,
	const bool in_gore,
	const float in_health
	)
{
	TPointerZombieComponent pointer;
	if (!in_sceneNodeParent)
	{
		return pointer;
	}
	GSceneNode& sceneNodeParent = *in_sceneNodeParent;

	TArrayZombiePart arrayZombiePart;
	TArrayArrayWeakSceneNodeComponentCollision arrayArrayCollision;
	TWeakSceneNodeComponentAnimation animation;
	TMapStringWeakSceneNodeComponentParticle mapNameParticle;

	//make the array zombie part
	const int zombiePartCount = in_zombieComponentLoad.GetArrayPartCount();
	arrayZombiePart.resize(zombiePartCount);
	for (int index = 0; index < zombiePartCount; ++index)
	{
		arrayZombiePart[index].reset(new ZombiePart(in_zombieComponentLoad.GetArrayPart()[index], in_health));

		GSceneNode::QueryAll(in_sceneNodeParent, *arrayZombiePart[index], GSceneNodeComponentVisual::GetComponentFlag());
	}

	//find the collision
	TArrayWeakSceneNodeComponentCollision arrayCollision;
	GSceneVisitorAllCollect<GSceneNodeComponentCollision>::Run(in_sceneNodeParent, arrayCollision);
	arrayArrayCollision.resize(in_zombieComponentLoad.GetArrayPartCount());
	for (TArrayWeakSceneNodeComponentCollision::iterator iterator = arrayCollision.begin(); iterator != arrayCollision.end(); ++iterator)
	{
		TPointerSceneNodeComponentCollision pointerCollision = (*iterator).lock();
		if (!pointerCollision)
		{
			continue;
		}
		GSceneNodeComponentCollision& collision = *pointerCollision;
		for (int index = 0; index < in_zombieComponentLoad.GetArrayPartCount(); ++index)
		{
			if (collision.GetData() == in_zombieComponentLoad.GetArrayPart()[index].GetPartName())
			{
				arrayArrayCollision[index].push_back(pointerCollision);
				break;
			}
		}
	}

	//find the animation
	const int componentCount = sceneNodeParent.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentAnimation pointerAnimation = sceneNodeParent.GetComponentPointer<GSceneNodeComponentAnimation>(index);
		if (pointerAnimation)
		{
			animation = pointerAnimation;
		}
	}

	//find the particles
	TArrayWeakSceneNodeComponentParticle arrayParticle;
	if (in_gore)
	{
		GSceneVisitorAllCollect<GSceneNodeComponentParticle>::Run(in_sceneNodeParent, arrayParticle);
		for (TArrayWeakSceneNodeComponentParticle::iterator iterator = arrayParticle.begin(); iterator != arrayParticle.end(); ++iterator)
		{
			TPointerSceneNodeComponentParticle particle = (*iterator).lock();
			if (!particle)
			{
				continue;
			}
			mapNameParticle[particle->GetData()] = (*iterator);
		}
	}

	pointer.reset(new ZombieComponent(
		in_zombieComponentLoad,
		arrayZombiePart,
		arrayArrayCollision,
		animation,
		mapNameParticle
		));

	return pointer;
}

/////////////////////////////////////////////////////////////////////
//constructor
/////////////////////////////////////////////////////////////////////
ZombieComponent::ZombieComponent(
	const ZombieComponentLoad& in_zombieComponentLoad,
	TArrayZombiePart& in_arrayZombiePart, //WARN: transfer ownership,
	TArrayArrayWeakSceneNodeComponentCollision& in_arrayCollision,
	TWeakSceneNodeComponentAnimation& in_animation,
	TMapStringWeakSceneNodeComponentParticle& in_mapNameParticle
	)
	: mZombieComponentLoad(in_zombieComponentLoad)
	, mArrayZombiePart(in_arrayZombiePart)
	, mArrayCollision(in_arrayCollision)
	, mAnimation(in_animation)
	, mMapNameParticle(in_mapNameParticle)
{
	return;
}

ZombieComponent::~ZombieComponent()
{
	return;
}

//public methods
void ZombieComponent::CollectDamageReceivers(
	SceneNodeComponentClientObjectBase* const in_parent,
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	TArrayPointerSceneNodeComponentCollisionResult arrayCollisionResult;

	for (int index = 0; index < mZombieComponentLoad.GetArrayPartCount(); ++index)
	{
		TArrayWeakSceneNodeComponentCollision& arrayComponentCollision = mArrayCollision[index];
		for (TArrayWeakSceneNodeComponentCollision::iterator iterator = arrayComponentCollision.begin(); iterator != arrayComponentCollision.end(); ++iterator)
		{
			TPointerSceneNodeComponentCollision pointerCollision = (*iterator).lock();
			if (!pointerCollision)
			{
				continue;
			}

			GSceneNodeComponentCollision& componentCollision = *pointerCollision;
			if (!componentCollision.GetActive())
			{
				continue;
			}
			TWeakSceneNodeComponentCollision weakCollision = pointerCollision;

			componentCollision.CollideCone(
				arrayCollisionResult,
				weakCollision,
				false,
				false,
				in_worldSpaceConeOrigin,
				in_worldSpaceConeDirection,
				in_coneLength,
				in_coneRadiusPoint,
				in_coneRadiusBase
				);
		}
	}

	for (TArrayPointerSceneNodeComponentCollisionResult::iterator iterator = arrayCollisionResult.begin(); iterator != arrayCollisionResult.end(); ++iterator)
	{
		out_arrayClientObjectCollisionResult.push_back(TPairClientObjectCollisionResult(
			in_parent,
			*(iterator)
			));
	}

	return;
}

const bool ZombieComponent::ApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	for (int index = 0; index < mZombieComponentLoad.GetArrayPartCount(); ++index)
	{
		if (in_data != mZombieComponentLoad.GetArrayPart()[index].GetPartName())
		{
			continue;
		}

		ZombiePart& zombiePart = *mArrayZombiePart[index];
		zombiePart.ModifiyHealth(inout_damage);

		return (zombiePart.GetHealth() <= 0.0F);
	}

	return false;
}

void ZombieComponent::AfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot, TPointerSceneNode& inout_zombieParentSceneNode)
{
	TArrayPointerZombiePartDetach arrayZombiePartDetach;

	//remove parts of zombie that need parent parts that are no longer there
	for (int index = 0; index < mZombieComponentLoad.GetArrayPartCount(); ++index)
	{
		ZombiePart& zombiePart = *mArrayZombiePart[index];
		const ZombiePartLoad& zombiePartLoad = mZombieComponentLoad.GetArrayPart()[index];
		const float health = zombiePart.GetHealth();
		if ((health <= 0.0F) || (0 == zombiePartLoad.GetArrayParentCount()))
		{
			continue;
		}

		bool partOk = false;
		for (int subIndex = 0; subIndex < zombiePartLoad.GetArrayParentCount(); ++subIndex)
		{
			if (LocalCheckParentsOk(mZombieComponentLoad, zombiePartLoad.GetArrayParent()[subIndex], mArrayZombiePart))
			{
				partOk = true;
				break;
			}
		}

		if (partOk)
		{
			continue;
		}

		//collect limb detach
		LocalDetachPart(
			mZombieComponentLoad, 
			arrayZombiePartDetach, 
			mArrayZombiePart, 
			index,
			inout_scene, 
			inout_sceenRoot, 
			inout_zombieParentSceneNode
			);
	}

	bool cutInHalf = true;
	for (int index = 0; index < mZombieComponentLoad.GetArrayCutInHalfIndexDeadCount(); ++index)
	{
		if (LocalPartOk(mArrayZombiePart, mZombieComponentLoad.GetArrayCutInHalfIndexDead()[index]))
		{
			cutInHalf = false;
			break;
		}
	}

	bool topOk = false;
	for (int index = 0; index < mZombieComponentLoad.GetArrayCutInHalfIndexAliveCount(); ++index)
	{
		if (LocalPartOk(mArrayZombiePart, mZombieComponentLoad.GetArrayCutInHalfIndexAlive()[index]))
		{
			topOk = true;
			break;
		}
	}

	bool bottomOk = false;
	for (int index = 0; index < mZombieComponentLoad.GetArrayCutInHalfIndexPartCount(); ++index)
	{
		if (LocalPartOk(mArrayZombiePart, mZombieComponentLoad.GetArrayCutInHalfIndexPart()[index]))
		{
			bottomOk = true;
			break;
		}
	}

	if (cutInHalf && topOk && bottomOk)
	{
		for (int index = 0; index < mZombieComponentLoad.GetArrayCutInHalfIndexPartCount(); ++index)
		{
			LocalDetachPart(
				mZombieComponentLoad, 
				arrayZombiePartDetach, 
				mArrayZombiePart, 
				mZombieComponentLoad.GetArrayCutInHalfIndexPart()[index],
				inout_scene,
				inout_sceenRoot, 
				inout_zombieParentSceneNode
			);
		}
	}



	////detach limbs
	//if (inout_sceenRoot)
	//{
	//	LocalCreateDetachedLimb(
	//		inout_scene,
	//		arrayZombiePartDetach,
	//		inout_sceenRoot,
	//		in_zombieParentSceneNode
	//		);
	//}

	//set bone not active
	for (int index = 0; index < mZombieComponentLoad.GetArrayPartCount(); ++index)
	{
		ZombiePart& zombiePart = *mArrayZombiePart[index];
		const float health = zombiePart.GetHealth();
		if (0.0F < health)
		{
			continue;
		}

		TArrayWeakSceneNodeComponentCollision& arrayComponentCollision = mArrayCollision[index];
		for (TArrayWeakSceneNodeComponentCollision::iterator iterator = arrayComponentCollision.begin(); iterator != arrayComponentCollision.end(); ++iterator)
		{
			TPointerSceneNodeComponentCollision pointerCollision = (*iterator).lock();
			if (!pointerCollision)
			{
				continue;
			}

			pointerCollision->SetActive(false);
		}
	}

	//update particle systems
	LocalUpdateParticleSystem(mZombieComponentLoad, mMapNameParticle, mArrayZombiePart);

	return;
}
