//
//  GSceneVisitorAll.h
//
//  Created by David Coen on 2011 01 07
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorAll_h_
#define _GSceneVisitorAll_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GScene.h"
#include "GSceneNode.h"

class GMatrix16Float;

/*
	//current limitation, node needs a component of some kind to find it? of 0 component flag mean all?
	hack, pass 0 as component flag 
*/
class GSceneVisitorAllFind : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	//public static methods
public:
	static void Run(GScene& inout_scene, TWeakSceneNode& out_node, const std::string& in_name);

	//constructor
private:
	GSceneVisitorAllFind(TWeakSceneNode& out_node, const std::string& in_name);
	~GSceneVisitorAllFind();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(TPointerSceneNode& inout_node);
	void QueryFinish();

private:
	TWeakSceneNode& m_node;
	const std::string& m_name;

};


/*
a scene visitor for calling Update(in_sceneNode) on all of IN_TYPE compoents
*/
template <typename IN_TYPE>
class GSceneVisitorAllUpdate : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(GScene& inout_scene)
	{
		GSceneVisitorAllUpdate visitor;
		inout_scene.QueryAll< GSceneVisitorAllUpdate<IN_TYPE> >(
			visitor, 
			IN_TYPE::GetComponentFlag()
			);
	}

	//constructor
private:
	GSceneVisitorAllUpdate()
	{
		return;
	}
	~GSceneVisitorAllUpdate()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		)
	{
		GSceneNode& sceneNode = *inout_node;
		const int componentCount = sceneNode.GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			IN_TYPE* const pComponent = sceneNode.GetComponent<IN_TYPE>(index);
			if (!pComponent)
			{
				continue;
			}
			pComponent->Update(inout_node);
		}

		return;
	}
	void QueryFinish()
	{
		return;
	}

};

template <typename IN_TYPE>
class GSceneVisitorAllTick : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(GScene& inout_scene, const float in_timeDelta)
	{
		GSceneVisitorAllTick visitor(in_timeDelta);
		inout_scene.QueryAll< GSceneVisitorAllTick<IN_TYPE> >(
			visitor, 
			IN_TYPE::GetComponentFlag()
			);
	}

	//constructor
private:
	GSceneVisitorAllTick(const float in_timeDelta)
		: mTimeDelta(in_timeDelta)
	{
		return;
	}
	~GSceneVisitorAllTick()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		)
	{
		GSceneNode& sceneNode = *inout_node;
		const int componentCount = sceneNode.GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			IN_TYPE* const pComponent = sceneNode.GetComponent<IN_TYPE>(index);
			if (!pComponent)
			{
				continue;
			}
			pComponent->Tick(inout_node, mTimeDelta);
		}

		return;
	}
	void QueryFinish()
	{
		return;
	}
private:
	const float mTimeDelta;
};

template <typename IN_TYPE>
class GSceneVisitorAllCamera : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(GScene& inout_scene, GCamera& in_camera)
	{
		GSceneVisitorAllCamera visitor(in_camera);
		inout_scene.QueryAll< GSceneVisitorAllCamera<IN_TYPE> >(
			visitor, 
			IN_TYPE::GetComponentFlag()
			);
	}

	//constructor
private:
	GSceneVisitorAllCamera(GCamera& in_camera)
		: mCamera(in_camera)
	{
		return;
	}
	~GSceneVisitorAllCamera()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		)
	{
		GSceneNode& sceneNode = *inout_node;
		const int componentCount = sceneNode.GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			IN_TYPE* const pComponent = sceneNode.GetComponent<IN_TYPE>(index);
			if (!pComponent)
			{
				continue;
			}
			pComponent->VisitorAllCamera(sceneNode, mCamera);
		}

		return;
	}
	void QueryFinish()
	{
		return;
	}
private:
	GCamera& mCamera;
};


template <typename IN_TYPE>
class GSceneVisitorAllCollect : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::shared_ptr<IN_TYPE> TPointerType;
	typedef boost::weak_ptr<IN_TYPE> TWeakType;
	typedef std::vector<TWeakType> TArrayWeakType;


	//public static methods
public:
	static void Run(GScene& inout_scene, TArrayWeakType& out_arrayType)
	{
		GSceneVisitorAllCollect visitor(out_arrayType);
		inout_scene.QueryAll< GSceneVisitorAllCollect<IN_TYPE> >(
			visitor, 
			IN_TYPE::GetComponentFlag()
			);
		return;
	}
	static void Run(TPointerSceneNode& inout_sceneNode, TArrayWeakType& out_arrayType)
	{
		GSceneVisitorAllCollect visitor(out_arrayType);
		GSceneNode::QueryAll(
			inout_sceneNode, 
			visitor,
			IN_TYPE::GetComponentFlag()
			);
		return;
	}
	//constructor
private:
	GSceneVisitorAllCollect(TArrayWeakType& out_arrayType)
		: mArrayType(out_arrayType)
	{
		return;
	}
	~GSceneVisitorAllCollect()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		)
	{
		GSceneNode& sceneNode = *inout_node;
		const int componentCount = sceneNode.GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			TPointerType pComponent = sceneNode.GetComponentPointer<IN_TYPE>(index);
			if (!pComponent)
			{
				continue;
			}
			TWeakType weakComponent(pComponent);
			mArrayType.push_back(weakComponent);
		}

		return;
	}
	void QueryFinish()
	{
		return;
	}
private:
	TArrayWeakType& mArrayType;
};

#endif 