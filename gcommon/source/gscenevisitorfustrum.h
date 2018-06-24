//
//  GSceneVisitorFustrum.h
//
//  Created by David Coen on 2011 02 15
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorFustrum_h_
#define _GSceneVisitorFustrum_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GVector3Float.h"

class GScene;
class GCamera;
class GSceneNode;

/*
a scene visitor for rendering
*/
class GSceneVisitorFustrum : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(
		GScene& inout_scene,
		GCamera& in_camera		
		);

	//constructor
private:
	GSceneVisitorFustrum(
		GCamera& in_camera
		);
	~GSceneVisitorFustrum();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		);
	void QueryFinish(); //draw pending objects

	//private members;
private:
	GCamera& mCamera;

};

#endif 