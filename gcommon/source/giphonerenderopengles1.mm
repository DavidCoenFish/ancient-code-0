//
//  giphonerenderopengles1.mm
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonerenderopengles1.h"

#include "gassert.h"
#include "gcamera.h"
#include "gcolour4byte.h"
#include "gcolour4float.h"
#include "giphonerenderopengles1info.h"
#include "gmesh.h"
#include "gmeshstreaminfo.h"
#include "gmeshtype.h"
#include "gtexture.h"
#include "gtextureinstance.h"
#include "gtexturetype.h"

#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <QuartzCore/QuartzCore.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

//typedef boost::shared_ptr<GRenderTextureResource> TPointerRenderTextureResource;
//typedef boost::shared_ptr<GShaderInstance> TPointerRenderShaderResource;
typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;

#ifdef DSC_DEBUG
	#define LOCAL_ERROR_CHECK() LocalErrorCheck()
#else
	#define LOCAL_ERROR_CHECK()
#endif

//static local methods
static void LocalErrorCheck()
{
//GL_INVALID_ENUM      0x0500
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

static const GLenum LocalModeFromPrimitiveType(const GMeshType::TPrimitiveType::TEnum in_primitiveType)
{
	switch (in_primitiveType)
	{
	default:
		break;
	case GMeshType::TPrimitiveType::TPoint:
		return GL_POINTS;
	case GMeshType::TPrimitiveType::TLine:
		return GL_LINES;
	case GMeshType::TPrimitiveType::TLineStrip:
		return GL_LINE_STRIP;
	case GMeshType::TPrimitiveType::TLineLoop:
		return GL_LINE_LOOP;
	case GMeshType::TPrimitiveType::TTriangle:
		return GL_TRIANGLES;
	case GMeshType::TPrimitiveType::TTriangleStrip:
		return GL_TRIANGLE_STRIP;
	case GMeshType::TPrimitiveType::TTriangleFan:
		return GL_TRIANGLE_FAN;
	}
	GASSERT(0, "invlid case");
	return 0;
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

/**/
static void LocalMaterialParam4Set(
	const GLenum in_face,
	const GLenum in_name,
	const GColour4Float& in_colour
	)
{
	glMaterialfv(in_face, in_name, in_colour.GetData());

	return;
}

static void LocalSetLightParam4(
								const GLenum in_lightId, 
								const unsigned int in_paramName, 
								const GVector3Float& in_value,
								const float in_flag
								)
{
	float data[4];
	data[0] = in_value.m_x;
	data[1] = in_value.m_y;
	data[2] = in_value.m_z;
	data[3] = in_flag;
	glLightfv(in_lightId, in_paramName, &data[0]);
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

/////////////////////////////////////////////////////
// static public methods
/*static*/ GIPhoneRenderOpenGLES1::TPointerIPhoneRenderOpenGLES1 GIPhoneRenderOpenGLES1::Factory(
	const GBOOL in_depth,
	const GBOOL in_stencil		
	)
{
	TPointerIPhoneRenderOpenGLES1 pointer;
	EAGLContext* const pEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	if (!pEAGLContext)
	{
		return pointer;
	}

    [EAGLContext setCurrentContext:pEAGLContext];
	pointer.reset(new GIPhoneRenderOpenGLES1(pEAGLContext, in_depth, in_stencil));
	return pointer;
}

//////////////////////////////////////////////////
//constructor
GIPhoneRenderOpenGLES1::GIPhoneRenderOpenGLES1(
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
	, mInfo(GNULL)
{
	mInfo.reset(new GIPhoneRenderOpenGLES1Info());

    glGenFramebuffersOES(1, &mFramebuffer);
    glGenRenderbuffersOES(1, &mRenderbuffer);
    
    if (in_depth && in_stencil && mInfo->ExtentionTest("GL_OES_packed_depth_stencil"))
    {
        glGenRenderbuffersOES(1, &mDepthStencilbuffer);
    }
	else
    {
        if (in_depth)
        {
            glGenRenderbuffersOES(1, &mDepthbuffer);
        }
        if (in_stencil && mInfo->ExtentionTest("GL_OES_stencil8"))
        {
            glGenRenderbuffersOES(1, &mStencilbuffer);
        }
    }

	LOCAL_ERROR_CHECK();

	return;
}

GIPhoneRenderOpenGLES1::~GIPhoneRenderOpenGLES1()
{
	LOCAL_ERROR_CHECK();
	
	if (mFramebuffer)
	{
		glDeleteFramebuffersOES(1, &mFramebuffer);
		mFramebuffer = 0;
	}

	if (mRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &mRenderbuffer);
		mRenderbuffer = 0;
	}
    
    if (mDepthStencilbuffer)
    {
		glDeleteRenderbuffersOES(1, &mDepthStencilbuffer);    
        mDepthStencilbuffer = 0;
    }

	if (mDepthbuffer)
	{
		glDeleteRenderbuffersOES(1, &mDepthbuffer);
		mDepthbuffer = 0;
	}

	if (mStencilbuffer)
	{
		glDeleteRenderbuffersOES(1, &mStencilbuffer);
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

//////////////////////////////////////////////////////////////
//public methods
const GBOOL GIPhoneRenderOpenGLES1::GetSupportShader()const
{
    return GFALSE;
}

const GBOOL GIPhoneRenderOpenGLES1::GetSupportStencil()const
{
	if (mInfo)
	{
		return ((mInfo->ExtentionTest("GL_OES_packed_depth_stencil")) ||
			(mInfo->ExtentionTest("GL_OES_stencil8")));
	}
	return GFALSE;
}

const GU32 GIPhoneRenderOpenGLES1::GetMaximumTextureDimention()const
{
	if (mInfo)
	{
		return mInfo->MaxTextureSizeGet();
	}
	return 0;
}

const GU32 GIPhoneRenderOpenGLES1::GetRenderBufferWidth()const
{
	GLint backingWidth = 0;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    return backingWidth; 
}

const GU32 GIPhoneRenderOpenGLES1::GetRenderBufferHeight()const
{
	GLint backingHeight = 0;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    return backingHeight;
}


/*static*/ const bool GIPhoneRenderOpenGLES1::Resize(
	CAEAGLLayer* in_layer	
	)
{
	LOCAL_ERROR_CHECK();

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
	[mEAGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:in_layer];
    
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
    glFramebufferRenderbufferOES(
        GL_FRAMEBUFFER_OES, 
        GL_COLOR_ATTACHMENT0_OES, 
        GL_RENDERBUFFER_OES, 
        mRenderbuffer
        );   

	const GLint backingWidth = GetRenderBufferWidth();
	const GLint backingHeight = GetRenderBufferHeight();
        
	SetViewport(0, 0, backingWidth, backingHeight);

    if (mDepthStencilbuffer)
    {
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthStencilbuffer);
        glRenderbufferStorageOES(
            GL_RENDERBUFFER_OES,
            GL_DEPTH24_STENCIL8_OES,
            backingWidth,
            backingHeight
            );
            
        glFramebufferRenderbufferOES(
            GL_FRAMEBUFFER_OES, 
            GL_DEPTH_ATTACHMENT_OES, 
            GL_RENDERBUFFER_OES, 
            mDepthStencilbuffer
            );        
        glFramebufferRenderbufferOES(
            GL_FRAMEBUFFER_OES, 
            GL_STENCIL_ATTACHMENT_OES, 
            GL_RENDERBUFFER_OES, 
            mDepthStencilbuffer
            );                
       
		const GLenum result = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		if (result != GL_FRAMEBUFFER_COMPLETE_OES)
		{
			NSLog(@"Failed to make complete framebuffer object [depth stencil] %x", result);
			return false;
		}            
    }
	if (mDepthbuffer)
	{
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthbuffer);
        glRenderbufferStorageOES(
            GL_RENDERBUFFER_OES,
            GL_DEPTH_COMPONENT16_OES,
            backingWidth,
            backingHeight
            );
            
        glFramebufferRenderbufferOES(
            GL_FRAMEBUFFER_OES, 
            GL_DEPTH_ATTACHMENT_OES, 
            GL_RENDERBUFFER_OES, 
            mDepthbuffer
            );        
            
		const GLenum result = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		if (result != GL_FRAMEBUFFER_COMPLETE_OES)
		{
			NSLog(@"Failed to make complete framebuffer object [depth] %x", result);
			return false;
		}
	}

	if (mStencilbuffer)
	{
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, mStencilbuffer);
        glRenderbufferStorageOES(
            GL_RENDERBUFFER_OES,
            GL_STENCIL_INDEX8_OES,
            backingWidth,
            backingHeight
            );
        glFramebufferRenderbufferOES(
            GL_FRAMEBUFFER_OES, 
            GL_STENCIL_ATTACHMENT_OES, 
            GL_RENDERBUFFER_OES, 
            mStencilbuffer
            );                
    
 		const GLenum result = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		if (result != GL_FRAMEBUFFER_COMPLETE_OES)
		{
			NSLog(@"Failed to make complete framebuffer object [stencil] %x", result);
			LOCAL_ERROR_CHECK();
			return false;
		}
	}

	LOCAL_ERROR_CHECK();

	return true;
}

GVOID GIPhoneRenderOpenGLES1::RenderStart(
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
	
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);

	//glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

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

GVOID GIPhoneRenderOpenGLES1::Present()
{
	LOCAL_ERROR_CHECK();
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mFramebuffer);
	[mEAGLContext presentRenderbuffer:GL_RENDERBUFFER_OES];	

	LOCAL_ERROR_CHECK();
	
	return;
}

GVOID GIPhoneRenderOpenGLES1::SetCamera(GCamera& in_camera)
{
	LOCAL_ERROR_CHECK();
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(in_camera.GetProjectionMatrix().GetData());

	LOCAL_ERROR_CHECK();

	return;
}

GVOID GIPhoneRenderOpenGLES1::SetObjectTransform(const GMatrix16Float& in_matrix)
{
	LOCAL_ERROR_CHECK();	
    glMatrixMode(GL_MODELVIEW);	
    glLoadMatrixf(in_matrix.GetData());

	LOCAL_ERROR_CHECK();	
	return;
}

GVOID GIPhoneRenderOpenGLES1::SetMaterial(
	const GMaterialInstance& in_material, 
	const GBOOL in_force
	)
{
	LOCAL_ERROR_CHECK();

	//colour
	if ((in_force) ||
		(in_material.GetColour() != mCurrentMaterial.GetColour()))
	{
		const GColour4Byte colourByte(in_material.GetColour());
		glColor4ub(colourByte.m_red, colourByte.m_green, colourByte.m_blue, colourByte.m_alpha);
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
		LocalEnableCap(GL_COLOR_MATERIAL, in_material.GetFlagUseLight()); 
	}

	LOCAL_ERROR_CHECK();

	//texture
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

	mCurrentMaterial = in_material;

	return;
}

GVOID GIPhoneRenderOpenGLES1::SetViewport(
	const GS32 in_lowX,
	const GS32 in_lowY,
	const GS32 in_highX,
	const GS32 in_highY
	)
{
	glViewport(in_lowX, in_lowY, in_highX, in_highY);
	return;
}

GVOID GIPhoneRenderOpenGLES1::DrawMesh(
	const GMesh& in_mesh,
	const GVOID* const in_vertexData
	)
{
	LOCAL_ERROR_CHECK();

	const int streamCount = in_mesh.GetStreamInfoCount();

	for (int index = 0; index < streamCount; ++index)
	{
		const GMeshStreamInfo& streamInfo = in_mesh.GetArrayStreamInfo()[index];
		switch (streamInfo.GetUsage())
		{
		default:
			break;
		case GMeshType::TStreamUsage::TPosition:
			glVertexPointer(
				streamInfo.GetCount(), 
				streamInfo.GetType(), 
				in_mesh.GetVertexByteStride(), 
				((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset()
				);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case GMeshType::TStreamUsage::TUv0:
			glTexCoordPointer(2, streamInfo.GetType(), in_mesh.GetVertexByteStride(), ((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset());
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		case GMeshType::TStreamUsage::TNormal:
			glNormalPointer(streamInfo.GetType(), in_mesh.GetVertexByteStride(), ((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset());
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case GMeshType::TStreamUsage::TColour:
			glColorPointer(streamInfo.GetCount(), streamInfo.GetType(), in_mesh.GetVertexByteStride(), ((unsigned char*)in_vertexData) + streamInfo.GetVertexDataByteOffset());
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		}
	}

	LocalDrawMesh(in_mesh);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	//we need to set the colour, as caling glDrawArrays could leave it in an undifined state...?
	const GColour4Float& tempColour = mCurrentMaterial.GetColour();
	glColor4f(tempColour.m_red, tempColour.m_green, tempColour.m_blue, tempColour.m_alpha);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	LOCAL_ERROR_CHECK();

	return;
}


const GU32 GIPhoneRenderOpenGLES1::BindTexture(const GTexture& in_texture)
{
	LOCAL_ERROR_CHECK();	
	
	GU32 textureId(0);

	glGenTextures(1, (GLuint*)&textureId);

	bool compressed = false;
	GLsizei pixelDataSize = 0;
	GLint internalFormat = 0;
	GLenum format = 0;
	
	switch (in_texture.GetType())
	{
	default:
		break;
	case GTextureType::TType::TRgba:
		internalFormat = GL_RGBA;
		format = GL_BGRA_EXT;
		break;
		
	case GTextureType::TType::TPVRTC4_rgb:
		format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() * 4) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC2_rgb:
		format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() * 2) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC4_rgba:
		format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() * 4) / 8;
		compressed = true;
		break;
	case GTextureType::TType::TPVRTC2_rgba:
		format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() * 2) / 8;
		compressed = true;
		break;			
		
	case GTextureType::TType::TPalette4_rgb8:
		format = GL_PALETTE4_RGB8_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() / 2) + (16 * 3);
		break;
	case GTextureType::TType::TPalette4_rgba8:
		format = GL_PALETTE4_RGBA8_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() / 2) + (16 * 4);
		break;
	case GTextureType::TType::TPalette4_r5g6b5:
		format = GL_PALETTE4_R5_G6_B5_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() / 2) + (16 * 2);
		break;
	case GTextureType::TType::TPalette4_rgba4:
		format = GL_PALETTE4_RGBA4_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() / 2) + (16 * 2);
		break;
	case GTextureType::TType::TPalette4_rgb5a1:
		format = GL_PALETTE4_RGB5_A1_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight() / 2) + (16 * 2);
		break;
	case GTextureType::TType::TPalette8_rgb8:
		format = GL_PALETTE8_RGB8_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight()) + (256 * 3);
		break;
	case GTextureType::TType::TPalette8_rgba8:
		format = GL_PALETTE8_RGBA8_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight()) + (256 * 4);
		break;
	case GTextureType::TType::TPalette8_r5g6b5:
		format = GL_PALETTE8_R5_G6_B5_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight()) + (256 * 2);
		break;
	case GTextureType::TType::TPalette8_rgba4:
		format = GL_PALETTE8_RGBA4_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight()) + (256 * 2);
		break;
	case GTextureType::TType::TPalette8_rgb5a1:
		format = GL_PALETTE8_RGB5_A1_OES;
		compressed = true;
		pixelDataSize = (in_texture.GetWidth() * in_texture.GetHeight()) + (256 * 2);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	if (compressed)
	{
		glCompressedTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			in_texture.GetWidth(),
			in_texture.GetHeight(),
			0,
			pixelDataSize,
			in_texture.GetData()
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
			in_texture.GetData()
			);
	}	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtering

	LOCAL_ERROR_CHECK();	
	
	return textureId;
}

GVOID GIPhoneRenderOpenGLES1::UnbindTexture(const GU32 in_textureHandle)
{
	glDeleteTextures(1, (GLuint*)&in_textureHandle);
	return;
}

GVOID GIPhoneRenderOpenGLES1::GenerateMipmap(const GU32 in_textureHandle)
{
	glBindTexture(GL_TEXTURE_2D, in_textureHandle);
	glGenerateMipmapOES(GL_TEXTURE_2D);
	return;
}


const GU32 GIPhoneRenderOpenGLES1::CreateRenderTextureTarget(
	const GU32 in_textureHandel,
	const GU32 in_width,
	const GU32 in_height,
	const GBOOL in_depth,
	const GBOOL in_stencil
	//RGBA? type
	)
{
	// create a renderbuffer object to store depth info
	GLuint rboId = 0;
	if (in_depth)
	{
		glGenRenderbuffersOES(1, &rboId);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, rboId);
		glRenderbufferStorageOES(
			GL_RENDERBUFFER_OES, 
			GL_DEPTH_COMPONENT16_OES,
			in_width, 
			in_height
			);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
	}

	// create a framebuffer object
	GLuint fboId = 0;
	glGenFramebuffersOES(1, &fboId);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2DOES(
		GL_FRAMEBUFFER_OES, 
		GL_COLOR_ATTACHMENT0_OES,
		GL_TEXTURE_2D, 
		in_textureHandel, 
		0
		);

	// attach the renderbuffer to depth attachment point
	if (in_depth)
	{
		glFramebufferRenderbufferOES(
			GL_FRAMEBUFFER_OES, 
			GL_DEPTH_ATTACHMENT_OES,
			GL_RENDERBUFFER_OES, 
			rboId
			);
	}

	// check FBO status
#if 1
	GASSERT(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) == GL_FRAMEBUFFER_COMPLETE_OES, "framebuffer error");
#else
	const GU32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER_OES);
	GASSERT(status == GL_FRAMEBUFFER_COMPLETE_OES, "framebuffer error");
#endif

	return fboId;
}
GVOID GIPhoneRenderOpenGLES1::SetRenderTarget(
	const GU32 in_frameBufferObjectId
	)
{
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, in_frameBufferObjectId);
	return;
}
GVOID GIPhoneRenderOpenGLES1::DestroyRenderTextureTarget(
	const GU32 in_frameBufferObjectId
	)
{
	return;
}


/*
void GIPhoneRenderOpenGLES1::SetLight(		
	const TArrayPointerRenderLight& in_arrayLight, 
	const GColour4Float& in_ambientColour
	)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, in_ambientColour.GetData());
	
	for (int index = 0; index < mInfo->MaxLightsGet(); ++index)
	{
		const GLenum lightId = GL_LIGHT0 + index;
		//is this needed to change lights?
		glDisable(lightId);
		
		if (index < (int)in_arrayLight.size())
		{
			const GRenderLight& renderLight = *in_arrayLight[index];
			
			glLightfv(lightId, GL_AMBIENT, renderLight.GetAmbient().GetData());
			glLightfv(lightId, GL_DIFFUSE, renderLight.GetDiffuse().GetData());
			glLightfv(lightId, GL_SPECULAR, GColour4Float::sWhite.GetData());
			
			//LocalSetLightParam4(lightId, GL_POSITION, renderLight.GetPosition(), 0.0F);
			glLightf(lightId, GL_LINEAR_ATTENUATION, renderLight.GetAttenuationLinear());
			glLightf(lightId, GL_CONSTANT_ATTENUATION, renderLight.GetAttenuationConstant());
			glLightf(lightId, GL_QUADRATIC_ATTENUATION, renderLight.GetAttenuationQuadratic());
			
			switch (renderLight.GetType())
			{
				default:
					break;
				case GRenderLight::TType::TPoint:
					LocalSetLightParam4(lightId, GL_POSITION, renderLight.GetPosition(), 1.0F);
					break;
				case GRenderLight::TType::TDirect:
					LocalSetLightParam4(lightId, GL_POSITION, renderLight.GetDirection(), 0.0F);
					break;
				case GRenderLight::TType::TSpot:
					LocalSetLightParam4(lightId, GL_SPOT_DIRECTION, renderLight.GetDirection(), -1.0F);
					glLightf(lightId, GL_SPOT_EXPONENT, renderLight.GetSpotExponent());
					glLightf(lightId, GL_SPOT_CUTOFF, renderLight.GetSpotCutoff());
					break;
			}
			
			glEnable(lightId);
		}
	}
	
	return;	
}

void GIPhoneRenderOpenGLES1::SetLightDisabled()
{
	for (int index = 0; index < mInfo->MaxLightsGet(); ++index)
	{
		const GLenum lightId = GL_LIGHT0 + index;
		glDisable(lightId);
	}
		
	return;
}
*/