//
//  GRenderBlur.cpp
//

#include "GRenderBlur.h"

#include "GRender.h"
#include "GCamera.h"
#include "GMeshStreamInfo.h"
#include "GMeshManual.h"
#include "GSceneVisitorRenderShadow.h"
#include "GMaterialType.h"
#include "GStringUtility.h"
#include "GCommon.h"
#include "GMaterial.h"
#include "GMaterialInstance.h"
#include "GResourceManager.h"
#include "gtextureinstance.h"
#include "GTexture.h"
#include "GTextureType.h"
#include "GTextureManager.h"
#include "GTextureManual.h"
#include "GMath.h"
#include "GMesh.h"
#include "gassert.h"

#define BLUR_ANTIALIAS

#define DEBUG_MESH_VALUE 1.0F

typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;
typedef boost::weak_ptr<GTextureInstance> TWeakTextureInstance;
typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

static const char* const sVertexShader = "\n\
#version 100\n\
//uniform mat4 u_mvpMatrix;\n\
attribute vec4 a_position;\n\
attribute vec2 a_uv;\n\
varying vec2 v_uv0;\n\
void main()\n\
{\n\
   //gl_Position = u_mvpMatrix * a_position;\n\
   gl_Position = a_position;\n\
	v_uv0 = a_uv;\n\
}\n\
";
static const char* const sFragmentShader = "\n\
#version 100\n\
\n\
#ifdef GL_ES\n\
	#define LOWP lowp\n\
	#define MEDIUMP mediump\n\
	#define HIGHP highp\n\
#else\n\
	#define LOWP\n\
	#define MEDIUMP\n\
	#define HIGHP\n\
#endif\n\
\n\
uniform sampler2D u_texture2d_0;\n\
varying LOWP vec2 v_uv0;\n\
\n\
void main()\n\
{\n\
	LOWP vec4 sampleColour = texture2D(u_texture2d_0, v_uv0);\n\
	gl_FragColor = vec4(\n\
		sampleColour.r,\n\
		sampleColour.g,\n\
		sampleColour.b,\n\
		1.0);\n\
}\n\
";
static const char* sShaderArrayAttributeName[] = {
	"a_position",
	"a_uv"
};
static const int sShaderArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sShaderArrayAttributeName);

static const char** sShaderArrayUniformName = 0;
static const int sShaderArrayUniformNameCount = 0;

static const char* sMultipleShaderArrayUniformName[] = {
	"u_mul"
};
static const int sMultipleShaderArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sMultipleShaderArrayUniformName);

/*
	iOS 5 removed support of texture2DLod
	LOWP vec4 sampleColour = texture2DLod(u_texture2d_0, v_uv0, u_lod);\n\
*/

static const char* const sFragmentMultipleShader = "\n\
#version 100\n\
\n\
#ifdef GL_ES\n\
	#define LOWP lowp\n\
	#define MEDIUMP mediump\n\
	#define HIGHP highp\n\
#else\n\
	#define LOWP\n\
	#define MEDIUMP\n\
	#define HIGHP\n\
#endif\n\
\n\
uniform sampler2D u_texture2d_0;\n\
uniform LOWP float u_mul;\n\
varying LOWP vec2 v_uv0;\n\
\n\
void main()\n\
{\n\
	LOWP vec4 sampleColour = texture2D(u_texture2d_0, v_uv0);\n\
	gl_FragColor = vec4(\n\
		sampleColour.r * sampleColour.r * u_mul,\n\
		sampleColour.g * sampleColour.g * u_mul * 0.5,\n\
		0.0,\n\
		1.0);\n\
}\n\
";

//constructor
GRenderBlur::GRenderBlur(
	GRender& inout_render,
	GResourceManager& inout_resourceManager,
	const GS32 in_pixelWidth,
	const GS32 in_pixelHeight,
	const GBOOL in_depth,
	const GBOOL in_stencil
	)
	: m_material()
	, m_mesh()
	, m_orthoCamera()
	, m_textureFullScreen()
	, m_renderTextureId(0)
	, m_width(in_pixelWidth)
	, m_height(in_pixelHeight)
    , m_renderBufferWidth(GMath::NextPow2(in_pixelWidth))
    , m_renderBufferHeight(GMath::NextPow2(in_pixelHeight))
#ifdef BLUR_ANTIALIAS
	//, m_viewwportWidth(GMath::Minimum(GMath::NextPow2(in_pixelWidth), GU32(in_pixelWidth * 3 / 2)))
    //, m_viewwportHeight(GMath::Minimum(GMath::NextPow2(in_pixelHeight), GU32(in_pixelHeight * 3 / 2)))
    , m_viewwportWidth(GMath::NextPow2(in_pixelWidth))
    , m_viewwportHeight(GMath::NextPow2(in_pixelHeight))
#else
	, m_viewwportWidth(in_pixelWidth)
    , m_viewwportHeight(in_pixelHeight)
#endif
{
	m_orthoCamera.reset(new GCamera);

    {
        GTexture texture(
			m_renderBufferWidth, 
			m_renderBufferHeight, 
			GTextureType::TType::TRenderTarget, 
			0,
			GTextureType::TMagnifyFilter::TNearest,
			GTextureType::TWrapMode::TClamp,
			GTextureType::TMipmapMagnifyFilter::TLinear,
			GTextureType::TMipmapMinifyFilter::TLinear
			);
        TWeakTextureInstance weakTexture = inout_resourceManager.GetTextureManager().MakeTexture("BlurTextureFullScreen", texture);
        const GU32 renderTextureHandle = weakTexture.lock()->GetTextureHandle();
        m_renderTextureId = inout_render.CreateRenderTextureTarget(
            renderTextureHandle,
            m_renderBufferWidth,
            m_renderBufferHeight,
            in_depth,
            in_stencil
            );
    }


    {
		GTexture texture(m_renderBufferWidth / 2, m_renderBufferHeight / 2, GTextureType::TType::TRenderTarget, 0, GTextureType::TMagnifyFilter::TNearest, GTextureType::TWrapMode::TClamp, GTextureType::TMipmapMagnifyFilter::TLinear, GTextureType::TMipmapMinifyFilter::TLinear);
        TWeakTextureInstance weakTexture = inout_resourceManager.GetTextureManager().MakeTexture("BlurTexture0", texture);
        const GU32 renderTextureHandle = weakTexture.lock()->GetTextureHandle();
        m_renderTexture0Id = inout_render.CreateRenderTextureTarget(
            renderTextureHandle,
            m_renderBufferWidth / 2,
            m_renderBufferHeight / 2,
            false,
            false
            );
    }
    {
        //GTexture texture(m_renderBufferWidth / 4, m_renderBufferHeight / 4, GTextureType::TType::TRenderTarget, 0);
        GTexture texture(m_renderBufferWidth / 4, m_renderBufferHeight / 4, GTextureType::TType::TRenderTarget, 0, GTextureType::TMagnifyFilter::TNearest, GTextureType::TWrapMode::TClamp, GTextureType::TMipmapMagnifyFilter::TLinear, GTextureType::TMipmapMinifyFilter::TLinear);
        TWeakTextureInstance weakTexture = inout_resourceManager.GetTextureManager().MakeTexture("BlurTexture1", texture);
        const GU32 renderTextureHandle = weakTexture.lock()->GetTextureHandle();
        m_renderTexture1Id = inout_render.CreateRenderTextureTarget(
            renderTextureHandle,
            m_renderBufferWidth / 4,
            m_renderBufferHeight / 4,
            false,
            false
            );
    }
    {
        //GTexture texture(m_renderBufferWidth / 8, m_renderBufferHeight / 8, GTextureType::TType::TRenderTarget, 0);
        GTexture texture(m_renderBufferWidth / 8, m_renderBufferHeight / 8, GTextureType::TType::TRenderTarget, 0, GTextureType::TMagnifyFilter::TNearest, GTextureType::TWrapMode::TClamp, GTextureType::TMipmapMagnifyFilter::TLinear, GTextureType::TMipmapMinifyFilter::TLinear);
        TWeakTextureInstance weakTexture = inout_resourceManager.GetTextureManager().MakeTexture("BlurTexture2", texture);
        const GU32 renderTextureHandle = weakTexture.lock()->GetTextureHandle();
        m_renderTexture2Id = inout_render.CreateRenderTextureTarget(
            renderTextureHandle,
            m_renderBufferWidth / 8,
            m_renderBufferHeight / 8,
            false,
            false
            );
    }

	//mMaterial
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TColourWrite |			
				GMaterialType::TState::TUseTexture                
			),
			"BlurTextureFullScreen",
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sShaderArrayUniformNameCount,
			sShaderArrayUniformName						
			);

		m_material = inout_resourceManager.GetMaterialManual(
			"GRenderBlurMaterial",
			sVertexShader,
			sFragmentShader,
			renderMaterialLoad,
			true,
			false
			);
	}
	//m_materialBlur0
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TOne,
			GMaterialType::TBlendMode::TOne,
			(	
				GMaterialType::TState::TColourWrite |			
				GMaterialType::TState::TUseTexture |
				GMaterialType::TState::TAlphaBlend
			),
			"BlurTexture0",
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sMultipleShaderArrayUniformNameCount,
			sMultipleShaderArrayUniformName						
			);

		m_materialBlur0 = inout_resourceManager.GetMaterialManual(
			"GRenderBlur0Material",
			sVertexShader,
			sFragmentMultipleShader,
			renderMaterialLoad,
			true,
			false
			);
	}
	//m_materialBlur1
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TOne,
			GMaterialType::TBlendMode::TOne,
			(	
				GMaterialType::TState::TColourWrite |			
				GMaterialType::TState::TUseTexture |
				GMaterialType::TState::TAlphaBlend
			),
			"BlurTexture1",
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sMultipleShaderArrayUniformNameCount,
			sMultipleShaderArrayUniformName						
			);

		m_materialBlur1 = inout_resourceManager.GetMaterialManual(
			"GRenderBlur1Material",
			sVertexShader,
			sFragmentMultipleShader,
			renderMaterialLoad,
			true,
			false
			);
	}
	//m_materialBlur2
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TOne,
			GMaterialType::TBlendMode::TOne,
			(	
				GMaterialType::TState::TColourWrite |			
				GMaterialType::TState::TUseTexture |
				GMaterialType::TState::TAlphaBlend
			),
			"BlurTexture2",
			0,
			0,
			sShaderArrayAttributeNameCount,
			sShaderArrayAttributeName,
			sMultipleShaderArrayUniformNameCount,
			sMultipleShaderArrayUniformName						
			);

		m_materialBlur2 = inout_resourceManager.GetMaterialManual(
			"GRenderBlur2Material",
			sVertexShader,
			sFragmentMultipleShader,
			renderMaterialLoad,
			true,
			false
			);
	}


	//mesh
	{
		TArrayStreamInfo arrayStreamLoad;
		GMeshStreamInfo::AppendStreamInfo(
			arrayStreamLoad,
			GMeshType::TStreamType::TFloat,
			2, 
			GMeshType::TStreamUsage::TPosition
			);
		GMeshStreamInfo::AppendStreamInfo(
			arrayStreamLoad,
			GMeshType::TStreamType::TFloat,
			2, 
			GMeshType::TStreamUsage::TUv0
			);

		m_mesh.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangleStrip,
			4,
			arrayStreamLoad
			));

		m_mesh->GetFloatRef(0, 0, 0) = -(DEBUG_MESH_VALUE);
		m_mesh->GetFloatRef(0, 0, 1) = -(DEBUG_MESH_VALUE);

		m_mesh->GetFloatRef(1, 0, 0) = -(DEBUG_MESH_VALUE);
		m_mesh->GetFloatRef(1, 0, 1) = (DEBUG_MESH_VALUE);

		m_mesh->GetFloatRef(2, 0, 0) = (DEBUG_MESH_VALUE);
		m_mesh->GetFloatRef(2, 0, 1) = -(DEBUG_MESH_VALUE);

		m_mesh->GetFloatRef(3, 0, 0) = (DEBUG_MESH_VALUE);
		m_mesh->GetFloatRef(3, 0, 1) = (DEBUG_MESH_VALUE);

		const float upperU = ((float)m_viewwportWidth) / ((float)m_renderBufferWidth);
		const float upperV = ((float)m_viewwportHeight) / ((float)m_renderBufferHeight);

		m_mesh->GetFloatRef(0, 1, 0) = 0.0F;
		m_mesh->GetFloatRef(0, 1, 1) = 0.0F;
		m_mesh->GetFloatRef(1, 1, 0) = 0.0F;
		m_mesh->GetFloatRef(1, 1, 1) = upperV;
		m_mesh->GetFloatRef(2, 1, 0) = upperU;
		m_mesh->GetFloatRef(2, 1, 1) = 0.0F;
		m_mesh->GetFloatRef(3, 1, 0) = upperU;
		m_mesh->GetFloatRef(3, 1, 1) = upperV;
	}

	return;
}

GRenderBlur::~GRenderBlur()
{
	return;
}

//public methods
//setup render
GVOID GRenderBlur::SetupRender(GRender& inout_render)
{
	inout_render.SetRenderTarget(m_renderTextureId);
	inout_render.SetViewport(0, 0, m_viewwportWidth, m_viewwportHeight);

	return;
}

//finish blur
GVOID GRenderBlur::FinishBlur(GRender& inout_render)
{
	//texture0
	inout_render.SetRenderTarget(m_renderTexture0Id);
	inout_render.SetViewport(0, 0, m_viewwportWidth / 2, m_viewwportHeight / 2);

	inout_render.SetCamera(*m_orthoCamera);
	inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	inout_render.SetMaterial(*m_material.lock());
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);

	//texture1
	inout_render.SetRenderTarget(m_renderTexture1Id);
	inout_render.SetViewport(0, 0, m_viewwportWidth / 4, m_viewwportHeight / 4);
	inout_render.SetCamera(*m_orthoCamera);
	inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	inout_render.SetMaterial(*m_material.lock());
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);

	//texture2
	inout_render.SetRenderTarget(m_renderTexture2Id);
	inout_render.SetViewport(0, 0, m_viewwportWidth / 8, m_viewwportHeight / 8);
	inout_render.SetCamera(*m_orthoCamera);
	inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	inout_render.SetMaterial(*m_material.lock());
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);


	//default frame buffer target
	inout_render.SetRenderTarget(inout_render.GetDefaultFrameBufferObject());
	inout_render.SetViewport(0, 0, m_width, m_height);

	//inout_render.RenderStart(true, GColour4Float::sGreen);
	inout_render.SetCamera(*m_orthoCamera);
	inout_render.SetObjectTransform(GMatrix16Float::sIdentity);
	inout_render.SetMaterial(*m_material.lock());

	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);

	//blur
	inout_render.SetMaterial(*m_materialBlur0.lock());
	inout_render.SetShaderUniform("u_mul", 0.1F);
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);
	inout_render.SetMaterial(*m_materialBlur1.lock());
	inout_render.SetShaderUniform("u_mul", 0.2F);
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);
	inout_render.SetMaterial(*m_materialBlur2.lock());
	inout_render.SetShaderUniform("u_mul", 0.4F);
	inout_render.DrawMesh(
		m_mesh->GetMesh(),
		m_mesh->GetMesh().GetVertexData()
		);

	return;
}

