//
//  GShaderInstance.h
//
//  Created by David Coen on 2011 03 31
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GShaderInstance_h_
#define _GShaderInstance_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include "gcommon.h"

class GRender;
class GFileManager;


class GShaderInstance : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GShaderInstance> TPointerRenderShaderResource;

	typedef boost::shared_ptr<GFileManager> TPointerFileManager;

	typedef boost::shared_ptr<GRender> TPointerRender;
	typedef boost::weak_ptr<GRender> TWeakRender;

	typedef std::map<std::string, GS32> TMapStringInt;

	//public static methods
public:
	static TPointerRenderShaderResource Factory(
		TPointerFileManager& inout_fileManager,
		TPointerRender& inout_render,
		const std::string& in_vertexShaderFileName,
		const std::string& in_fragmentShaderFileName,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	static TPointerRenderShaderResource FactorySource(
		TPointerRender& inout_render,
		const std::string& in_vertexShaderSource,
		const std::string& in_fragmentShaderSource,
		const int in_arrayAttributeNameCount,
		const char** in_arrayAttributeName,
		const int in_arrayUniformNameCount,
		const char** in_arrayUniformName
		);

	//constructor
public:
	GShaderInstance(
		TPointerRender& inout_render,
		const unsigned int in_programIndex,
		const TMapStringInt& in_mapUniformNameIndex
		);

	~GShaderInstance();

	//public methods
public:

	//public accessors
public:
	const unsigned int GetProgramIndex()const{ return mProgramIndex; }

	const int GetUniformIndex(const std::string& in_uniformName)const;

	const int GetUniformIndexModelViewPerspectiveMatrix()const{ return mUniformIndexModelViewPerspectiveMatrix; }
	const int GetUniformIndexModelMatrix()const{ return mUniformIndexModelMatrix; }
	const int GetUniformIndexCameraPosition()const{ return mUniformIndexCameraPosition; }
	const int GetUniformIndexColour()const{ return mUniformIndexColour; }
	const int GetUniformIndexTexture()const{ return mUniformIndexTexture; }

	//private members;
private:
	TWeakRender mWeakRender;
	const unsigned int mProgramIndex;

	const TMapStringInt mMapUniformNameIndex;

	//some common UniformIndex to save map lookup
	const int mUniformIndexModelViewPerspectiveMatrix; //u_mvpMatrix
	const int mUniformIndexModelMatrix;	//u_mMatrix
	const int mUniformIndexCameraPosition; //u_cameraPosition
	const int mUniformIndexColour; //u_colour
	const int mUniformIndexTexture; //u_texture2d_0

};

#endif 