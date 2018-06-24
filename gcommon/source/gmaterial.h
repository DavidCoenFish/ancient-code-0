//
//  gmaterial.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmaterial_h_
#define _gmaterial_h_

#include <boost/noncopyable.hpp>
#include "gcommon.h"
#include "gcolour4float.h"
#include "gmaterialtype.h"

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMaterial : public boost::noncopyable
{
	////////////////////////////////////////////////////
	//constructor
public:
	GMaterial(
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
		);
	~GMaterial();

	////////////////////////////////////////////////////
	//public accessors
public:
	const GColour4Float& GetColour()const{ return mColour; }
	const GMaterialType::TBlendMode::TEnum GetAlphaBlendSource()const{ return (GMaterialType::TBlendMode::TEnum)mAlphaBlendSource; }
	const GMaterialType::TBlendMode::TEnum GetAlphaBlendDestination()const{ return (GMaterialType::TBlendMode::TEnum)mAlphaBlendDestination; }
	const GMaterialType::TState::TFlag GetFlag()const{ return (GMaterialType::TState::TFlag)mFlag; }

	const GCHAR* const GetTextureDataName()const{ return mTextureDataName; }

	const GCHAR* const GetVertexShaderFileName()const{ return mVertexShaderFileName; }
	const GCHAR* const GetFragmentShaderFileName()const{ return mFragmentShaderFileName; }

	const GU32 GetArrayAttributeNameCount()const{ return mArrayAttributeNameCount; }
	const GCHAR** GetArrayAttributeName()const{ return mArrayAttributeName; }

	const GU32 GetArrayUniformNameCount()const{ return mArrayUniformNameCount; }
	const GCHAR** GetArrayUniformName()const{ return mArrayUniformName; }

	////////////////////////////////////////////////////
	//private members;
private:
	const GColour4Float mColour;
	const GU32 mAlphaBlendSource;
	const GU32 mAlphaBlendDestination;
	const GU32 mFlag;

	const GCHAR* const mTextureDataName;

	const GCHAR* const mVertexShaderFileName;
	const GCHAR* const mFragmentShaderFileName;

	const GU32 mArrayAttributeNameCount;
	const GCHAR** mArrayAttributeName;

	const GU32 mArrayUniformNameCount;
	const GCHAR** mArrayUniformName;

};

#endif //_gmaterial_h_