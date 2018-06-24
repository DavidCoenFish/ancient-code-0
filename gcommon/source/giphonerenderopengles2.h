//
//  giphonerenderopengles2.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneRenderOpenGLES2_h_
#define _GIPhoneRenderOpenGLES2_h_

#include "gcommon.h"
#include "gcamera.h"
#include "gcolour4float.h"
#include "gmatrix16float.h"
#include "gmaterialinstance.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <map>
#include <string>

#include <Foundation/Foundation.h>

class GApplicationWindowCallback;
class GDictionary;
class GRenderLight;
class GColour4Float;
class GCamera;
class GMatrix16Float;
class GVector3Float;
class GMaterial;
class GMaterialShader;
class GIPhoneRenderOpenGLES2Info;
class GMesh;
class GTexture;
@class CAEAGLLayer;
@class EAGLContext;

/*
openGLES2 & 2 can not live in the same translation unit? so split them
*/
class GIPhoneRenderOpenGLES2 : public boost::noncopyable
{
	/////////////////////////////////////////////////////
	//typedef
private:
	//typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;
	//typedef std::vector<TPointerRenderLight> TArrayPointerRenderLight;
	
	typedef std::vector<std::string> TArrayString;
	typedef std::map<std::string, GS32> TMapStringInt;
	
	//typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	//typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GIPhoneRenderOpenGLES2> TPointerIPhoneRenderOpenGLES2;
	typedef boost::scoped_ptr<GIPhoneRenderOpenGLES2Info> TPointerIPhoneRenderOpenGLES2Info;

	/////////////////////////////////////////////////////
	// static public methods
public:
	static TPointerIPhoneRenderOpenGLES2 Factory(
		const GBOOL in_depth,
		const GBOOL in_stencil		
		);

	/////////////////////////////////////////////////////
	//constructor
public:
	GIPhoneRenderOpenGLES2(
	    EAGLContext* in_EAGLContext,
		const GBOOL in_depth,
		const GBOOL in_stencil
		);
	~GIPhoneRenderOpenGLES2();
	
	/////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL GetSupportShader()const;
	const GBOOL GetSupportStencil()const;
	const GU32 GetMaximumTextureDimention()const;
    const GU32 GetRenderBufferWidth()const;
    const GU32 GetRenderBufferHeight()const;
    const GU32 GetDefaultFrameBufferObject()const{ return mFramebuffer; }

	const bool Resize(
		CAEAGLLayer* in_layer
		);

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

	/////////////////////////////////////////////////////
	//private methods
private:
	GVOID UpdateShaderVar();

	/////////////////////////////////////////////////////
	//private members
private:
    EAGLContext* mEAGLContext;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render
    GU32 mFramebuffer; 
	GU32 mRenderbuffer;
	GU32 mDepthStencilbuffer;
	GU32 mDepthbuffer;
	GU32 mStencilbuffer;

	TPointerIPhoneRenderOpenGLES2Info mInfo;

	GMatrix16Float mObjectTransform;
	GCamera mCamera;
	GColour4Float mColour;
	GMaterialInstance mCurrentMaterial;

	bool mDirtyShader;
	bool mDirtyCamera;
	bool mDirtyObjectTransform;
	bool mDirtyColour;

};

#endif //_GIPhoneRenderOpenGLES2_h_