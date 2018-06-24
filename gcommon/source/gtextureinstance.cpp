//
//  gtextureinstance.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gtextureinstance.h"

#include "gtexture.h"
#include "gtexturetype.h"
#include "gbuffer.h"
#include "gbufferutility.h"
#include "grender.h"
#include "GTextureManual.h"

typedef boost::shared_ptr<GBuffer> TPointerBuffer;
typedef boost::shared_ptr<GRender> TPointerRender;

////////////////////////////////////////////////////
//static local methods
static const GU32 LocalBindTexture(
	TPointerBuffer& inout_buffer,
	TPointerRender& inout_render
	)
{
	if (!inout_buffer || !inout_render)
	{
		return 0;
	}
	const GTexture& texture = GBufferUtility::GetLoadBufferRoot<GTexture>(*inout_buffer);
	const GU32 textureHandel = inout_render->BindTexture(texture);
	return textureHandel;
}

////////////////////////////////////////////////////
//static public methods
/*static*/ GTextureInstance::TPointerTextureInstance GTextureInstance::Factory(
	TPointerBuffer& inout_buffer,
	TPointerRender& inout_render,
	const bool in_bindTexture,
	const bool in_allowRuntimeBindTexture
	)
{
	TPointerTextureInstance pointer;
	pointer.reset(new GTextureInstance(inout_render));
	if (pointer)
	{
		pointer->Replace(inout_buffer, in_bindTexture, in_allowRuntimeBindTexture);
	}

	return pointer;
}

/*static*/ GTextureInstance::TPointerTextureInstance GTextureInstance::Factory(
	TPointerRender& inout_render,
	const GTextureManual& in_textureManual
	)
{
	const GU32 textureId = inout_render->BindTexture(
		in_textureManual.GetTexture()
		);
	TPointerTextureInstance pointer;
	pointer.reset(new GTextureInstance(inout_render, textureId));
	return pointer;
}

////////////////////////////////////////////////////
//constructor
GTextureInstance::GTextureInstance(
	TPointerRender& inout_render,
	const GU32 in_textureHandle
	)
	: mBuffer()
	, mRender(inout_render)
	, mTextureHandle(in_textureHandle)
{
	return;
}

GTextureInstance::GTextureInstance(
	TPointerRender& inout_render
	)
	: mBuffer()
	, mRender(inout_render)
	, mTextureHandle(0)
{
	return;
}

GTextureInstance::~GTextureInstance()
{
	Unbind();
	return;
}

////////////////////////////////////////////////////
//public methods
void GTextureInstance::Bind()
{
	if (0 != mTextureHandle)
	{
		return;
	}

	TPointerRender pointerRender = mRender.lock();
	mTextureHandle = LocalBindTexture(
		mBuffer,
		pointerRender
		);

	return;
}

void GTextureInstance::Unbind()
{
	if (0 == mTextureHandle)
	{
		return;
	}

	TPointerRender pointerRender = mRender.lock();
	if (pointerRender)
	{
		pointerRender->UnbindTexture(mTextureHandle);
		mTextureHandle = 0;
	}

	return;
}

void GTextureInstance::Replace(
	TPointerBuffer& inout_buffer, //can be null, WARN: transfer of ownership
	const bool in_bindTexture,
	const bool in_allowRuntimeBindTexture
	)
{
	Unbind();
	mBuffer.reset();
	TPointerRender pointerRender = mRender.lock();
	if (in_allowRuntimeBindTexture)
	{
		mBuffer = inout_buffer;

		if (in_bindTexture)
		{
			Bind();
		}
	}
	else if (in_bindTexture)
	{
		mTextureHandle = LocalBindTexture(inout_buffer, pointerRender);
	}
	else
	{
		GASSERT(GFALSE, "unexpected case, do we need this code path");
	}

	return;
}