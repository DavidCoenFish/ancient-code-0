//
//  giphonerender.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneRender_h_
#define _GIPhoneRender_h_

#include "gcommon.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include <Foundation/Foundation.h>

class GApplicationWindowCallback;
class GMaterialInstance;
class GColour4Float;
class GCamera;
class GMatrix16Float;
class GVector3Float;
class GIPhoneRenderOpenGLES1;
class GIPhoneRenderOpenGLES2;
class GMesh;
class GTexture;
@class CAEAGLLayer;

/*
only make things object c if they need to be, ie to derrive objective c classes
*/
class GIPhoneRender : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GIPhoneRenderOpenGLES1> TPointerIPhoneRenderOpenGLES1;
	typedef boost::shared_ptr<GIPhoneRenderOpenGLES2> TPointerIPhoneRenderOpenGLES2;

	typedef std::vector<std::string> TArrayString;
	typedef std::map<std::string, GS32> TMapStringInt;

	/////////////////////////////////////////////
	//constructor
public:
	GIPhoneRender(const GBOOL in_depth, const GBOOL in_stencil);
	~GIPhoneRender();

	/////////////////////////////////////////////
	//public methods
public:
	const GBOOL GetSupportShader()const;
	const GBOOL GetSupportStencil()const;
	const GU32 GetMaximumTextureDimention()const;
    const GU32 GetRenderBufferWidth()const;
    const GU32 GetRenderBufferHeight()const;
	const GU32 GetDefaultFrameBufferObject()const;

	const GBOOL Resize(CAEAGLLayer* in_layer);

	GVOID RenderStart(
		const GBOOL in_clearColour, 
		const GColour4Float& in_clearColourValue,
		const GBOOL in_clearDepth, 
		const GR32 in_clearDepthValue,
		const GBOOL in_clearStencil, 
		const GU32 in_clearStencilValue
		);

	GVOID Present();

	GVOID SetCamera(GCamera& in_camera);
	GVOID SetObjectTransform(const GMatrix16Float& in_matrix);
	GVOID SetMaterial(
		const GMaterialInstance& in_material,
		const GBOOL in_force
		);

	GVOID SetViewport(
		const GS32 in_lowX,
		const GS32 in_lowY,
		const GS32 in_highX,
		const GS32 in_highY
		);

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

	/////////////////////////////////////////////
	//private members
private:
	GU32 mType;
	//smart pointers or not. both? smart pointer for scope, normal pointers for quick access?
	TPointerIPhoneRenderOpenGLES1 mPointerRenderOpenGLES1;
	GIPhoneRenderOpenGLES1* mRenderOpenGLES1;

	TPointerIPhoneRenderOpenGLES2 mPointerRenderOpenGLES2;
	GIPhoneRenderOpenGLES2* mRenderOpenGLES2;

};

#endif //_GIPhoneRender_h_