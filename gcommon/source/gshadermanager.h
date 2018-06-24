//
//  GShaderManager.h
//
//  Created by David Coen on 2011 03 31
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GShaderManager_h_
#define _GShaderManager_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>
#include <vector>
#include <string>

class GFileManager;
class GRender;
class GShaderInstance;

class GShaderManager : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GRender> TPointerRender;
	typedef boost::weak_ptr<GRender> TWeakRender;

	typedef boost::shared_ptr<GFileManager> TPointerFileManager;
	typedef boost::weak_ptr<GFileManager> TWeakFileManager;

	typedef boost::shared_ptr<GShaderInstance> TPointerShaderInstance;
	typedef boost::weak_ptr<GShaderInstance> TWeakShaderInstance;

	typedef std::pair<std::string, std::string> TPairStringString;
	typedef std::map<TPairStringString, TPointerShaderInstance> TMapPairStringStringRenderShaderInstance;

	//constructor
public:
	GShaderManager(
		TPointerRender& inout_render,
		TPointerFileManager& inout_fileManager
		);
	~GShaderManager();

	//public methods
public:
	TWeakShaderInstance GetShaderInstance(
		const std::string& in_vertexShaderFileName,
		const std::string& in_fragmentShaderFileName,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	TWeakShaderInstance GetShaderInstanceSource(
		const std::string& in_name,
		const std::string& in_vertexShaderSource,
		const std::string& in_fragmentShaderSource,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	//private methods
private:

	//private members;
private:
	TWeakRender mRender;
	TWeakFileManager mFileManager;
	TMapPairStringStringRenderShaderInstance mMapShaderResource;

};

#endif 