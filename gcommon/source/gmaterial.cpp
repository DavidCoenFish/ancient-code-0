//
//  gmaterial.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmaterial.h"

////////////////////////////////////////////////////
//constructor
GMaterial::GMaterial(
	const GColour4Float& in_colour,
	const GU32 in_alphaBlendSource,
	const GU32 in_alphaBlendDestination,
	const GU32 in_flag,

	const GCHAR* const in_textureDataName,

	const GCHAR* const in_vertexShaderFileName,
	const GCHAR* const in_fragmentShaderFileName,

	const GU32 in_arrayAttributeNameCount,
	const GCHAR** in_arrayAttributeName,

	const GU32 in_arrayUniformNameCount,
	const GCHAR** in_arrayUniformName
	)
	: mColour(in_colour)
	, mAlphaBlendSource(in_alphaBlendSource)
	, mAlphaBlendDestination(in_alphaBlendDestination)
	, mFlag(in_flag)
	, mTextureDataName(in_textureDataName)
	, mVertexShaderFileName(in_vertexShaderFileName)
	, mFragmentShaderFileName(in_fragmentShaderFileName)
	, mArrayAttributeNameCount(in_arrayAttributeNameCount)
	, mArrayAttributeName(in_arrayAttributeName)
	, mArrayUniformNameCount(in_arrayUniformNameCount)
	, mArrayUniformName(in_arrayUniformName)
{
	return;
}

GMaterial::~GMaterial()
{
	return;
}
