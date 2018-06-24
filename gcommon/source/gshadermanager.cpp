//
//  GShaderManager.cpp
//
//  Created by David Coen on 2011 03 31
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GShaderManager.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GFileSystem.h"
#include "GTexture.h"
#include "GShaderInstance.h"
#include "GRender.h"
#include "GFileManager.h"
#include "GStringUtility.h"

typedef boost::shared_ptr<GBuffer> TPointerBuffer;
typedef boost::weak_ptr<GBuffer> TWeakAsset;

//constructor
GShaderManager::GShaderManager(
	TPointerRender& inout_render,
	TPointerFileManager& inout_fileManager
	)
	: mRender(inout_render)
	, mFileManager(inout_fileManager)
	, mMapShaderResource()
{
	return;
}

GShaderManager::~GShaderManager()
{
	return;
}

//public methods
GShaderManager::TWeakShaderInstance GShaderManager::GetShaderInstance(
	const std::string& in_vertexShaderFileName,
	const std::string& in_fragmentShaderFileName,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TPairStringString pairStringString(in_vertexShaderFileName, in_fragmentShaderFileName);

	TMapPairStringStringRenderShaderInstance::iterator iterator = mMapShaderResource.find(pairStringString);
	if (iterator != mMapShaderResource.end())
	{
		return (*iterator).second;
	}

	TPointerRender render = mRender.lock();
	TPointerFileManager fileManager = mFileManager.lock();

	TPointerShaderInstance pointer = GShaderInstance::Factory(
		fileManager,
		render,
		in_vertexShaderFileName,
		in_fragmentShaderFileName,
		in_arrayAttributeNameCount,
		in_arrayAttributeName,
		in_arrayUniformNameCount,
		in_arrayUniformName
		);

	mMapShaderResource[pairStringString] = pointer;
	return pointer;
}

GShaderManager::TWeakShaderInstance GShaderManager::GetShaderInstanceSource(
	const std::string& in_name,
	const std::string& in_vertexShaderSource,
	const std::string& in_fragmentShaderSource,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TPairStringString pairStringString(in_name, in_name);

	TMapPairStringStringRenderShaderInstance::iterator iterator = mMapShaderResource.find(pairStringString);
	if (iterator != mMapShaderResource.end())
	{
		return (*iterator).second;
	}

	TPointerRender render = mRender.lock();
	TPointerShaderInstance pointer = GShaderInstance::FactorySource(
		render,
		in_vertexShaderSource,
		in_fragmentShaderSource,
		in_arrayAttributeNameCount,
		in_arrayAttributeName,
		in_arrayUniformNameCount,
		in_arrayUniformName
		);

	mMapShaderResource[pairStringString] = pointer;
	return pointer;
}
