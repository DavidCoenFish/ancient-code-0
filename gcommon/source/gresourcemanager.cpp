//
//  GResourceManager.h
//
//  Created by David Coen on 2010 12 20
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "gresourcemanager.h"

#include "gfilesystem.h"
#include "gfilemanager.h"
#include "gtexturemanager.h"
#include "gshadermanager.h"
#include "gmaterialmanager.h"

////////////////////////////////////////////////////////////////
//constructor
GResourceManager::GResourceManager(
	TPointerFileSystem& inout_fileSystem,
	TPointerRender& inout_render
	)
	: mFileManager()
	, mTextureManager(GNULL)
	, mShaderManager(GNULL)
	, mMaterialManager(GNULL)
{
	mFileManager.reset(new GFileManager(inout_fileSystem));
	mTextureManager.reset(new GTextureManager(
		mFileManager,
		inout_render
		));
	mShaderManager.reset(new GShaderManager(
		inout_render,
		mFileManager
		));
	mMaterialManager.reset(new GMaterialManager(
		*this, 
		mFileManager
		));
	return;
}

GResourceManager::~GResourceManager()
{
	return;
}

////////////////////////////////////////////////////////////////
//public methods
GResourceManager::TWeakMaterialInstance GResourceManager::GetMaterialInstance(
	const std::string& in_fileName,
	const bool in_bindTexture, //start texture bound
	const bool in_allowRuntimeBindTexture //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakMaterialInstance weakMaterialInstance = mMaterialManager->GetMaterial(
		in_fileName,
		in_bindTexture,
		in_allowRuntimeBindTexture
		);

	return weakMaterialInstance;
}

GResourceManager::TWeakMaterialInstance GResourceManager::GetMaterialManual(
	const std::string& in_materialName, //used as key
	const std::string& in_vertexShader,
	const std::string& in_fragmentShader,
	const GMaterial& in_material,
	const bool in_bindTexture, //start texture bound
	const bool in_allowRuntimeBindTexture //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakMaterialInstance weakMaterialInstance = mMaterialManager->GetMaterialManual(
		in_materialName,
		in_vertexShader,
		in_fragmentShader,
		in_material,
		in_bindTexture,
		in_allowRuntimeBindTexture 
		);

	return weakMaterialInstance;
}

GResourceManager::TWeakTextureInstance GResourceManager::GetTextureInstance(
	const std::string& in_fileName,
	const bool in_bindTexture, //start texture bound
	const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakTextureInstance weakTextureInstance = mTextureManager->GetTexture(
		in_fileName,
		in_bindTexture,
		in_allowRuntimeBind);

	return weakTextureInstance;
}

void GResourceManager::ReplaceTexture(
	const std::string& in_oldfileName,
	const std::string& in_newFileName,
	const bool in_bindTexture, //start texture bound
	const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	mTextureManager->ReplaceTexture(
		in_oldfileName, 
		in_newFileName,
		in_bindTexture,
		in_allowRuntimeBind
		);
}

GResourceManager::TWeakShaderInstance GResourceManager::GetShaderInstance(
	const std::string& in_fileNameVertex,
	const std::string& in_fileNameFragment,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TWeakShaderInstance shaderInstance = mShaderManager->GetShaderInstance(
		in_fileNameVertex,
		in_fileNameFragment,
		in_arrayAttributeNameCount,
		in_arrayAttributeName,
		in_arrayUniformNameCount,
		in_arrayUniformName
		);

	return shaderInstance;
}

GResourceManager::TWeakShaderInstance GResourceManager::GetShaderInstanceSource(
	const std::string& in_name,
	const std::string& in_vertexShader,
	const std::string& in_fragmentShader,
	const int in_arrayAttributeNameCount,
	const char** in_arrayAttributeName,
	const int in_arrayUniformNameCount,
	const char** in_arrayUniformName
	)
{
	TWeakShaderInstance shaderInstance = mShaderManager->GetShaderInstanceSource(
		in_name,
		in_vertexShader,
		in_fragmentShader,
		in_arrayAttributeNameCount,
		in_arrayAttributeName,
		in_arrayUniformNameCount,
		in_arrayUniformName
		);

	return shaderInstance;
}

GFileManager& GResourceManager::GetFileManager()
{
	return (*mFileManager);
}

GTextureManager& GResourceManager::GetTextureManager()
{
	return (*mTextureManager);
}
