//
//  gwin32windowopengl.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "gwin32windowopengl.h"
#include "gassert.h"
#include "gcolour4float.h"
#include "gcamera.h"
#include "gmesh.h"
#include "gmeshstreaminfo.h"
#include "gtexture.h"
#include "gtexturetype.h"
#include "gwin32openglinfo.h"
#include "gshaderinstance.h"
#include "GTextureInstance.h"
#include <windef.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <openglext/glext.h>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<GShaderInstance> TPointerShaderInstance;
typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;

#ifdef DSC_DEBUG
	#define LOCAL_ERROR_CHECK() LocalErrorCheck()
	#define LOCAL_LOG_SHADER_ERROR(IN_INDEX) LocalLogShaderError(IN_INDEX, this)
	#define LOCAL_LOG_PROGRAM_ERROR(IN_INDEX) LocalLogProgramError(IN_INDEX, this)
#else
	#define LOCAL_ERROR_CHECK()
	#define LOCAL_LOG_SHADER_ERROR(IN_INDEX)
	#define LOCAL_LOG_PROGRAM_ERROR(IN_INDEX)
#endif


static GVOID LocalErrorCheck()
{
//GL_INVALID_ENUM      0x0500 //1280
//GL_INVALID_VALUE     0x0501
//GL_INVALID_OPERATION 0x0502
//GL_STACK_OVERFLOW    0x0503
//GL_STACK_UNDERFLOW   0x0504
//GL_OUT_OF_MEMORY     0x0505
//GL_INVALID_FRAMEBUFFER_OPERATION 0x0506 1286
	GLenum error = GL_NO_ERROR;
	do
	{
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			GASSERT(0, "open gl error");
			//assert(0); //"open gl error");
			break;
		}
	}
	while (error != GL_NO_ERROR);

	return;
}


static void LocalDrawMesh(
	const GMesh& in_mesh
	)
{
	const int indexType = in_mesh.GetVertexIndexType();
	if (0 != indexType)
	{
		glDrawElements(
			in_mesh.GetTopologyType(), 
			in_mesh.GetElementCount(), 
			indexType, 
			in_mesh.GetVertexIndexArray()
			);
	}
	else
	{
		glDrawArrays(
			in_mesh.GetTopologyType(),
			0,
			in_mesh.GetElementCount() 
			);
	}

	return;
}

static void LocalEnableCap(const GLenum in_cap, const bool in_state)
{
	if (in_state)
	{
		glEnable(in_cap);
	}
	else
	{
		glDisable(in_cap);
	}
	return;
}

static void LocalLogShaderError(const unsigned int index, GWin32WindowOpenGL* const in_this)
{
	int logLength;
	in_this->GlGetObjectParameterivARB(index, GL_INFO_LOG_LENGTH, logLength);
	if (logLength > 1)
	{
		const std::string log = in_this->GlGetInfoLogARB(index);
		GASSERT(log.empty(), log.c_str());
		log;
	}
	return;
}

static void LocalLogProgramError(const unsigned int index, GWin32WindowOpenGL* const in_this)
{
	int logLength;
	in_this->GlGetObjectParameterivARB(index, GL_INFO_LOG_LENGTH, logLength);
	if (logLength > 0)
	{
		const std::string log = in_this->GlGetInfoLogARB(index);
		GASSERT(log.empty(), log.c_str());
		log;
	}
	return;
}

///////////////////////////////////////////////////////
//constructor
GWin32WindowOpenGL::GWin32WindowOpenGL(HWND in_parentWnd, const GBOOL in_depth, const GBOOL in_stencil)
: mParentWnd(in_parentWnd)
, mDeviceContext(GNULL)
, mGLRenderContext(GNULL)
, mCurrentMaterial()
, mInfo()
, m_width(0)
, m_height(0)
#ifdef USE_SHADER
, mCreateShaderObjectARB(NULL)
, mShaderSourceARB(NULL)
, mCompileShaderARB(NULL)
, mCreateProgramObjectARB(NULL)
, mAttachObjectARB(NULL)
, mLinkProgramARB(NULL)
, mUseProgramObjectARB(NULL)
, mGetInfoLogARB(NULL)
, mGetObjectParameterivARB(NULL)
, mDeleteShaderARB(NULL)
, mBindAttribLocationARB(NULL)
, mDeleteProgramARB(NULL)
, mGetUniformLocationARB(NULL)
, mUniform1iARB(NULL)
, mUniform1fARB(NULL)
, mUniform3fvARB(NULL)
, mUniform4fvARB(NULL)
, mUniformMatrix4fvARB(NULL)
, mVertexAttribPointerARB(NULL)
, mEnableVertexAttribArrayARB(NULL)
, mDisableVertexAttribArrayARB(NULL)
, mActiveTextureARB(NULL)
, mCamera()
, mDirtyCamera(true)
, mDirtyObjectTransform(true)
, mDirtyColour(true)
, mDirtyShader(true)
#endif
#ifdef USE_RENDER_TEXTURE
, mGenRenderbuffersEXT(NULL)
, mBindRenderbufferEXT(NULL)
, mGenFramebuffersEXT(NULL)
, mBindFramebufferEXT(NULL)
, mFramebufferTexture2DEXT(NULL)
, mRenderbufferStorageEXT(NULL)
, mFramebufferRenderbufferEXT(NULL)
, mCheckFramebufferStatusEXT(NULL)
, mGetRenderbufferParameterivEXT(NULL)
, mGenerateMipmapEXT(NULL)
#endif
{
	mDeviceContext = GetDC(mParentWnd);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	SecureZeroMemory( &pixelFormatDescriptor, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize = sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion = 1;
	//double buffering was resulting in extra paint calls of the surface?
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 32; //24?
	if (in_depth)
	{
		pixelFormatDescriptor.cDepthBits = 24;
	}
	else
	{
		pixelFormatDescriptor.cDepthBits = 0;
	}
	if (in_stencil)
	{
		pixelFormatDescriptor.cStencilBits = 8;
	}
	else
	{
		pixelFormatDescriptor.cStencilBits = 0;
	}

	pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;
	GS32 iFormat = ChoosePixelFormat( 
		mDeviceContext, 
		&pixelFormatDescriptor 
		);

	SetPixelFormat( 
		mDeviceContext, 
		iFormat, 
		&pixelFormatDescriptor 
		);

	mGLRenderContext = wglCreateContext(mDeviceContext);

	wglMakeCurrent(mDeviceContext, mGLRenderContext);

	mInfo.reset(new GWin32OpenGLInfo);

#ifdef USE_SHADER
	mCreateShaderObjectARB = wglGetProcAddress( "glCreateShaderObjectARB" );
	mShaderSourceARB = wglGetProcAddress( "glShaderSourceARB" );
	mCompileShaderARB = wglGetProcAddress( "glCompileShaderARB" );
	mCreateProgramObjectARB = wglGetProcAddress( "glCreateProgramObjectARB" );
	mAttachObjectARB = wglGetProcAddress( "glAttachObjectARB" );
	mLinkProgramARB = wglGetProcAddress( "glLinkProgramARB" );
	mUseProgramObjectARB = wglGetProcAddress( "glUseProgramObjectARB" );
	mDeleteShaderARB = wglGetProcAddress( "glDeleteShader" );
	mDeleteProgramARB = wglGetProcAddress( "glDeleteProgram" );

	mBindAttribLocationARB = wglGetProcAddress( "glBindAttribLocation" );

	mGetInfoLogARB = wglGetProcAddress( "glGetInfoLogARB" );
	mGetObjectParameterivARB = wglGetProcAddress( "glGetObjectParameterivARB" );

	mGetUniformLocationARB = wglGetProcAddress( "glGetUniformLocationARB" );
	mUniform1iARB = wglGetProcAddress( "glUniform1iARB" );
	mUniform1fARB = wglGetProcAddress( "glUniform1fARB" );
	mUniform3fvARB = wglGetProcAddress( "glUniform3fvARB" );
	mUniform4fvARB = wglGetProcAddress( "glUniform4fvARB" );
	mUniformMatrix4fvARB = wglGetProcAddress( "glUniformMatrix4fv" );

	mVertexAttribPointerARB = wglGetProcAddress( "glVertexAttribPointerARB" );
	mEnableVertexAttribArrayARB = wglGetProcAddress( "glEnableVertexAttribArrayARB" );
	mDisableVertexAttribArrayARB = wglGetProcAddress( "glDisableVertexAttribArrayARB" );

	mActiveTextureARB = wglGetProcAddress( "glActiveTextureARB" );
#endif

#ifdef USE_RENDER_TEXTURE
	mGenRenderbuffersEXT = wglGetProcAddress( "glGenRenderbuffersEXT" );
	mBindRenderbufferEXT = wglGetProcAddress( "glBindRenderbufferEXT" );
	mGenFramebuffersEXT = wglGetProcAddress( "glGenFramebuffersEXT" );
	mBindFramebufferEXT = wglGetProcAddress( "glBindFramebufferEXT" );
	mFramebufferTexture2DEXT = wglGetProcAddress( "glFramebufferTexture2DEXT" );
	mRenderbufferStorageEXT = wglGetProcAddress( "glRenderbufferStorageEXT" );
	mFramebufferRenderbufferEXT = wglGetProcAddress( "glFramebufferRenderbufferEXT" );
	mCheckFramebufferStatusEXT = wglGetProcAddress( "glCheckFramebufferStatusEXT" );
	mGetRenderbufferParameterivEXT = wglGetProcAddress( "glGetRenderbufferParameterivEXT" );
	mGenerateMipmapEXT = wglGetProcAddress( "glGenerateMipmapEXT" );
#endif

}

/*virtual*/ GWin32WindowOpenGL::~GWin32WindowOpenGL()
{
	wglMakeCurrent(mDeviceContext, GNULL);
	wglDeleteContext(mGLRenderContext);
	mGLRenderContext = GNULL;
	ReleaseDC(mParentWnd, mDeviceContext);
	mDeviceContext = GNULL;
}

///////////////////////////////////////////////////////
//implement GView
/*virtual*/ GVOID GWin32WindowOpenGL::OnResize(const GU32 in_width, const GU32 in_height)
{
	m_width = in_width;
	m_height = in_height;

	wglMakeCurrent(mDeviceContext, mGLRenderContext);

	SetViewport(0, 0, (GLsizei)in_width, (GLsizei)in_height);

	return;
}

/*virtual*/ GVOID GWin32WindowOpenGL::OnPaint()
{
	Present();
	return;
}

///////////////////////////////////////////////////////
//public methods
const GBOOL GWin32WindowOpenGL::GetSupportShader()const
{
#ifdef USE_SHADER
	return GTRUE;
#else
	return GFALSE;
#endif
}

const GBOOL GWin32WindowOpenGL::GetSupportStencil()const
{
	return (0 < mInfo->GetStencilBits());
}

const GU32 GWin32WindowOpenGL::GetMaximumTextureDimention()const
{
	return mInfo->MaxTextureSizeGet();
}

const GU32 GWin32WindowOpenGL::GetRenderBufferWidth()const
{
	//GU32 backingWidth = 0;
	//GlGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &backingWidth);
    //return backingWidth; 
	return m_width;
}

const GU32 GWin32WindowOpenGL::GetRenderBufferHeight()const
{
	//GU32 backingHeight = 0;
	//GlGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &backingHeight);
	//return backingHeight;
	return m_height;
}

const GU32 GWin32WindowOpenGL::GetDefaultFrameBufferObject()const
{
	return 0;
}

GVOID GWin32WindowOpenGL::RenderStart(
	const GBOOL in_clearColour, 
	const GColour4Float& in_clearColourValue,
	const GBOOL in_clearDepth, 
	const GR32 in_clearDepthValue,
	const GBOOL in_clearStencil, 
	const GU32 in_clearStencilValue
	)
{
	LOCAL_ERROR_CHECK();

	wglMakeCurrent(mDeviceContext, mGLRenderContext);
	GU32 clearMask = 0;

	if (in_clearColour)
	{
		glClearColor( 
			in_clearColourValue.m_red,
			in_clearColourValue.m_green,
			in_clearColourValue.m_blue,
			in_clearColourValue.m_alpha
			);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		clearMask |= GL_COLOR_BUFFER_BIT;
	}

	if (in_clearDepth)
	{
		glClearDepth(in_clearDepthValue);
		glDepthMask(GL_TRUE);
		clearMask |= GL_DEPTH_BUFFER_BIT;
	}

	if (in_clearStencil)
	{
		glClearStencil(in_clearStencilValue);
		glStencilMask(0xFFFFFFFF);
		clearMask |= GL_STENCIL_BUFFER_BIT;
	}

	if (0 != clearMask)
	{
		glClear(clearMask);
	}

	LOCAL_ERROR_CHECK();

	SetMaterial(mCurrentMaterial, GTRUE);

	return;
}

GVOID GWin32WindowOpenGL::Present()
{
	LOCAL_ERROR_CHECK();

	wglMakeCurrent(mDeviceContext, mGLRenderContext);
	SwapBuffers(mDeviceContext);

	glFinish();

	LOCAL_ERROR_CHECK();

	return;
}

GVOID GWin32WindowOpenGL::SetCamera(GCamera& in_camera)
{
#ifdef USE_SHADER
	mDirtyCamera = true;
	mCamera = in_camera;
#else
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(in_camera.GetProjectionMatrix().GetData());
#endif
}

GVOID GWin32WindowOpenGL::SetObjectTransform(const GMatrix16Float& in_matrix)
{	
#ifdef USE_SHADER
	mDirtyObjectTransform = true;
	mObjectTransform = in_matrix;
#else
    glMatrixMode(GL_MODELVIEW);	
    glLoadMatrixf(in_matrix.GetData());
#endif
}

GVOID GWin32WindowOpenGL::SetMaterial(
	const GMaterialInstance& in_material,
	const GBOOL in_force
	)
{
	LOCAL_ERROR_CHECK();

	//colour
	if ((in_force) ||
		(in_material.GetColour() != mCurrentMaterial.GetColour()))
	{
#ifdef USE_SHADER
		mDirtyColour = true;
#else
		glColor4fv(in_material.GetColour().GetData());
#endif
	}

	LOCAL_ERROR_CHECK();

	//alpha
	if ((in_force) ||
		(in_material.GetFlagAlphaBlend() != mCurrentMaterial.GetFlagAlphaBlend()))
	{
		LocalEnableCap(GL_BLEND, in_material.GetFlagAlphaBlend());
	}
	if ((in_force) ||
		(in_material.GetFlagAlphaBlend() && (in_material.GetFlagAlphaBlend() != mCurrentMaterial.GetFlagAlphaBlend())) ||
		(in_material.GetFlagAlphaBlend() && (
			(in_material.GetAlphaBlendDestination() != mCurrentMaterial.GetAlphaBlendDestination()) ||
			(in_material.GetAlphaBlendSource() != mCurrentMaterial.GetAlphaBlendSource())
			)))
	{
		glBlendFunc(in_material.GetAlphaBlendSource(), in_material.GetAlphaBlendDestination());
	}

	LOCAL_ERROR_CHECK();

	//cull backfacing
	if ((in_force) ||
		(in_material.GetFlagFaceCull() != mCurrentMaterial.GetFlagFaceCull()))
	{
		LocalEnableCap(GL_CULL_FACE, in_material.GetFlagFaceCull());
	}

	LOCAL_ERROR_CHECK();

	//face clockwise
	if ((in_force) ||
		(in_material.GetFlagFaceCW() != mCurrentMaterial.GetFlagFaceCW()))
	{
		if (in_material.GetFlagFaceCW())
		{
			glFrontFace(GL_CW);
		}
		else
		{
			glFrontFace(GL_CCW);
		}
	}

	LOCAL_ERROR_CHECK();

	//colour write
	if ((in_force) ||
		(in_material.GetFlagColourWrite() != mCurrentMaterial.GetFlagColourWrite()))
	{
		if (in_material.GetFlagColourWrite())
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
		else
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}
	}

	LOCAL_ERROR_CHECK();

	//depth read /depth write
	if ((in_force) ||
		(in_material.GetFlagDepthRead() != mCurrentMaterial.GetFlagDepthRead()) ||
		(in_material.GetFlagDepthWrite() != mCurrentMaterial.GetFlagDepthWrite()))
	{
		if (in_material.GetFlagDepthRead() &&
			in_material.GetFlagDepthWrite())
		{
			LocalEnableCap(GL_DEPTH_TEST, true);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
		}
		else if (in_material.GetFlagDepthRead())
		{
			LocalEnableCap(GL_DEPTH_TEST, true);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_FALSE);
		}
		else if (in_material.GetFlagDepthWrite())
		{
			LocalEnableCap(GL_DEPTH_TEST, true);
			glDepthFunc(GL_ALWAYS);
			glDepthMask(GL_TRUE);
		}
		else
		{
			LocalEnableCap(GL_DEPTH_TEST, false);
		}
	}


	LOCAL_ERROR_CHECK();

	if ((in_force) ||
		((in_material.GetFlagStencilShadowFront() != mCurrentMaterial.GetFlagStencilShadowFront()) ||
		 (in_material.GetFlagStencilShadowBack() != mCurrentMaterial.GetFlagStencilShadowBack()) ||
		 (in_material.GetFlagStencilShadow() != mCurrentMaterial.GetFlagStencilShadow())))
	{
		if (in_material.GetFlagStencilShadowBack())
		{
			LocalEnableCap(GL_STENCIL_TEST, true);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glStencilMask(0xFF);
		}
		else if (in_material.GetFlagStencilShadowFront())
		{
			LocalEnableCap(GL_STENCIL_TEST, true);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glStencilMask(0xFF);
		}
		else if (in_material.GetFlagStencilShadow())
		{
			LocalEnableCap(GL_STENCIL_TEST, true);
			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
			glStencilMask(0xFF);
		}
		else
		{
			LocalEnableCap(GL_STENCIL_TEST, false);
			glStencilMask(0);
		}
	}

	LOCAL_ERROR_CHECK();

	//light
	if ((in_force) ||
		(in_material.GetFlagUseLight() != mCurrentMaterial.GetFlagUseLight()))
	{
		LocalEnableCap(GL_LIGHTING, in_material.GetFlagUseLight());
	}

	LOCAL_ERROR_CHECK();

	//use texture
	if ((in_force) ||
		(in_material.GetFlagUseTexture() != mCurrentMaterial.GetFlagUseTexture()))
	{
		LocalEnableCap(GL_TEXTURE_2D, in_material.GetFlagUseTexture());
	}

	LOCAL_ERROR_CHECK();

	//texture bind
	if (in_material.GetFlagUseTexture())
	{
		TPointerTextureInstance pointerTextureInstance = in_material.GetTextureInstance().lock();
		TPointerTextureInstance currentTextureInstance = mCurrentMaterial.GetTextureInstance().lock();
		if ((in_force) ||
			(pointerTextureInstance != currentTextureInstance))
		{
			GU32 textureHandel = 0;
			if (pointerTextureInstance)
			{
				textureHandel = pointerTextureInstance->GetTextureHandle();
			}

			glBindTexture(GL_TEXTURE_2D, textureHandel); 
		}
	}

	LOCAL_ERROR_CHECK();

#ifdef USE_SHADER
	if ((in_force) ||
		(((GMaterialInstance&)in_material).GetShaderInstance().lock() != mCurrentMaterial.GetShaderInstance().lock()))
	{
		TPointerShaderInstance pointerShader = ((GMaterialInstance&)in_material).GetShaderInstance().lock();
		if (pointerShader)
		{
			const GShaderInstance& shader = *pointerShader;
			mDirtyShader = true;
			GlUseProgramObjectARB(shader.GetProgramIndex());

			const int textureUniform = shader.GetUniformIndexTexture();
			if (GCOMMON_INVALID_INDEX != textureUniform)
			{
				GlUniform1iARB(textureUniform, 0);
			}
		}
		else
		{
			GlUseProgramObjectARB(0);
		}
	}
#endif

	LOCAL_ERROR_CHECK();

	mCurrentMaterial = in_material;

	return;
}

GVOID GWin32WindowOpenGL::SetViewport(
	const GS32 in_lowX,
	const GS32 in_lowY,
	const GS32 in_highX,
	const GS32 in_highY
	)
{
	glViewport(in_lowX, in_lowY, in_highX, in_highY);	
	return;
}

GVOID GWin32WindowOpenGL::DrawMesh(
	const GMesh& in_mesh,
	const GVOID* const in_vertexData
	)
{
	UpdateShaderVar();
	DrawMeshInit(in_mesh, in_vertexData);
	LocalDrawMesh(in_mesh);
	DrawMeshRelease(in_mesh);
	return;
}

const GU32 GWin32WindowOpenGL::BindTexture(
	const GTexture& in_texture
	)
{
	LOCAL_ERROR_CHECK();
	GU32 textureId(0);

	glGenTextures(1, (GLuint*)&textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	switch (in_texture.GetWrapMode())
	{
	case GTextureType::TWrapMode::TClamp:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		break;
	default:
	case GTextureType::TWrapMode::TRepeat:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		break;
	}

	switch (in_texture.GetMipmapMagnifyFilter())
	{
	default:
	case GTextureType::TMipmapMagnifyFilter::TLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	case GTextureType::TMipmapMagnifyFilter::TNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	}

	GBOOL generateMipMap = GFALSE;
	switch (in_texture.GetMipmapMinifyFilter())
	{
	default:
	case GTextureType::TMipmapMinifyFilter::TLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		break;
	case GTextureType::TMipmapMinifyFilter::TLinearMipmapLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		generateMipMap = GTRUE;
		break;
	case GTextureType::TMipmapMinifyFilter::TLinearMipmapNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		generateMipMap = GTRUE;
		break;
	case GTextureType::TMipmapMinifyFilter::TNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		break;
	case GTextureType::TMipmapMinifyFilter::TNearestMipmapLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
		generateMipMap = GTRUE;
		break;
	case GTextureType::TMipmapMinifyFilter::TNearestMipmapNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		generateMipMap = GTRUE;
		break;
	}

	GLint internalFormat = 0;
	GLenum format = 0;
	bool compressed = false;
	switch (in_texture.GetType())
	{
	default:
		return 0;
		break;
	case GTextureType::TType::TRgba:
		internalFormat = GL_RGBA;
		format = GL_BGRA_EXT;
		break;
	case GTextureType::TType::TRgb:
		internalFormat = GL_RGB;
		format = GL_BGR_EXT;
		break;
	case GTextureType::TType::TGreyscale:
		internalFormat = GL_ALPHA;
		format = GL_ALPHA8;
		break;
	case GTextureType::TType::TRenderTarget:
		internalFormat = GL_RGBA;
		format = GL_RGBA;
		break;
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0, //in_mipMapLevel,
		internalFormat, //GL_LUMINANCE_ALPHA, damn, internal format changes application of texture
		in_texture.GetWidth(),
		in_texture.GetHeight(),
		0,
		format,
		GL_UNSIGNED_BYTE,
		in_texture.GetData()
		);

	if (generateMipMap)
	{
		GenerateMipmap(textureId);
	}

	LOCAL_ERROR_CHECK();
	return textureId;
}

GVOID GWin32WindowOpenGL::UnbindTexture(const GU32 in_textureHandle)
{
	LOCAL_ERROR_CHECK();
	glDeleteTextures(1, (GLuint*)&in_textureHandle);
	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GenerateMipmap(const GU32 in_textureHandle)
{
	glBindTexture(GL_TEXTURE_2D, in_textureHandle);
	GLGenerateMipmapEXT(GL_TEXTURE_2D);
	return;
}

const GBOOL GWin32WindowOpenGL::LoadShader(
	GU32& out_programIndex,
	TMapStringInt & out_mapUniformNameIndex,
	const std::string& in_vertexShaderSource,
	const std::string& in_fragmentShaderSource,
	const TArrayString& in_arrayAttributeName,
	const TArrayString& in_arrayUniformName
	)
{
	LOCAL_ERROR_CHECK();
	GBOOL result = GTRUE;

#ifdef USE_SHADER
	const GLuint vertShader = GlCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLchar* pSource = (GLchar*)in_vertexShaderSource.c_str();
    GlShaderSourceARB(vertShader, 1, (const GLchar**)&pSource, NULL);
    GlCompileShaderARB(vertShader);
	LOCAL_LOG_SHADER_ERROR(vertShader);
	int status = 0;
    GlGetObjectParameterivARB(vertShader, GL_OBJECT_COMPILE_STATUS_ARB, status);
    if (status == 0)
    {
        GlDeleteShaderARB(vertShader);
		return GFALSE;
	}

	const GLuint fragShader = GlCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	pSource = (GLchar*)in_fragmentShaderSource.c_str();
    GlShaderSourceARB(fragShader, 1, (const GLchar**)&pSource, NULL);
    GlCompileShaderARB(fragShader);
	LOCAL_LOG_SHADER_ERROR(fragShader);
    GlGetObjectParameterivARB(fragShader, GL_OBJECT_COMPILE_STATUS_ARB, status);
    if (status == 0)
    {
        GlDeleteShaderARB(vertShader);
        GlDeleteShaderARB(fragShader);
		return GFALSE;
	}

	out_programIndex = GlCreateProgramObjectARB();
	GlAttachObjectARB(out_programIndex, vertShader);
	GlAttachObjectARB(out_programIndex, fragShader);

    // Bind attribute locations
    // this needs to be done prior to linking
	for (int index = 0; index < (int)in_arrayAttributeName.size(); ++index)
	{
	    //glBindAttribLocation(out_programIndex, index, in_arrayAttributeName[index].c_str());
		GLBindAttribLocation(out_programIndex, index, in_arrayAttributeName[index].c_str());
	}

    GlLinkProgramARB(out_programIndex);
	LOCAL_LOG_PROGRAM_ERROR(out_programIndex);

	GlDeleteShaderARB(vertShader);
	GlDeleteShaderARB(fragShader);

	GlGetObjectParameterivARB(out_programIndex, GL_OBJECT_LINK_STATUS_ARB, status);
    if (status == 0)
	{
		GlDeleteProgramARB(out_programIndex);
		return GFALSE;
	}

	for (TArrayString::const_iterator iterator = in_arrayUniformName.begin(); iterator != in_arrayUniformName.end(); ++iterator)
	{
		const std::string name = (*iterator);
		out_mapUniformNameIndex[name] = GlGetUniformLocationARB(out_programIndex, name.c_str());		
	}
	LOCAL_ERROR_CHECK();
#endif //#ifdef USE_SHADER

	return GTRUE;
}

void GWin32WindowOpenGL::ReleaseShader(const GU32 in_programIndex)
{
#ifdef USE_SHADER
	LOCAL_ERROR_CHECK();
	GlDeleteProgramARB(in_programIndex);
	LOCAL_ERROR_CHECK();
#endif //#ifdef USE_SHADER
	return;
}

void GWin32WindowOpenGL::SetShaderUniform(const std::string& in_key, const GR32 in_value)
{
#ifdef USE_SHADER
	LOCAL_ERROR_CHECK();

	TPointerShaderInstance pointerShader = mCurrentMaterial.GetShaderInstance().lock();
	if (!pointerShader)
	{
		return;
	}

	GShaderInstance& shader = *pointerShader;
	const GU32 uniformIndex = shader.GetUniformIndex(in_key);
	if (GCOMMON_INVALID_INDEX != uniformIndex) 
	{
		GLUniform1fARB(uniformIndex, in_value);
	}

	LOCAL_ERROR_CHECK();
#endif //#ifdef USE_SHADER
}

/*
http://www.songho.ca/opengl/gl_fbo.html#example
*/
const GU32 GWin32WindowOpenGL::CreateRenderTextureTarget(
	const GU32 in_textureHandel,
	const GU32 in_width,
	const GU32 in_height,
	const GBOOL in_depth,
	const GBOOL in_stencil
	//RGBA? type
	)
{
	GLuint depthRenderbufferIndex = 0;
	GLuint stencilRenderbufferIndex = 0;
	GLuint depthStencilRenderbufferIndex = 0;
    if (in_depth && in_stencil)
    {
        GlGenRenderbuffersEXT(1, &depthStencilRenderbufferIndex);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthStencilRenderbufferIndex);
		GlRenderbufferStorageEXT(
			GL_RENDERBUFFER_EXT, 
			GL_DEPTH_STENCIL_NV,
			in_width, 
			in_height
			);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}
	else if (in_depth)
	{
		GlGenRenderbuffersEXT(1, &depthRenderbufferIndex);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderbufferIndex);
		GlRenderbufferStorageEXT(
			GL_RENDERBUFFER_EXT, 
			GL_DEPTH_COMPONENT,
			in_width, 
			in_height
			);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}
	else if (in_stencil)
	{
		GlGenRenderbuffersEXT(1, &stencilRenderbufferIndex);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencilRenderbufferIndex);
		GlRenderbufferStorageEXT(
			GL_RENDERBUFFER_EXT, 
			GL_STENCIL_INDEX,
			in_width, 
			in_height
			);
		GlBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}

	// create a framebuffer object
	GLuint fboId = 0;
	GlGenFramebuffersEXT(1, &fboId);
	GlBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// attach the texture to FBO color attachment point
	GlFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT, 
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, 
		in_textureHandel, 
		0
		);

	// attach the renderbuffer to depth attachment point
    if (in_depth && in_stencil)
    {
		GlFramebufferRenderbufferEXT(
			GL_FRAMEBUFFER_EXT, 
			GL_DEPTH_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, 
			depthStencilRenderbufferIndex
			);
		GlFramebufferRenderbufferEXT(
			GL_FRAMEBUFFER_EXT, 
			GL_STENCIL_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, 
			depthStencilRenderbufferIndex
			);

	}
	else if (in_depth)
	{
		GlFramebufferRenderbufferEXT(
			GL_FRAMEBUFFER_EXT, 
			GL_DEPTH_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, 
			depthRenderbufferIndex
			);
	}
	else if (in_stencil)
	{
		GlFramebufferRenderbufferEXT(
			GL_FRAMEBUFFER_EXT, 
			GL_STENCIL_ATTACHMENT_EXT,
			GL_RENDERBUFFER_EXT, 
			stencilRenderbufferIndex
			);
	}

	// check FBO status
#if 0
	GASSERT(GlCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT, "framebuffer error");
#else
	const GU32 status = GlCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	GASSERT(status == GL_FRAMEBUFFER_COMPLETE_EXT, "framebuffer error");
#endif

	return fboId;
}

GVOID GWin32WindowOpenGL::SetRenderTarget(
	const GU32 in_frameBufferObjectId
	)
{
	GlBindFramebufferEXT(GL_FRAMEBUFFER_EXT, in_frameBufferObjectId);
	return;
}
GVOID GWin32WindowOpenGL::DestroyRenderTextureTarget(
	const GU32 in_frameBufferObjectId
	)
{
}



///////////////////////////////////////////////////////
//private methods
#ifdef USE_SHADER
GVOID GWin32WindowOpenGL::DrawMeshInit(const GMesh& in_mesh, const GVOID* const in_vertexData)
{
	const int streamCount = in_mesh.GetStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfo = in_mesh.GetArrayStreamInfo()[index];

		bool normalise = false;

		if ((GMeshType::TStreamUsage::TColour == streamInfo.GetUsage()) &&
			((GMeshType::TStreamType::TByte == streamInfo.GetType()) ||
			 (GMeshType::TStreamType::TUnsignedByte == streamInfo.GetType())))
		{
			normalise = true;
		}

		GLVertexAttribPointerARB(
			index, 
			streamInfo.GetCount(), 
			streamInfo.GetType(), 
			normalise, 
			in_mesh.GetVertexByteStride(), 
			((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset()
			);
		GLEnableVertexAttribArrayARB(index);
	}

	return;
}
GVOID GWin32WindowOpenGL::DrawMeshRelease(const GMesh& in_mesh)
{
	const int streamCount = in_mesh.GetStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		GLDisableVertexAttribArrayARB(index);
	}

	return;
}

//opengl extentions
/**/
const unsigned int GWin32WindowOpenGL::GlCreateShaderObjectARB( const int in_target )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mCreateShaderObjectARB )
	{
		const unsigned int handle = ( ( PFNGLCREATESHADEROBJECTARBPROC )mCreateShaderObjectARB )( 
			in_target
			);
		return handle;
	}
	LOCAL_ERROR_CHECK();
	return 0;
}

/**/
void GWin32WindowOpenGL::GlShaderSourceARB( const unsigned int in_handle, const int in_count, const char** in_arrayString, const int* length )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mShaderSourceARB )
	{
		( ( PFNGLSHADERSOURCEARBPROC )mShaderSourceARB )( 
			in_handle,
			in_count,
			in_arrayString,
			length
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GlCompileShaderARB( const unsigned int in_handle )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mCompileShaderARB )
	{
		( ( PFNGLCOMPILESHADERARBPROC )mCompileShaderARB )( 
			in_handle
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
const unsigned int GWin32WindowOpenGL::GlCreateProgramObjectARB()
{
	LOCAL_ERROR_CHECK();
	if( NULL != mCreateProgramObjectARB )
	{
		const unsigned int handle = ( ( PFNGLCREATEPROGRAMOBJECTARBPROC )mCreateProgramObjectARB )();
		return handle;
	}
	LOCAL_ERROR_CHECK();
	return 0;
}

/**/
void GWin32WindowOpenGL::GlAttachObjectARB( const unsigned int in_program, const unsigned int in_shader )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mAttachObjectARB )
	{
		( ( PFNGLATTACHOBJECTARBPROC )mAttachObjectARB )( 
			in_program,
			in_shader
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GlLinkProgramARB( const unsigned int in_program )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mLinkProgramARB )
	{
		( ( PFNGLLINKPROGRAMARBPROC )mLinkProgramARB )( 
			in_program
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GlUseProgramObjectARB( const unsigned int in_program )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUseProgramObjectARB )
	{
		( ( PFNGLUSEPROGRAMOBJECTARBPROC )mUseProgramObjectARB )( 
			in_program
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GlDeleteShaderARB( const unsigned int in_handle )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mDeleteShaderARB )
	{
		( ( PFNGLDELETESHADERPROC )mDeleteShaderARB )( 
			in_handle
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GlDeleteProgramARB( const unsigned int in_program )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mDeleteProgramARB )
	{
		( ( PFNGLDELETEPROGRAMPROC )mDeleteProgramARB )(
			in_program
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GLBindAttribLocation( const unsigned int in_program, const unsigned int in_index, const std::string& in_name )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mBindAttribLocationARB )
	{
		( ( PFNGLBINDATTRIBLOCATIONPROC )mBindAttribLocationARB )(
			in_program,
			in_index,
			in_name.c_str()
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
const std::string GWin32WindowOpenGL::GlGetInfoLogARB( const unsigned int in_handle )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGetInfoLogARB )
	{
		int length = 0;
		GlGetObjectParameterivARB( in_handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, length );
		char* data = (char*)malloc(length);

		( ( PFNGLGETINFOLOGARBPROC )mGetInfoLogARB )( 
			in_handle,
			length,
			0,
			( GLcharARB * )data
			);
		std::string returnString(data);
		free(data);
		return returnString;
	}

	LOCAL_ERROR_CHECK();
	return NULL;
}

/**/
void GWin32WindowOpenGL::GlGetObjectParameterivARB( const unsigned int in_handle, const int in_pname, int& out_value )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGetObjectParameterivARB )
	{
		( ( PFNGLGETOBJECTPARAMETERIVARBPROC )mGetObjectParameterivARB )(
			in_handle,
			in_pname,
			&out_value
			);
	}

	LOCAL_ERROR_CHECK();
	return;
}

/**/
const int GWin32WindowOpenGL::GlGetUniformLocationARB( const unsigned int in_programHandle, const std::string& in_name )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGetUniformLocationARB )
	{
		const int location = ( ( PFNGLGETUNIFORMLOCATIONARBPROC )mGetUniformLocationARB )(
			in_programHandle,
			in_name.c_str()
			);
		return location;
	}

	LOCAL_ERROR_CHECK();
	return 0;
}

/**/
void GWin32WindowOpenGL::GlUniform1iARB( const int in_location, const int in_value )
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUniform1iARB )
	{
		( ( PFNGLUNIFORM1IARBPROC )mUniform1iARB )(
			in_location,
			in_value
			);
	}

	LOCAL_ERROR_CHECK();
	return;
}

//typedef void (APIENTRYP PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
void GWin32WindowOpenGL::GLUniform1fARB(const int in_location, const float in_value)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUniform1fARB )
	{
		( ( PFNGLUNIFORM1FARBPROC )mUniform1fARB )(
			in_location,
			in_value
			);
	}

	LOCAL_ERROR_CHECK();
	return;
}

void GWin32WindowOpenGL::GLUniform3fvARB(const int in_location, const unsigned int in_count, const float* const in_value)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUniform3fvARB )
	{
		//typedef void (APIENTRYP PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat *value);
		( ( PFNGLUNIFORM3FVARBPROC )mUniform3fvARB )(
			in_location,
			in_count,
			in_value
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

void GWin32WindowOpenGL::GLUniform4fvARB(const int in_location, const unsigned int in_count, const float* const in_value)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUniform4fvARB )
	{
		//typedef void (APIENTRYP PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat *value);
		( ( PFNGLUNIFORM4FVARBPROC )mUniform4fvARB )(
			in_location,
			in_count,
			in_value
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GLUniformMatrix4fvARB(const int in_location, const unsigned int in_count, const bool in_transpose, const GMatrix16Float& in_matrix)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mUniformMatrix4fvARB )
	{
		//typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		( ( PFNGLUNIFORMMATRIX4FVARBPROC )mUniformMatrix4fvARB )(
			in_location,
			in_count,
			in_transpose,
			in_matrix.GetData()
			);
	}
	LOCAL_ERROR_CHECK();
	return;
}


/**/
void GWin32WindowOpenGL::GLVertexAttribPointerARB(const unsigned int in_index, const int in_size, const unsigned int in_type, const bool in_normalised, const unsigned int in_stride, const void* const in_pointer)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mVertexAttribPointerARB )
	{
		( ( PFNGLVERTEXATTRIBPOINTERPROC )mVertexAttribPointerARB )(
			in_index, 
			in_size, 
			GLenum(in_type), 
			GLboolean(in_normalised), 
			GLsizei(in_stride), 
			(GLvoid *)(in_pointer)
			);
	}

	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GLEnableVertexAttribArrayARB(const unsigned int in_index)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mEnableVertexAttribArrayARB )
	{
		( ( PFNGLENABLEVERTEXATTRIBARRAYPROC )mEnableVertexAttribArrayARB )(in_index);
	}

	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GLDisableVertexAttribArrayARB(const unsigned int in_index)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mDisableVertexAttribArrayARB )
	{
		( ( PFNGLDISABLEVERTEXATTRIBARRAYPROC )mDisableVertexAttribArrayARB )(in_index);
	}

	LOCAL_ERROR_CHECK();
	return;
}

/**/
void GWin32WindowOpenGL::GLActiveTextureARB(const unsigned int in_texture)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mActiveTextureARB )
	{
		( ( PFNGLACTIVETEXTUREPROC )mActiveTextureARB )(in_texture);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::UpdateShaderVar()
{
	if (!mDirtyShader && !mDirtyCamera && !mDirtyObjectTransform && !mDirtyColour)
	{
		return;
	}
	TPointerShaderInstance pointerShader = mCurrentMaterial.GetShaderInstance().lock();
	if (!pointerShader)
	{
		return;
	}

	GShaderInstance& shader = *pointerShader;

	if (mDirtyShader || mDirtyCamera || mDirtyObjectTransform)
	{
		const int modelViewPerspectiveMatrix = shader.GetUniformIndexModelViewPerspectiveMatrix();
		if (GCOMMON_INVALID_INDEX != modelViewPerspectiveMatrix)
		{
			GLUniformMatrix4fvARB(modelViewPerspectiveMatrix, 1, false, mObjectTransform * mCamera.GetProjectionMatrix());
		}

		const int modelMatrix = shader.GetUniformIndexModelMatrix();
		if (GCOMMON_INVALID_INDEX != modelMatrix)
		{
			GLUniformMatrix4fvARB(modelMatrix, 1, false, mObjectTransform);
		}

		mDirtyObjectTransform = false;
	}

	if (mDirtyShader || mDirtyCamera)
	{
		const int cameraPositionIndex = shader.GetUniformIndexCameraPosition();
		if (GCOMMON_INVALID_INDEX != cameraPositionIndex)
		{
			GLUniform3fvARB(cameraPositionIndex, 1, mCamera.GetTransformPos().GetData());
		}

		mDirtyCamera = false;
	}

	if (mDirtyShader || mDirtyColour)
	{
		const int uniformIndexColour = shader.GetUniformIndexColour();
		if (GCOMMON_INVALID_INDEX != uniformIndexColour)
		{
			GLUniform4fvARB(uniformIndexColour, 1, mCurrentMaterial.GetColour().GetData());
		}

		mDirtyColour = false;
	}

	mDirtyShader = false;

	return;
}

#else
GVOID GWin32WindowOpenGL::DrawMeshInit(const GMesh& in_mesh, const GVOID* const in_vertexData)
{
	const GS32 streamCount = in_mesh.GetStreamInfoCount();
	for (GS32 index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfoLoad = in_mesh.GetArrayStreamInfo()[index];
		switch (streamInfoLoad.GetUsage())
		{
		default:
			break;
		case GMeshType::TStreamUsage::TPosition:
			glVertexPointer(
				streamInfoLoad.GetCount(), 
				streamInfoLoad.GetType(), 
				in_mesh.GetVertexByteStride(), 
				((GU8*)in_vertexData) + streamInfoLoad.GetVertexDataByteOffset()
				);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case GMeshType::TStreamUsage::TUv0:
			glTexCoordPointer(2, streamInfoLoad.GetType(), in_mesh.GetVertexByteStride(), ((GU8*)in_vertexData) + streamInfoLoad.GetVertexDataByteOffset());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		case GMeshType::TStreamUsage::TNormal:
			glNormalPointer(streamInfoLoad.GetType(), in_mesh.GetVertexByteStride(), ((GU8*)in_vertexData) + streamInfoLoad.GetVertexDataByteOffset());
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case GMeshType::TStreamUsage::TColour:
			glColorPointer(streamInfoLoad.GetCount(), streamInfoLoad.GetType(), in_mesh.GetVertexByteStride(), ((GU8*)in_vertexData) + streamInfoLoad.GetVertexDataByteOffset());
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		}
	}

	return;
}

GVOID GWin32WindowOpenGL::DrawMeshRelease(const GMesh&)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	//we need to set the colour, as caling glDrawArrays could leave it in an undifined state...?
	//glColor4fv(mCurrentMaterial.GetDiffuseColour().GetData());
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

#endif //USE_SHADER

#ifdef USE_RENDER_TEXTURE
GVOID GWin32WindowOpenGL::GlGenRenderbuffersEXT(const GU32 in_count, GU32* out_renderbuffers) // PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint *renderbuffers);
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGenRenderbuffersEXT )
	{
		( ( PFNGLGENRENDERBUFFERSEXTPROC )mGenRenderbuffersEXT )(
				in_count,
				out_renderbuffers
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlBindRenderbufferEXT(const GU32 in_target, const GU32 in_renderbuffer) //PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
{
	LOCAL_ERROR_CHECK();
	if( NULL != mBindRenderbufferEXT )
	{
		( ( PFNGLBINDRENDERBUFFEREXTPROC )mBindRenderbufferEXT )(
				in_target,
				in_renderbuffer
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlGenFramebuffersEXT(const GU32 in_count, GU32* out_framebuffers)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGenFramebuffersEXT )
	{
		( ( PFNGLGENFRAMEBUFFERSEXTPROC )mGenFramebuffersEXT )(
				in_count,
				out_framebuffers
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlBindFramebufferEXT(const GU32 in_target, const GU32 in_framebuffer)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mBindFramebufferEXT )
	{
		( ( PFNGLBINDFRAMEBUFFEREXTPROC )mBindFramebufferEXT )(
				in_target,
				in_framebuffer
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlFramebufferTexture2DEXT(//GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level
	const GU32 in_target, 
	const GU32 in_attachment,
	const GU32 in_textarget, 
	const GU32 in_texture, 
	const GU32 in_level
	)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mFramebufferTexture2DEXT )
	{
		( ( PFNGLFRAMEBUFFERTEXTURE2DEXTPROC )mFramebufferTexture2DEXT )(
				in_target,
				in_attachment,
				in_textarget,
				in_texture,
				in_level
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlRenderbufferStorageEXT(//GLenum target, GLenum internalformat, GLsizei width, GLsizei height
	const GU32 in_target, 
	const GU32 in_internalformat,
	const GU32 in_width, 
	const GU32 in_height
	)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mRenderbufferStorageEXT )
	{
		( ( PFNGLRENDERBUFFERSTORAGEEXTPROC )mRenderbufferStorageEXT )(
				in_target,
				in_internalformat,
				in_width,
				in_height
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

GVOID GWin32WindowOpenGL::GlFramebufferRenderbufferEXT(//GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer
	const GU32 in_target, 
	const GU32 in_attachment,
	const GU32 in_renderbuffertarget, 
	const GU32 in_renderBuffer
	)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mFramebufferRenderbufferEXT )
	{
		( ( PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC )mFramebufferRenderbufferEXT )(
				in_target,
				in_attachment,
				in_renderbuffertarget,
				in_renderBuffer
				);
	}

	LOCAL_ERROR_CHECK();
	return;
}

const GU32 GWin32WindowOpenGL::GlCheckFramebufferStatusEXT(const GU32 in_target)
{
	GU32 result = 0;
	LOCAL_ERROR_CHECK();
	if( NULL != mCheckFramebufferStatusEXT )
	{
		result = ( ( PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC )mCheckFramebufferStatusEXT )(in_target);
	}

	LOCAL_ERROR_CHECK();
	return result;
}

//typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
void GWin32WindowOpenGL::GlGetRenderbufferParameterivEXT(
	const GU32 in_target, 
	const GU32 in_name, 
	GU32* out_param
	)const
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGetRenderbufferParameterivEXT )
	{
		( ( PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC )mGetRenderbufferParameterivEXT )(in_target, in_name, (GLint*)out_param);
	}

	LOCAL_ERROR_CHECK();
	return;
}

//typedef void (APIENTRYP PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
void GWin32WindowOpenGL::GLGenerateMipmapEXT(const GU32 in_target)
{
	LOCAL_ERROR_CHECK();
	if( NULL != mGenerateMipmapEXT )
	{
		( ( PFNGLGENERATEMIPMAPEXTPROC )mGenerateMipmapEXT )(in_target);
	}

	LOCAL_ERROR_CHECK();
	return;
}
#endif