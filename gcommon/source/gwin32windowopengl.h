//
//  gwin32windowopengl.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gwin32windowopengl_H_
#define _gwin32windowopengl_H_

#include "gcommon.h"
#include "gwin32view.h"
#include "gmaterialinstance.h"
#include <WINDOWS.H>
#include <map>
#include <vector>
#include <string>
#include <boost/scoped_ptr.hpp>

#define USE_SHADER
#define USE_RENDER_TEXTURE

#ifdef USE_SHADER
#include "gcamera.h"
#endif

class GWin32WindowParam;
class GColour4Float;
class GCamera;
class GMatrix16Float;
class GMesh;
class GTexture;
class GWin32OpenGLInfo;

class GWin32WindowOpenGL : public GWin32View
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef std::map<std::string, GS32> TMapStringInt;
	typedef std::vector<std::string> TArrayString;
	typedef boost::scoped_ptr<GWin32OpenGLInfo> TPointerWin32OpenGLInfo;

	///////////////////////////////////////////////////////
	//constructor
public:
	GWin32WindowOpenGL(HWND in_parentWnd, const GBOOL in_depth, const GBOOL in_stencil);
	virtual ~GWin32WindowOpenGL();

	///////////////////////////////////////////////////////
	//implement GView
private:
	virtual void OnResize(const GU32 in_width, const GU32 in_height);
	virtual void OnPaint();

	///////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL GetSupportShader()const;
	const GBOOL GetSupportStencil()const;
	const GU32 GetMaximumTextureDimention()const;
    const GU32 GetRenderBufferWidth()const;
    const GU32 GetRenderBufferHeight()const;
	const GU32 GetDefaultFrameBufferObject()const;

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

	const GU32 BindTexture(
		const GTexture& in_texture
		);
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

	///////////////////////////////////////////////////////
	//private methods
private:
	void DrawMeshInit(const GMesh& in_mesh, const GVOID* const in_vertexData);
	void DrawMeshRelease(const GMesh& in_mesh);

#ifdef USE_SHADER
public:
	//opengl extentions
	const unsigned int GlCreateShaderObjectARB( const int in_target );
	void GlShaderSourceARB( const unsigned int in_handle, const int in_count, const char** in_arrayString, const int* length );
	void GlCompileShaderARB( const unsigned int in_handle );
	const unsigned int GlCreateProgramObjectARB();
	void GlAttachObjectARB( const unsigned int in_program, const unsigned int in_shader );
	void GlLinkProgramARB( const unsigned int in_program );
	void GlUseProgramObjectARB( const unsigned int in_program );
	void GlDeleteShaderARB( const unsigned int in_handle );
	void GlDeleteProgramARB( const unsigned int in_program );

	void GLBindAttribLocation( const unsigned int in_program, const unsigned int in_index, const std::string& in_name );

	const std::string GlGetInfoLogARB( const unsigned int in_handle );
	void GlGetObjectParameterivARB( const unsigned int in_handle, const int in_pname, int& out_value );

	const int GlGetUniformLocationARB( const unsigned int in_programHandle, const std::string& in_name );
	//typedef void (APIENTRYP PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
	void GLUniform1fARB(const int in_location, const float in_value);
	void GLUniform3fvARB(const int in_location, const unsigned int in_count, const float* const in_value);
	void GLUniform4fvARB(const int in_location, const unsigned int in_count, const float* const in_value);
	void GLUniformMatrix4fvARB(const int in_location, const unsigned int in_count, const bool in_transpose, const GMatrix16Float& in_value);
	void GlUniform1iARB( const int in_location, const int in_value );

	void GLVertexAttribPointerARB(const unsigned int in_index, const int in_size, const unsigned int in_type, const bool in_normalised, const unsigned int in_stride, const void* const in_pointer);
	void GLEnableVertexAttribArrayARB(const unsigned int in_index);
	void GLDisableVertexAttribArrayARB(const unsigned int in_index);

	void GLActiveTextureARB(const unsigned int in_texture);

	void UpdateShaderVar();
#endif

#ifdef USE_RENDER_TEXTURE
public:
	//opengl extentions
	GVOID GlGenRenderbuffersEXT(const GU32 in_count, GU32* out_renderbuffers); // PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint *renderbuffers);
	GVOID GlBindRenderbufferEXT(const GU32 in_target, const GU32 in_renderbuffer); //PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);

	GVOID GlGenFramebuffersEXT(const GU32 in_count, GU32* out_framebuffers);
	GVOID GlBindFramebufferEXT(const GU32 in_target, const GU32 in_framebuffer);

	GVOID GlFramebufferTexture2DEXT(//GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level
		const GU32 in_target, 
		const GU32 in_attachment,
		const GU32 in_textarget, 
		const GU32 in_texture, 
		const GU32 in_level
		);
	GVOID GlRenderbufferStorageEXT(//GLenum target, GLenum internalformat, GLsizei width, GLsizei height
			const GU32 in_target, 
			const GU32 in_internalformat,
			const GU32 in_width, 
			const GU32 in_height
			);
	GVOID GlFramebufferRenderbufferEXT(//GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer
			const GU32 in_target, 
			const GU32 in_attachment,
			const GU32 in_renderbuffertarget, 
			const GU32 in_renderBuffer
			);
	const GU32 GlCheckFramebufferStatusEXT(const GU32 in_target);

	//typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
	void GlGetRenderbufferParameterivEXT(
		const GU32 in_target, 
		const GU32 in_name, 
		GU32* out_param
		)const;

	//typedef void (APIENTRYP PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
	void GLGenerateMipmapEXT(const GU32 in_target);

#endif

	///////////////////////////////////////////////////////
	//private members
private:
	HWND mParentWnd; //parent window we attach to
	HDC mDeviceContext;
	HGLRC mGLRenderContext;
	GMaterialInstance mCurrentMaterial;

	GU32 m_width;
	GU32 m_height;

	TPointerWin32OpenGLInfo mInfo;
	GBOOL mHasDepth;
	GBOOL mHasStencil;

#ifdef USE_SHADER
	void* mCreateShaderObjectARB;
	void* mShaderSourceARB;
	void* mCompileShaderARB;
	void* mCreateProgramObjectARB;
	void* mAttachObjectARB;
	void* mLinkProgramARB;
	void* mUseProgramObjectARB;
	void* mGetInfoLogARB;
	void* mGetObjectParameterivARB;
	void* mDeleteShaderARB;
	void* mDeleteProgramARB;

	void* mBindAttribLocationARB;

	void* mGetUniformLocationARB;
	void* mUniform1iARB;
	void* mUniform1fARB;
	void* mUniform3fvARB;
	void* mUniform4fvARB;
	void* mUniformMatrix4fvARB;

	void* mVertexAttribPointerARB;
	void* mEnableVertexAttribArrayARB;
	void* mDisableVertexAttribArrayARB;

	void* mActiveTextureARB;

	GCamera mCamera;
	GMatrix16Float mObjectTransform;
	GBOOL mDirtyCamera;
	GBOOL mDirtyObjectTransform;
	GBOOL mDirtyColour;
	GBOOL mDirtyShader;
#endif

#ifdef USE_RENDER_TEXTURE
	void* mGenRenderbuffersEXT;
	void* mBindRenderbufferEXT;
	void* mGenFramebuffersEXT;
	void* mBindFramebufferEXT;
	void* mFramebufferTexture2DEXT;
	void* mRenderbufferStorageEXT;
	void* mFramebufferRenderbufferEXT;
	void* mCheckFramebufferStatusEXT;
	void* mGetRenderbufferParameterivEXT;
	void* mGenerateMipmapEXT;
#endif

};

#endif //#ifndef _gwin32windowopengl_H_
