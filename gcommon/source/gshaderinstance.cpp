//
//  GShaderInstance.cpp
//
//  Created by David Coen on 2011 03 31
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GShaderInstance.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GFileManager.h"
#include "GRender.h"
#include "GStringUtility.h"

typedef std::map<std::string, GS32> TMapStringInt;
typedef std::vector<std::string> TArrayString;
typedef boost::shared_ptr<GBuffer> TPointerBuffer;

static const int LocalGetUniformIndex(
	const std::string& in_uniformName,
	const TMapStringInt& in_mapUniform
	)
{
	TMapStringInt::const_iterator iterator = in_mapUniform.find(in_uniformName);
	if (iterator != in_mapUniform.end())
	{
		return (*iterator).second;
	}
	return GCOMMON_INVALID_INDEX;
}

//public static methods
/*static*/ GShaderInstance::TPointerRenderShaderResource GShaderInstance::Factory(
	TPointerFileManager& inout_fileManager,
	TPointerRender& inout_render,
	const std::string& in_vertexShaderFileName,
	const std::string& in_fragmentShaderFileName,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TPointerRenderShaderResource pointer;

	TPointerBuffer vertexShaderSource = inout_fileManager->LoadUnmanagedFile(
		in_vertexShaderFileName, 
		true,
		false
		);

	TPointerBuffer fragmentShaderSource = inout_fileManager->LoadUnmanagedFile(
		in_fragmentShaderFileName, 
		true,
		false
		);

	if (!vertexShaderSource || !fragmentShaderSource)
	{
		return pointer;
	}

	GBufferUtility::AppendNull(*vertexShaderSource);
	GBufferUtility::AppendNull(*fragmentShaderSource);

	pointer = FactorySource(
		inout_render,
		std::string((char*)vertexShaderSource->GetData()),
		std::string((char*)fragmentShaderSource->GetData()),
		in_arrayAttributeNameCount,
		in_arrayAttributeName,
		in_arrayUniformNameCount,
		in_arrayUniformName
		);

	return pointer;
}


/*static*/ GShaderInstance::TPointerRenderShaderResource GShaderInstance::FactorySource(
	TPointerRender& inout_render,
	const std::string& in_vertexShaderSource,
	const std::string& in_fragmentShaderSource,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TPointerRenderShaderResource pointer;

	unsigned int programIndex = 0;
	TMapStringInt mapUniformNameIndex;

	TArrayString arrayAttributeName;
	for (int index = 0; index < in_arrayAttributeNameCount; ++index)
	{
		arrayAttributeName.push_back(GStringUtility::SafeString(in_arrayAttributeName[index]));
	}

	TArrayString arrayUniformName;
	for (int index = 0; index < in_arrayUniformNameCount; ++index)
	{
		arrayUniformName.push_back(GStringUtility::SafeString(in_arrayUniformName[index]));
	}

	arrayUniformName.push_back("u_texture2d_0");
	
	bool ok = true;

	//asemble shader
	ok &= inout_render->LoadShader(
		programIndex,
		mapUniformNameIndex,
		in_vertexShaderSource,
		in_fragmentShaderSource,
		arrayAttributeName,
		arrayUniformName
		);

	if (ok)
	{
		pointer.reset(new GShaderInstance(
			inout_render,
			programIndex,
			mapUniformNameIndex
			));
	}

	return pointer;
}

//constructor
GShaderInstance::GShaderInstance(
	TPointerRender& inout_render,
	const unsigned int in_programIndex,
	const TMapStringInt& in_mapUniformNameIndex
	)
	: mWeakRender(inout_render)
	, mProgramIndex(in_programIndex)
	, mMapUniformNameIndex(in_mapUniformNameIndex)
	, mUniformIndexModelViewPerspectiveMatrix(LocalGetUniformIndex("u_mvpMatrix", in_mapUniformNameIndex))
	, mUniformIndexModelMatrix(LocalGetUniformIndex("u_mMatrix", in_mapUniformNameIndex))
	, mUniformIndexCameraPosition(LocalGetUniformIndex("u_cameraPosition", in_mapUniformNameIndex))
	, mUniformIndexColour(LocalGetUniformIndex("u_colour", in_mapUniformNameIndex))
	, mUniformIndexTexture(LocalGetUniformIndex("u_texture2d_0", in_mapUniformNameIndex))
{
	return;
}

GShaderInstance::~GShaderInstance()
{
	TPointerRender pointerRender = mWeakRender.lock();
	if (!pointerRender)
	{
		return;
	}

	pointerRender->ReleaseShader(mProgramIndex);
	return;
}

//public accessors
const int GShaderInstance::GetUniformIndex(const std::string& in_uniformName)const
{
	return LocalGetUniformIndex(in_uniformName, mMapUniformNameIndex);
}
