//
//  applicationtask.cpp
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
	#include <vld.h>
#include "ApplicationTaskTestParticle.h"

#include <gapplicationwindow.h>
#include <grender.h>
#include <gresourcemanager.h>
#include <gfilesystem.h>
#include <gcamera.h>
#include <gmath.h>
#include <GStringUtility.h>
#include <GScratchPad.h>
#include <GGuiManager.h>
#include <GMissionMaster.h>
#include <GView.h>
#include <GResourceManager.h>
#include <GMeshStreamInfo.h>
#include <GMeshManual.h>
#include <GMaterialType.h>
#include <GStringUtility.h>
#include <GMaterial.h>
#include <GMaterialInstance.h>
#include <GCamera.h>
#include <GRender.h>
#include <GMesh.h>
#include <GScene.h>
#include <GSceneNodeFactory.h>
#include <GSceneVisitorRender.h>
#include <GCameraInput.h>
#include <GInput.h>
#include <GInputButton.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentClient.h>
#include <GSceneNodeComponentVisual.h>
#include <GRenderBlur.h>
#include <GRenderWash.h>
#include <GColour4Float.h>
#include <GColour4Byte.h>
#include <GRenderShadow.h>
#include <GParticleManager.h>
#include <GSceneNodeComponentParticleLoad.h>
#include <GScenePostLoadCallback.h>
#include <GSceneNodeComponentParticle.h>

#ifdef IPHONE
	#include <GIPhoneView.h>
#elif WIN32
	#include <GWin32View.h>
#endif

typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;
typedef std::vector<GU8> TArrayByte;


typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	boost::shared_ptr< GSceneNode >& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	);
typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;

typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;

static const float sDefaultCameraPerspectiveNear = 0.1F;
static const float sDefaultCameraPerspectiveFar = 100.0F;
static const float sDefaultCameraBounds = 0.05F;

///////////////////////////////////////////////////////
//constructor
ApplicationTaskTestParticle::ApplicationTaskTestParticle()
: mApplicationWindow()
, mViewRender()
, mRender()
, mFileSystem()
, mResourceManager()
{
	mApplicationWindow.reset(new GApplicationWindow);

	mViewRender = mApplicationWindow->CreateViewRender(mRender, false, true, false, 1.0F);
	mApplicationWindow->PushViewFront(mViewRender);

	mFileSystem.reset(new GFileSystem);	
	mScratchPad.reset(new GScratchPad);
	mResourceManager.reset(new GResourceManager(mFileSystem, mRender));
		
	mApplicationWindow->Show();

	m_cameraInput.reset(new GCameraInput(
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		sDefaultCameraBounds,
		-GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		GVector3Float(0.0F, 0.0F, 0.0F),
		-4.0F
		));

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
			2,
			GMeshType::TStreamUsage::TUv0
			);

		TArrayByte arrayVertexIndex;
		arrayVertexIndex.push_back(0);
		arrayVertexIndex.push_back(1);
		arrayVertexIndex.push_back(2);

		arrayVertexIndex.push_back(0);
		arrayVertexIndex.push_back(2);
		arrayVertexIndex.push_back(3);

		arrayVertexIndex.push_back(0);
		arrayVertexIndex.push_back(3);
		arrayVertexIndex.push_back(1);

		arrayVertexIndex.push_back(1);
		arrayVertexIndex.push_back(3);
		arrayVertexIndex.push_back(2);

		m_mesh.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangle,
			arrayVertexIndex,
			4,
			arrayStreamLoad
			));

		//GetFloatRef(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)
		m_mesh->GetFloatRef(0, 0, 0) = 0.0f;
		m_mesh->GetFloatRef(0, 0, 1) = 0.0f;
		m_mesh->GetFloatRef(0, 0, 2) = 0.0f;

		m_mesh->GetFloatRef(1, 0, 0) = 1.0f;
		m_mesh->GetFloatRef(1, 0, 1) = 0.0f;
		m_mesh->GetFloatRef(1, 0, 2) = 0.0f;

		m_mesh->GetFloatRef(2, 0, 0) = 0.0f;
		m_mesh->GetFloatRef(2, 0, 1) = 2.0f;
		m_mesh->GetFloatRef(2, 0, 2) = 0.0f;

		m_mesh->GetFloatRef(3, 0, 0) = 0.0f;
		m_mesh->GetFloatRef(3, 0, 1) = 0.0f;
		m_mesh->GetFloatRef(3, 0, 2) = 1.0f;

		m_mesh->GetFloatRef(0, 1, 0) = 1.0f;
		m_mesh->GetFloatRef(0, 1, 1) = 1.0f;

		m_mesh->GetFloatRef(1, 1, 0) = 1.0f;
		m_mesh->GetFloatRef(1, 1, 1) = 0.0f;

		m_mesh->GetFloatRef(2, 1, 0) = 0.0f;
		m_mesh->GetFloatRef(2, 1, 1) = 1.0f;

		m_mesh->GetFloatRef(3, 1, 0) = 0.0f;
		m_mesh->GetFloatRef(3, 1, 1) = 0.0f;
	}

	TMapStringPairFactoryClientCallbackData clientComponentFactory;
	TPointerSceneNodeFactory sceneNodeFactory;
	mParticleManager = GParticleManager::Factory(
		mResourceManager->GetFileManager(),
		"particlemanager.data"
		);

#if 0
	m_scene = GScene::Factory(mResourceManager, clientComponentFactory, sceneNodeFactory, "scenetest.data", particleManager);
#else
	m_scene.reset(new GScene(
		mResourceManager,
		clientComponentFactory,
		sceneNodeFactory,
		mParticleManager
		));

	if (m_scene)
	{
		TArrayScenePostLoadCallback arrayPostLoad;

		m_sceneNodeParticle.reset(new GSceneNode("sceneNodeParticle"));
		GMatrix16Float matrix(GMatrix16Float::sIdentity);
		matrix.SetPosition(GVector3Float(1.0F, 1.0F, 1.0F));
		m_sceneNodeParticle->SetWorldTransform(matrix);

		boost::shared_ptr<GSceneNodeComponentBase> pComponent = GSceneNodeComponentParticle::FactoryManual(
			*m_scene, 
			m_sceneNodeParticle, 
			"material_particle00a.data",
			2.0F,
			1000,
			1000,
			2
			);
		m_sceneNodeParticle->AddComponent(pComponent);
		m_sceneNodeComponentParticle = boost::dynamic_pointer_cast< GSceneNodeComponentParticle >(pComponent);

		boost::shared_ptr< GSceneNode > sceneNodeRoot(new GSceneNode("root"));
		sceneNodeRoot->AddChildNode(m_sceneNodeParticle);
		m_scene->AddSceneRoot(sceneNodeRoot);

		GScene::FactoryPostLoad(arrayPostLoad, *m_scene);
	}
#endif
	m_material = mResourceManager->GetMaterialInstance(
		"material_particle00a.data",
		true,
		false
		);

	if (m_sceneNodeComponentParticle)
	{
		m_sceneNodeComponentParticle->RequestParticle(
			1, 
			GVector3Float::sZero,
			GVector3Float::sUnitX,
			GVector3Float::sUnitY,
			10000000.0,
			1.0F,
			1.0F,
			0.5F,
			0.5F
			);

		m_sceneNodeComponentParticle->RequestEmittor(1, 0.0F);
	}


	return;
}

/*virtual*/ ApplicationTaskTestParticle::~ApplicationTaskTestParticle()
{
	mApplicationWindow.reset();
	return;
}

///////////////////////////////////////////////////////
//implement GApplicationTask
/*virtual*/ const GBOOL ApplicationTaskTestParticle::OnTick(const GR32 in_timeDelta)
{
#ifdef AUTO_BAIL
	static int sLimit = 0;
	sLimit += 1;
	if (100 < sLimit)
	{
		VLDReportLeaks ();
	 	return false;
	}
#endif

	GBOOL result = GTRUE;

    GR32 localTick = in_timeDelta;

	if (mApplicationWindow)
	{
		mApplicationWindow->PreTick(localTick);

		result = mApplicationWindow->Tick(localTick);

		if (result && mRender)
		{
			GApplicationWindowType::TOrientation::TEnum screenOrientation = GApplicationWindowType::TOrientation::TCount;
			int width = 10;
			int height = 10;
			mApplicationWindow->GetScreenDimention(screenOrientation, width, height);

			SetScreen(width, height, screenOrientation);
			Input(mApplicationWindow->GetInput());
    		Tick(localTick);
    		Draw(*mRender);
		}
           
		mApplicationWindow->PostTick(localTick);
	}

	return result;
}

/*virtual*/ void ApplicationTaskTestParticle::OnSetActive(const GBOOL in_active)
{
	return;
}

/*virtual*/ GVOID ApplicationTaskTestParticle::Input(const GInput& in_input)
{
	if (m_cameraInput)
	{
		if ((0 < in_input.GetButtonCount()) && 
            in_input.GetButton(0).GetDown())
		{
			m_cameraInput->Reset(
				-GVector3Float::sUnitZ,
				GVector3Float::sUnitY,
				GVector3Float::sZero,
				-5.0F
				);
		}

		m_cameraInput->Input(in_input);
	}
}

GVOID ApplicationTaskTestParticle::Tick(const GR32 in_timeDelta)
{
	if (m_cameraInput)
	{
		m_cameraInput->Tick(in_timeDelta);
	}

	if (m_scene)
	{
		GScene& scene = *m_scene;

		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, in_timeDelta);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(scene);
		GSceneVisitorAllTick<GSceneNodeComponentParticle>::Run(scene, in_timeDelta);
		GSceneVisitorAllCamera<GSceneNodeComponentParticle>::Run(scene, m_cameraInput->GetCamera());
		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}
}

GVOID ApplicationTaskTestParticle::Draw(GRender& inout_render)
{
	inout_render.RenderStart(true, GColour4Float::sBlack, true);

	//boost::shared_ptr<GMaterialInstance> materialInstance = m_material.lock();
	//if (materialInstance)
	//{
	//	inout_render.SetMaterial(*materialInstance);
	//}
	if (m_cameraInput)
	{
		inout_render.SetCamera(m_cameraInput->GetCamera());
	}
	//inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	//if (m_mesh)
	//{
	//	inout_render.DrawMesh(m_mesh->GetMesh(), m_mesh->GetMesh().GetVertexData());
 //   }

	if (m_cameraInput && m_scene)
	{
		GSceneVisitorRender::Run(
			*m_scene, 
			inout_render, 
			m_cameraInput->GetCamera()
			);

#ifdef DSC_DEBUG
		m_scene->DebugDraw(
			inout_render, 
			m_cameraInput->GetCamera()			
			);
		m_scene->GetArrayDebugMatrix().clear();
#endif
	}

	inout_render.Present();
}

GVOID ApplicationTaskTestParticle::SetScreen(
	const GS32 in_screenWidth, 
	const GS32 in_screenHeight, 
	const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
	)
{
	m_cameraInput->SetScreen(
		in_screenWidth, 
		in_screenHeight, 
		in_screenOrientation	
		);
}
