//
//  applicationtask.cpp
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "applicationtasktest.h"

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

static const char* const sVertexShader = "\n\
#ifdef GL_ES\n\
	#version 100\n\
	#define UNIFORM uniform\n\
	#define ATTRIBUTE attribute\n\
	#define VARYING varying\n\
#else\n\
	#version 330\n\
	#define UNIFORM uniform\n\
	#define ATTRIBUTE in\n\
	#define VARYING out\n\
#endif\n\
UNIFORM mat4 u_mvpMatrix;\n\
ATTRIBUTE vec4 a_position;\n\
ATTRIBUTE vec4 a_colour;\n\
VARYING vec4 v_color;\n\
void main()\n\
{\n\
	gl_Position = u_mvpMatrix * a_position;\n\
	v_color = a_colour;\n\
}\n\
";
static const char* const sFragmentShader = "\n\
#ifdef GL_ES\n\
	#version 100\n\
	#define LOWP lowp\n\
	#define MEDIUMP mediump\n\
	#define HIGHP highp\n\
	#define UNIFORM uniform\n\
	#define ATTRIBUTE attribute\n\
	#define VARYING varying\n\
	#define FRAGCOLOR gl_FragColor\n\
#else\n\
	#version 330\n\
	#define LOWP\n\
	#define MEDIUMP\n\
	#define HIGHP\n\
	#define UNIFORM uniform\n\
	#define ATTRIBUTE in\n\
	#define VARYING in\n\
	#define FRAGCOLOR FragColor\n\
out vec4 FragColor;\n\
#endif\n\
VARYING LOWP vec4 v_color;\n\
\n\
void main()\n\
{\n\
	//FRAGCOLOR = vec4(0.0, 1.0, 0.0, 1.0);\n\
	FRAGCOLOR = v_color;\n\
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


///////////////////////////////////////////////////////
//constructor
ApplicationTaskTest::ApplicationTaskTest()
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

	m_camera.reset(new GCamera(GApplicationWindowType::TOrientation::TTwelveOclock, GCamera::TType::TPerspective));

	m_cameraInput.reset(new GCameraInput(
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		sDefaultCameraBounds,
		-GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		GVector3Float(0.0F, 2.0F, 0.0F),
		-4.0F
		));


	//m_material
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
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

		m_material = mResourceManager->GetMaterialManual(
			"StateTest",
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
		m_mesh->GetFloatRef(2, 0, 1) = 3.0f;
		m_mesh->GetFloatRef(2, 0, 2) = 0.0f;

		m_mesh->GetFloatRef(3, 0, 0) = 0.0f;
		m_mesh->GetFloatRef(3, 0, 1) = 0.0f;
		m_mesh->GetFloatRef(3, 0, 2) = 1.0f;

		m_mesh->GetFloatRef(0, 1, 0) = 1.0f;
		m_mesh->GetFloatRef(0, 1, 1) = 1.0f;
		m_mesh->GetFloatRef(0, 1, 2) = 1.0f;
		m_mesh->GetFloatRef(0, 1, 3) = 1.0f;

		m_mesh->GetFloatRef(1, 1, 0) = 1.0f;
		m_mesh->GetFloatRef(1, 1, 1) = 0.0f;
		m_mesh->GetFloatRef(1, 1, 2) = 0.0f;
		m_mesh->GetFloatRef(1, 1, 3) = 1.0f;

		m_mesh->GetFloatRef(2, 1, 0) = 0.0f;
		m_mesh->GetFloatRef(2, 1, 1) = 1.0f;
		m_mesh->GetFloatRef(2, 1, 2) = 0.0f;
		m_mesh->GetFloatRef(2, 1, 3) = 1.0f;

		m_mesh->GetFloatRef(3, 1, 0) = 0.0f;
		m_mesh->GetFloatRef(3, 1, 1) = 0.0f;
		m_mesh->GetFloatRef(3, 1, 2) = 1.0f;
		m_mesh->GetFloatRef(3, 1, 3) = 1.0f;
	}

	TMapStringPairFactoryClientCallbackData clientComponentFactory;
	TPointerSceneNodeFactory sceneNodeFactory = GSceneNodeFactory::Factory(
		mResourceManager->GetFileManager(),
		"factoryscene.data"
		);

	m_scene = GScene::Factory(
		mResourceManager,
		clientComponentFactory,
		sceneNodeFactory,
		"scenetest.data"
		);

	return;
}

/*virtual*/ ApplicationTaskTest::~ApplicationTaskTest()
{
	mApplicationWindow.reset();
	return;
}

///////////////////////////////////////////////////////
//implement GApplicationTask
/*virtual*/ const GBOOL ApplicationTaskTest::OnTick(const GR32 in_timeDelta)
{
	//static int sLimit = 0;
	//sLimit += 1;
	//if (100 < sLimit)
	//	return false;

	GBOOL result = GTRUE;

    GR32 localTick = in_timeDelta;
    //GR32 localTick = 0.0F;

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
        
#ifdef PRINT_FPS
		if (in_timeDelta)
		{
            static bool s_first = true;
            static GR32 s_averageTick = 0.0F;
            if (s_first)
            {
                s_first = false;
                s_averageTick = 1.0F / in_timeDelta;
            }
            else
            {
                s_averageTick = ((1.0F / in_timeDelta) * 0.05F) + (s_averageTick * 0.95F);
            }
            
			std::string text("FPS: ");
			text += GStringUtility::PrintMethod(s_averageTick);
			mApplicationWindow->PutText(text);
		}
#endif
	}

	return result;
}

/*virtual*/ void ApplicationTaskTest::OnSetActive(const GBOOL in_active)
{
	return;
}

/*virtual*/ GVOID ApplicationTaskTest::Input(const GInput& in_input)
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

GVOID ApplicationTaskTest::Tick(const GR32 in_timeDelta)
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

		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, in_timeDelta);
	}
}

GVOID ApplicationTaskTest::Draw(GRender& inout_render)
{
	inout_render.RenderStart(true, GColour4Float::sBlack, true);

	boost::shared_ptr<GMaterialInstance> materialInstance = m_material.lock();
	if (materialInstance)
	{
		inout_render.SetMaterial(*materialInstance);
	}
	if (m_cameraInput)
	{
		inout_render.SetCamera(m_cameraInput->GetCamera());
	}
	inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	if (m_mesh)
	{
		inout_render.DrawMesh(m_mesh->GetMesh(), m_mesh->GetMesh().GetVertexData());
    }

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

GVOID ApplicationTaskTest::SetScreen(
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
