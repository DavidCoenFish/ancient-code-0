//
//  GScenePostLoadCallback.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GScenePostLoadCallback.h"

#include "GSceneNodeComponentBase.h"

//constructor
GScenePostLoadCallback::GScenePostLoadCallback(const GScenePostLoadCallback& in_src)
: mSceneNode()
, mComponentBase()
, mComponentPostLoadCallback(NULL)
{
	(*this) = in_src;
	return;
}

GScenePostLoadCallback::GScenePostLoadCallback(
	TPointerSceneNode& in_sceneNode,
	TPointerSceneNodeComponentBase& inout_componentBase,
	const TComponentPostLoadCallback in_componentPostLoadCallback
	)
: mSceneNode(in_sceneNode)
, mComponentBase(inout_componentBase)
, mComponentPostLoadCallback(in_componentPostLoadCallback)
{
	return;
}

GScenePostLoadCallback::GScenePostLoadCallback()
: mSceneNode()
, mComponentBase()
, mComponentPostLoadCallback(NULL)
{
	return;
}

GScenePostLoadCallback::~GScenePostLoadCallback()
{
	return;
}

//operator
const GScenePostLoadCallback& GScenePostLoadCallback::operator=(const GScenePostLoadCallback& in_rhs)
{
	if (this != &in_rhs)
	{
		mSceneNode = in_rhs.mSceneNode;
		mComponentBase = in_rhs.mComponentBase;
		mComponentPostLoadCallback = in_rhs.mComponentPostLoadCallback;
	}

	return (*this);
}

//public methods
void GScenePostLoadCallback::Run(
	GScene& inout_scene, 
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNode pointerSceneNode = mSceneNode.lock();
	if ((!mComponentPostLoadCallback) || (!pointerSceneNode))
	{
		return;
	}

	TPointerSceneNodeComponentBase pointer = mComponentBase.lock();
	if (!pointer)
	{
		return;
	}

	(*mComponentPostLoadCallback)(
		inout_scene,
		pointerSceneNode,
		pointer,
		inout_arrayPostLoadCallback
		);

	return;
}