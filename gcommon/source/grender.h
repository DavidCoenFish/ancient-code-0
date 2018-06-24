//
//  grender.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _grender_H_
#define _grender_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>
#include "gcolour4float.h"

class GCamera;
class GMatrix16Float;
class GMesh;
class GTexture;
class GMaterialInstance;

#ifdef IPHONE
	class GIPhoneRender;
	typedef GIPhoneRender GRenderImplementation;
#elif WIN32
	class GWin32WindowOpenGL;
	typedef GWin32WindowOpenGL GRenderImplementation;
#endif

//interface class for application tasks
class GRender : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef std::map<std::string, GS32> TMapStringInt;
	typedef std::vector<std::string> TArrayString;


//HACK
public:
	GRenderImplementation* const GETHACK(){return m_renderImplementation; }
//END HACK

	///////////////////////////////////////////////////////
	//constructor
public:
	GRender(
		GRenderImplementation* inout_pointerRenderImplementation //WARN: raw pointer, avoiding cost of locking weak pointer, WE DO NOT OWN IT
		);
	~GRender();

	///////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL GetSupportShader()const;
	const GBOOL GetSupportStencil()const;
	const GU32 GetMaximumTextureDimention()const;
    const GU32 GetRenderBufferWidth()const;
    const GU32 GetRenderBufferHeight()const;
	const GU32 GetDefaultFrameBufferObject()const;

	//the mess of param is so clear can be optimised, ideally we don't use clear as it is slow (example. draw skybox with depth write always)
	GVOID RenderStart(
		const GBOOL in_clearColour = false, 
		const GColour4Float& in_clearColourValue = GColour4Float(),
		const GBOOL in_clearDepth = false, 
		const GR32 in_clearDepthValue = 1.0,
		const GBOOL in_clearStencil = false, 
		const GU32 in_clearStencilValue = 0
		);

	//game client should not need to call this, let application window component decide when to paint/ present? but they call render start?
	GVOID Present();

	void SetCamera(GCamera& in_camera);
	void SetObjectTransform(const GMatrix16Float& in_matrix);
	void SetMaterial(const GMaterialInstance& in_material);

	GVOID SetViewport(
		const GS32 in_lowX,
		const GS32 in_lowY,
		const GS32 in_highX,
		const GS32 in_highY
		);

	//vertex data param as default NULL and have it custom, when null use what is in meshLoad or is that too confusing
	// presume always making client specify the vertex data is better than hidden logic
	GVOID DrawMesh(
		const GMesh& in_mesh,
		const GVOID* const in_vertexData
		);

	const GU32 BindTexture(const GTexture& in_texture);
	GVOID UnbindTexture(const GU32 in_textureHandle);
	GVOID GenerateMipmap(const GU32 in_textureHandle);

	const GBOOL LoadShader(
		GU32& out_programIndex,
		TMapStringInt & out_mapUniformNameIndex,
		const std::string& in_vertexShaderSource,
		const std::string& in_fragmentShaderSource,
		const TArrayString& in_arrayAttributeName,
		const TArrayString& in_arrayUniformName
		);
	void ReleaseShader(const GU32 in_programIndex);
	void SetShaderUniform(const std::string& in_key, const GR32 in_value);

	//NOTE: if textureHandel has mipmaps, may need to GenerateMipmap before it can be used
	const GU32 CreateRenderTextureTarget(
		const GU32 in_textureHandel,
		const GU32 in_width,
		const GU32 in_height,
		const GBOOL in_depth,
		const GBOOL in_stencil
		//RGBA? type
		);
	GVOID SetRenderTarget(
		const GU32 in_frameBufferObjectId
		);
	GVOID DestroyRenderTextureTarget(
		const GU32 in_frameBufferObjectId
		);

	///////////////////////////////////////////////////////
	//private members
private:
	GRenderImplementation* const m_renderImplementation; //NO OWNERSHIP. avoiding cost of locking weak pointer

};

#endif //#ifndef _grender_H_
