//
//  GSceneNodeComponentFade.h
//
//  Created by David Coen on 2011 02 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentFade_h_
#define _GSceneNodeComponentFade_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GScene;
class GSceneNode;
class GScenePostLoadCallback;
class GComponentLerpFloat;


/*
	eventually to fade out item (alpha material override)
	for now, just delete parent node at end of duration
*/
class GSceneNodeComponentFade : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentFade> TPointerSceneNodeComponentFade;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

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
	GSceneNodeComponentFade(
		GScene& inout_scene,
		const float in_duration
		);
	virtual ~GSceneNodeComponentFade();

	//public methods
public:
	void Tick(TPointerSceneNode& inout_node, const float in_timeDelta);

	//private members;
private:
	GScene& mScene;// for GScene::SceneNodeRequestDelete. alt is to pass scene in tick
	TPointerComponentLerpFloat mLerpFade;

};

#endif 