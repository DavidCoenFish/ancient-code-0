//
//  gmaterialmanager.h
//  GCommon
//
//  Created by David Coen on 2011 06 04
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmaterialmanager_h_
#define _gmaterialmanager_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class GBuffer;
class GRender;
class GMaterial;
class GMaterialInstance;
class GFileManager;
class GResourceManager;

class GMaterialManager : public boost::noncopyable
{
	//////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GRender> TPointerRender;
	typedef boost::weak_ptr<GRender> TWeakRender;

	typedef boost::shared_ptr<GFileManager> TPointerFileManager;
	typedef boost::weak_ptr<GFileManager> TWeakFileManager;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef std::map<std::string, TPointerMaterialInstance> TMapStringMaterialInstance;

	////////////////////////////////////////////////////
	//constructor
public:
	GMaterialManager(
		GResourceManager& inout_resourceManager, //load textures/ shaders (parent)
		TPointerFileManager& inout_fileManager
		);
	~GMaterialManager();

	////////////////////////////////////////////////////
	//public methods
public:
	TWeakMaterialInstance GetMaterial(
		const std::string& in_name, //file path
		const GBOOL in_bindTexture,
		const GBOOL in_allowRuntimeBindTexture //does material need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	TWeakMaterialInstance GetMaterialManual(
		const std::string& in_name, 
		const std::string& in_vertexShader,
		const std::string& in_fragmentShader,
		const GMaterial& in_material,
		const GBOOL in_bind,
		const GBOOL in_allowRuntimeBind
		);

	//remove by name is currently faster
	void RemoveMaterial(
		const std::string& in_name
		);

	//void Removematerial(
	//	TWeakMaterialInstance& in_material
	//	);

	//////////////////////////////////////////////////////
	//private members
private:
	GResourceManager& mResourceManager; //parent
	TWeakFileManager mFileManager;
	TMapStringMaterialInstance mMapNameMaterialInstance;

};


#endif //_gmaterialmanager_h_