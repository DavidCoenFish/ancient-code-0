//
//  gtexturemanager.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 04
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gtexturemanager.h"

#include "gtexture.h"
#include "gtextureinstance.h"
#include "gtexturetype.h"
#include "gbuffer.h"
#include "gbufferutility.h"
#include "grender.h"
#include "gfilemanager.h"
#include "GTextureManual.h"

typedef boost::shared_ptr<GBuffer> TPointerBuffer;
typedef boost::weak_ptr<GBuffer> TWeakBuffer;
typedef boost::shared_ptr<GRender> TPointerRender;

////////////////////////////////////////////////////
//constructor
GTextureManager::GTextureManager(
	TPointerFileManager& inout_fileManager,
	TPointerRender& inout_render
	)
	: mFileManager(inout_fileManager)
	, mRender(inout_render)
	, mMapNameTextureInstance()
{
	return;
}

GTextureManager::~GTextureManager()
{
	return;
}

////////////////////////////////////////////////////
//public methods

GTextureManager::TWeakTextureInstance GTextureManager::MakeTexture(
    const std::string& in_name, //unique name
    const GTexture& in_texture
    )
{
	TWeakTextureInstance weakTextureInstance;
	TMapStringTextureInstance::iterator iterator = mMapNameTextureInstance.find(in_name);
	if (iterator != mMapNameTextureInstance.end())
	{
		weakTextureInstance = (*iterator).second;
		return weakTextureInstance;
	}
	
	TPointerRender pointerRender = mRender.lock();

    const GU32 textureHandel = pointerRender->BindTexture(
        in_texture
        );
    
	TPointerTextureInstance textureInstance;
    textureInstance.reset(new GTextureInstance(pointerRender, textureHandel));
    
	mMapNameTextureInstance[in_name] = textureInstance;
	weakTextureInstance = textureInstance;

	return weakTextureInstance;
}

GTextureManager::TWeakTextureInstance GTextureManager::GetTexture(
	const std::string& in_name, //file path
	const GBOOL in_bind,
	const GBOOL in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakTextureInstance weakTextureInstance;
	TMapStringTextureInstance::iterator iterator = mMapNameTextureInstance.find(in_name);
	if (iterator != mMapNameTextureInstance.end())
	{
		weakTextureInstance = (*iterator).second;
		return weakTextureInstance;
	}

	TPointerFileManager pointerFileManager = mFileManager.lock();
	if (GNULL == pointerFileManager)
	{
		return weakTextureInstance;
	}

	TPointerBuffer pointerBuffer = pointerFileManager->LoadUnmanagedFile(
		in_name,
		GTRUE, //in_bundelRelativePath,
		GTRUE //in_performPointerResolution
		);

	if (GNULL == pointerBuffer)
	{
		return weakTextureInstance;
	}

	TPointerRender pointerRender = mRender.lock();

	TPointerTextureInstance textureInstance = GTextureInstance::Factory(
		pointerBuffer,
		pointerRender,
		in_bind,
		in_allowRuntimeBind
		);

	mMapNameTextureInstance[in_name] = textureInstance;
	weakTextureInstance = textureInstance;

	return weakTextureInstance;
}

void GTextureManager::RemoveTexture(
	const std::string& in_name
	)
{
	TMapStringTextureInstance::iterator iterator = mMapNameTextureInstance.find(in_name);
	if (iterator != mMapNameTextureInstance.end())
	{
		mMapNameTextureInstance.erase(iterator);
	}

	return;
}

void GTextureManager::ReplaceTexture(
	const std::string& in_oldfileName,
	const std::string& in_newFileName,
	const bool in_bindTexture, //start texture bound
	const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakTextureInstance weakTextureInstance;
	TMapStringTextureInstance::iterator iterator = mMapNameTextureInstance.find(in_oldfileName);
	if (iterator != mMapNameTextureInstance.end())
	{
		weakTextureInstance = (*iterator).second;
	}
	TPointerTextureInstance textureInstance = weakTextureInstance.lock();
	if (!textureInstance)
	{
		return;
	}

	TPointerFileManager pointerFileManager = mFileManager.lock();
	if (!pointerFileManager)
	{
		return;
	}
	TPointerBuffer pointerBuffer = pointerFileManager->LoadUnmanagedFile(
		in_newFileName,
		GTRUE, //in_bundelRelativePath,
		GTRUE //in_performPointerResolution
		);

	if (!pointerBuffer)
	{
		return;
	}

	textureInstance->Replace(
		pointerBuffer,
		in_bindTexture,
		in_allowRuntimeBind
		);

	return;
}
