//
//  giphoneapplicationwindow.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneRenderOpenGLES1Info_H_
#define _GIPhoneRenderOpenGLES1Info_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <map>
#include <string>
#include <vector>

/**/
class GIPhoneRenderOpenGLES1Info : public boost::noncopyable 
{
	// typedef
private:
	typedef std::map<std::string, int> TTreeStringInt;
	typedef std::vector<std::string> TArrayString;

	// creation
public:
	GIPhoneRenderOpenGLES1Info();
	~GIPhoneRenderOpenGLES1Info();

	// public methods
public:
	const bool ExtentionTest( const std::string& in_extention );

	// public accessors
public:
	const int MaxTextureSizeGet()const{ return m_maxTextureSize; }
	const int MaxLightsGet()const{ return m_maxLights; }

	// private members
private:
	std::string m_vendor;
	std::string m_renderer;
	std::string m_version;
	TTreeStringInt m_treeExtensions;
	TArrayString m_arrayExtentions; //easier to look at in debug than tree
	int m_redBits;
	int m_greenBits;
	int m_blueBits;
	int m_alphaBits;
	int m_depthBits;
	int m_stencilBits;
	int m_maxTextureSize;
	int m_maxLights;
	int m_maxAttribStacks;
	int m_maxModelViewStacks;
	int m_maxProjectionStacks;
	int m_maxClipPlanes;
	int m_maxTextureStacks;
	int m_maxMultipassTexureUnit;

};

#endif // _GIPhoneRenderOpenGLES1Info_H_