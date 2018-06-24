//
//  gtexturemanager.h
//  GCommon
//
//  Created by David Coen on 2011 06 04
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gtexturemanager_h_
#define _gtexturemanager_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class GBuffer;
class GRender;
class GTexture;
class GTextureInstance;
class GFileManager;
class GTextureManual;

class GTextureManager : public boost::noncopyable
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

	typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;
	typedef boost::weak_ptr<GTextureInstance> TWeakTextureInstance;

	typedef std::map<std::string, TPointerTextureInstance> TMapStringTextureInstance;

	////////////////////////////////////////////////////
	//constructor
public:
	GTextureManager(
		TPointerFileManager& inout_fileManager,
		TPointerRender& inout_render
		);
	~GTextureManager();

	////////////////////////////////////////////////////
	//public methods
public:
	TWeakTextureInstance MakeTexture(
		const std::string& in_name, //unique name
		const GTexture& in_texture
		);

	TWeakTextureInstance GetTexture(
		const std::string& in_name, //file path
		const GBOOL in_bind,
		const GBOOL in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	//remove by name is currently faster
	void RemoveTexture(
		const std::string& in_name
		);

	//DOES NOT change name of original texture, texture instance name is still mapped to old name. this changes the data old name points to
	void ReplaceTexture(
		const std::string& in_oldfileName,
		const std::string& in_newFileName,
		const bool in_bindTexture, //start texture bound
		const bool in_allowRuntimeBind //does texture need to support runtime bind/ unbind (false, still unbinds on destruction)
		);

	//void RemoveTexture(
	//	TWeakTextureInstance& in_texture
	//	);

	//////////////////////////////////////////////////////
	//private members
private:
	TWeakFileManager mFileManager;
	TWeakRender mRender;

	TMapStringTextureInstance mMapNameTextureInstance;

};


#endif //_gtexturemanager_h_