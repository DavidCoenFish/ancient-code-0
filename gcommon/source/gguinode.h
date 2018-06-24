//
//  GGuiNode.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNode_h_
#define _GGuiNode_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>

#include "GGuiCoordinate.h"
#include "GVector2Float.h"

class GGui;
class GGuiNodeComponentBase;
class GGuiNodeLoad;
class GGuiManager;
class GGuiNode;
class GGuiNodeComponentPostLoadData;

/**/
class GGuiNode : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;
	typedef boost::weak_ptr<GGuiNode> TWeakGuiNode;
	typedef std::vector<TPointerGuiNode> TArrayPointerGuiNode;

	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef boost::weak_ptr<GGuiNodeComponentBase> TWeakGuiNodeComponentBase;
	typedef std::vector<TPointerGuiNodeComponentBase> TArrayPointerGuiNodeComponentBase;

	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

	//static public methods
public:
	static TPointerGuiNode Factory(
		const GGuiNodeLoad& in_guiNodeLoad,
		GGuiManager& inout_manager,
		TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
		);

	//constructor
public:
	GGuiNode(
		const std::string& in_name,
		const GU32 in_flag,
		const GGuiCoordinate& in_topLeft,
		const GGuiCoordinate& in_size
		);
	~GGuiNode();

	//public methods
public:
	//query all, visit all suitable components and all children
	template <typename IN_VISITOR>
	static void QueryAll(TPointerGuiNode& inout_node, IN_VISITOR& inout_visitor, const int in_compositeComponentFlag)
	{
		if (!inout_node)
		{
			return;
		}
		GGuiNode& node = *inout_node;
		//do i satisfy query
		if (in_compositeComponentFlag & node.GetComponentComposititeFlag())
		{
			inout_visitor.QuerySubmitNode(inout_node);
		}

		//bail if no children satisfy query
		if (0 == (in_compositeComponentFlag & node.GetChildComponentComposititeFlag()))
		{
			return;
		}

		//visit all children
		for (TArrayPointerGuiNode::iterator iterator = node.mArrayChildren.begin(); iterator != node.mArrayChildren.end(); ++iterator)
		{
			QueryAll<IN_VISITOR>((*iterator), inout_visitor, in_compositeComponentFlag);
		}

		return;
	}

	const unsigned int GetComponentComposititeFlag();
	const unsigned int GetChildComponentComposititeFlag();

	void AddChild(
		TPointerGuiNode& inout_childNode
		);
	void RemoveChild(
		TPointerGuiNode& inout_childNode
		);


	void AddComponent(
		TPointerGuiNodeComponentBase& inout_component
		);
	void RemoveComponent(
		TPointerGuiNodeComponentBase& inout_component
		);

	void SetFlagTransformDirty();

	//public accessors
public:
	void SetTopLeft(const GGuiCoordinate& in_topLeft);
	void SetSize(const GGuiCoordinate& in_size);

	const std::string& GetName()const{ return mName; }

	const int GetComponentCount()const;

	template <typename IN_COMPONENT>
	IN_COMPONENT* const GetComponent(const int in_index)
	{
		TPointerGuiNodeComponentBase& pointerBase = mArrayComponentBase[in_index];
		boost::shared_ptr<IN_COMPONENT> pointerComponent = boost::shared_dynamic_cast<IN_COMPONENT, GGuiNodeComponentBase>(pointerBase);
		if (pointerComponent)
		{
			return pointerComponent.get();
		}

		return NULL;
	}

	template <typename IN_COMPONENT>
	boost::shared_ptr<IN_COMPONENT> const GetComponentPointer(const int in_index)
	{
		TPointerGuiNodeComponentBase& pointerBase = mArrayComponentBase[in_index];
		boost::shared_ptr<IN_COMPONENT> pointerComponent = boost::shared_dynamic_cast<IN_COMPONENT, GGuiNodeComponentBase>(pointerBase);
		if (pointerComponent)
		{
			return pointerComponent;
		}

		return boost::shared_ptr<IN_COMPONENT>();
	}

	const GVector2Float& GetTransformScreenTopLeft(const GGui& in_gui);
	const GVector2Float& GetTransformScreenBottomRight(const GGui& in_gui);
	const int GetTransformChangeId()const{ return mTransformChangeId; }

	//private methods
private:
	//write flag to self and all children
	void WriteFlagDownTree(const unsigned int in_flag, const bool in_active);
	//write flag to self and all parents
	void WriteFlagUpTree(const unsigned int in_flag, const bool in_active);

	void UpdateTransform(const GGui& in_gui);

	//private members
private:
	unsigned int mFlag;

	const std::string mName;
	GGuiCoordinate mTopLeft;
	GGuiCoordinate mSize;
	int mTransformChangeId;

	GGuiNode* mParent;
	TArrayPointerGuiNode mArrayChildren;
	TArrayPointerGuiNodeComponentBase mArrayComponentBase;

	//client component compositite flag. combine flags of all the node's components
	unsigned int mComponentComposititeFlag;

	//client child component compositite flag. combine all children node's mComponentComposititeFlag
	unsigned int mChildComponentComposititeFlag;

	GVector2Float mTransformScreenTopLeft;
	GVector2Float mTransformScreenBottomRight;

};

#endif 