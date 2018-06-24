//
//  grender.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "grender.h"
#include "gassert.h"

#ifdef IPHONE
	#include "giphonerender.h"
#elif WIN32
	#include "gwin32windowopengl.h"
#endif

///////////////////////////////////////////////////////
//constructor
GRender::GRender(
	GRenderImplementation* inout_pointerRenderImplementation //WARN: raw pointer, avoiding cost of locking weak pointer, WE DO NOT OWN IT
	)
	: m_renderImplementation(inout_pointerRenderImplementation)
{
	GASSERT(GNULL != m_renderImplementation, "invalid case");
	return;
}

GRender::~GRender()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
const GBOOL GRender::GetSupportShader()const
{
	return m_renderImplementation->GetSupportShader();
}

const GBOOL GRender::GetSupportStencil()const
{
	return m_renderImplementation->GetSupportStencil();
}

const GU32 GRender::GetMaximumTextureDimention()const
{
	return m_renderImplementation->GetMaximumTextureDimention();
}
const GU32 GRender::GetRenderBufferWidth()const
{
	return m_renderImplementation->GetRenderBufferWidth();
}
const GU32 GRender::GetRenderBufferHeight()const
{
	return m_renderImplementation->GetRenderBufferHeight();
}
const GU32 GRender::GetDefaultFrameBufferObject()const
{
    return m_renderImplementation->GetDefaultFrameBufferObject();
}

GVOID GRender::RenderStart(
	const GBOOL in_clearColour, 
	const GColour4Float& in_clearColourValue,
	const GBOOL in_clearDepth, 
	const GR32 in_clearDepthValue,
	const GBOOL in_clearStencil, 
	const GU32 in_clearStencilValue
	)
{
	m_renderImplementation->RenderStart(
		in_clearColour, 
		in_clearColourValue,
		in_clearDepth, 
		in_clearDepthValue,
		in_clearStencil, 
		in_clearStencilValue
		);
	return;
}

GVOID GRender::Present()
{
	m_renderImplementation->Present();
	return;
}

void GRender::SetCamera(GCamera& in_camera)
{
	m_renderImplementation->SetCamera(in_camera);
	return;
}

void GRender::SetObjectTransform(const GMatrix16Float& in_matrix)
{
	m_renderImplementation->SetObjectTransform(in_matrix);
	return;
}

void GRender::SetMaterial(const GMaterialInstance& in_material)
{
	m_renderImplementation->SetMaterial(in_material, GFALSE);
	return;
}

GVOID GRender::SetViewport(
	const GS32 in_lowX,
	const GS32 in_lowY,
	const GS32 in_highX,
	const GS32 in_highY
	)
{
	m_renderImplementation->SetViewport(
		in_lowX,
		in_lowY,
		in_highX,
		in_highY
		);
	return;
}

GVOID GRender::DrawMesh(
	const GMesh& in_mesh,
	const GVOID* const in_vertexData
	)
{
	m_renderImplementation->DrawMesh(
		in_mesh,
		in_vertexData
		);
	return;
}

const GU32 GRender::BindTexture(const GTexture& in_texture)
{
	return m_renderImplementation->BindTexture(in_texture);
}

GVOID GRender::UnbindTexture(const GU32 in_textureHandle)
{
	m_renderImplementation->UnbindTexture(in_textureHandle);
	return;
}
GVOID GRender::GenerateMipmap(const GU32 in_textureHandle)
{
	m_renderImplementation->GenerateMipmap(in_textureHandle);
	return;
}


const GBOOL GRender::LoadShader(
	GU32& out_programIndex,
	TMapStringInt & out_mapUniformNameIndex,
	const std::string& in_vertexShaderSource,
	const std::string& in_fragmentShaderSource,
	const TArrayString& in_arrayAttributeName,
	const TArrayString& in_arrayUniformName
	)
{
	return m_renderImplementation->LoadShader(
		out_programIndex,
		out_mapUniformNameIndex,
		in_vertexShaderSource,
		in_fragmentShaderSource,
		in_arrayAttributeName,
		in_arrayUniformName);
}

void GRender::ReleaseShader(const GU32 in_programIndex)
{
	m_renderImplementation->ReleaseShader(in_programIndex);
	return;
}
void GRender::SetShaderUniform(const std::string& in_key, const GR32 in_value)
{
	m_renderImplementation->SetShaderUniform(in_key, in_value);
	return;
}


const GU32 GRender::CreateRenderTextureTarget(
	const GU32 in_textureHandel,
	const GU32 in_width,
	const GU32 in_height,
	const GBOOL in_depth,
	const GBOOL in_stencil
	//RGBA? type
	)
{
	return m_renderImplementation->CreateRenderTextureTarget(
		in_textureHandel,
		in_width,
		in_height,
		in_depth,
		in_stencil
		);
}

GVOID GRender::SetRenderTarget(
	const GU32 in_frameBufferObjectId
	)
{
	m_renderImplementation->SetRenderTarget(in_frameBufferObjectId);
	return;
}

GVOID GRender::DestroyRenderTextureTarget(
	const GU32 in_frameBufferObjectId
	)
{
	m_renderImplementation->DestroyRenderTextureTarget(in_frameBufferObjectId);
	return;
}