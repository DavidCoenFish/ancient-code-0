//
//  ZombieFade.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieFade.h"

#include <GApplicationWindow.h>
#include <GVector2Float.h>
#include <GGuiCoordinate.h>
#include <GColour4Float.h>

//constructor
ZombieFade::ZombieFade()
//: mFade()
: mFadeIn(false)
, mFadeOut(false)
, mDuration(0.0F)
, mTime(0.0F)
{
	return;
}

ZombieFade::~ZombieFade()
{
	return;
}

//public methods
void ZombieFade::Tick(
	const float in_timeDelta, 
	GApplicationWindow& inout_applicationWindow
	)
{
	//if (mFadeIn && mFade)
	//{
	//	mTime += in_timeDelta;
	//	if (mDuration < mTime)
	//	{
	//		inout_applicationWindow.RemoveViewComponent(mFade);
	//		mFade.reset();
	//		mFadeIn = false;
	//	}
	//	else
	//	{
	//		mFade->SetAlpha(1.0F - (mTime / mDuration));
	//	}
	//}
	//else if (mFadeOut && mFade)
	//{
	//	mTime += in_timeDelta;
	//	if (mDuration < mTime)
	//	{
	//		mFadeOut = false;
	//	}
	//	else
	//	{
	//		mFade->SetAlpha(mTime / mDuration);
	//	}
	//}

	return;
}

void ZombieFade::Clear(GApplicationWindow& inout_applicationWindow)
{
	//if (mFade)
	//{
	//	inout_applicationWindow.RemoveViewComponent(mFade);
	//	mFade.reset();
	//}
	mFadeIn = false;
	mFadeOut = false;
	return;
}


void ZombieFade::RequestFadeIn(
	const float in_duration,
	GApplicationWindow& inout_applicationWindow
	)
{
	Clear(inout_applicationWindow);
	mFadeIn = true;
	mTime = 0.0F;
	mDuration = in_duration;
	//mFade = inout_applicationWindow.CreateViewComponent(
	//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
	//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
	//	GColour4Float::sBlack
	//	);
	//if (mFade)
	//{
	//	mFade->SetAlpha(0.0F);
	//	inout_applicationWindow.AddViewComponentFront(mFade);
	//}

	return;
}

void ZombieFade::RequestFadeOut(
	const float in_duration,
	GApplicationWindow& inout_applicationWindow
	)
{
	Clear(inout_applicationWindow);
	mFadeOut = true;
	mTime = 0.0F;
	mDuration = in_duration;
	//mFade = inout_applicationWindow.CreateViewComponent(
	//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
	//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
	//	GColour4Float::sBlack
	//	);
	//if (mFade)
	//{
	//	mFade->SetAlpha(0.0F);
	//	inout_applicationWindow.AddViewComponentFront(mFade);
	//}

	return;
}
