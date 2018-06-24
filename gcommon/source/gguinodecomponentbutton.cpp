//
//  GGuiNodeComponentButton.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentButton.h"

#include "GGuiNodeComponentButtonLoad.h"
#include "GGuiNodeComponentButtonCallback.h"
#include "GGuiType.h"
#include "GStringUtility.h"
#include "GVector2Float.h"

struct TButtonState
{
	enum TEnum
	{
		TEnabled = 0,
		TDisabled,
		TClicked,
		TCount
	};
};

typedef std::vector<unsigned int> TArrayInt;

static void LocalAddClickId(
	TArrayInt& inout_arrayInsideClickId, 
	const unsigned int in_clickId
	)
{
	for (TArrayInt::iterator iterator = inout_arrayInsideClickId.begin(); iterator != inout_arrayInsideClickId.end(); ++iterator)
	{
		if (in_clickId == (*iterator))
		{
			return;
		}
	}
	inout_arrayInsideClickId.push_back(in_clickId);
	return;
}

static const bool LocalFindRemove(
	TArrayInt& inout_arrayInsideClickId, 
	const unsigned int in_clickId
	)
{
	for (TArrayInt::iterator iterator = inout_arrayInsideClickId.begin(); iterator != inout_arrayInsideClickId.end(); ++iterator)
	{
		if (in_clickId == (*iterator))
		{
			inout_arrayInsideClickId.erase(iterator);
			return true;
		}
	}
	return false;
}

//static public methods
/*static*/ GGuiNodeComponentButton::TPointerGuiNodeComponentBase GGuiNodeComponentButton::Factory(
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
	const GGuiNodeComponentButtonLoad& buttonLoad = *(GGuiNodeComponentButtonLoad*)in_data;
	TPointerGuiNodeComponentAnimation componentAnimation;

	pointer.reset(new GGuiNodeComponentButton(
		inout_parent,
		GStringUtility::SafeString(buttonLoad.GetName()),
		componentAnimation
		));

	return pointer;
}

/*static*/ const unsigned int GGuiNodeComponentButton::GetComponentFlag()
{
	return GGuiType::TComponentFlag::TButton;
}

//constructor
GGuiNodeComponentButton::GGuiNodeComponentButton(
	GGuiNode& in_parentNode,
	const std::string& in_name,
	TPointerGuiNodeComponentAnimation& in_componentAnimation
	)
: GGuiNodeComponentBase(GetComponentFlag())
, mParentNode(in_parentNode)
, mName(in_name)
, mCallback()
, mComponentAnimation()
, mClickState(TButtonState::TEnabled)
, m_active(GTRUE)
{
	return;
}

/*virtual*/ GGuiNodeComponentButton::~GGuiNodeComponentButton()
{
	return;
}

//public methods
void GGuiNodeComponentButton::ClickDown(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio)
{
	if (mCallback)// && m_active)
	{
		mCallback->ClickDown(
			mName,
			in_screenPos,
			in_buttionRelativeRatio,
			in_clickId
			);
	}

	//LocalAddClickId(mArrayInsideClickId, in_clickId);
	return;
}

void GGuiNodeComponentButton::ClickDrag(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio)
{
	if (mCallback)// && m_active)
	{
		mCallback->ClickDrag(
			mName,
			in_screenPos,
			in_buttionRelativeRatio,
			in_clickId
			);
	}
	return;
}

void GGuiNodeComponentButton::ClickRelease(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio)
{
	if (mCallback)// && m_active)
	{
		mCallback->ClickRelease(
			mName,
			in_screenPos,
			in_buttionRelativeRatio,
			in_clickId,
			GTRUE //TODO: was click always inside button
			);
	}

	return;
}


//public accessors
void GGuiNodeComponentButton::SetCallback(
	TPointerGuiNodeComponentButtonCallback& in_callback //WARNING: transferal ownership
	)
{
	mCallback = in_callback;
	return;
}

//private methods
void GGuiNodeComponentButton::SetButtonState(const int in_buttonState)
{

	return;
}
