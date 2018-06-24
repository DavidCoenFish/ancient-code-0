//
//  GSceneNodeUtility.h
//
//  Created by David Coen on 2011 01 29
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeUtility_h_
#define _GSceneNodeUtility_h_

#include <boost/shared_ptr.hpp>

class GScene;
class GSceneNode;

class GSceneNodeUtility 
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//disabled
private:
	~GSceneNodeUtility ();

	//public static methods
public:
	static void RecusiveSetVisualComponentVisible(TPointerSceneNode& inout_sceneNode, const bool in_visible);

	static TPointerSceneNode FindSceneNodeName(GScene& inout_scene, const std::string& in_name);
	static TPointerSceneNode FindSceneNodeName(TPointerSceneNode& inout_sceneNode, const std::string& in_name);

};

class GSceneNodeUtilityVisitorVisiblity
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

public:
	static void Run(TPointerSceneNode& inout_sceneNode, const bool in_visible);

private:
	GSceneNodeUtilityVisitorVisiblity(const bool in_visiblity);
	~GSceneNodeUtilityVisitorVisiblity();

	//contract with with GSceneNode::QueryAll
public:
	void QuerySubmitNode(TPointerSceneNode& inout_sceneNode);
	void QueryFinish();

private:
	const bool mVisible;

};

class GSceneNodeUtilityVisitorName
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

public:
	static TPointerSceneNode Run(GScene& inout_scene, const std::string& in_name);
	static TPointerSceneNode Run(TPointerSceneNode& inout_sceneNode, const std::string& in_name);

private:
	GSceneNodeUtilityVisitorName(const std::string& in_name);
	~GSceneNodeUtilityVisitorName();

	//contract with with GSceneNode::QueryAll
public:
	void QuerySubmitNode(TPointerSceneNode& inout_sceneNode);
	void QueryFinish();

private:
	const std::string& mName;
	TPointerSceneNode mFoundSceneNode;
	bool mFound;

};
#endif 