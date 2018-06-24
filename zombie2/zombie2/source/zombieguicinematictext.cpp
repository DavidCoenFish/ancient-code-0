//
//  ZombieGuiCinematicText.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiCinematicText.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>

#include <GVector2Float.h>
#include <GGuiCoordinate.h>
//
//
#include <GColour4Float.h>

static const float sCinematicTextFadeTime= 0.5F;

//constructor
ZombieGuiCinematicText::ZombieGuiCinematicText(GApplicationWindow& inout_window)
//: mViewComponentText()
: mComponentFade()
, mAlpha(0.0F)
{
	mComponentFade.reset(new GComponentLerpFloat);

	GGuiCoordinate position(GVector2Float(0.5F, 1.0F), GVector2Float(-150, -256));
	GGuiCoordinate size(GVector2Float(0.0F, 0.0F), GVector2Float(300, 240));
//#ifndef WIN32
//	mViewComponentText = inout_window.CreateViewComponentText(
//		"", 
//		position, 
//		size, 
//		GColour4Float::sWhite,
//		false,
//		true,
//		false,
//		true
//		);
//#endif
//	if (mViewComponentText)
//	{
//		mViewComponentText->SetAlpha(mAlpha);
//		TPointerApplicationViewComponent viewComponent(mViewComponentText);
//		inout_window.AddViewComponentFront(viewComponent);
//	}

	return;
}

ZombieGuiCinematicText::~ZombieGuiCinematicText()
{
	return;
}

//public methods
void ZombieGuiCinematicText::ResetVisual()
{
	mHoldTextCountdown = 0.0F;
	if (mComponentFade)
	{
		mComponentFade->SetValue(0.0F);
	}

	//if (mViewComponentText)
	//{
	//	GApplicationViewComponentText& componentText = *mViewComponentText;
	//	const bool show = (0.0F != mAlpha);
	//	if (show)
	//	{
	//		componentText.Show();
	//	}
	//	else
	//	{
	//		componentText.Hide();
	//	}
	//	componentText.SetAlpha(mAlpha);
	//}

	return;
}

void ZombieGuiCinematicText::Tick(const float in_timeDelta)
{
	if (0.0F < mHoldTextCountdown)
	{
		mHoldTextCountdown -= in_timeDelta;
		if (mHoldTextCountdown <= 0.0F)
		{
			mComponentFade->SetValue(0.0F, sCinematicTextFadeTime);
		}
	}

	bool alphaChange = false;
	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
		const float value = mComponentFade->GetValue();
		if (value != mAlpha)
		{
			mAlpha = value;
			alphaChange = true;
		}
	}

	//if (mViewComponentText && alphaChange)
	//{
	//	GApplicationViewComponentText& componentText = *mViewComponentText;
	//	const bool show = (0.0F != mAlpha);
	//	if (show)
	//	{
	//		componentText.Show();
	//	}
	//	else
	//	{
	//		componentText.Hide();
	//	}
	//	componentText.SetAlpha(mAlpha);
	//}

	return;
}

void ZombieGuiCinematicText::RequestText(const std::string& in_textKey, const float in_holdTextTime)
{
	//if (mViewComponentText)
	//{
	//	GApplicationViewComponentText& viewComponentText = *mViewComponentText;
	//	viewComponentText.SetText(in_textKey);
	//}
	mHoldTextCountdown = in_holdTextTime;
	if (mComponentFade)
	{
		mComponentFade->SetValue(1.0F, 0.0F);
	}
	return;
}


