//
//  SceneNodeComponentClientTarget.h
//  Zombie
//
//  Created by David Coen on 2011 02 09
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientTarget_h_
#define _SceneNodeComponentClientTarget_h_

#include "GSceneNodeComponentClient.h"
#include <boost/intrusive/list_hook.hpp>
#include <GVector3Float.h>

class GCamera;
class GSceneNode;
class GSceneNodeComponentBase;
class GScenePostLoadCallback;
class GVector2Float;
class ZombieMission;

class SceneNodeComponentClientTarget : public GSceneNodeComponentClient
{
	//typedef
public:
	typedef boost::intrusive::list_member_hook< boost::intrusive::link_mode<boost::intrusive::auto_unlink> > TListHook;

	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<SceneNodeComponentClientTarget> TPointerSceneNodeComponentClientTarget;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

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
	SceneNodeComponentClientTarget(ZombieMission& inout_zombieMission);
	virtual ~SceneNodeComponentClientTarget();

	//public methods
public:
	const GVector2Float GetScreenPos(GCamera& in_camera);

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//public members
public:
	TListHook mListHook;

	//private members
private:
	GVector3Float mWorldPos;

};

#endif //_SceneNodeComponentClientTarget_h_
