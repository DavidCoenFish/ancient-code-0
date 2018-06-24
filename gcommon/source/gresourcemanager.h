//
//  GResourceManager.h
//
//  Created by David Coen on 2010 12 20
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GResourceManager_h_
#define _GResourceManager_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class GFileSystem;
class GFileManager;
class GRender;
class GTextureInstance;
class GTextureManager;
class GMaterial;
class GMaterialInstance;
class GMaterialManager;
class GShaderInstance;
class GShaderManager;

/*
there is a file manager that can look after ref count/ managed/ unmanaged files (GBuffer) being loaded and cached

resources are for when additional objects need creation, like textures being bound to renderer.

don't abstract base, just allow different types of resources? GResourceTexture, GResourceMaterial, GResourceShader?
	-> GTextureInstance, GMaterialInstance, GShaderInstance
*/
class GResourceManager : public boost::noncopyable
{
	////////////////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::weak_ptr<GFileSystem> TWeakFileSystem;

	typedef boost::shared_ptr<GRender> TPointerRender;
	typedef boost::weak_ptr<GRender> TWeakRender;

	typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;
	typedef boost::weak_ptr<GTextureInstance> TWeakTextureInstance;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GShaderInstance> TPointerShaderInstance;
	typedef boost::weak_ptr<GShaderInstance> TWeakShaderInstance;

	typedef boost::shared_ptr<GFileManager> TPointerFileManager;
	typedef boost::scoped_ptr<GTextureManager> TPointerTextureManager;
	typedef boost::scoped_ptr<GShaderManager> TPointerShaderManager;
	typedef boost::scoped_ptr<GMaterialManager> TPointerMaterialManager;


	////////////////////////////////////////////////////////////////
	//constructor
public:
	GResourceManager(
		TPointerFileSystem& inout_fileSystem,
		TPointerRender& inout_render
		);
	~GResourceManager();

	////////////////////////////////////////////////////////////////
	//public methods
public:
	TWeakMaterialInstance GetMaterialInstance(
		const std::string& in_fileName, //used as key
		const bool in_bindTexture, //start texture bound
		const bool in_allowRuntimeBindTexture //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	TWeakMaterialInstance GetMaterialManual(
		const std::string& in_materialName, //used as key
		const std::string& in_vertexShader,
		const std::string& in_fragmentShader,
		const GMaterial& in_material,
		const bool in_bindTexture, //start texture bound
		const bool in_allowRuntimeBindTexture
		);

	TWeakTextureInstance GetTextureInstance(
		const std::string& in_fileName,
		const bool in_bindTexture, //start texture bound
		const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	//DOES NOT change name of original texture, texture instance name is still mapped to old name. this changes the data old name points to
	void ReplaceTexture(
		const std::string& in_oldfileName,
		const std::string& in_newFileName,
		const bool in_bindTexture, //start texture bound
		const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	TWeakShaderInstance GetShaderInstance(
		const std::string& in_fileNameVertex,
		const std::string& in_fileNameFragment,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	TWeakShaderInstance GetShaderInstanceSource(
		const std::string& in_name,
		const std::string& in_vertexShader,
		const std::string& in_fragmentShader,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	GFileManager& GetFileManager();
	GTextureManager& GetTextureManager();

	////////////////////////////////////////////////////////////////
	//private members;
private:
	TPointerFileManager mFileManager;
	TPointerTextureManager mTextureManager;
	TPointerShaderManager mShaderManager;
	TPointerMaterialManager mMaterialManager;

};

#endif 