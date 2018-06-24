//
//  GTextureManual.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "GTextureManual.h"

#include "gtexture.h"
#include "gtexturetype.h"
#include "gbuffer.h"

////////////////////////////////////////////////////
//constructor
GTextureManual::GTextureManual(
	const GU32 in_width,
	const GU32 in_height
	)
	: m_buffer()
	, m_texture()
{
	m_buffer.reset(new GBuffer(4 * in_width * in_height));
	m_texture.reset( new GTexture(
		in_width,
		in_height,
		GTextureType::TType::TRgba,
//		GTextureType::TType::TRgb,
		m_buffer->GetData()
		));
	return;
}

GTextureManual::~GTextureManual()
{
	return;
}

////////////////////////////////////////////////////
//public accessors
const GTexture& GTextureManual::GetTexture()const
{
	return (*m_texture);
}

GTexture& GTextureManual::GetTexture()
{
	return (*m_texture);
}
