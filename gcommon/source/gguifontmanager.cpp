//
//  GGuiFontManager.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiFontManager.h"

#include "GBuffer.h"
#include "GResourceManager.h"
#include "GFileManager.h"
#include "GGuiFont.h"

typedef boost::shared_ptr<GBuffer> TPointerFile;
typedef boost::weak_ptr<GBuffer> TWeakFile;

//constructor
GGuiFontManager::GGuiFontManager(TPointerResourceManager& inout_resourceManager)
	: mResourceManager(inout_resourceManager)
	, mMapNameFont()
{
	return;
}

GGuiFontManager::~GGuiFontManager()
{
	return;
}

//public accessors
GGuiFontManager::TWeakGuiFont GGuiFontManager::GetFont(const std::string& in_fontFilePath)
{
	TMapStringGuiFont::iterator iterator = mMapNameFont.find(in_fontFilePath);
	if (iterator != mMapNameFont.end())
	{
		return (*iterator).second;
	}

	TPointerResourceManager resourceManager = mResourceManager.lock();
	if (!resourceManager)
	{
		return TWeakGuiFont();
	}

	TWeakFile weakAsset = resourceManager->GetFileManager().LoadManagedFile(in_fontFilePath, true, true);
	TPointerFile asset = weakAsset.lock();
	if (!asset)
	{
		return TWeakGuiFont();
	}

	TPointerGuiFont pointerFont;
	pointerFont.reset(new GGuiFont(asset));

	mMapNameFont[in_fontFilePath] = pointerFont;

	return pointerFont;
}
