//
//  GSceneNodeComponentPhysics.h
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentPhysics_h_
#define _GSceneNodeComponentPhysics_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GSceneNodeComponentBase.h"

class GAsset;
class GScene;
class GSceneNode;
class GScenePostLoadCallback;
class GSceneNodeComponentPhysicsLoad;
class GPhysicsComponentPosition;
class GVector3Float;

/*
animation component may keep a weak reference to the visual to access the
meshExtended to update bone positions or whatever...
*/
class GSceneNodeComponentPhysics : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentPhysics> TPointerSceneNodeComponentPhysics;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GPhysicsComponentPosition> TPointerPhysicsComponentPosition;


	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading. return base rather than type pointer due to generic interface
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentPhysics(
		const float in_massInvert,
		const GVector3Float& in_position,
		const GVector3Float& in_velocity,
		const int in_flag
		);
	virtual ~GSceneNodeComponentPhysics();

	//public methods
public:
	//if the scene node has a teleport flag, use the scene node pos/ rotation, else set the scene node with out own
	void Tick(TPointerSceneNode& in_parent, const float in_timeDelta);
	void SetVelocity(const GVector3Float& in_velocity);

	void AddForce(const GVector3Float& in_force);
	void AddForce(
		const GVector3Float& in_force, 
		const GVector3Float& in_worldSpaceForceOrigin,
		const GVector3Float& in_worldSpaceForceDirection
		);

	//public accessors
public:

	//private members;
private:
	TPointerPhysicsComponentPosition mPhysicsComponentPosition;
	int mFlag; //freeze, allow rotate, use collision data, use collision y==0


};

#endif 