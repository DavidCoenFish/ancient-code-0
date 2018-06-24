//
//  GSceneNodeComponentPhysics.cpp
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentPhysics.h"

#include "GSceneNodeComponentPhysicsLoad.h"
#include "GSceneType.h"
#include "GSceneNode.h"
#include "GPhysicsComponentPosition.h"

//static public methods
/*static*/ GSceneNodeComponentPhysics::TPointerSceneNodeComponentBase GSceneNodeComponentPhysics::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_componentData)
	{
		return pointer;
	}

	const GSceneNodeComponentPhysicsLoad& componentLoad = *((GSceneNodeComponentPhysicsLoad*)in_componentData);

	pointer.reset(new GSceneNodeComponentPhysics(
		componentLoad.GetMassInvert(),
		inout_owner->GetWorldTransform().GetPosition(),
		*componentLoad.GetInitialVelocity(),
		componentLoad.GetFlag()
		));

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentPhysics::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TPhysics;
}

//constructor
GSceneNodeComponentPhysics::GSceneNodeComponentPhysics(
	const float in_massInvert,
	const GVector3Float& in_position,
	const GVector3Float& in_velocity,
	const int in_flag
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mPhysicsComponentPosition()
	, mFlag(in_flag)
{
	mPhysicsComponentPosition.reset(new GPhysicsComponentPosition(
		in_massInvert,
		in_position,
		in_velocity
		));
	return;
}

/*virtual*/ GSceneNodeComponentPhysics::~GSceneNodeComponentPhysics()
{
	return;
}

//public methods
void GSceneNodeComponentPhysics::Tick(TPointerSceneNode& in_parent, const float in_timeDelta)
{
	if (GSceneNodeComponentPhysicsLoad::TFlag::TFreeze & mFlag)
	{
		return;
	}

	if (mPhysicsComponentPosition && 
		(GSceneNodeComponentPhysicsLoad::TFlag::TAllowMove & mFlag))
	{
		mPhysicsComponentPosition->AddAcceleration(GVector3Float(0.0F, -9.8F, 0.0F));
		mPhysicsComponentPosition->Intergrate(in_timeDelta);
		mPhysicsComponentPosition->ClearForceSum();

		GVector3Float position = mPhysicsComponentPosition->GetPosition();
		if ((GSceneNodeComponentPhysicsLoad::TFlag::TBounceYZero & mFlag) &&
			(position.m_y < 0.0F))
		{
			position.m_y = -(position.m_y);
			mPhysicsComponentPosition->ManualSetPosition(
				position,
				(mPhysicsComponentPosition->GetVelocity()) * (-0.75F)
				);
		}

		GMatrix16Float transform = in_parent->GetWorldTransform();
		transform.SetPosition(position);

		//HACK
		if (GSceneNodeComponentPhysicsLoad::TFlag::TAllowRotate & mFlag) 
		{
			static GMatrix16Float sRotateMatrix = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitY, 0.05F);
			transform = sRotateMatrix * transform;
		}

		in_parent->SetWorldTransform(transform);

	}
	return;
}

void GSceneNodeComponentPhysics::SetVelocity(const GVector3Float& in_velocity)
{
	if (mPhysicsComponentPosition)
	{
		mPhysicsComponentPosition->ManualSetPosition(
			mPhysicsComponentPosition->GetPosition(),
			in_velocity
			);
	}
	return;
}

void GSceneNodeComponentPhysics::AddForce(const GVector3Float& in_force)
{
	if (mPhysicsComponentPosition)
	{
		mPhysicsComponentPosition->AddForce(in_force);
	}
	return;
}

void GSceneNodeComponentPhysics::AddForce(
	const GVector3Float& in_force, 
	const GVector3Float& in_worldSpaceForceOrigin,
	const GVector3Float& in_worldSpaceForceDirection
	)
{
	//TODO
	return;
}
