//
//  ZombieGuiOffscreenData.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiOffscreenData.h"

#include <GApplicationWindow.h>


#include <GVector2Float.h>
#include <GGuiCoordinate.h>
#include <GMath.h>

#ifdef WIN32
static const char* const sOffscreen0 = "win32\\offscreen_00.bmp";
static const char* const sOffscreen1 = "win32\\offscreen_01.bmp";
static const char* const sOffscreen2 = "win32\\offscreen_02.bmp";
static const char* const sOffscreen3 = "win32\\offscreen_03.bmp";
#else
static const char* const sOffscreen0 = "offscreen_00.png";
static const char* const sOffscreen1 = "offscreen_01.png";
static const char* const sOffscreen2 = "offscreen_02.png";
static const char* const sOffscreen3 = "offscreen_03.png";
#endif
static const int sOffscreenSize = 24;
static const int sOffscreenSizeHalf = sOffscreenSize / 2;
static const float sHackWidthOffset = (float)sOffscreenSizeHalf / 320.0F;
static const float sHackHeightOffset = (float)sOffscreenSizeHalf / 480.0F;

/*
	gui
	->+x
   |
   v
  +y

	screen pos
	  -1 -->+1 x

   +1 y
	A
	|
   -1
*/

//constructor
ZombieGuiOffscreenData::ZombieGuiOffscreenData(GApplicationWindow& in_applicationWindow)
: mViewComponentImage()
, mAlpha(1.0F)
, mVisible(false)
, mImageName(sOffscreen0)
{
	const GGuiCoordinate position;
	const GGuiCoordinate size(GVector2Float::sZero, GVector2Float(16, 16));
	//mViewComponentImage = in_applicationWindow.CreateViewComponentImage(
	//	sOffscreen0,
	//	position,
	//	size
	//	);

	//mViewComponentImage->Hide();

	//TPointerApplicationViewComponent pointerViewComponent(mViewComponentImage);
	//in_applicationWindow.AddViewComponentFront(pointerViewComponent);

	return;
}

ZombieGuiOffscreenData::~ZombieGuiOffscreenData()
{
	return;
}

//public methods
void ZombieGuiOffscreenData::SetVisiblie(const bool in_visible)
{
	//if (!mViewComponentImage || (mVisible == in_visible))
	//{
	//	return;
	//}
	//mVisible = in_visible;
	//if (in_visible)
	//{
	//	mViewComponentImage->Show();
	//}
	//else
	//{
	//	mViewComponentImage->Hide();
	//}
	return;
}

void ZombieGuiOffscreenData::SetAlpha(const float in_alpha)
{
	//if (!mViewComponentImage || (mAlpha == in_alpha))
	//{
	//	return;
	//}
	//mAlpha = in_alpha;
	//
	//mViewComponentImage->SetAlpha(in_alpha);

	return;
}

void ZombieGuiOffscreenData::SetPosition(const GVector2Float in_screenPos)
{
	//if (!mViewComponentImage)
	//{
	//	return;
	//}

	//GGuiCoordinate position;
	//const GGuiCoordinate size(
	//	GVector2Float::sZero, 
	//	GVector2Float(sOffscreenSize, sOffscreenSize)
	//	);
	//
	//const float x = in_screenPos.m_x;
	//const float y = in_screenPos.m_y;
	//std::string imageName;

	//if (GMath::Abs(x) < GMath::Abs(y))
	//{
	//	const float doctoredX = GMath::Clamp(((x / GMath::Abs(y)) * 0.5F) + 0.5F, sHackWidthOffset, 1.0F - sHackWidthOffset);
	//	if (0.0F < y)
	//	{
	//		position = GGuiCoordinate(
	//			GVector2Float(doctoredX, 0.0F),
	//			GVector2Float(-sOffscreenSizeHalf, 0)
	//			);
	//		imageName = sOffscreen2;
	//	}
	//	else
	//	{
	//		position = GGuiCoordinate(
	//			GVector2Float(doctoredX, 1.0F),
	//			GVector2Float(-sOffscreenSizeHalf, -sOffscreenSize)
	//			);
	//		imageName = sOffscreen0;
	//	}
	//}
	//else
	//{
	//	const float doctoredY = GMath::Clamp(((-(y / GMath::Abs(x))) * 0.5F) + 0.5F, sHackHeightOffset, 1.0F - sHackHeightOffset);
	//	if (0.0F < x)
	//	{
	//		position = GGuiCoordinate(
	//			GVector2Float(1.0F, doctoredY),
	//			GVector2Float(-sOffscreenSize, -sOffscreenSizeHalf)
	//			);
	//		imageName = sOffscreen3;
	//	}
	//	else
	//	{
	//		position = GGuiCoordinate(
	//			GVector2Float(0.0F, doctoredY),
	//			GVector2Float(0, -sOffscreenSizeHalf)
	//			);
	//		imageName = sOffscreen1;
	//	}
	//}


	//mViewComponentImage->SetPosition(
	//	position, 
	//	size
	//	);
	//	
	//if (imageName != mImageName)
	//{
	//	mImageName = imageName;
	//	mViewComponentImage->SetImage(
	//		mImageName
	//		);
	//}

	return;
}
