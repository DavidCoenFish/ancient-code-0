//
//  GScene.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GScene_h_
#define _GScene_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <map>
#include "GSceneNode.h"
#include "GColour4Float.h"

class GFileManager;
class GResourceManager;
class GBuffer;
class GRender;
class GCamera;
class GSceneNodeComponentBase;
class GSceneNodeComponentClient;
class GSceneNodeComponentLoad;
class GSceneNodeFactory;
class GMeshManual;
class GMaterialInstance;
/*
allow get scene node by name? or a search query that you can quit?
*/
class GScene : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GScene> TPointerScene;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef std::vector<TPointerSceneNode> TArrayPointerSceneNode;
	typedef std::vector<TWeakSceneNode> TArrayWeakSceneNode;

	typedef std::vector<std::string> TArrayString;

	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

	typedef boost::shared_ptr<GFileManager> TPointerFileManager;
	typedef boost::weak_ptr<GFileManager> TWeakFileManager;
	typedef boost::shared_ptr<GResourceManager> TPointerResourceManager;
	typedef boost::weak_ptr<GResourceManager> TWeakResourceManager;
	typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;
	typedef boost::weak_ptr<GSceneNodeFactory> TWeakSceneNodeFactory;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data
		);
	typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
	typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;

#ifdef DSC_DEBUG
	typedef std::vector<GMatrix16Float> TArrayMatrix;
	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;
#endif

	//static public methods
public:
	static TPointerScene Factory(
		TPointerResourceManager& inout_resourceManager,
		const TMapStringPairFactoryClientCallbackData& in_clientComponentFactory,
		TPointerSceneNodeFactory& inout_sceneNodeFactory,
		const std::string& in_fileName
		);

	static void FactoryPostLoad(
			TArrayScenePostLoadCallback& in_arrayPostLoad,
			GScene& inout_scene
			);

	//constructor
public:
	GScene(
		TPointerResourceManager& inout_resourceManager,
		const TMapStringPairFactoryClientCallbackData& in_clientComponentFactory,
		TPointerSceneNodeFactory& inout_sceneNodeFactory
		);
	~GScene();

	//public methods
public:
	TPointerSceneNodeComponentBase LoadComponent(
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const GSceneNodeComponentLoad& in_componentLoad
		);

	template <typename IN_VISITOR>
	void QueryAll(IN_VISITOR& inout_visitor, const unsigned int in_componentFlag)
	{
		//visit all children
		for (TArrayPointerSceneNode::iterator iterator = mArraySceneNode.begin(); iterator != mArraySceneNode.end(); ++iterator)
		{
			GSceneNode::QueryAll<IN_VISITOR>(*iterator, inout_visitor, in_componentFlag);
		}

		inout_visitor.QueryFinish();

		return;
	}

	TPointerSceneNodeComponentBase ClientComponentFactory(
		const std::string& in_clientName,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//use given factor as template to create a scene node heirarchy under given owner
	void SceneNodeFactory(
		const std::string& in_factoryStubName,
		const std::string& in_factoryData,
		TPointerSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	void LoadSceneUnderNode(
		const std::string& in_sceneName,
		TPointerSceneNode& inout_owner		
		);

	//we queue up delete requests to told to do them
	void SceneNodeRequestDelete(TPointerSceneNode& inout_sceneNode);
	void SceneNodeProcessDeleteQueue();

	TPointerSceneNode CreateSceneNode(const std::string& in_sceneNodeName);

#ifdef DSC_DEBUG
	void DebugDraw(
		GRender& in_renderer,
		GCamera& in_camera		
		);
#endif

	//public accessors
public:
	TPointerResourceManager GetResourceManager();

	const int GetSceneRootCount()const;
	const TPointerSceneNode& GetSceneRoot(const int in_index)const;
	TPointerSceneNode& GetSceneRoot(const int in_index);
	void AddSceneRoot(TPointerSceneNode& inout_sceneNode);

	const std::string& GetFactoryData()const{ return mArrayFactoryData.back(); }

	const TMapStringPairFactoryClientCallbackData& GetClientComponentFactory()const{ return mClientComponentFactory; }
	TMapStringPairFactoryClientCallbackData& GetClientComponentFactory(){ return mClientComponentFactory; }

#ifdef DSC_DEBUG
	static GScene* sScene;
	TArrayMatrix& GetArrayDebugMatrix(){ return mArrayDebugMatrix; }
#endif

	//private members;
private:
	TArrayPointerSceneNode mArraySceneNode;

	//we keep a reference so that we can load nodes/ components at any point in time
	TWeakResourceManager mResourceManager;
	TWeakSceneNodeFactory mSceneNodeFactory;
	TMapStringPairFactoryClientCallbackData mClientComponentFactory;

	TArrayWeakSceneNode mArrayPendingSceneNodeDelete;

	//factory data from the last factory stub call
	TArrayString mArrayFactoryData;

#ifdef DSC_DEBUG
	TArrayMatrix mArrayDebugMatrix;
	TPointerMeshManual mAxisMesh;
	TWeakMaterialInstance mAxisMaterial;
#endif

	//todo: if you want to do a spacial query on a component, then add a sphere tree for that component?
};

#endif 