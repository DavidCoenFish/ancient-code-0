//
//  GGuiVisitor.h
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiVisitor_h_
#define _GGuiVisitor_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GGui.h"

class GGuiManager;
class GRender;
class GGuiNode;

class GGuiVisitorNodeName : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;
	typedef boost::weak_ptr<GGuiNode> TWeakGuiNode;

	//public static methods
public:
	static TWeakGuiNode FindNode(
		GGui& inout_Gui, 
		const std::string& in_nodeName
		);

	//constructor
private:
	GGuiVisitorNodeName(
		TWeakGuiNode& inout_node, 
		const std::string& in_nodeName		
		);
	~GGuiVisitorNodeName();

	//public methods
public:
	void QuerySubmitNode(
		TPointerGuiNode& inout_node
		);

	//private members
private:
	TWeakGuiNode& mNode;
	const std::string mNodeName;

};

//gather array of components of type
template <typename IN_TYPE>
class GGuiVisitor : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;
	typedef boost::shared_ptr<IN_TYPE> TPointerType;
	typedef boost::weak_ptr<IN_TYPE> TWeakType;
	typedef std::vector<TWeakType> TArrayWeakType;


	//public static methods
public:
	static void Run(
		GGui& inout_Gui, 
		TArrayWeakType& out_arrayType
		)
	{
		GGuiVisitor visitor(out_arrayType);
		inout_Gui.QueryAll(visitor, IN_TYPE::GetComponentFlag());
		return;
	}
	//constructor
private:
	GGuiVisitor(TArrayWeakType& out_arrayType)
		: mArrayType(out_arrayType)
	{
		return;
	}
	~GGuiVisitor()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerGuiNode& inout_node
		)
	{
		GGuiNode& node = *inout_node;
		const int componentCount = node.GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			TPointerType pComponent = node.GetComponentPointer<IN_TYPE>(index);
			if (!pComponent)
			{
				continue;
			}
			TWeakType weakComponent(pComponent);
			mArrayType.push_back(weakComponent);
		}

		return;
	}
private:
	TArrayWeakType& mArrayType;
};

#endif 