//
//  GScene.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GScene.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GFileManager.h"
#include "GResourceManager.h"
#include "GMesh.h"
#include "GMeshManual.h"
#include "GMaterial.h"
#include "GMaterialInstance.h"
#include "GSceneNodeComponentAnimation.h"
#include "GSceneNodeComponentClient.h"
#include "GSceneNodeComponentCollision.h"
#include "GSceneNodeComponentFade.h"
#include "GSceneNodeComponentLight.h"
#include "GSceneNodeComponentLightCollector.h"
#include "GSceneNodeComponentPointAtBone.h"
#include "GSceneNodeComponentNodeAttach.h"
#include "GSceneNodeComponentParticle.h"
#include "GSceneNodeComponentPhysics.h"
#include "GSceneNodeComponentSpringBone.h"
#include "GSceneNodeComponentVisual.h"
#include "GSceneNode.h"
#include "GColour4Byte.h"
#include "GColour4Float.h"
#include "GScenePostLoadCallback.h"
#include "GSceneNodeComponentLoad.h"
#include "GSceneLoad.h"
#include "GSceneNodeFactory.h"
#include "GSceneNodeComponentFactoryStub.h"

#include <map>
#include <string>

#ifdef DSC_DEBUG
	#include "GRender.h"
	#include "GMaterial.h"
	#include "GMeshStreamInfo.h"

	typedef std::vector<GU8> TArrayByte;
	typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

	/*static*/ GScene* GScene::sScene = NULL;

	static const char* const sVertexShader = "\n\
	#version 100\n\
	uniform mat4 u_mvpMatrix;\n\
	attribute vec4 a_position;\n\
	attribute vec4 a_colour;\n\
	varying vec4 v_color;\n\
	void main()\n\
	{\n\
	   gl_Position = u_mvpMatrix * a_position;\n\
		v_color = a_colour;\n\
	}\n\
	";
	static const char* const sFragmentShader = "\n\
	#version 100\n\
	\n\
	#ifdef GL_ES\n\
		#define LOWP lowp\n\
		#define MEDIUMP mediump\n\
		#define HIGHP highp\n\
	#else\n\
		#define LOWP\n\
		#define MEDIUMP\n\
		#define HIGHP\n\
	#endif\n\
	varying LOWP vec4 v_color;\n\
	\n\
	void main()\n\
	{\n\
		gl_FragColor = v_color;\n\
	}\n\
	";
	static const char* sShaderArrayAttributeName[] = {
		"a_position",
		"a_colour"
	};
	static const int sShaderArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayAttributeName);
	static const char* sShaderArrayUniformName[] = {
		"u_mvpMatrix"
	};
	static const int sShaderArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayUniformName);

#endif

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::shared_ptr<GBuffer> TPointerFile;

typedef TPointerSceneNodeComponentBase (*TComponentFactoryCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	);
typedef std::map<int, TComponentFactoryCallback> TMapIntFactoryCallback;

//static local methods
static const TMapIntFactoryCallback& LocalGetComponentFactoryMap()
{
	static TMapIntFactoryCallback sFactoryMap;
	if (!sFactoryMap.size())
	{
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentAnimation::GetComponentFlag(), GSceneNodeComponentAnimation::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentClient::GetComponentFlag(), GSceneNodeComponentClient::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentCollision::GetComponentFlag(), GSceneNodeComponentCollision::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentFactoryStub::GetComponentFlag(), GSceneNodeComponentFactoryStub::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentFade::GetComponentFlag(), GSceneNodeComponentFade::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentNodeAttach::GetComponentFlag(), GSceneNodeComponentNodeAttach::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentParticle::GetComponentFlag(), GSceneNodeComponentParticle::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentPhysics::GetComponentFlag(), GSceneNodeComponentPhysics::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentPointAtBone::GetComponentFlag(), GSceneNodeComponentPointAtBone::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentSpringBone::GetComponentFlag(), GSceneNodeComponentSpringBone::Factory));
		sFactoryMap.insert(TMapIntFactoryCallback::value_type(GSceneNodeComponentVisual::GetComponentFlag(), GSceneNodeComponentVisual::Factory));
	}
	return sFactoryMap;
}

//static public methods
/*static*/ GScene::TPointerScene GScene::Factory(
	TPointerResourceManager& inout_resourceManager,
	const TMapStringPairFactoryClientCallbackData& in_clientComponentFactory,
	TPointerSceneNodeFactory& inout_sceneNodeFactory,
	const std::string& in_fileName
	)
{
	TPointerScene pointer;

	pointer.reset(new GScene(
		inout_resourceManager,
		in_clientComponentFactory,
		inout_sceneNodeFactory
		));

	TPointerFile assetSceneLoad = inout_resourceManager->GetFileManager().LoadUnmanagedFile(in_fileName, true, true);
	if (!assetSceneLoad)
	{
		return pointer;
	}

	const GSceneLoad& sceneLoad = GBufferUtility::GetLoadBufferRoot<GSceneLoad>(*assetSceneLoad);

	TArrayScenePostLoadCallback arrayPostLoad;
	for (int index = 0; index < sceneLoad.GetSceneNodeLoadCount(); ++index)
	{
		TPointerSceneNode pointerChild = GSceneNode::Factory(
			*pointer,
			arrayPostLoad,
			sceneLoad.GetSceneNodeLoad(index)
			);

		if (!pointerChild)
		{
			continue;
		}
		pointer->mArraySceneNode.push_back(pointerChild);
	}

	FactoryPostLoad(arrayPostLoad, *pointer);

	return pointer;
}

/*static*/ void GScene::FactoryPostLoad(
		TArrayScenePostLoadCallback& in_arrayPostLoad,
		GScene& inout_scene
		)
{
	TArrayScenePostLoadCallback localArrayPostLoad(in_arrayPostLoad);
	while (localArrayPostLoad.size())
	{
		TArrayScenePostLoadCallback newArrayPostLoad;
		for (TArrayScenePostLoadCallback::iterator iterator = localArrayPostLoad.begin(); iterator != localArrayPostLoad.end(); ++iterator)
		{
			(*iterator).Run(
				inout_scene,
				newArrayPostLoad
				);
		}

		localArrayPostLoad = newArrayPostLoad;
	}

	return;
}

//constructor
GScene::GScene(
	TPointerResourceManager& inout_resourceManager,
	const TMapStringPairFactoryClientCallbackData& in_clientComponentFactory,
	TPointerSceneNodeFactory& inout_sceneNodeFactory
	)
	: mArraySceneNode()
	, mResourceManager(inout_resourceManager)
	, mSceneNodeFactory(inout_sceneNodeFactory)
	, mClientComponentFactory(in_clientComponentFactory)
	, mArrayPendingSceneNodeDelete()
	, mArrayFactoryData()
#ifdef DSC_DEBUG
	, mArrayDebugMatrix()
	, mAxisMesh()
	, mAxisMaterial()
#endif
{
#ifdef DSC_DEBUG
	sScene = this;

	if (inout_resourceManager)
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TAlphaBlend |
				GMaterialType::TState::TColourWrite
			),
			0,
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sShaderArrayUniformNameCount,
			sShaderArrayUniformName						
			);

		mAxisMaterial = inout_resourceManager->GetMaterialManual(
			"GSceneAxisMaterial",
			sVertexShader,
			sFragmentShader,
			renderMaterialLoad,
			true,
			false
			);
	}

	{
		TArrayStreamInfo arrayStreamLoad;
		GMeshStreamInfo::AppendStreamInfo(
			arrayStreamLoad,
			GMeshType::TStreamType::TFloat,
			3, 
			GMeshType::TStreamUsage::TPosition
			);
		GMeshStreamInfo::AppendStreamInfo(
			arrayStreamLoad,
			GMeshType::TStreamType::TFloat,
			4,
			GMeshType::TStreamUsage::TColour
			);

		TArrayByte arrayIndex;
		arrayIndex.push_back(0);
		arrayIndex.push_back(1);
		arrayIndex.push_back(3);
		arrayIndex.push_back(0);
		arrayIndex.push_back(2);
		arrayIndex.push_back(1);
		arrayIndex.push_back(0);
		arrayIndex.push_back(3);
		arrayIndex.push_back(2);
		arrayIndex.push_back(1);
		arrayIndex.push_back(2);
		arrayIndex.push_back(3);

		mAxisMesh.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangle,
			arrayIndex,
			4,
			arrayStreamLoad
			));


		mAxisMesh->GetFloatRef(0, 0, 0) = 0.0F;
		mAxisMesh->GetFloatRef(0, 0, 1) = 0.0F;
		mAxisMesh->GetFloatRef(0, 0, 2) = 0.0F;

		mAxisMesh->GetFloatRef(1, 0, 0) = 0.1F;
		mAxisMesh->GetFloatRef(1, 0, 1) = 0.0F;
		mAxisMesh->GetFloatRef(1, 0, 2) = 0.0F;

		mAxisMesh->GetFloatRef(2, 0, 0) = 0.0F;
		mAxisMesh->GetFloatRef(2, 0, 1) = 0.1F;
		mAxisMesh->GetFloatRef(2, 0, 2) = 0.0F;

		mAxisMesh->GetFloatRef(3, 0, 0) = 0.0F;
		mAxisMesh->GetFloatRef(3, 0, 1) = 0.0F;
		mAxisMesh->GetFloatRef(3, 0, 2) = 0.1F;

		mAxisMesh->GetFloatRef(0, 1, 0) = 1.0f;
		mAxisMesh->GetFloatRef(0, 1, 1) = 1.0f;
		mAxisMesh->GetFloatRef(0, 1, 2) = 1.0f;
		mAxisMesh->GetFloatRef(0, 1, 3) = 0.5f;

		mAxisMesh->GetFloatRef(1, 1, 0) = 1.0f;
		mAxisMesh->GetFloatRef(1, 1, 1) = 0.0f;
		mAxisMesh->GetFloatRef(1, 1, 2) = 0.0f;
		mAxisMesh->GetFloatRef(1, 1, 3) = 0.5f;

		mAxisMesh->GetFloatRef(2, 1, 0) = 0.0f;
		mAxisMesh->GetFloatRef(2, 1, 1) = 1.0f;
		mAxisMesh->GetFloatRef(2, 1, 2) = 0.0f;
		mAxisMesh->GetFloatRef(2, 1, 3) = 0.5f;

		mAxisMesh->GetFloatRef(3, 1, 0) = 0.0f;
		mAxisMesh->GetFloatRef(3, 1, 1) = 0.0f;
		mAxisMesh->GetFloatRef(3, 1, 2) = 1.0f;
		mAxisMesh->GetFloatRef(3, 1, 3) = 0.5f;
	}

#endif

	return;
}

GScene::~GScene()
{
#ifdef DSC_DEBUG
	sScene = NULL;
#endif

	return;
}

//public methods
GScene::TPointerSceneNodeComponentBase GScene::LoadComponent(
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const GSceneNodeComponentLoad& in_componentLoad
	)
{
	const TMapIntFactoryCallback& factoryMap = LocalGetComponentFactoryMap();
	TMapIntFactoryCallback::const_iterator iterator = factoryMap.find(in_componentLoad.GetType());
	if (iterator != factoryMap.end())
	{
		return (*(*iterator).second)(
			*this,
			inout_owner,
			inout_arrayPostLoadCallback,
			in_componentLoad.GetData()
			);
	}

	return TPointerSceneNodeComponentBase();
}

GScene::TPointerSceneNodeComponentBase GScene::ClientComponentFactory(
	const std::string& in_clientName,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TMapStringPairFactoryClientCallbackData::const_iterator iterator = mClientComponentFactory.find(in_clientName);
	if (iterator != mClientComponentFactory.end())
	{
		return (*(*iterator).second.first)(
			*this,
			inout_owner, 
			inout_arrayPostLoadCallback,
			(*iterator).second.second
			);
	}

	return TPointerSceneNodeComponentBase();
}

void GScene::SceneNodeFactory(
	const std::string& in_factoryStubName,
	const std::string& in_factoryData,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	mArrayFactoryData.push_back(in_factoryData);
	TPointerSceneNodeFactory pointerSceneNodeFactory = mSceneNodeFactory.lock();
	if (!pointerSceneNodeFactory)
	{
		return;
	}

	const GSceneNodeLoad* const sceneNodeLoad = pointerSceneNodeFactory->GetSceneNodeLoad(in_factoryStubName);
	if (!sceneNodeLoad)
	{
		return;
	}

	GSceneNode::FactoryLoadChildrenAndComponents(
		inout_owner,
		*this,
		inout_arrayPostLoadCallback,
		*sceneNodeLoad
		);

	mArrayFactoryData.pop_back();

	return;
}

void GScene::LoadSceneUnderNode(
	const std::string& in_fileName,
	TPointerSceneNode& inout_owner		
	)
{
	if (!inout_owner || in_fileName.empty())
	{
		return;
	}

	TPointerResourceManager pointerResourceManager = mResourceManager.lock();
	if (!pointerResourceManager)
	{
		return;
	}

	TPointerFile assetSceneLoad = pointerResourceManager->GetFileManager().LoadUnmanagedFile(in_fileName, true, true);
	if (!assetSceneLoad)
	{
		return;
	}

	const GSceneLoad& sceneLoad = GBufferUtility::GetLoadBufferRoot<GSceneLoad>(*assetSceneLoad);
	GSceneNode& parentSceneNode = *inout_owner;

	TArrayScenePostLoadCallback arrayPostLoad;
	for (int index = 0; index < sceneLoad.GetSceneNodeLoadCount(); ++index)
	{
		TPointerSceneNode pointerChild = GSceneNode::Factory(
			*this,
			arrayPostLoad,
			sceneLoad.GetSceneNodeLoad(index)
			);

		if (!pointerChild)
		{
			continue;
		}

		parentSceneNode.AddChildNode(pointerChild);
	}

	FactoryPostLoad(arrayPostLoad, *this);

	return;
}

//we queue up delete requests to told to do them
void GScene::SceneNodeRequestDelete(TPointerSceneNode& inout_sceneNode)
{
	if (inout_sceneNode)
	{
		mArrayPendingSceneNodeDelete.push_back(inout_sceneNode);
	}

	return;
}

void GScene::SceneNodeProcessDeleteQueue()
{
	for (TArrayWeakSceneNode::iterator iterator = mArrayPendingSceneNodeDelete.begin(); iterator != mArrayPendingSceneNodeDelete.end(); ++iterator)
	{
		TPointerSceneNode pointerSceneNode = (*iterator).lock();
		if (!pointerSceneNode)
		{
			continue;
		}

		GSceneNode* const parent = pointerSceneNode->GetParent();
		if (parent)
		{
			parent->RemoveChild(pointerSceneNode);
		}
		else 
		{
			TArrayPointerSceneNode::iterator iterator = mArraySceneNode.begin(); 
			while (iterator != mArraySceneNode.end())
			{
				if (pointerSceneNode == (*iterator))
				{
					iterator = mArraySceneNode.erase(iterator);
				}
				else
				{
					++iterator;
				}
			}
		}
	}

	mArrayPendingSceneNodeDelete.clear();

	return;
}

GScene::TPointerSceneNode GScene::CreateSceneNode(const std::string& in_sceneNodeName)
{
	TPointerSceneNode pointerSceneNode;
	pointerSceneNode.reset(new GSceneNode(in_sceneNodeName));
	return pointerSceneNode;
}

#ifdef DSC_DEBUG
void GScene::DebugDraw(
	GRender& in_renderer,
	GCamera& in_camera		
	)
{
	if (0 == mArrayDebugMatrix.size())
	{
		return;
	}

	TPointerMaterialInstance axisMaterial = mAxisMaterial.lock();
	if ((mAxisMesh) && (axisMaterial))
	{
		const GMesh& mesh = mAxisMesh->GetMesh();
		in_renderer.SetMaterial(*axisMaterial);
		in_renderer.SetCamera(in_camera);
		for (TArrayMatrix::const_iterator iterator = mArrayDebugMatrix.begin(); iterator != mArrayDebugMatrix.end(); ++iterator)
		{
			in_renderer.SetObjectTransform(*iterator);

			in_renderer.DrawMesh(
				mesh,
				mesh.GetVertexData()
				);
		}
	}

	return;
}
#endif

//public accessors
GScene::TPointerResourceManager GScene::GetResourceManager()
{
	return mResourceManager.lock();
}

const int GScene::GetSceneRootCount()const
{
	return mArraySceneNode.size();
}

const GScene::TPointerSceneNode& GScene::GetSceneRoot(const int in_index)const
{
	return mArraySceneNode[in_index];
}

GScene::TPointerSceneNode& GScene::GetSceneRoot(const int in_index)
{
	return mArraySceneNode[in_index];
}

void GScene::AddSceneRoot(TPointerSceneNode& inout_sceneNode)
{
	if (inout_sceneNode)
	{
		GASSERT(NULL == inout_sceneNode->GetParent(), "invlid state");
		mArraySceneNode.push_back(inout_sceneNode);
	}

	return;
}
