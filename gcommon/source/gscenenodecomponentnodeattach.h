//
//  GSceneNodeComponentNodeAttach.h
//
//  Created by David Coen on 2011 01 04
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentNodeAttach_h_
#define _GSceneNodeComponentNodeAttach_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GSceneNodeComponentBase.h"
#include "GMatrix16Float.h"

class GScene;
class GSceneNode;
class GSkeletonInstance;
class GScenePostLoadCallback;

/*
have removed the parent relative offset matrix from the node attach.
the scene node that we 'own', we set it's parent relative offset with the bone object space matrix.
if you need a offset, use another scene node 
(ie, parent relative matrix live in scene nodes, and scene nodes only)
*/
class GSceneNodeComponentNodeAttach : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentNodeAttach> TPointerSceneNodeComponentNodeAttach;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

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

	static void PostLoadCallbackOne(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);
	static void PostLoadCallbackTwo(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentNodeAttach(
		TPointerSceneNode& in_childNode,
		const std::string& in_boneName//,
		//const GMatrix16Float& in_boneRelativeMatrix
		);
	virtual ~GSceneNodeComponentNodeAttach();

	//public accessors
public:
	//set the child node transform relative to a bone
	void Update(TPointerSceneNode& inout_owner);

	//private methods
private:
	//look for our bone in the scene node
	void AttachToNode(TPointerSceneNode& inout_sceneNode);

	//private members;
private:
	TWeakSceneNode mWeakChildNode; //the child node we control the position of
	TWeakSkeletonInstance mWeakSkeletonInstance; //reference to the skeleton instance that has the bone we track

	std::string mBoneName;

	int mBoneIndex;

};

#endif 