//
//  GGuiManager.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiManager.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GFileManager.h"
#include "GGui.h"
#include "GGuiAnimationManager.h"
#include "GGuiFontManager.h"
#include "GColour4Float.h"
#include "GMaterial.h"
#include "GMaterialInstance.h"
#include "GMaterialType.h"
#include "GResourceManager.h"
#include "GApplicationWindowType.h"

typedef boost::shared_ptr<GBuffer> TPointerAsset;
typedef boost::weak_ptr<GBuffer> TWeakAsset;

static const char* const sVertexShaderColour = "\n\
#version 100\n\
\n\
uniform mat4 u_mvpMatrix;\n\
attribute vec3 a_position;\n\
attribute vec4 a_colour;\n\
varying vec4 v_colour;\n\
\n\
void main()\n\
{\n\
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);\n\
	v_colour = a_colour;\n\
}\n\
";

static const char* const sFragmentShaderColour = "\n\
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
varying LOWP vec4 v_colour;\n\
uniform LOWP vec4 u_colour;\n\
void main()\n\
{\n\
	gl_FragColor = v_colour * u_colour;\n\
}\n\
";

static const char* sColourArrayAttributeName[] = {
	"a_position",
	"a_colour"
};
static const int sColourArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sColourArrayAttributeName);

static const char* sColourArrayUniformName[] = {
	"u_mvpMatrix",
	"u_colour"
};
static const int sColourArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sColourArrayUniformName);

static const char* const sVertexShaderColourTexture = "\n\
#version 100\n\
\n\
uniform mat4 u_mvpMatrix;\n\
attribute vec3 a_position;\n\
attribute vec4 a_colour;\n\
attribute vec2 a_uv0;\n\
varying vec4 v_colour;\n\
varying vec2 v_uv0;\n\
\n\
void main()\n\
{\n\
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);\n\
	v_colour = a_colour;\n\
	v_uv0 = a_uv0;\n\
}\n\
";

static const char* const sFragmentShaderColourTexture = "\n\
#version 100\n\
#ifdef GL_ES\n\
	#define LOWP lowp\n\
	#define MEDIUMP mediump\n\
	#define HIGHP highp\n\
#else\n\
	#define LOWP\n\
	#define MEDIUMP\n\
	#define HIGHP\n\
#endif\n\
varying LOWP vec4 v_colour;\n\
varying LOWP vec2 v_uv0;\n\
uniform sampler2D u_texture2d_0;\n\
uniform LOWP vec4 u_colour;\n\
void main()\n\
{\n\
	gl_FragColor = texture2D(u_texture2d_0, v_uv0) * v_colour * u_colour;\n\
}\n\
";
	//gl_FragColor = vec4(v_uv0.x, v_uv0.y, 0.0, 1.0);\n\
	//gl_FragColor = texture2D(u_texture2d_0, v_uv0);\n\
	//gl_FragColor = texture2D(u_texture2d_0, v_uv0) * v_colour;\n\

static const char* sColourTextureArrayAttributeName[] = {
        "a_position",
        "a_colour",
        "a_uv0"
};
static const int sColourTextureArrayAttributeNameCount = GCOMMON_ARRAY_SIZE(sColourTextureArrayAttributeName);

static const char* sColourTextureArrayUniformName[] = {
	"u_mvpMatrix",
	"u_colour"
};
static const int sColourTextureArrayUniformNameCount = GCOMMON_ARRAY_SIZE(sColourTextureArrayUniformName);

//constructor
GGuiManager::GGuiManager(
	TPointerResourceManager& inout_resourceManager,
	const std::string& in_textureName
	)
	: mResourceManager(inout_resourceManager)
	, mMapNameGui()
	, mMaterialColour()
	, mMaterialColourTexture()
	, mAnimationManager()
	, mFontManager()
	, mSizeChangeIndex(0)
	, mScreenSize()
	, mScreenOrientation(GApplicationWindowType::TOrientation::TCount)
	//, mDeviceAxisX()
	//, mDeviceAxisY()
{
	mFontManager.reset(new GGuiFontManager(inout_resourceManager));

	//material colour
	{
		GMaterial renderMaterialLoad(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TAlphaBlend |
				GMaterialType::TState::TColourWrite |
				GMaterialType::TState::TDoBasePass |				
				//GMaterialType::TState::TFaceCull |
				GMaterialType::TState::TFaceCW
			),
			"",
			"",
			"",
			sColourArrayAttributeNameCount,
			sColourArrayAttributeName,
			sColourArrayUniformNameCount,
			sColourArrayUniformName						
			);

		mMaterialColour = inout_resourceManager->GetMaterialManual(
			"GuiMaterialColour",
			sVertexShaderColour,
			sFragmentShaderColour,
			renderMaterialLoad,
			true,
			false
			);
	}

	//material colour texture
	{
		GMaterial renderMaterialLoad2(
			GColour4Float::sWhite,
			GMaterialType::TBlendMode::TSrcAlpha,
			GMaterialType::TBlendMode::TOneMinusSrcAlpha,
			(	
				GMaterialType::TState::TAlphaBlend |
				GMaterialType::TState::TColourWrite |
				//GMaterialType::TState::TFaceCull |
				GMaterialType::TState::TFaceCW |
				GMaterialType::TState::TUseTexture
			),
			in_textureName.c_str(),
			"",
			"",
			sColourTextureArrayAttributeNameCount,
			sColourTextureArrayAttributeName,
			sColourTextureArrayUniformNameCount,
			sColourTextureArrayUniformName						
			);

		mMaterialColourTexture = inout_resourceManager->GetMaterialManual(
			"GuiMaterialColourTexture",//"GuiMaterialColourTexture", //"axes_material.data", //"GuiMaterialColourTexture",
			sVertexShaderColourTexture,
			sFragmentShaderColourTexture,
			renderMaterialLoad2,
			true,
			false
			);
	}

	return;
}

GGuiManager::~GGuiManager()
{
	return;
}

//public methods
GGuiManager::TWeakGui GGuiManager::CreateGui(const std::string& in_name, const GGuiLoad& in_guiLoad)
{
	TMapStringGui::iterator iterator = mMapNameGui.find(in_name);
	if (iterator != mMapNameGui.end())
	{
		return (*iterator).second;
	}

	TPointerGui pointerGui = GGui::Factory(in_guiLoad, *this);
	mMapNameGui[in_name] = pointerGui;
	TWeakGui weakGui = pointerGui;
	return weakGui;
}

GGuiManager::TWeakGui GGuiManager::CreateBlankGui(const std::string& in_name)
{
	TMapStringGui::iterator iterator = mMapNameGui.find(in_name);
	if (iterator != mMapNameGui.end())
	{
		return (*iterator).second;
	}

	TPointerGui pointerGui;
	pointerGui.reset(new GGui(*this));
	mMapNameGui[in_name] = pointerGui;
	TWeakGui weakGui = pointerGui;
	return weakGui;
}

GGuiManager::TWeakGui GGuiManager::LoadGui(const std::string& in_name)
{
	TMapStringGui::iterator iterator = mMapNameGui.find(in_name);
	if (iterator != mMapNameGui.end())
	{
		return (*iterator).second;
	}

	TWeakGui weakGui;
	TPointerResourceManager resourceManager = mResourceManager.lock();
	if (!resourceManager)
	{
		return weakGui;
	}

	TPointerAsset pointerAsset = resourceManager->GetFileManager().LoadUnmanagedFile(in_name, true, true);
	if (!pointerAsset)
	{
		return weakGui;
	}

	const GGuiLoad& guiLoad = GBufferUtility::GetLoadBufferRoot<GGuiLoad>(*pointerAsset);
	weakGui = CreateGui(in_name, guiLoad);

	return weakGui;
}

GGuiManager::TWeakGui GGuiManager::FindGui(const std::string& in_name)
{
	TMapStringGui::iterator iterator = mMapNameGui.find(in_name);
	if (iterator != mMapNameGui.end())
	{
		return (*iterator).second;
	}

	TWeakGui weakGui;
	return weakGui;
}

void GGuiManager::DestroyGui(const std::string& in_name)
{
	TMapStringGui::iterator iterator = mMapNameGui.find(in_name);
	if (iterator != mMapNameGui.end())
	{
		mMapNameGui.erase(iterator);
	}

	return;
}

void GGuiManager::DestroyGui(const TWeakGui& in_gui)
{
	TPointerGui gui = in_gui.lock();
	if (!gui)
	{
		return;
	}

	for (TMapStringGui::iterator iterator = mMapNameGui.begin(); iterator != mMapNameGui.end(); ++iterator)
	{
		if (gui == (*iterator).second)
		{
			mMapNameGui.erase(iterator);
			return;
		}
	}

	return;
}

void GGuiManager::SetScreen(
	const GVector2Float& in_screenSize,
	const GApplicationWindowType::TOrientation::TEnum in_screenOrientation		
	)
{
	if ((mScreenSize == in_screenSize) &&
		(mScreenOrientation == in_screenOrientation))
	{
		return;
	}
	mSizeChangeIndex += 1;

	//init case, set size
	if (GApplicationWindowType::TOrientation::TCount == mScreenOrientation)
	{
		mScreenSize = in_screenSize;
	}

	//don't change orientation on faceup or facedown
	switch (in_screenOrientation)
	{
	default:
		break;
	case GApplicationWindowType::TOrientation::TTwelveOclock:
	case GApplicationWindowType::TOrientation::TSixOclock:
	case GApplicationWindowType::TOrientation::TThreeOclock:
	case GApplicationWindowType::TOrientation::TNineOclock:
		mScreenOrientation = in_screenOrientation;
		break;
	}

	switch (mScreenOrientation)
	{
	default:
	case GApplicationWindowType::TOrientation::TTwelveOclock:
	case GApplicationWindowType::TOrientation::TSixOclock:
		mScreenSize = in_screenSize;
		break;
	case GApplicationWindowType::TOrientation::TThreeOclock:
	case GApplicationWindowType::TOrientation::TNineOclock:
		mScreenSize.m_x = in_screenSize.m_y;
		mScreenSize.m_y = in_screenSize.m_x;
		break;
	}

	return;
}

void GGuiManager::SetGuiColour(const GColour4Float& in_colour)
{
	TPointerMaterialInstance pointerMaterialColour = mMaterialColour.lock();
	TPointerMaterialInstance pointerMaterialColourTexture = mMaterialColourTexture.lock();
	if (pointerMaterialColour)
	{
		pointerMaterialColour->SetColour(in_colour);
	}
	if (pointerMaterialColourTexture)
	{
		pointerMaterialColourTexture->SetColour(in_colour);
	}
	return;
}

const GVector2Float GGuiManager::GetDeviceCoordinate(const GVector2Float& in_location)const
{
	const GVector2Float point(
		((in_location.m_x / mScreenSize.m_x) - 0.5F) * 2.0F, 
		((in_location.m_y / mScreenSize.m_y) - 0.5F) * (-2.0F)
		);
	//return (mDeviceAxisX * point.m_x) + (mDeviceAxisY * point.m_y);
	return point;
}

const GVector2Float GGuiManager::GetInputCoordinate(const GVector2Float& in_location)const
{
	const GVector2Float point(
		(in_location.m_x / mScreenSize.m_x), 
		((in_location.m_y / mScreenSize.m_y))
		);
	//return (mDeviceAxisX * point.m_x) + (mDeviceAxisY * point.m_y) + GVector2Float(0.5F, 0.5F);
	return point;
}

/*
void GGuiManager::SetScreenOrientation(const GApplicationWindowType::TOrientation::TEnum in_screenOrientation)
{
	mScreenOrientation = in_screenOrientation;

	switch (mScreenOrientation)
	{
	default:
	case GApplicationWindowType::TOrientation::TTwelveOclock:
		mDeviceAxisX.SetData(1.0F, 0.0F);
		mDeviceAxisY.SetData(0.0F, -1.0F);
		break;
	case GApplicationWindowType::TOrientation::TThreeOclock:
		mDeviceAxisX.SetData(0.0F, 1.0F);
		mDeviceAxisY.SetData(1.0F, 0.0F);
		break;
	case GApplicationWindowType::TOrientation::TSixOclock:
		mDeviceAxisX.SetData(-1.0F, 0.0F);
		mDeviceAxisY.SetData(0.0F, 1.0F);
		break;
	case GApplicationWindowType::TOrientation::TNineOclock:
		mDeviceAxisX.SetData(0.0F, -1.0F);
		mDeviceAxisY.SetData(-1.0F, 0.0F);
		break;
	}
	return;
}
*/