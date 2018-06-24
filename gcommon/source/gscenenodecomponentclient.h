//
//  GSceneNodeComponentClient.h
//
//  Created by David Coen on 2010 10 28
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentClient_h_
#define _GSceneNodeComponentClient_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GScenePostLoadCallback;
class GScene;
class GSceneNode;
class GBuffer;

/*
expect client to derrive this class if they want to get access to tick scene visitor
if they don't want client interface, they can derrive off GSceneNodeComponentBase
also expect client to provide compopnent factory function to GScene
*/
class GSceneNodeComponentClient : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentClient> TPointerSceneNodeComponentClient;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentClient();
	virtual ~GSceneNodeComponentClient();

	//public methods
public:
	//todo: a universal time int to help keep scene animations in sync?
	void Tick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//private methods
private:
	virtual void OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta);

	//private members;
private:

};

#endif 