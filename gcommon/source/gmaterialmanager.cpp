//
//  gmaterialmanager.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 04
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmaterialmanager.h"

#include "gmaterial.h"
#include "gmaterialinstance.h"
#include "gmaterialtype.h"
#include "gbuffer.h"
#include "gbufferutility.h"
#include "grender.h"
#include "gfilemanager.h"

typedef boost::shared_ptr<GBuffer> TPointerBuffer;
typedef boost::weak_ptr<GBuffer> TWeakBuffer;
typedef boost::shared_ptr<GRender> TPointerRender;

////////////////////////////////////////////////////
//constructor
GMaterialManager::GMaterialManager(
	GResourceManager& inout_resourceManager, //load textures/ shaders (parent)
	TPointerFileManager& inout_fileManager
	)
	: mResourceManager(inout_resourceManager)
	, mFileManager(inout_fileManager)
	, mMapNameMaterialInstance()
{
	return;
}

GMaterialManager::~GMaterialManager()
{
	return;
}

////////////////////////////////////////////////////
//public methods
GMaterialManager::TWeakMaterialInstance GMaterialManager::GetMaterial(
	const std::string& in_name, //file path
	const GBOOL in_bind,
	const GBOOL in_allowRuntimeBind //does material need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakMaterialInstance weakMaterialInstance;
	TMapStringMaterialInstance::iterator iterator = mMapNameMaterialInstance.find(in_name);
	if (iterator != mMapNameMaterialInstance.end())
	{
		weakMaterialInstance = (*iterator).second;
		return weakMaterialInstance;
	}

	TPointerFileManager pointerFileManager = mFileManager.lock();
	if (GNULL == pointerFileManager)
	{
		return weakMaterialInstance;
	}

	TPointerBuffer pointerBuffer = pointerFileManager->LoadUnmanagedFile(
		in_name,
		GTRUE, //in_bundelRelativePath,
		GTRUE //in_performPointerResolution
		);

	if (GNULL == pointerBuffer)
	{
		return weakMaterialInstance;
	}

	const GMaterial& material = GBufferUtility::GetLoadBufferRoot<GMaterial>(*pointerBuffer);

	TPointerMaterialInstance materialInstance = GMaterialInstance::Factory(
		mResourceManager,
		material,
		in_bind,
		in_allowRuntimeBind
		);

	//NOTE: null materialInstance is cached

	mMapNameMaterialInstance[in_name] = materialInstance;
	weakMaterialInstance = materialInstance;

	return weakMaterialInstance;
}

GMaterialManager::TWeakMaterialInstance GMaterialManager::GetMaterialManual(
	const std::string& in_name, 
	const std::string& in_vertexShader,
	const std::string& in_fragmentShader,
	const GMaterial& in_material,
	const GBOOL in_bind,
	const GBOOL in_allowRuntimeBind //does material need to support runtime bind/ unbind (false, still unbinds on destruction)
	)
{
	TWeakMaterialInstance weakMaterialInstance;
	TMapStringMaterialInstance::iterator iterator = mMapNameMaterialInstance.find(in_name);
	if (iterator != mMapNameMaterialInstance.end())
	{
		weakMaterialInstance = (*iterator).second;
		return weakMaterialInstance;
	}

	TPointerMaterialInstance materialInstance = GMaterialInstance::FactoryManualShader(
		mResourceManager, //load textures/ shaders
		in_name,
		in_vertexShader,
		in_fragmentShader,
		in_material,
		in_bind,
		in_allowRuntimeBind //can texture be bound/ unbound during it's scope (else bind/unbind on scope)
		);

	//NOTE: null materialInstance is cached

	mMapNameMaterialInstance[in_name] = materialInstance;
	weakMaterialInstance = materialInstance;

	return weakMaterialInstance;
}

void GMaterialManager::RemoveMaterial(
	const std::string& in_name
	)
{
	TMapStringMaterialInstance::iterator iterator = mMapNameMaterialInstance.find(in_name);
	if (iterator != mMapNameMaterialInstance.end())
	{
		mMapNameMaterialInstance.erase(iterator);
	}

	return;
}
