//
//  giphonerender.mm
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "giphonerender.h"

#include "gassert.h"
#include "giphonerenderopengles1.h"
#include "giphonerenderopengles2.h"

#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <QuartzCore/QuartzCore.h>

#define USE_SHADER

struct TIPhoneOpenGLES
{
	enum Type
	{
		TOpenGLES1 = 0,
		TOpenGLES2,
		TCount
	};
};

//constructor
GIPhoneRender::GIPhoneRender(const GBOOL in_depth, const GBOOL in_stencil)
	: mType(TIPhoneOpenGLES::TCount)
	, mPointerRenderOpenGLES1()
	, mRenderOpenGLES1(GNULL)
	, mPointerRenderOpenGLES2()
	, mRenderOpenGLES2(GNULL)
{	
#ifdef USE_SHADER
	//opengles 2
	mPointerRenderOpenGLES2 = GIPhoneRenderOpenGLES2::Factory(in_depth, in_stencil);
	if (GNULL != mPointerRenderOpenGLES2)
	{
		mType = TIPhoneOpenGLES::TOpenGLES2;
		mRenderOpenGLES2 = mPointerRenderOpenGLES2.get();
	}
	else
#endif	
	{
		mPointerRenderOpenGLES1 = GIPhoneRenderOpenGLES1::Factory(in_depth, in_stencil);
		if (GNULL != mPointerRenderOpenGLES1)
		{
			mType = TIPhoneOpenGLES::TOpenGLES1;
			mRenderOpenGLES1 = mPointerRenderOpenGLES1.get();
		}
	}

    return;
}

GIPhoneRender::~GIPhoneRender()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
const GBOOL GIPhoneRender::GetSupportShader()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetSupportShader();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetSupportShader();
	}

	return 0;
}
const GBOOL GIPhoneRender::GetSupportStencil()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetSupportStencil();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetSupportStencil();
	}

	return 0;
}

const GU32 GIPhoneRender::GetMaximumTextureDimention()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetMaximumTextureDimention();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetMaximumTextureDimention();
	}

	return 0;
}

const GU32 GIPhoneRender::GetRenderBufferWidth()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetRenderBufferWidth();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetRenderBufferWidth();
	}

	return 0;
}

const GU32 GIPhoneRender::GetRenderBufferHeight()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetRenderBufferHeight();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetRenderBufferHeight();
	}

	return 0;
}

const GU32 GIPhoneRender::GetDefaultFrameBufferObject()const
{	
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->GetDefaultFrameBufferObject();
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->GetDefaultFrameBufferObject();
	}

	return 0;
}

const GBOOL GIPhoneRender::Resize(CAEAGLLayer* in_layer)
{	
	GBOOL result = GFALSE;
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		result = mRenderOpenGLES1->Resize(in_layer);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		result = mRenderOpenGLES2->Resize(in_layer);
		break;
	}

	return result;
}

GVOID GIPhoneRender::RenderStart(
	const GBOOL in_clearColour, 
	const GColour4Float& in_clearColourValue,
	const GBOOL in_clearDepth, 
	const GR32 in_clearDepthValue,
	const GBOOL in_clearStencil, 
	const GU32 in_clearStencilValue
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->RenderStart(
			in_clearColour,
			in_clearColourValue,
			in_clearDepth,
			in_clearDepthValue,
			in_clearStencil,
			in_clearStencilValue
			);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->RenderStart(
			in_clearColour,
			in_clearColourValue,
			in_clearDepth,
			in_clearDepthValue,
			in_clearStencil,
			in_clearStencilValue
			);
		break;
	}
	return;
}

GVOID GIPhoneRender::Present()
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->Present();
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->Present();
		break;
	}
	return;
}

GVOID GIPhoneRender::SetCamera(GCamera& in_camera)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->SetCamera(in_camera);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetCamera(in_camera);
		break;
	}
	return;
}

GVOID GIPhoneRender::SetObjectTransform(const GMatrix16Float& in_matrix)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->SetObjectTransform(in_matrix);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetObjectTransform(in_matrix);
		break;
	}
	return;
}

GVOID GIPhoneRender::SetMaterial(
	const GMaterialInstance& in_material,
	const GBOOL in_force
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->SetMaterial(in_material, in_force);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetMaterial(in_material, in_force);
		break;
	}
	return;
}

GVOID GIPhoneRender::SetViewport(
	const GS32 in_lowX,
	const GS32 in_lowY,
	const GS32 in_highX,
	const GS32 in_highY
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->SetViewport(in_lowX, in_lowY, in_highX, in_highY);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetViewport(in_lowX, in_lowY, in_highX, in_highY);
		break;
	}
	return;
}

GVOID GIPhoneRender::DrawMesh(
	const GMesh& in_mesh,
	const GVOID* const in_vertexData
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->DrawMesh(in_mesh, in_vertexData);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->DrawMesh(in_mesh, in_vertexData);
		break;
	}
	return;
}

const GU32 GIPhoneRender::BindTexture(const GTexture& in_texture)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->BindTexture(in_texture);
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->BindTexture(in_texture);
	}
	return 0;
}

GVOID GIPhoneRender::UnbindTexture(const GU32 in_textureHandle)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->UnbindTexture(in_textureHandle);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->UnbindTexture(in_textureHandle);
		break;
	}
	return;
}

GVOID GIPhoneRender::GenerateMipmap(const GU32 in_textureHandle)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->GenerateMipmap(in_textureHandle);
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->GenerateMipmap(in_textureHandle);
		break;
	}
	return;
}

const GBOOL GIPhoneRender::LoadShader(
    GU32& out_programIndex,
    TMapStringInt & out_mapUniformNameIndex,
    const std::string& in_vertexShaderSource,
    const std::string& in_fragmentShaderSource,
    const TArrayString& in_arrayAttributeName,
    const TArrayString& in_arrayUniformName
    )
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->LoadShader(
            out_programIndex,
            out_mapUniformNameIndex,
            in_vertexShaderSource,
            in_fragmentShaderSource,
            in_arrayAttributeName,
            in_arrayUniformName);
	}
	return GFALSE;    
}
void GIPhoneRender::ReleaseShader(const GU32 in_programIndex)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->ReleaseShader(in_programIndex);
		break;
	}
	return;    
}

void GIPhoneRender::SetShaderUniform(const std::string& in_key, const GR32 in_value)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetShaderUniform(in_key, in_value);
		break;
	}
	return;    
}

const GU32 GIPhoneRender::CreateRenderTextureTarget(
	const GU32 in_textureHandel,
	const GU32 in_width,
	const GU32 in_height,
	const GBOOL in_depth,
	const GBOOL in_stencil
	//RGBA? type
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		return mRenderOpenGLES1->CreateRenderTextureTarget(in_textureHandel, in_width, in_height, in_depth, in_stencil);
	case TIPhoneOpenGLES::TOpenGLES2:
		return mRenderOpenGLES2->CreateRenderTextureTarget(in_textureHandel, in_width, in_height, in_depth, in_stencil);
	}
	return 0;
}

GVOID GIPhoneRender::SetRenderTarget(
	const GU32 in_frameBufferObjectId
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->SetRenderTarget(in_frameBufferObjectId);
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->SetRenderTarget(in_frameBufferObjectId);
	}
	return;
}

GVOID GIPhoneRender::DestroyRenderTextureTarget(
	const GU32 in_frameBufferObjectId
	)
{
	switch(mType)
	{
	default:
		break;
	case TIPhoneOpenGLES::TOpenGLES1:
		mRenderOpenGLES1->DestroyRenderTextureTarget(in_frameBufferObjectId);
	case TIPhoneOpenGLES::TOpenGLES2:
		mRenderOpenGLES2->DestroyRenderTextureTarget(in_frameBufferObjectId);
	}
	return;
}