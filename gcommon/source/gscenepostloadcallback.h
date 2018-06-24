//
//  GScenePostLoadCallback.h
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GScenePostLoadCallback_h_
#define _GScenePostLoadCallback_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GScene;
class GSceneNode;
class GSceneNodeComponentBase;

class GScenePostLoadCallback
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef boost::weak_ptr<GSceneNodeComponentBase> TWeakSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef void (*TComponentPostLoadCallback)(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//constructor
public:
	GScenePostLoadCallback(const GScenePostLoadCallback& in_src);
	GScenePostLoadCallback(
		TPointerSceneNode& in_sceneNode,
		TPointerSceneNodeComponentBase& inout_componentBase,
		const TComponentPostLoadCallback in_componentPostLoadCallback
		);
	GScenePostLoadCallback();
	~GScenePostLoadCallback();

	//operator
public:
	const GScenePostLoadCallback& operator=(const GScenePostLoadCallback& in_rhs);

	//public methods
public:
	void Run(
		GScene& inout_scene, 
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//private members;
private:
	TWeakSceneNode mSceneNode;
	TWeakSceneNodeComponentBase mComponentBase;
	TComponentPostLoadCallback mComponentPostLoadCallback;

};

#endif 