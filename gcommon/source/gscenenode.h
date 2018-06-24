//
//  GSceneNode.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNode_h_
#define _GSceneNode_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GMatrix16Float.h"

class GSceneNodeComponentBase;
class GScenePostLoadCallback;
class GScene;
class GSceneNodeLoad;

/*
scene node owns components
has a spacial position
move parent relative transform into a component? if it is optional, then yes. makes calculating radius a pain though. 
move radius into component? only some things need it
how about world transform invert?
was hoping for down only information tree, but now have reference to parent for marking parent flags dirty
*/
class GSceneNode : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef std::vector<TPointerSceneNodeComponentBase> TArrayPointerSceneNodeComponentBase;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;
	typedef std::vector<TPointerSceneNode> TArrayPointerSceneNode;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

private:
	struct TFlag
	{
		enum TEnum
		{
			TNone							= 0x00000000

			,TWorldTransformDirty			= 0x00000001 //parent relative changed or parent moved
			,TWorldInvertDirty				= 0x00000002 

			,TComponentFlagDirty			= 0x00000004 //component added or removed to this node
			,TComponentFlagHierarchyDirty	= 0x00000008 //child node added or removed, could be grandchild, great grand child ...

			,TTeleport						= 0x00000010 //notify node and decendants that node has instantaniouslt move, so no sweep/ drag of cloth

			,TDontInheritParentTransform	= 0x00000020 //be relative to world origin rather than partent world transform
		};
	};

	//static public methods
public:
	static TPointerSceneNode Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TArrayScenePostLoadCallback& inout_arrayPostLoad,
		const GSceneNodeLoad& in_sceneNodeLoad
		);

	//exposed for sceneNodeFactory
	static void FactoryLoadChildrenAndComponents(
		TPointerSceneNode& inout_owner,
		GScene& inout_scene, //access to mesh/ material manager
		TArrayScenePostLoadCallback& inout_arrayPostLoad,
		const GSceneNodeLoad& in_sceneNodeLoad
		);

	//constructor
public:
	GSceneNode(const std::string& in_name);
	~GSceneNode();

	//public methods
public:
	//query all, visit all suitable components and all children
	template <typename IN_VISITOR>
	static void QueryAll(TPointerSceneNode& inout_sceneNode, IN_VISITOR& inout_visitor, const int in_compositeComponentFlag)
	{
		GSceneNode& sceneNode = *inout_sceneNode;
		//do i satisfy query
		if ((0 == in_compositeComponentFlag) ||
			(in_compositeComponentFlag & sceneNode.GetComponentComposititeFlag()))
		{
			inout_visitor.QuerySubmitNode(inout_sceneNode);
		}

		//bail if no children satisfy query
		if ((0 != in_compositeComponentFlag) &&
			(0 == (in_compositeComponentFlag & sceneNode.GetChildComponentComposititeFlag())))
		{
			return;
		}

		//visit all children
		for (TArrayPointerSceneNode::iterator iterator = sceneNode.mArrayChildren.begin(); iterator != sceneNode.mArrayChildren.end(); ++iterator)
		{
			GSceneNode::QueryAll<IN_VISITOR>(*iterator, inout_visitor, in_compositeComponentFlag);
		}

		return;
	}
	//write flag to self and all children
	void WriteFlagDownTree(const unsigned int in_flag, const bool in_active);

	//write flag to self and all parents
	void WriteFlagUpTree(const unsigned int in_flag, const bool in_active);

	//public accessors
public:
	const std::string& GetName()const{ return mName; }

	const int GetComponentCount()const;

	template <typename IN_COMPONENT>
	IN_COMPONENT* const GetComponent(const int in_index)
	{
		TPointerSceneNodeComponentBase& pointerBase = mArrayComponent[in_index];
		boost::shared_ptr<IN_COMPONENT> pointerComponent = boost::shared_dynamic_cast<IN_COMPONENT, GSceneNodeComponentBase>(pointerBase);
		if (pointerComponent)
		{
			return pointerComponent.get();
		}

		return NULL;
	}

	template <typename IN_COMPONENT>
	boost::shared_ptr<IN_COMPONENT> const GetComponentPointer(const int in_index)
	{
		TPointerSceneNodeComponentBase& pointerBase = mArrayComponent[in_index];
		boost::shared_ptr<IN_COMPONENT> pointerComponent = boost::shared_dynamic_cast<IN_COMPONENT, GSceneNodeComponentBase>(pointerBase);
		if (pointerComponent)
		{
			return pointerComponent;
		}

		return boost::shared_ptr<IN_COMPONENT>();
	}

	const GMatrix16Float& GetParentRelativeTransform()const;
	void SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform);

	const GMatrix16Float& GetWorldTransform();
	void SetWorldTransform(const GMatrix16Float& in_worldTransform);

	const GMatrix16Float& GetWorldTransformInvert();

	const unsigned int GetComponentComposititeFlag();
	const unsigned int GetChildComponentComposititeFlag();

	const GSceneNode* const GetParent()const{ return mParent; }
	GSceneNode* const GetParent(){ return mParent; }

	void AddChildNode(TPointerSceneNode& inout_childSceneNode); //WARN: transferal of ownership
	void RemoveChild(const TPointerSceneNode& in_sceneNode); //WARNING: DO NOT DO THIS WHILE IN A QUERY... actually you could, but not a safe idea, 

	void AddComponent(TPointerSceneNodeComponentBase& inout_component); //WARN: transferal of ownership
	void RemoveComponent(const TPointerSceneNodeComponentBase& in_component); 

	//teleport flag writes down tree
	void SetTeleportFlag(const bool in_state);
	const bool GetTeleportFlag()const;
	
	void SetDontInheritParentTransform(const bool in_state);
	const bool GetDontInheritParentTransform()const;

	//private methods
private:
	void SetParent(GSceneNode* const inout_parentSceneNode){ mParent = inout_parentSceneNode; return; }

	//private members
private:
	GSceneNode* mParent;

	const std::string mName;
	GMatrix16Float mParentRelativeTransform;
	GMatrix16Float mWorldTransform;
	GMatrix16Float mWorldTransformInvert;
	unsigned int mFlag;

	TArrayPointerSceneNodeComponentBase mArrayComponent;
	TArrayPointerSceneNode mArrayChildren;

	//allow speedup of query?
	//client component compositite flag. combine flags of all the node's components
	unsigned int mComponentComposititeFlag;

	//client child component compositite flag. combine all children node's mComponentComposititeFlag
	unsigned int mChildComponentComposititeFlag;
};

#endif 