//
//  GSceneVisitorAll.cpp
//
//  Created by David Coen on 2011 01 07
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorAll.h"

//public static methods
/*static*/ void GSceneVisitorAllFind::Run(GScene& inout_scene, TWeakSceneNode& out_node, const std::string& in_name)
{
	GSceneVisitorAllFind visitor(out_node, in_name);
	inout_scene.QueryAll< GSceneVisitorAllFind >(
		visitor, 
		0
		);
	return;
}

//constructor
GSceneVisitorAllFind::GSceneVisitorAllFind(TWeakSceneNode& out_node, const std::string& in_name)
: m_node(out_node)
, m_name(in_name)
{
	return;
}

GSceneVisitorAllFind::~GSceneVisitorAllFind()
{
	return;
}

//public methods
void GSceneVisitorAllFind::QuerySubmitNode(TPointerSceneNode& inout_node)
{
	if (!inout_node)
	{
		return;
	}
	if (m_name == inout_node->GetName())
	{
		m_node = inout_node;
	}

	return;
}
void GSceneVisitorAllFind::QueryFinish()
{
	return;
}
