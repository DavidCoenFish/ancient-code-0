//
//  GSceneNodeComponentFactoryStub.h
//
//  Created by David Coen on 2010 10 28
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentFactoryStub_h_
#define _GSceneNodeComponentFactoryStub_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GScenePostLoadCallback;
class GScene;
class GSceneNode;
class GBuffer;

/*
this is a cookuu class, it pretendes to load a component with the factory, but instead shunts the owner scene node off to the 
scene node factory to be populated there. the problem with this approach is that it returns a nill component as the stub is not preserved,
also that it is not 'truth in advertising' as to the original intent of the code
*/
class GSceneNodeComponentFactoryStub : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
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

	//disabled
private:
	~GSceneNodeComponentFactoryStub();

};

#endif 