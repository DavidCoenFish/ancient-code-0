//
//  GGuiVisitorInput.cpp
//
//  Created by David Coen on 2011 04 22
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiVisitorInput.h"

#include "GGui.h"
#include "GGuiNode.h"
#include "GInput.h"
#include "GInputTouch.h"
#include "GGuiNodeComponentButton.h"
#include "GMath.h"

static const float sInputPad = 3.0F;

//public static methods
/*static*/ void GGuiVisitorInput::Run(
	GGui& inout_gui, 
	const GInput& in_input
	)
{
	const GApplicationWindowType::TOrientation::TEnum orientation = inout_gui.GetScreenOrientation();
	const int touchCount = in_input.GetTouchCount();
	for (int index = 0; index < touchCount; ++index)
	{
		const GInputTouch& touch = in_input.GetTouch(index);
		const GVector2Float touchLocation = touch.GetTouchLocationOrientation(orientation);
		GVector2Float foundClickRatio; 
		GGuiVisitorInput guiVisitorInput(inout_gui, touchLocation, foundClickRatio);
		inout_gui.QueryAll(guiVisitorInput, GGuiNodeComponentButton::GetComponentFlag());

		TPointerGuiNodeComponentButton foundButton = guiVisitorInput.GetWeakFoundButton().lock();
		if (foundButton)
		{
			if (touch.GetTouchBegin())
			{
				foundButton->ClickDown(touchLocation, touch.GetTouchId(), foundClickRatio);
			}
			else
			{
				foundButton->ClickDrag(touchLocation, touch.GetTouchId(), foundClickRatio);
			}
		}
	}

	return;
}

//constructor
GGuiVisitorInput::GGuiVisitorInput(
	GGui& inout_gui, 
	const GVector2Float& in_touchLocation,
	GVector2Float& out_foundClickRatio
	)
	: mGui(inout_gui)
	, m_foundClickRatio(out_foundClickRatio)
	, mTouchLocation(in_touchLocation)
	, mFound(false)
{
	return;
}

GGuiVisitorInput::~GGuiVisitorInput()
{
	return;
}

//public methods
void GGuiVisitorInput::QuerySubmitNode(
	TPointerGuiNode& inout_node
	)
{
	if (mFound)
	{
		return;
	}

	GGuiNode& node = *inout_node;

	//is touch in range of this node
#if 0
	const GVector2Float pad = mGui.CalculateInputCoordsFromScreenCoordinate(GVector2Float(sInputPad, sInputPad));
	const GVector2Float boundOne = mGui.CalculateInputCoordsFromScreenCoordinate(inout_node.GetTransformScreenTopLeft(mGui));
	const GVector2Float boundTwo = mGui.CalculateInputCoordsFromScreenCoordinate(inout_node.GetTransformScreenBottomRight(mGui)); 
	const GVector2Float topLeft(
		GMath::Minimum(boundOne.m_x, boundTwo.m_x) - pad.m_x,
		GMath::Minimum(boundOne.m_y, boundTwo.m_y) - pad.m_y
		);
	const GVector2Float bottomRight(
		GMath::Maximum(boundOne.m_x, boundTwo.m_x) + pad.m_x,
		GMath::Maximum(boundOne.m_y, boundTwo.m_y) + pad.m_y
		);
#else
	const GVector2Float topLeft = mGui.CalculateInputCoordsFromScreenCoordinate(node.GetTransformScreenTopLeft(mGui) - GVector2Float(sInputPad, sInputPad));
	const GVector2Float bottomRight = mGui.CalculateInputCoordsFromScreenCoordinate(node.GetTransformScreenBottomRight(mGui) + GVector2Float(sInputPad, sInputPad)); 
#endif

	if ((topLeft.m_x <= mTouchLocation.m_x) &&
		(mTouchLocation.m_x <= bottomRight.m_x) &&
		(topLeft.m_y <= mTouchLocation.m_y) &&
		(mTouchLocation.m_y <= bottomRight.m_y))
	{
		const int count = node.GetComponentCount();
		for (int index = 0; index < count; ++index)
		{
			TPointerGuiNodeComponentButton pointerButton = node.GetComponentPointer<GGuiNodeComponentButton>(index);
			if (!pointerButton || !pointerButton->GetActive())
			{
				continue;
			}

			mWeakFoundButton = pointerButton;
			mFound = true;
			m_foundClickRatio.m_x = (mTouchLocation.m_x - topLeft.m_x) / (bottomRight.m_x - topLeft.m_x);
			m_foundClickRatio.m_y = (mTouchLocation.m_y - topLeft.m_y) / (bottomRight.m_y - topLeft.m_y);
			//pButton->
		}
	}

	return;
}
