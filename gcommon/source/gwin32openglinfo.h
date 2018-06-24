//file: GWin32OpenGLInfo.h
#ifndef _GWin32OpenGLInfo_H_
#define _GWin32OpenGLInfo_H_

#include <boost/noncopyable.hpp>
#include <map>
#include <string>
#include <vector>

/**/
class GWin32OpenGLInfo : public boost::noncopyable 
{
	// typedef
private:
	typedef std::map<std::string, int> TTreeStringInt;
	typedef std::vector<std::string> TArrayString;

	// creation
public:
	GWin32OpenGLInfo();
	~GWin32OpenGLInfo();

	// public methods
public:
	const bool ExtentionTest( const std::string& in_extention );

	// public accessors
public:
	const int MaxTextureSizeGet()const{ return m_maxTextureSize; }
	const int MaxLightsGet()const{ return m_maxLights; }
	const int GetStencilBits()const{ return m_stencilBits; }

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

#endif // _GWin32OpenGLInfo_H_