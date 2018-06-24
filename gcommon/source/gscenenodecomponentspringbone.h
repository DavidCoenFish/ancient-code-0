//
//  GSceneNodeComponentSpringBone.h
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentSpringBone_h_
#define _GSceneNodeComponentSpringBone_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GBuffer;
class GScene;
class GSceneNode;
class GScenePostLoadCallback;
class GSceneNodeComponentSpringBoneData;

/*
	hold spring bone data
	expect animation componet to tick bones during skeleton update
*/
class GSceneNodeComponentSpringBone : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentSpringBone> TPointerSceneNodeComponentSpringBone;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentSpringBoneData> TPointerSceneNodeComponentSpringBoneData;
	typedef std::vector<TPointerSceneNodeComponentSpringBoneData> TArrayPointerSceneNodeComponentSpringBoneData;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

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

	static void PostLoadCallbackA(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	static void PostLoadCallbackB(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentSpringBone(TArrayPointerSceneNodeComponentSpringBoneData& in_springBoneData);
	virtual ~GSceneNodeComponentSpringBone();

	//private members;
private:
	TArrayPointerSceneNodeComponentSpringBoneData mArrayBoneData;

};

#endif 