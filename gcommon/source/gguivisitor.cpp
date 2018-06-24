//
//  GGuiVisitor.cpp
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiVisitor.h"

//public static methods
/*static*/ GGuiVisitorNodeName::TWeakGuiNode GGuiVisitorNodeName::FindNode(
	GGui& inout_Gui, 
	const std::string& in_nodeName
	)
{
	TWeakGuiNode pNode;
	GGuiVisitorNodeName visitor(pNode, in_nodeName);
	inout_Gui.QueryAll(visitor, 0xFFFFFFFF);
	return pNode;
}

//constructor
GGuiVisitorNodeName::GGuiVisitorNodeName(
	TWeakGuiNode& inout_node, 
	const std::string& in_nodeName		
	)
	: mNode(inout_node)
	, mNodeName(in_nodeName)
{
	return;
}

GGuiVisitorNodeName::~GGuiVisitorNodeName()
{
	return;
}

//public methods
void GGuiVisitorNodeName::QuerySubmitNode(
	TPointerGuiNode& inout_node
	)
{
	if (inout_node->GetName() == mNodeName)
	{
		mNode = inout_node;
	}
}
