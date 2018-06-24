//
//  GGuiNodeComponentPanel.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentPanel.h"

#include "GGuiNodeComponentPanelLoad.h"
#include "GGui.h"
#include "GGuiType.h"
#include "GGuiManager.h"
#include "GGuiNodeComponentVisual.h"
#include "GStringUtility.h"
#include "GMeshManual.h"
#include "GMeshType.h"
#include "GMeshStreamInfo.h"
#include "GColour4Byte.h"
#include "GColour4Float.h"
#include "GMaterial.h"
#include "GVector3Float.h"

struct TGuiNodeComponentPanelFlag
{
	enum TEnum
	{
		TNone		= 0x0000
		, TVisible	= 0x0001
	};
};

typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
typedef boost::weak_ptr<GMeshManual> TWeakMeshManual;
typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;
typedef std::vector<GMeshStreamInfo> TArrayStreamInfoLoad;
typedef std::vector<unsigned char> TArrayByte;
typedef std::vector<unsigned short> TArrayShort;

//static public methods
/*static*/ GGuiNodeComponentPanel::TPointerGuiNodeComponentBase GGuiNodeComponentPanel::Factory(
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
	const GGuiNodeComponentPanelLoad& panelLoad = *(GGuiNodeComponentPanelLoad*)in_data;

	TPointerRenderMeshManual pointerRenderMeshManual;
	TArrayFloat arrayAlpha;
	TArrayFloat arrayRatioHorizontal;
	TArrayFloat arrayRatioVertical;

	const bool hasUV = panelLoad.GetFlagUv();

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
	if (hasUV)
	{
		GMeshStreamInfo::AppendStreamInfo(
			arrayStreamInfoLoad,
			GMeshType::TStreamType::TFloat,
			2,
			GMeshType::TStreamUsage::TUv0
			);
	}

	const int vertexCount = 4;
	{
		TArrayByte vertexIndexArray;
		vertexIndexArray.push_back(0);
		vertexIndexArray.push_back(1);
		vertexIndexArray.push_back(2);
		vertexIndexArray.push_back(2);
		vertexIndexArray.push_back(1);
		vertexIndexArray.push_back(3);

		pointerRenderMeshManual.reset(new GMeshManual(
			GMeshType::TPrimitiveType::TTriangle,
			vertexIndexArray,
			vertexCount,
			arrayStreamInfoLoad
			));
	}
	GMeshManual& renderMeshManual = *pointerRenderMeshManual;

	GColour4Float colour[4];
	if (panelLoad.GetFlagColour1())
	{
		const GColour4Float panelColour(panelLoad.GetArrayColour()[0]);
		colour[0] = panelColour;
		colour[1] = panelColour;
		colour[2] = panelColour;
		colour[3] = panelColour;
	}
	else if (panelLoad.GetFlagColour4())
	{
		colour[0] = GColour4Float(panelLoad.GetArrayColour()[0]);
		colour[1] = GColour4Float(panelLoad.GetArrayColour()[1]);
		colour[2] = GColour4Float(panelLoad.GetArrayColour()[2]);
		colour[3] = GColour4Float(panelLoad.GetArrayColour()[3]);
	}
	else
	{
		colour[0] = GColour4Float::sWhite;
		colour[1] = GColour4Float::sWhite;
		colour[2] = GColour4Float::sWhite;
		colour[3] = GColour4Float::sWhite;
	}

	arrayAlpha.resize(vertexCount);
	for (int index = 0; index < 4; ++index)
	{
		arrayAlpha[index] = colour[index].m_alpha;
		*(GColour4Float*)(&renderMeshManual.GetFloatRef(index, 1, 0)) = colour[index];
	}

	if (hasUV)
	{
		const GVector2Float& uvTopLeft = panelLoad.GetArrayUVPair()[0];
		const GVector2Float& uvBottomRight = panelLoad.GetArrayUVPair()[1];

		*(GVector2Float*)(&renderMeshManual.GetFloatRef(0, 2, 0)) = uvTopLeft;
		*(GVector2Float*)(&renderMeshManual.GetFloatRef(1, 2, 0)) = GVector2Float(uvBottomRight.m_x, uvTopLeft.m_y);
		*(GVector2Float*)(&renderMeshManual.GetFloatRef(2, 2, 0)) = GVector2Float(uvTopLeft.m_x, uvBottomRight.m_y);
		*(GVector2Float*)(&renderMeshManual.GetFloatRef(3, 2, 0)) = uvBottomRight;
	}

	TWeakMaterialInstance weakMaterialInstance;
	if (hasUV)
	{
		weakMaterialInstance = inout_manager.GetMaterialColourTexture();
	}
	else
	{
		weakMaterialInstance = inout_manager.GetMaterialColour();
	}
	TPointerMaterialInstance materialInstance = weakMaterialInstance.lock();
	TPointerGuiNodeComponentVisual pointerVisual;
	pointerVisual.reset(new GGuiNodeComponentVisual(pointerRenderMeshManual, materialInstance, panelLoad.GetFlagVisible()));
	TWeakGuiNodeComponentVisual weakVisual = pointerVisual;
	{
		TPointerGuiNodeComponentBase pointerVisualBase = pointerVisual;
		inout_parent.AddComponent(pointerVisualBase);
	}

	pointer.reset(new GGuiNodeComponentPanel(
		GStringUtility::SafeString(panelLoad.GetName()),
		pointerRenderMeshManual,
		weakVisual,
		arrayAlpha,
		panelLoad.GetFlagVisible()
		));

	return pointer;
}

/*static*/ const unsigned int GGuiNodeComponentPanel::GetComponentFlag()
{
	return GGuiType::TComponentFlag::TPanel;
}

//constructor
GGuiNodeComponentPanel::GGuiNodeComponentPanel(
	const std::string& in_name,
	TPointerRenderMeshManual& in_renderMeshManual, //WARN: transferal of ownership
	TWeakGuiNodeComponentVisual& in_guiNodeComponentVisual,
	const TArrayFloat& in_arrayAlpha,
	const bool in_visible
	)
	: GGuiNodeComponentBase(GetComponentFlag())
	, mName(in_name)
	, mFlag(0)
	, mRenderMeshManual(in_renderMeshManual)
	, mAlpha(1.0F)
	, mArrayAlpha(in_arrayAlpha)
	, mVisual(in_guiNodeComponentVisual)
{
	SetVisible(in_visible);
	return;
}

/*virtual*/ GGuiNodeComponentPanel::~GGuiNodeComponentPanel()
{
	return;
}

//public methods
void GGuiNodeComponentPanel::Update(
	GGui& inout_gui, 
	GGuiNode& inout_node
	)
{
	if ((0 == (mFlag & TGuiNodeComponentPanelFlag::TVisible)) ||
		(!mRenderMeshManual))
	{
		return;
	}

	const GVector2Float& screenTopLeft = inout_node.GetTransformScreenTopLeft(inout_gui);
	const GVector2Float& screenBottomRight = inout_node.GetTransformScreenBottomRight(inout_gui);
	const GVector2Float deviceTopLeft = inout_gui.CalculateDeviceFromScreenCoordinates(screenTopLeft);
	const GVector2Float deviceBottomRight = inout_gui.CalculateDeviceFromScreenCoordinates(screenBottomRight);

	if ((mOldDeviceTopLeft == deviceTopLeft) &&
		(mOldDeviceBottomRight == deviceBottomRight))
	{
		return;
	}

	mOldDeviceTopLeft = deviceTopLeft;
	mOldDeviceBottomRight = deviceBottomRight;

	const GVector2Float deviceTopRight = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(screenBottomRight.m_x, screenTopLeft.m_y));
	const GVector2Float deviceBottomLeft = inout_gui.CalculateDeviceFromScreenCoordinates(GVector2Float(screenTopLeft.m_x, screenBottomRight.m_y));


	//update the mesh
	GMeshManual& renderMeshManual = *mRenderMeshManual;

	{
		GVector2Float& vertexPos = *(GVector2Float*)(&renderMeshManual.GetFloatRef(0, 0, 0));
		vertexPos = GVector2Float(deviceTopLeft.m_x, deviceTopLeft.m_y);
	}
	{
		GVector2Float& vertexPos = *(GVector2Float*)(&renderMeshManual.GetFloatRef(1, 0, 0));
		vertexPos = GVector2Float(deviceTopRight.m_x, deviceTopRight.m_y);
	}
	{
		GVector2Float& vertexPos = *(GVector2Float*)(&renderMeshManual.GetFloatRef(2, 0, 0));
		vertexPos = GVector2Float(deviceBottomLeft.m_x, deviceBottomLeft.m_y);
	}
	{
		GVector2Float& vertexPos = *(GVector2Float*)(&renderMeshManual.GetFloatRef(3, 0, 0));
		vertexPos = GVector2Float(deviceBottomRight.m_x, deviceBottomRight.m_y);
	}

	return;
}

//public accessors
const bool GGuiNodeComponentPanel::GetVisible()const
{
	return (0 != (mFlag & TGuiNodeComponentPanelFlag::TVisible));
}

void GGuiNodeComponentPanel::SetVisible(const bool in_visible)
{
	if (in_visible)
	{
		mFlag |= TGuiNodeComponentPanelFlag::TVisible;
	}
	else
	{
		mFlag &= ~TGuiNodeComponentPanelFlag::TVisible;
	}
	TPointerGuiNodeComponentVisual pointerVisual = mVisual.lock();
	if (pointerVisual)
	{
		pointerVisual->SetVisible(in_visible);
	}

	return;
}

void GGuiNodeComponentPanel::SetAlpha(const float in_alpha)
{
	if ((mAlpha == in_alpha) ||
		(!mRenderMeshManual))
	{
		return;
	}

	mAlpha = in_alpha;

	GMeshManual& renderMeshManual = *mRenderMeshManual;
	for (int index = 0; index < (int)mArrayAlpha.size(); ++index)
	{
		renderMeshManual.GetFloatRef(index, 1, 3) = (mAlpha * mArrayAlpha[index]);
	}

	return;
}
