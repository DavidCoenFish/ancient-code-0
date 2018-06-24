//
//  ZombieGuiBossHealthData.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiBossHealthData.h"

#include <GApplicationWindow.h>
#include <GGuiCoordinate.h>
//
//

#ifdef WIN32
static const char* const sMaggotName = "win32\\guimaggot.bmp";
#else
static const char* const sMaggotName = "guimaggot.png";
#endif

static const float sBossAlphaConstant = 0.75F;

//constructor
ZombieGuiBossHealthData::ZombieGuiBossHealthData(GApplicationWindow& in_applicationWindow, const int in_index)
//: mViewImage()
: mAlpha(1.0F)
{
	GGuiCoordinate position;
	GGuiCoordinate size;

	position.GetOriginRatio().SetData(0.0F, 0.5F);
	position.GetPixelOffset().SetData(16.0F, (in_index * 32.0F) - 160.0F);
	size.GetPixelOffset().SetData(32.0F, 32.0F);

	//mViewImage = in_applicationWindow.CreateViewComponentImage(
	//	sMaggotName,
	//	position,
	//	size
	//	);

	//TPointerApplicationViewComponent pointerViewComponent(mViewImage);
	//in_applicationWindow.AddViewComponentFront(pointerViewComponent);

	//mViewImage->Hide();
}

ZombieGuiBossHealthData::~ZombieGuiBossHealthData()
{
	//leak, but i don't care anymore
	return;
}

//public methods
void ZombieGuiBossHealthData::SetState(const bool in_visible)
{
	//if (in_visible)
	//{
	//	mViewImage->Show();
	//}
	//else
	//{
	//	mViewImage->Hide();
	//}

	return;
}

void ZombieGuiBossHealthData::SetAlpha(const float in_alpha)
{
	if (mAlpha == in_alpha)
	{
		return;
	}

	mAlpha = in_alpha;
	//if (mViewImage)
	//{
	//	mViewImage->SetAlpha(mAlpha * sBossAlphaConstant);
	//}

	return;
}
