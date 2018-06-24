//
//  GGuiNodeComponentText.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentText.h"

#include "GGui.h"
#include "GGuiType.h"
#include "GGuiFont.h"
#include "GGuiFontDataLoad.h"
#include "GGuiNode.h"
#include "GGuiNodeComponentVisual.h"
#include "GGuiNodeComponentTextLoad.h"
#include "GStringUtility.h"
#include "GMeshManual.h"
#include "GMeshType.h"
#include "GMeshStreamInfo.h"
#include "GColour4Byte.h"
#include "GColour4Float.h"
#include "GMaterial.h"
#include "GVector3Float.h"
#include "GGuiManager.h"
#include "GGuiFontManager.h"

struct TGuiNodeComponentTextFlag
{
	enum TEnum
	{
		TNone				= 0x0000
		, TVisible			= 0x0001
		, TMeshDirtyPos		= 0x0002
		//, TMeshDirtyAlpha	= 0x0004	
		, TMeshDirtyColour	= 0x0008	

	};
};

typedef boost::shared_ptr<GGuiNodeComponentText> TPointerGuiNodeComponentText;
typedef boost::shared_ptr<GGuiNodeComponentVisual> TPointerGuiNodeComponentVisual;
typedef boost::shared_ptr<GMeshManual> TPointerRenderMeshManual;
typedef boost::weak_ptr<GMeshManual> TWeakRenderMeshManual;
typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;
typedef std::vector<GMeshStreamInfo> TArrayStreamInfoLoad;
typedef std::vector<unsigned char> TArrayByte;
typedef std::vector<unsigned short> TArrayShort;

//static void LocalUpdateAlpha(
//	TPointerRenderMeshManual& in_renderMeshManual,
//	const float in_alpha
//	)
//{
//	if (!in_renderMeshManual)
//	{
//		return;
//	}
//
//	GMeshManual& renderMeshManual = *in_renderMeshManual;
//	const int vertexCount = (renderMeshManual.GetMaxElementCount() / 6) * 4;
//	for (int index = 0; index < vertexCount; ++index)
//	{
//		renderMeshManual.GetFloatRef(index, 1, 3) = in_alpha;
//	}
//
//	return;
//}

static void LocalUpdateColour(
	TPointerRenderMeshManual& in_renderMeshManual,
	const float in_red,
	const float in_green,
	const float in_blue,
	const float in_alpha
	)
{
	if (!in_renderMeshManual)
	{
		return;
	}

	GMeshManual& renderMeshManual = *in_renderMeshManual;
	const int vertexCount = (renderMeshManual.GetMaxElementCount() / 6) * 4;
	for (int index = 0; index < vertexCount; ++index)
	{
		float* const pData = &renderMeshManual.GetFloatRef(index, 1, 0);
		pData[0] = in_red;
		pData[1] = in_green;
		pData[2] = in_blue;
		pData[3] = in_alpha;
	}

	return;
}

static void LocalAddFontData(
	GMeshManual& inout_renderMeshManual,
	int& inout_vertexTrace,
	const GGuiFontDataLoad& in_fontData,
	const float in_scale,
	const GColour4Float& in_colour,
	const GVector2Float& in_origin,
	GGui& inout_gui
	)
{
	if (!in_fontData.GetFlagVisible())
	{
		return;
	}

	const GVector2Float posLeftTop = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(
		in_origin.m_x,
		in_origin.m_y - (in_fontData.GetBaseLine() * in_scale)
		));
	const GVector2Float posRightTop = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(
		in_origin.m_x + (in_fontData.GetSize().m_x * in_scale),
		in_origin.m_y - (in_fontData.GetBaseLine() * in_scale)
		));

	const GVector2Float posLeftBottom = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(
		in_origin.m_x,
		in_origin.m_y + ((in_fontData.GetSize().m_y - in_fontData.GetBaseLine()) * in_scale)
		));
	const GVector2Float posRightBottom = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(
		in_origin.m_x + (in_fontData.GetSize().m_x * in_scale),
		in_origin.m_y + ((in_fontData.GetSize().m_y - in_fontData.GetBaseLine()) * in_scale)
		));

	//vertex0 pos
	{
		float* const pData = &inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 0, 0, 0);
		pData[0] = posLeftTop.m_x;
		pData[1] = posLeftTop.m_y;
	}
	//vertex0 colour
	{
		*(GColour4Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 0, 1, 0) = in_colour;
	}
	//vertex0 uv
	{
		*(GVector2Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 0, 2, 0) = in_fontData.GetUVTopLeft();
	}

	//vertex1 pos
	{
		float* const pData = &inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 1, 0, 0);
		pData[0] = posRightTop.m_x;
		pData[1] = posRightTop.m_y;
	}
	//vertex1 colour
	{
		*(GColour4Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 1, 1, 0) = in_colour;
	}
	//vertex1 uv
	{
		*(GVector2Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 1, 2, 0) = GVector2Float(
			in_fontData.GetUVBottomRight().m_x,
			in_fontData.GetUVTopLeft().m_y
			);
	}

	//vertex2 pos
	{
		float* const pData = &inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 2, 0, 0);
		pData[0] = posLeftBottom.m_x;
		pData[1] = posLeftBottom.m_y;
	}
	//vertex2 colour
	{
		*(GColour4Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 2, 1, 0) = in_colour;
	}
	//vertex2 uv
	{
		*(GVector2Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 2, 2, 0) = GVector2Float(
			in_fontData.GetUVTopLeft().m_x,
			in_fontData.GetUVBottomRight().m_y
			);
	}

	//vertex3 pos
	{
		float* const pData = &inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 3, 0, 0);
		pData[0] = posRightBottom.m_x;
		pData[1] = posRightBottom.m_y;
	}
	//vertex3 colour
	{
		*(GColour4Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 3, 1, 0) = in_colour;
	}
	//vertex3 uv
	{
		*(GVector2Float*)&inout_renderMeshManual.GetFloatRef(inout_vertexTrace + 3, 2, 0) = in_fontData.GetUVBottomRight();
	}

	inout_vertexTrace += 4;
	return;
}


//static public methods
/*static*/ GGuiNodeComponentText::TPointerGuiNodeComponentBase GGuiNodeComponentText::Factory(
	const void* const in_data,
	GGuiManager& inout_manager,
	GGuiNode& inout_parent,
	TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
	)
{
	TPointerGuiNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}
	const GGuiNodeComponentTextLoad& textLoad = *(GGuiNodeComponentTextLoad*)in_data;

	TPointerRenderMeshManual renderMeshManual;
	TWeakGuiFont guiFont = inout_manager.GetFontManager()->GetFont(GStringUtility::SafeString(textLoad.GetFontName()));

	TArrayStreamInfoLoad arrayStreamInfoLoad;
	GMeshStreamInfo::AppendStreamInfo(
		arrayStreamInfoLoad,
		GMeshType::TStreamType::TFloat,
		2,
		GMeshType::TStreamUsage::TPosition
		);
	GMeshStreamInfo::AppendStreamInfo(
		arrayStreamInfoLoad,
		GMeshType::TStreamType::TFloat,
		4,
		GMeshType::TStreamUsage::TColour
		);
	GMeshStreamInfo::AppendStreamInfo(
		arrayStreamInfoLoad,
		GMeshType::TStreamType::TFloat,
		2,
		GMeshType::TStreamUsage::TUv0
		);

	const int maxCount = textLoad.GetMaximumCharCount();
	const int vertexCount = maxCount * 4;
	if (maxCount <= 64)
	{
		TArrayByte vertexIndexArray;
		for (int index = 0; index < maxCount; ++index)
		{
			vertexIndexArray.push_back((index* 4) + 0);
			vertexIndexArray.push_back((index* 4) + 1);
			vertexIndexArray.push_back((index* 4) + 2);
			vertexIndexArray.push_back((index* 4) + 2);
			vertexIndexArray.push_back((index* 4) + 1);
			vertexIndexArray.push_back((index* 4) + 3);
		}

		renderMeshManual.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangle,
			vertexIndexArray,
			vertexCount,
			arrayStreamInfoLoad
			));
	}
	else
	{
		TArrayShort vertexIndexArray;
		for (int index = 0; index < maxCount; ++index)
		{
			vertexIndexArray.push_back((index* 4) + 0);
			vertexIndexArray.push_back((index* 4) + 1);
			vertexIndexArray.push_back((index* 4) + 2);
			vertexIndexArray.push_back((index* 4) + 2);
			vertexIndexArray.push_back((index* 4) + 1);
			vertexIndexArray.push_back((index* 4) + 3);
		}

		renderMeshManual.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangle,
			vertexIndexArray,
			vertexCount,
			arrayStreamInfoLoad
			));
	}

	renderMeshManual->SetElementCount(0);

	TPointerMaterialInstance materialInstance = inout_manager.GetMaterialColourTexture().lock();

	TPointerGuiNodeComponentVisual pointerVisual;
	pointerVisual.reset(new GGuiNodeComponentVisual(renderMeshManual, materialInstance, textLoad.GetFlagVisible()));
	TWeakGuiNodeComponentVisual weakVisual = pointerVisual;
	{
		TPointerGuiNodeComponentBase pointerVisualBase = pointerVisual;
		inout_parent.AddComponent(pointerVisualBase);
	}

	TPointerGuiNodeComponentText pointerText;
	pointerText.reset(new GGuiNodeComponentText(
		GStringUtility::SafeString(textLoad.GetName()),
		maxCount,
		renderMeshManual,
		weakVisual,
		guiFont,
		GColour4Float(textLoad.GetColour()),
		textLoad.GetScale(),
		textLoad.GetHorizontalCharPad(),
		textLoad.GetFlagVisible()
		));

	if (textLoad.GetText())
	{
		pointerText->SetText(textLoad.GetText());
	}
	pointer = pointerText;

	return pointer;
}

/*static*/ const unsigned int GGuiNodeComponentText::GetComponentFlag()
{
	return GGuiType::TComponentFlag::TText;
}

//constructor
GGuiNodeComponentText::GGuiNodeComponentText(
	const std::string& in_name,
	const GS32 in_maxCharCount,
	TPointerRenderMeshManual& in_renderMeshManual, //WARN: transferal of ownership
	TWeakGuiNodeComponentVisual& in_guiNodeComponentVisual,
	TWeakGuiFont& in_guiFont,
	const GColour4Float& in_colour,
	const float in_scale,
	const float in_horizontalCharPad,
	const bool in_visible	
	)
: GGuiNodeComponentBase(GetComponentFlag())
, mName(in_name)
, mMaxCharCount(in_maxCharCount)
, mFlag(0)
, mRenderMeshManual(in_renderMeshManual)
, mColour(in_colour)
, mScale(in_scale)
, mHorizontalCharPad(in_horizontalCharPad)
, mVisual(in_guiNodeComponentVisual)
, mFont(in_guiFont)
, mArrayFontDataIndex()
, mParentTransformChangeId(0)
, m_alpha(1.0F)
{
	SetVisible(in_visible);
	return;
}

/*virtual*/ GGuiNodeComponentText::~GGuiNodeComponentText()
{
	return;
}

//public methods
void GGuiNodeComponentText::Update(
	GGui& inout_gui, 
	GGuiNode& inout_node
	)
{
	if (mParentTransformChangeId != inout_node.GetTransformChangeId())
	{
		mParentTransformChangeId = inout_node.GetTransformChangeId();
		mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyPos;
	}

	if (0 != (mFlag & TGuiNodeComponentTextFlag::TMeshDirtyPos))
	{
		UpdateMesh(inout_gui, inout_node);
		//| TGuiNodeComponentTextFlag::TMeshDirtyAlpha
		mFlag &= ~(TGuiNodeComponentTextFlag::TMeshDirtyPos  | TGuiNodeComponentTextFlag::TMeshDirtyColour);
	}
		
	if (0 != (mFlag & TGuiNodeComponentTextFlag::TMeshDirtyColour))
	{
		LocalUpdateColour(
			mRenderMeshManual,
			mColour.m_red,
			mColour.m_green,
			mColour.m_blue,
			mColour.m_alpha * m_alpha
			);
		mFlag &= ~TGuiNodeComponentTextFlag::TMeshDirtyColour;
	}

	return;
}

//public accessors
const bool GGuiNodeComponentText::GetVisible()const
{
	return (0 != (mFlag & TGuiNodeComponentTextFlag::TVisible));
}

void GGuiNodeComponentText::SetVisible(const bool in_visible)
{
	if (in_visible == GetVisible())
	{
		return;
	}
	if (in_visible)
	{
		mFlag |= TGuiNodeComponentTextFlag::TVisible;
	}
	else
	{
		mFlag &= ~TGuiNodeComponentTextFlag::TVisible;
	}

	TPointerGuiNodeComponentVisual pointerVisual = mVisual.lock();
	if (pointerVisual)
	{
		pointerVisual->SetVisible(in_visible);
	}

	return;
}

void GGuiNodeComponentText::SetAlpha(const float in_alpha)
{
	if (m_alpha == in_alpha)
	{
		return;
	}
	m_alpha = in_alpha;

	mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyColour;
	
	return;
}

void GGuiNodeComponentText::SetScale(const GR32 in_scale)
{
	if (mScale == in_scale)
	{
		return;
	}
	mScale = in_scale;

	mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyPos;
	
	return;
}


void GGuiNodeComponentText::SetColour(const GColour4Float& in_colour)
{
	if (mColour == in_colour)
	{
		return;
	}
	mColour = in_colour;

	mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyColour;
	//mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyAlpha;

	return;
}

void GGuiNodeComponentText::SetText(const std::string& in_text)
{
	TPointerGuiFont pointerFont = mFont.lock();
	if (!pointerFont)
	{
		return;
	}
	GGuiFont& guiFont = *pointerFont;
	TArrayInt arrayFontDataIndex;
	for (std::string::const_iterator iterator = in_text.begin(); iterator != in_text.end(); ++iterator)
	{
		const int fontDataIndex = guiFont.GetIndexForChar(*iterator);
		arrayFontDataIndex.push_back(fontDataIndex);
	}

	SetArrayFontDataIndex(arrayFontDataIndex);

	return;
}

void GGuiNodeComponentText::SetText(const TArrayInt& in_text)
{
	TPointerGuiFont pointerFont = mFont.lock();
	if (!pointerFont || (0 == mMaxCharCount))
	{
		return;
	}
	GGuiFont& guiFont = *pointerFont;
	TArrayInt arrayFontDataIndex;
	for (TArrayInt::const_iterator iterator = in_text.begin(); iterator != in_text.end(); ++iterator)
	{
		const int fontDataIndex = guiFont.GetIndexForChar(*iterator);
		arrayFontDataIndex.push_back(fontDataIndex);
	}

	SetArrayFontDataIndex(arrayFontDataIndex);

	return;
}

//private methods
void GGuiNodeComponentText::SetArrayFontDataIndex(const TArrayInt& in_arrayFontDataIndex)
{
	if (mArrayFontDataIndex == in_arrayFontDataIndex)
	{
		return;
	}
	mFlag |= TGuiNodeComponentTextFlag::TMeshDirtyPos;

	mArrayFontDataIndex.resize(0);
	for (GS32 index = 0; index < (GS32)in_arrayFontDataIndex.size(); ++index)
	{
		if (mMaxCharCount <= index)
		{
			break;
		}
		mArrayFontDataIndex.push_back(in_arrayFontDataIndex[index]);
	}

	return;
}

void GGuiNodeComponentText::UpdateMesh(GGui& inout_gui, GGuiNode& inout_node)
{
	TPointerGuiFont pointerGuiFont = mFont.lock();
	if (!mRenderMeshManual || !pointerGuiFont)
	{
		return;
	}

	GMeshManual& renderMeshManual = *mRenderMeshManual;
	GGuiFont& guiFont = *pointerGuiFont;

	float traceHorizontal = 0.0F;
	float traceVertical = guiFont.GetLineVerticalPad() * mScale;
	const GVector2Float& topLeft = inout_node.GetTransformScreenTopLeft(inout_gui);
	int vertexTrace = 0;
	const GColour4Float colour(
		mColour.m_red, 
		mColour.m_green, 
		mColour.m_blue, 
		mColour.m_alpha * m_alpha
		);

	for (TArrayInt::iterator iterator = mArrayFontDataIndex.begin(); iterator != mArrayFontDataIndex.end(); ++iterator)
	{
		const GGuiFontDataLoad* const pFontData = guiFont.GetIndexGliph(*iterator);
		if (!pFontData)
		{
			continue;
		}

		LocalAddFontData(
			renderMeshManual,
			vertexTrace,
			*pFontData,
			mScale,
			colour,
			topLeft + GVector2Float(traceHorizontal, traceVertical),
			inout_gui
			);

		traceHorizontal += (pFontData->GetSize().m_x * mScale) + mHorizontalCharPad;
	}
	renderMeshManual.SetElementCount((vertexTrace / 4) * 6);

	return;
}
