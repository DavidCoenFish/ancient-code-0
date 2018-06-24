//
//  SceneNodeComponentClientDropShadow.h
//  Zombie
//
//  Created by David Coen on 2011 03 14
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientDropShadow_h_
#define _SceneNodeComponentClientDropShadow_h_

#include "GSceneNodeComponentClient.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GCamera;
class GSceneNode;
class GSceneNodeComponentBase;
class GScenePostLoadCallback;
class GVector2Float;
class ZombieMission;

class SceneNodeComponentClientDropShadow : public GSceneNodeComponentClient
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientDropShadow> TPointerSceneNodeComponentClientDropShadow;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	//static public methods
public:
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of ApplicationTaskGame
		);

	//constructor
public:
	SceneNodeComponentClientDropShadow(TPointerSceneNode& inout_childSceneNode);
	virtual ~SceneNodeComponentClientDropShadow();

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//private members
private:
	TWeakSceneNode mChildSceneNode;

};

#endif //_SceneNodeComponentClientDropShadow_h_
