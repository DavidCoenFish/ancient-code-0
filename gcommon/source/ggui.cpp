//
//  GGui.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGui.h"

#include "GGuiLoad.h"
#include "GGuiNode.h"
#include "GGuiNodeLoad.h"
#include "GGuiManager.h"
#include "GGuiNodeComponentBase.h"
#include "GGuiNodeComponentPostLoadData.h"
#include "GMath.h"

typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

//static public methods
/*static*/ GGui::TPointerGui GGui::Factory(
	const GGuiLoad& in_guiLoad,
	GGuiManager& inout_manager
	)
{
	TPointerGui pointer;
	pointer.reset(new GGui(inout_manager));

	TArrayGuiNodeComponentPostLoadData arrayPostLoad;

	for (int index = 0; index < (int)in_guiLoad.GetArrayGuiNodeCount(); ++index)
	{
		TPointerGuiNode pointerGuiNode = GGuiNode::Factory(
			in_guiLoad.GetArrayGuiNode()[index],
			inout_manager,
			arrayPostLoad
			);
		pointer->AddGuiNode(pointerGuiNode);
	}

	int postLoadCount = 0;
	while (arrayPostLoad.size())
	{
		TArrayGuiNodeComponentPostLoadData oldArray(arrayPostLoad);
		arrayPostLoad.clear();

		for (TArrayGuiNodeComponentPostLoadData::iterator iterator = oldArray.begin(); iterator != oldArray.end(); ++iterator)
		{
			GGuiNodeComponentPostLoadData& postLoadData = *iterator;
			postLoadData.GetGuiNodeComponentBase()->PostLoad(
				postLoadCount,
				inout_manager,
				*postLoadData.GetGuiNode(),
				postLoadData.GetData(),
				arrayPostLoad
				);
		}
		postLoadCount += 1;
	}

	return pointer;
}

//constructor
GGui::GGui(GGuiManager& in_parent)
	: mParent(in_parent)
	, mArrayGuiNode()
	, mSizeChangeIndex(0)
{
	return;
}

GGui::~GGui()
{
	return;
}

//public methods
void GGui::Update()
{
	if (mSizeChangeIndex == mParent.GetSizeChangeIndex())
	{
		return;
	}
	mSizeChangeIndex = mParent.GetSizeChangeIndex();

	for (TArrayPointerGuiNode::iterator iterator = mArrayGuiNode.begin(); iterator != mArrayGuiNode.end(); ++iterator)
	{
		(*iterator)->SetFlagTransformDirty();
	}
	return;
}

//public accessors
const GVector2Float& GGui::GetScreenSize()const
{
	return mParent.GetScreenSize();
}

const GVector2Float GGui::CalculateDeviceFromScreenCoordinates(const GVector2Float& in_screenCoordinates)
{
	return mParent.GetDeviceCoordinate(in_screenCoordinates);
}
const GVector2Float GGui::CalculateInputCoordsFromScreenCoordinate(const GVector2Float& in_screenCoordinates)
{
	return mParent.GetInputCoordinate(in_screenCoordinates);
}

void GGui::AddGuiNode(TPointerGuiNode& in_guiNode)
{
	if (!in_guiNode)
	{
		return;
	}
	mArrayGuiNode.push_back(in_guiNode);
	return;
}

void GGui::RemoveGuiNode(TPointerGuiNode& in_guiNode)
{
	for (TArrayPointerGuiNode::iterator iterator = mArrayGuiNode.begin(); iterator != mArrayGuiNode.end(); ++iterator)
	{
		if (in_guiNode == (*iterator))
		{
			mArrayGuiNode.erase(iterator);
			return;
		}
	}

	return;
}

const GApplicationWindowType::TOrientation::TEnum GGui::GetScreenOrientation()const
{
	return mParent.GetScreenOrientation();
}
