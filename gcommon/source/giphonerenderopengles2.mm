//
//  giphonerenderopengles2.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonerenderopengles2.h"

#include "gassert.h"
#include "gcolour4float.h"
#include "gcolour4byte.h"
#include "gcamera.h"
#include "giphonerenderopengles2info.h"
#include "gmesh.h"
#include "gmeshstreaminfo.h"
#include "gtexture.h"
#include "gtextureinstance.h"
#include "gshaderinstance.h"

#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <QuartzCore/QuartzCore.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;
typedef boost::shared_ptr<GShaderInstance> TPointerShaderInstance;

#ifdef DSC_DEBUG
	#define LOCAL_ERROR_CHECK() LocalErrorCheck()
	#define LOCAL_LOG_SHADER_ERROR(IN_INDEX) LocalLogShaderError(IN_INDEX)
	#define LOCAL_LOG_PROGRAM_ERROR(IN_INDEX) LocalLogProgramError(IN_INDEX)
#else
	#define LOCAL_ERROR_CHECK()
	#define LOCAL_LOG_SHADER_ERROR(IN_INDEX)
	#define LOCAL_LOG_PROGRAM_ERROR(IN_INDEX)
#endif

//static local methods
static void LocalErrorCheck()
{
//GL_INVALID_ENUM      0x0500 1280
//GL_INVALID_VALUE     0x0501
//GL_INVALID_OPERATION 0x0502
//GL_STACK_OVERFLOW    0x0503
//GL_STACK_UNDERFLOW   0x0504
//GL_OUT_OF_MEMORY     0x0505
//GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

	GLenum error = GL_NO_ERROR;
	do
	{
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			GASSERT(0, "open gl error");
            int foo = 0;
            ++foo;
			break;
		}
	}
	while (error != GL_NO_ERROR);

	return;
}

static void LocalLogShaderError(const unsigned int index)
{
	GLint logLength;
	glGetShaderiv(index, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetShaderInfoLog(index, logLength, &logLength, log);
		GASSERT(false, log);
		//NSLog(@"Shader compile log:\n%s", log);
		free(log);
	}
	return;
}

static void LocalLogProgramError(const unsigned int index)
{
	GLint logLength;
	glGetProgramiv(index, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(index, logLength, &logLength, log);
		//GASSERT(false, log);
		NSLog(@"Program link log:\n%s", log);
		free(log);
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

/////////////////////////////////////////////////////
// static public methods
/*static*/ GIPhoneRenderOpenGLES2::TPointerIPhoneRenderOpenGLES2 GIPhoneRenderOpenGLES2::Factory(
	const GBOOL in_depth,
	const GBOOL in_stencil		
	)
{
	TPointerIPhoneRenderOpenGLES2 pointer;
	EAGLContext* const pEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!pEAGLContext)
	{
		return pointer;
	}

    [EAGLContext setCurrentContext:pEAGLContext];
	pointer.reset(new GIPhoneRenderOpenGLES2(pEAGLContext, in_depth, in_stencil));
	return pointer;
}

/////////////////////////////////////////////////////
//constructor
GIPhoneRenderOpenGLES2::GIPhoneRenderOpenGLES2(
    EAGLContext* in_EAGLContext,
	const GBOOL in_depth,
	const GBOOL in_stencil	
	)
: mEAGLContext(in_EAGLContext)
, mFramebuffer(0) 
, mRenderbuffer(0)
, mDepthStencilbuffer(0)
, mDepthbuffer(0)
, mStencilbuffer(0)
, mInfo()
, mObjectTransform(GMatrix16Float::sIdentity)
, mCamera()
, mColour()
//, mCurrentMaterial(sDummmyRenderTextureResource, sDummmyRenderShaderResource)
, mDirtyShader(false)
, mDirtyCamera(false)
, mDirtyObjectTransform(false)
, mDirtyColour(false)
{
    [EAGLContext setCurrentContext:mEAGLContext];
    
	LOCAL_ERROR_CHECK();

	mInfo.reset(new GIPhoneRenderOpenGLES2Info());

	LOCAL_ERROR_CHECK();
        
    // Create default framebuffer object.
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
        
    // Create color render buffer and allocate backing store.
    glGenRenderbuffers(1, &mRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
    //[mEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];

    if (in_depth && in_stencil && mInfo->ExtentionTest("GL_OES_packed_depth_stencil"))
    {
        glGenRenderbuffers(1, &mDepthStencilbuffer);
        LOCAL_ERROR_CHECK();
    }
	else
    {
        if (in_depth)
        {
            glGenRenderbuffers(1, &mDepthbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, mDepthbuffer);
        }
        if (in_stencil)// && mInfo->ExtentionTest("GL_OES_stencil8"))
        {
            glGenRenderbuffers(1, &mStencilbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, mStencilbuffer);
        }
    }

    return;
}

GIPhoneRenderOpenGLES2::~GIPhoneRenderOpenGLES2()
{
	LOCAL_ERROR_CHECK();
	
	if (mFramebuffer)
	{
		glDeleteFramebuffers(1, &mFramebuffer);
		mFramebuffer = 0;
	}

	if (mRenderbuffer)
	{
		glDeleteRenderbuffers(1, &mRenderbuffer);
		mRenderbuffer = 0;
	}	
	
	if (mDepthStencilbuffer)
	{
		glDeleteRenderbuffers(1, &mDepthStencilbuffer);
		mDepthStencilbuffer = 0;
	}

	if (mDepthbuffer)
	{
		glDeleteRenderbuffers(1, &mDepthbuffer);
		mDepthbuffer = 0;
	}

	if (mStencilbuffer)
	{
		glDeleteRenderbuffers(1, &mStencilbuffer);
		mStencilbuffer = 0;
	}

	if ([EAGLContext currentContext] == mEAGLContext)
	{
		[EAGLContext setCurrentContext:nil];
	}

	[mEAGLContext release];
	mEAGLContext = nil;

	LOCAL_ERROR_CHECK();

	return;
}

////////////////////////////////////////////////////////
//public methods
const GBOOL GIPhoneRenderOpenGLES2::GetSupportShader()const
{
    return GTRUE;
}

const GBOOL GIPhoneRenderOpenGLES2::GetSupportStencil()const
{
	return GTRUE;
}

const GU32 GIPhoneRenderOpenGLES2::GetMaximumTextureDimention()const
{
	if (mInfo)
	{
		return mInfo->MaxTextureSizeGet();
	}
	return 0;
}

const GU32 GIPhoneRenderOpenGLES2::GetRenderBufferWidth()const
{
	GLint backingWidth = 0;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    return backingWidth; 
}

const GU32 GIPhoneRenderOpenGLES2::GetRenderBufferHeight()const
{
	GLint backingHeight = 0;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    return backingHeight;
}


//#define GL_FRAMEBUFFER_COMPLETE                             0x8CD5
//#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                0x8CD6
//#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT        0x8CD7
//#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS                0x8CD9
//#define GL_FRAMEBUFFER_UNSUPPORTED                          0x8CDD

const GBOOL GIPhoneRenderOpenGLES2::Resize(
	CAEAGLLayer* in_layer
	)
{
#if 1
	LOCAL_ERROR_CHECK();

    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);

    [mEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:in_layer];
    
	const GLint backingWidth = GetRenderBufferWidth();
	const GLint backingHeight = GetRenderBufferHeight();
         
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRenderbuffer);
    
    GLint format = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); //GL_RGBA8_OES 0x8058   32856
    
	SetViewport(0, 0, backingWidth, backingHeight);

	LOCAL_ERROR_CHECK();
    
    if (mDepthStencilbuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilbuffer);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH24_STENCIL8_OES,
            backingWidth,
            backingHeight
            );
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilbuffer);        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilbuffer);                
        
        GLint depthSize = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &depthSize);//24
        GLint stencilSize = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &stencilSize);//8

        
        LOCAL_ERROR_CHECK();
    }
	else
    {
        if (mDepthbuffer)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, mDepthbuffer);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT16,
                backingWidth,
                backingHeight
                );
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthbuffer);
            LOCAL_ERROR_CHECK();
        }
    
        if (mStencilbuffer)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, mStencilbuffer);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_STENCIL_INDEX8,
                backingWidth,
                backingHeight
                );
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mStencilbuffer);
            LOCAL_ERROR_CHECK();
        }
    }

    const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE) //0x8CD5  36053
    {
        NSLog(@"Failed to make complete framebuffer object %x", result);
        return false;
    }            


	LOCAL_ERROR_CHECK();
    return true;
}
#else
	LOCAL_ERROR_CHECK();
	
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
	[mEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:in_layer];
    
	GLint backingWidth = 0;
	GLint backingHeight = 0;
        
	LOCAL_ERROR_CHECK();
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	LOCAL_ERROR_CHECK();

	glViewport(0, 0, backingWidth, backingHeight);

	LOCAL_ERROR_CHECK();
    if (mDepthStencilbuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilbuffer);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH24_STENCIL8_OES,
            backingWidth,
            backingHeight
            );
	LOCAL_ERROR_CHECK();
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, 
            GL_DEPTH_ATTACHMENT, 
            GL_RENDERBUFFER, 
            mDepthStencilbuffer
            );        
	LOCAL_ERROR_CHECK();
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);   
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, 
            GL_STENCIL_ATTACHMENT, 
            GL_RENDERBUFFER, 
            mDepthStencilbuffer
            );                
	LOCAL_ERROR_CHECK();
       
		const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			NSLog(@"Failed to make complete framebuffer object [depth stencil] %x", result);
			return false;
		}            
    }
	if (mDepthbuffer)
	{
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthbuffer);
	LOCAL_ERROR_CHECK();
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT16,
            backingWidth,
            backingHeight
            );
	LOCAL_ERROR_CHECK();
            
            
		const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			NSLog(@"Failed to make complete framebuffer object [depth] %x", result);
			return false;
		}
	}

	if (mStencilbuffer)
	{
	LOCAL_ERROR_CHECK();
        glBindRenderbuffer(GL_RENDERBUFFER, mStencilbuffer);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_STENCIL_INDEX8,
            backingWidth,
            backingHeight
            );
	LOCAL_ERROR_CHECK();
    
 		const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			NSLog(@"Failed to make complete framebuffer object [stencil] %x", result);
			LOCAL_ERROR_CHECK();
			return false;
		}
	}

	LOCAL_ERROR_CHECK();

	return true;
}
#endif

GVOID GIPhoneRenderOpenGLES2::RenderStart(
	const GBOOL in_clearColour, 
	const GColour4Float& in_clearColourValue,
	const GBOOL in_clearDepth, 
	const GR32 in_clearDepthValue,
	const GBOOL in_clearStencil, 
	const GU32 in_clearStencilValue
	)
{
	LOCAL_ERROR_CHECK();
	
	// needed if dealing with multiple contexts.
	[EAGLContext setCurrentContext:mEAGLContext];

	//glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);

	LOCAL_ERROR_CHECK();

	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GU32 clearFlag = 0;
	if (in_clearColour)
	{
		glClearColor(in_clearColourValue.m_red, in_clearColourValue.m_green, in_clearColourValue.m_blue, in_clearColourValue.m_alpha);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		clearFlag |= GL_COLOR_BUFFER_BIT;
	}
	if (in_clearDepth)
	{
		glClearDepthf(in_clearDepthValue);
		glDepthMask(GL_TRUE);
		clearFlag |= GL_DEPTH_BUFFER_BIT;
	}
	if (in_clearStencil)
	{
		glClearStencil(in_clearStencilValue);
		glStencilMask(0xFFFFFFFF);
		clearFlag |= GL_STENCIL_BUFFER_BIT;
	}

	if (clearFlag)
	{
		glClear(clearFlag);
	}

	LOCAL_ERROR_CHECK();

	SetMaterial(mCurrentMaterial, GTRUE);

	return;
}

GVOID GIPhoneRenderOpenGLES2::Present()
{
	LOCAL_ERROR_CHECK();
	
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
	[mEAGLContext presentRenderbuffer:GL_RENDERBUFFER];	

	LOCAL_ERROR_CHECK();

	return;
}

GVOID GIPhoneRenderOpenGLES2::SetCamera(GCamera& in_camera)
{
	mDirtyCamera = true;
	mCamera = in_camera;
	return;
}

GVOID GIPhoneRenderOpenGLES2::SetObjectTransform(const GMatrix16Float& in_matrix)
{
	mDirtyObjectTransform = true;
	mObjectTransform = in_matrix;
	return;
}

GVOID GIPhoneRenderOpenGLES2::SetMaterial(
	const GMaterialInstance& in_material, 
	const GBOOL in_force
	)
{
	LOCAL_ERROR_CHECK();

	//colour
	if ((in_force) ||
		(in_material.GetColour() != mCurrentMaterial.GetColour()))
	{
		mColour = in_material.GetColour();
		mDirtyColour = true;
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
			glDepthMask(GL_FALSE);
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
	//if ((in_force) ||
	//	(in_material.GetFlagUseLight() != mCurrentMaterial.GetFlagUseLight()))
	//{
	//	LocalEnableCap(GL_LIGHTING, in_material.GetFlagUseLight());
	//}

	LOCAL_ERROR_CHECK();

	//texture
	if ((in_force) ||
		(in_material.GetFlagUseTexture() != mCurrentMaterial.GetFlagUseTexture()))
	{
		//LocalEnableCap(GL_TEXTURE_2D, in_material.GetFlagUseTexture());
		if (in_material.GetFlagUseTexture())
		{
			glActiveTexture(GL_TEXTURE0);
		}
		//else 
		//{
		//	glDeactiveTexture(GL_TEXTURE0);
		//}

	}

	LOCAL_ERROR_CHECK();

	if ((in_force) ||
		(in_material.GetShaderInstance().lock() != mCurrentMaterial.GetShaderInstance().lock()))
	{
		const TPointerShaderInstance pointerShader = in_material.GetShaderInstance().lock();
        if (pointerShader)
		{
			GShaderInstance& shader = *pointerShader;
			mDirtyShader = true;
			glUseProgram(shader.GetProgramIndex());
			
			const int textureUniform = shader.GetUniformIndexTexture();
			if (GCOMMON_INVALID_INDEX != textureUniform)
			{
				glUniform1i(textureUniform, 0);
			}
		}
		else
		{
			glUseProgram(0);
		}
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

	mCurrentMaterial = in_material;

	return;
}


GVOID GIPhoneRenderOpenGLES2::SetViewport(
	const GS32 in_lowX,
	const GS32 in_lowY,
	const GS32 in_highX,
	const GS32 in_highY
	)
{
	glViewport(in_lowX, in_lowY, in_highX, in_highY);
	return;
}

GVOID GIPhoneRenderOpenGLES2::DrawMesh(
	const GMesh& in_mesh,
	const GVOID* const in_vertexData
	)
{
	LOCAL_ERROR_CHECK();
	
	UpdateShaderVar();
    
	LOCAL_ERROR_CHECK();

	const int streamCount = in_mesh.GetStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfo = in_mesh.GetArrayStreamInfo()[index];
		int normalise = 0;

		if ((GMeshType::TStreamUsage::TColour == streamInfo.GetUsage()) &&
			((GMeshType::TStreamType::TByte == streamInfo.GetType()) ||
			 (GMeshType::TStreamType::TUnsignedByte == streamInfo.GetType())))
		{
			normalise = 1;
		}

		glVertexAttribPointer(
			index, 
			streamInfo.GetCount(), 
			streamInfo.GetType(), 
			normalise, 
			in_mesh.GetVertexByteStride(), 
			((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset()
			);
		glEnableVertexAttribArray(index);
	}
	LOCAL_ERROR_CHECK();

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
	LOCAL_ERROR_CHECK();

	for (int index = 0; index < streamCount; ++index)
	{
		glDisableVertexAttribArray(index);
	}

	LOCAL_ERROR_CHECK();
	return;
}

const GU32 GIPhoneRenderOpenGLES2::BindTexture(const GTexture& in_texture)
{
	LOCAL_ERROR_CHECK();	
	
	GU32 textureId(0);

	GLint internalFormat = 0;
	GLenum format = 0;
	bool compressed = false;
	GLint compressedSize = 0;

	glGenTextures(1, (GLuint*)&textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

    GU8* pData = (GU8*)(in_texture.GetData());
    
	switch (in_texture.GetType())
	{
	default:
		return 0;
		
	case GTextureType::TType::TRgba:
		internalFormat = GL_RGBA;
		format = GL_BGRA_EXT;
		//format = GL_RGBA;
		break;
        
	case GTextureType::TType::TRgb:
		internalFormat = GL_RGB;
		format = GL_RGB;
		break;
        
    case GTextureType::TType::TRenderTarget:
		internalFormat = GL_RGBA;
		format = GL_RGBA;
        pData = NULL;
        break;
        
	case GTextureType::TType::TPVRTC4_rgb:
		format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
		compressedSize = (in_texture.GetWidth() * in_texture.GetHeight() * 4) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC2_rgb:
		format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		compressedSize = (in_texture.GetWidth() * in_texture.GetHeight() * 2) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC4_rgba:
		format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		compressedSize = (in_texture.GetWidth() * in_texture.GetHeight() * 4) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC2_rgba:
		format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		compressedSize = (in_texture.GetWidth() * in_texture.GetHeight() * 2) / 8;
		compressed = true;
		break;			
	}
	
	switch (in_texture.GetMagnifyFilter())
	{
	default:
	case GTextureType::TMagnifyFilter::TLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	case GTextureType::TMagnifyFilter::TNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	}

	switch (in_texture.GetWrapMode())
	{
	case GTextureType::TWrapMode::TClamp:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
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

	switch (in_texture.GetMipmapMinifyFilter())
	{
	case GTextureType::TMipmapMinifyFilter::TLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		break;
	case GTextureType::TMipmapMinifyFilter::TLinearMipmapLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		break;
	default:
	case GTextureType::TMipmapMinifyFilter::TLinearMipmapNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		break;
	case GTextureType::TMipmapMinifyFilter::TNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		break;
	case GTextureType::TMipmapMinifyFilter::TNearestMipmapLinear:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
		break;
	case GTextureType::TMipmapMinifyFilter::TNearestMipmapNearest:
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		break;
	}

	if (compressed)
	{
//GL_API void         GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data);
	
		glCompressedTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			in_texture.GetWidth(),
			in_texture.GetHeight(),
			0,
			compressedSize,
			pData
			);
	}
	else 
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0, //in_mipMapLevel,
			internalFormat, //GL_LUMINANCE_ALPHA, damn, internal format changes application of texture
			in_texture.GetWidth(),
			in_texture.GetHeight(),
			0,
			format,
			GL_UNSIGNED_BYTE,
			pData
			);
	}

	LOCAL_ERROR_CHECK();	
	
	return textureId;

}

GVOID GIPhoneRenderOpenGLES2::UnbindTexture(const GU32 in_textureHandle)
{
	glDeleteTextures(1, (GLuint*)&in_textureHandle);
	return;
}

GVOID GIPhoneRenderOpenGLES2::GenerateMipmap(const GU32 in_textureHandle)
{
	glBindTexture(GL_TEXTURE_2D, in_textureHandle);
	glGenerateMipmap(GL_TEXTURE_2D);
	return;
}


const GBOOL GIPhoneRenderOpenGLES2::LoadShader(
	GU32& out_programIndex,
	TMapStringInt & out_mapUniformNameIndex,
	const std::string& in_vertexShaderSource,
	const std::string& in_fragmentShaderSource,
	const TArrayString& in_arrayAttributeName,
	const TArrayString& in_arrayUniformName
	)
{
	LOCAL_ERROR_CHECK();

	const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar* pSource = (GLchar*)in_vertexShaderSource.c_str();
    glShaderSource(vertShader, 1, (const GLchar**)&pSource, NULL);
    glCompileShader(vertShader);
	LOCAL_LOG_SHADER_ERROR(vertShader);
	GLint status = 0;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vertShader);
	LOCAL_ERROR_CHECK();
		return false;
	}

	const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	pSource = (GLchar*)in_fragmentShaderSource.c_str();
    glShaderSource(fragShader, 1, (const GLchar**)&pSource, NULL);
    glCompileShader(fragShader);
	LOCAL_LOG_SHADER_ERROR(fragShader);
	status = 0;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
	LOCAL_ERROR_CHECK();
		return false;
	}

	out_programIndex = glCreateProgram();
	glAttachShader(out_programIndex, vertShader);
	glAttachShader(out_programIndex, fragShader);

    // Bind attribute locations
    // this needs to be done prior to linking
	for (int index = 0; index < (int)in_arrayAttributeName.size(); ++index)
	{
	    glBindAttribLocation(out_programIndex, index, in_arrayAttributeName[index].c_str());
	}

    glLinkProgram(out_programIndex);
	LOCAL_LOG_PROGRAM_ERROR(out_programIndex);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glGetProgramiv(out_programIndex, GL_LINK_STATUS, &status);
    if (status == 0)
	{
		glDeleteProgram(out_programIndex);
	LOCAL_ERROR_CHECK();
		return false;
	}

	for (TArrayString::const_iterator iterator = in_arrayUniformName.begin(); iterator != in_arrayUniformName.end(); ++iterator)
	{
		const std::string name = (*iterator);
		out_mapUniformNameIndex[name] = glGetUniformLocation(out_programIndex, name.c_str());		
	}

	LOCAL_ERROR_CHECK();

	return true;
}

GVOID GIPhoneRenderOpenGLES2::ReleaseShader(const unsigned int in_programIndex)
{
	LOCAL_ERROR_CHECK();
	glDeleteProgram(in_programIndex);
	LOCAL_ERROR_CHECK();
	return;
}

void GIPhoneRenderOpenGLES2::SetShaderUniform(const std::string& in_key, const GR32 in_value)
{
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
		glUniform1f(uniformIndex, in_value);
	}

	LOCAL_ERROR_CHECK();
}

const GU32 GIPhoneRenderOpenGLES2::CreateRenderTextureTarget(
	const GU32 in_textureHandel,
	const GU32 in_width,
	const GU32 in_height,
	const GBOOL in_depth,
	const GBOOL in_stencil
	//RGBA? type
	)
{
	LOCAL_ERROR_CHECK();
    
	// create a renderbuffer object to store depth info
	GLuint depthStencilbuffer = 0;
	GLuint depthBuffer = 0;
	GLuint stencilBuffer = 0;

    if (in_depth && in_stencil && mInfo->ExtentionTest("GL_OES_packed_depth_stencil"))
    {
        glGenRenderbuffers(1, &depthStencilbuffer);
        LOCAL_ERROR_CHECK();
    }
	else
    {
        if (in_depth)
        {
            glGenRenderbuffers(1, &depthBuffer);
        }
        if (in_stencil)// && mInfo->ExtentionTest("GL_OES_stencil8"))
        {
            glGenRenderbuffers(1, &stencilBuffer);
        }
    }

    LOCAL_ERROR_CHECK();

	// create a framebuffer object
	GLuint fboId = 0;
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	LOCAL_ERROR_CHECK();

    GLuint rboIdColour = 0;
    glGenRenderbuffers(1, &rboIdColour);
    glBindRenderbuffer(GL_RENDERBUFFER, rboIdColour);

	LOCAL_ERROR_CHECK();
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, in_width, in_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboIdColour);
    
	LOCAL_ERROR_CHECK();

    glBindTexture(GL_TEXTURE_2D, in_textureHandel);
    
	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, 
		in_textureHandel, 
		0
		);

	LOCAL_ERROR_CHECK();

    if (depthStencilbuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilbuffer);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH24_STENCIL8_OES,
            in_width,
            in_height
            );
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilbuffer);        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilbuffer);                
                
        LOCAL_ERROR_CHECK();
    }
	else
    {
        if (depthBuffer)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT16,
                in_width,
                in_height
                );
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
            LOCAL_ERROR_CHECK();
        }
    
        if (mStencilbuffer)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_STENCIL_INDEX8,
                in_width,
                in_height
                );
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);
            LOCAL_ERROR_CHECK();
        }
    }


//#define GL_FRAMEBUFFER_COMPLETE                             0x8CD5 36053
//#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                0x8CD6 36054
//#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT        0x8CD7 36055
//#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS                0x8CD9 36056
//#define GL_FRAMEBUFFER_UNSUPPORTED                          0x8CDD 36057
 //Texture2D
 
	// check FBO status
#if 0
	GASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer error");
#else
	const GU32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	GASSERT(status == GL_FRAMEBUFFER_COMPLETE, "framebuffer error");
#endif
    
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	LOCAL_ERROR_CHECK();
    
	return fboId;
}

GVOID GIPhoneRenderOpenGLES2::SetRenderTarget(
	const GU32 in_frameBufferObjectId
	)
{
	glBindFramebuffer(GL_FRAMEBUFFER, in_frameBufferObjectId);
	return;
}
GVOID GIPhoneRenderOpenGLES2::DestroyRenderTextureTarget(
	const GU32 in_frameBufferObjectId
	)
{
	return;
}


/////////////////////////////////////////////////////
//private methods
void GIPhoneRenderOpenGLES2::UpdateShaderVar()
{
//return;
	LOCAL_ERROR_CHECK();
    
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
			glUniformMatrix4fv(modelViewPerspectiveMatrix, 1, false, (mObjectTransform * mCamera.GetProjectionMatrix()).GetData());
		}

        LOCAL_ERROR_CHECK();
        
        const int modelMatrix = shader.GetUniformIndexModelMatrix();
		if (GCOMMON_INVALID_INDEX != modelMatrix)
		{
			glUniformMatrix4fv(modelMatrix, 1, false, mObjectTransform.GetData());
		}

        LOCAL_ERROR_CHECK();

		mDirtyObjectTransform = false;
        
	}

	if (mDirtyShader || mDirtyCamera)
	{
		const int indexCameraPosition = shader.GetUniformIndexCameraPosition();
		if (GCOMMON_INVALID_INDEX != indexCameraPosition)
		{
			glUniform3fv(indexCameraPosition, 1, mCamera.GetTransformPos().GetData());
		}

        LOCAL_ERROR_CHECK();

		mDirtyCamera = false;
	}

	
	if (mDirtyShader || mDirtyColour)
	{
		const int uniformIndexColour = shader.GetUniformIndexColour();
		if (GCOMMON_INVALID_INDEX != uniformIndexColour)
		{
			glUniform4fv(uniformIndexColour, 1, mColour.GetData());
		}

		LOCAL_ERROR_CHECK();

		mDirtyColour = false;
	}

	mDirtyShader = false;

	return;
}
