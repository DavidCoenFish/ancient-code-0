//
//  GRenderShadow.cpp
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GRenderShadow.h"

#include "GRender.h"
#include "GCamera.h"
#include "GMeshStreamInfo.h"
#include "GMeshManual.h"
#include "GSceneVisitorRenderShadow.h"
#include "GMaterialType.h"
#include "GStringUtility.h"
#include "GCommon.h"
#include "GMaterial.h"
#include "GMaterialInstance.h"
#include "GResourceManager.h"

typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

static const char* const sVertexShader = "\n\
#version 100\n\
uniform mat4 u_mvpMatrix;\n\
attribute vec4 a_position;\n\
void main()\n\
{\n\
   gl_Position = u_mvpMatrix * a_position;\n\
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
\n\
void main()\n\
{\n\
	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n\
}\n\
";
static const char* sShaderArrayAttributeName[] = {
	"a_position"
};
static const int sShaderArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayAttributeName);
static const char* sShaderArrayUniformName[] = {
	"u_mvpMatrix"
};
static const int sShaderArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayUniformName);


static const char* const sVertexShaderShadow = "\n\
#version 100\n\
uniform mat4 u_mvpMatrix;\n\
attribute vec4 a_position;\n\
void main()\n\
{\n\
   gl_Position = u_mvpMatrix * a_position;\n\
}\n\
";
static const char* const sFragmentShaderShadow = "\n\
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
uniform LOWP vec4 u_colour;\n\
\n\
void main()\n\
{\n\
gl_FragColor = u_colour;\n\
}\n\
";
static const char* sShaderShadowArrayAttributeName[] = {
	"a_position"
};
static const int sShaderShadowArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sShaderShadowArrayAttributeName);
static const char* sShaderShadowArrayUniformName[] = {
	"u_mvpMatrix",
	"u_colour"
};
static const int sShaderShadowArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sShaderShadowArrayUniformName);


//constructor
GRenderShadow::GRenderShadow(
	GResourceManager& inout_resourceManager,
	const GVector3Float& in_shadowDirection,
	const GColour4Float& in_shadowColour
	)
	: mShadowDirection(in_shadowDirection)
	//, mShadowColour(in_shadowColour)
	, mMaterialShadowStencilFront()
	, mMaterialShadowStencilBack()
	, mMaterialShadow()
	, mMeshShadow()
	, mOrthoCamera()
{
	mOrthoCamera.reset(new GCamera);

	//mMaterialShadowStencilFront
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TStencilShadowFront |
				GMaterialType::TState::TDepthRead |				
				GMaterialType::TState::TFaceCull |
				GMaterialType::TState::TFaceCW
			),
			0,
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sShaderArrayUniformNameCount,
			sShaderArrayUniformName						
			);

		mMaterialShadowStencilFront = inout_resourceManager.GetMaterialManual(
			"GRenderShadowStencilFront",
			sVertexShader,
			sFragmentShader,
			renderMaterialLoad,
			true,
			false
			);
	}

	//mMaterialShadowStencilBack
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TStencilShadowBack |
				GMaterialType::TState::TDepthRead |				
				GMaterialType::TState::TFaceCull
			),
			0,
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sShaderArrayUniformNameCount,
			sShaderArrayUniformName						
			);

		mMaterialShadowStencilBack = inout_resourceManager.GetMaterialManual(
			"GRenderShadowStencilBack",
			sVertexShader,
			sFragmentShader,
			renderMaterialLoad,
			true,
			false
			);
	}
	//mMaterialShadow
	{
		GMaterial renderMaterialLoad(
			in_shadowColour,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TAlphaBlend |
				GMaterialType::TState::TColourWrite |
				GMaterialType::TState::TFaceCull |
				GMaterialType::TState::TFaceCW |
				GMaterialType::TState::TStencilShadow
			),
			0,
			0,
			0,
			sShaderShadowArrayAttributeNameCount,
			sShaderShadowArrayAttributeName,
			sShaderShadowArrayUniformNameCount,
			sShaderShadowArrayUniformName						
			);

		mMaterialShadow = inout_resourceManager.GetMaterialManual(
			"GRenderShadow",
			sVertexShaderShadow,
			sFragmentShaderShadow,
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
			2, 
			GMeshType::TStreamUsage::TPosition
			);
		mMeshShadow.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangleStrip,
			4,
			arrayStreamLoad
			));

		mMeshShadow->GetFloatRef(0, 0, 0) = -1.0F;
		mMeshShadow->GetFloatRef(0, 0, 1) = -1.0F;

		mMeshShadow->GetFloatRef(1, 0, 0) = -1.0F;
		mMeshShadow->GetFloatRef(1, 0, 1) = 1.0F;

		mMeshShadow->GetFloatRef(2, 0, 0) = 1.0F;
		mMeshShadow->GetFloatRef(2, 0, 1) = -1.0F;

		mMeshShadow->GetFloatRef(3, 0, 0) = 1.0F;
		mMeshShadow->GetFloatRef(3, 0, 1) = 1.0F;
	}

	return;
}

GRenderShadow::~GRenderShadow()
{
	return;
}

//public methods
void GRenderShadow::RenderShadows(
	GRender& inout_render,
	GScene& inout_scene,
	GCamera& inout_sceneCamera
	)
{
	TPointerMaterialInstance materialShadowStencilFront = mMaterialShadowStencilFront.lock();
	TPointerMaterialInstance materialShadowStencilBack = mMaterialShadowStencilBack.lock();
	TPointerMaterialInstance materialShadow = mMaterialShadow.lock();

	if (mMeshShadow && 
		mOrthoCamera &&
		materialShadow &&
		materialShadowStencilFront &&
		materialShadowStencilBack
		)
	{
		GSceneVisitorRenderShadow::Run(
			inout_scene,
			inout_render,
			mShadowDirection,
			1.0F,
			*materialShadowStencilFront,
			*materialShadowStencilBack,
			*materialShadow,
			*mMeshShadow,
			inout_sceneCamera,
			*mOrthoCamera
			);
	}

	return;
}