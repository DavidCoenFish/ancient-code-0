//
//  GRenderWash.cpp
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GRenderWash.h"

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
#include "GMesh.h"

typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

static const char* const sVertexShader = "\n\
#version 100\n\
attribute vec4 a_position;\n\
void main()\n\
{\n\
   gl_Position = a_position;\n\
   gl_Position.z = 1.0;\n\
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
uniform LOWP vec4 u_colour;\n\
\n\
void main()\n\
{\n\
	gl_FragColor = u_colour;\n\
}\n\
";
static const char* sShaderArrayAttributeName[] = {
	"a_position"
};
static const int sShaderArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayAttributeName);
static const char* sShaderArrayUniformName[] = {
	"u_colour"
};
static const int sShaderArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayUniformName);

//constructor
GRenderWash::GRenderWash(
	GResourceManager& inout_resourceManager,
	const GColour4Float& in_colour,
	const GBOOL in_alpha,
	const GBOOL in_clearDepth
	)
	: m_colour(in_colour)
	, m_material()
	, m_mesh()
	, m_orthoCamera()
{
	m_orthoCamera.reset(new GCamera);

	//mMaterial
	{
		GU32 flag = (	
			GMaterialType::TState::TFaceCull
			| GMaterialType::TState::TFaceCW
			| GMaterialType::TState::TColourWrite
			);
		if (in_alpha)
		{
			flag |= GMaterialType::TState::TAlphaBlend;
		}
		if (in_clearDepth)
		{
			flag |= GMaterialType::TState::TDepthWrite;
		}

		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			flag,
			0,
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sShaderArrayUniformNameCount,
			sShaderArrayUniformName						
			);

		m_material = inout_resourceManager.GetMaterialManual(
			"GRenderWash",
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
			2, 
			GMeshType::TStreamUsage::TPosition
			);
		m_mesh.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangleStrip,
			4,
			arrayStreamLoad
			));

		m_mesh->GetFloatRef(0, 0, 0) = -1.0F;
		m_mesh->GetFloatRef(0, 0, 1) = -1.0F;

		m_mesh->GetFloatRef(1, 0, 0) = -1.0F;
		m_mesh->GetFloatRef(1, 0, 1) = 1.0F;

		m_mesh->GetFloatRef(2, 0, 0) = 1.0F;
		m_mesh->GetFloatRef(2, 0, 1) = -1.0F;

		m_mesh->GetFloatRef(3, 0, 0) = 1.0F;
		m_mesh->GetFloatRef(3, 0, 1) = 1.0F;
	}

	return;
}

GRenderWash::~GRenderWash()
{
	return;
}

//public methods
void GRenderWash::Draw(
	GRender& inout_render
	)
{
	TPointerMaterialInstance material = m_material.lock();

	if (m_mesh && 
		m_orthoCamera &&
		material 
		)
	{
		material->SetColour(m_colour);
		inout_render.SetCamera(*m_orthoCamera);
		inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
		inout_render.SetMaterial(*material);
		inout_render.DrawMesh(
			m_mesh->GetMesh(),
			m_mesh->GetMesh().GetVertexData()
			);
	}

	return;
}