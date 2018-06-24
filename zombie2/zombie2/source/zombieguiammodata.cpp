//
//  ZombieGuiAmmoData.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiAmmoData.h"
#include <GApplicationWindow.h>
#include <GGuiCoordinate.h>
//

//typedef
//typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

#ifdef WIN32
static const char* const sAmmoNameLoadedGold = "win32\\shell_00.bmp";
static const char* const sAmmoNameLoadedSivler = "win32\\shell_02.bmp";
static const char* const sAmmoNameLoadedBronze = "win32\\shell_03.bmp";
static const char* const sAmmoNameEmpty = "win32\\shell_01.bmp";
#else
static const char* const sAmmoNameLoadedGold = "shell_00.png";
static const char* const sAmmoNameLoadedSivler = "shell_02.png";
static const char* const sAmmoNameLoadedBronze = "shell_03.png";
static const char* const sAmmoNameEmpty = "shell_01.png";
#endif
static const float sAmmoAlphaConstant = 0.75F;

static void LocalGetPosition(
	GGuiCoordinate& out_position,
	GGuiCoordinate& out_size,
	const int in_index, 
	const int in_maximumCount						 
	)
{
	out_position.GetOriginRatio().SetData(0.5F, 1.0F);
	out_position.GetPixelOffset().SetData((16.0F * (in_index + 1.0F)) - 160.0F, -48.0F);
	out_size.GetPixelOffset().SetData(32, 32);

	return;
}

static const char* const LocalGetShotName(
	const ZombieStateType::TShotType::TEnum in_shotType, 
	const bool in_loaded
	)

{
	if (in_loaded)
	{
		switch (in_shotType)
		{
		default:
		case ZombieStateType::TShotType::TBronze:
			return sAmmoNameLoadedBronze;
		case ZombieStateType::TShotType::TSilver:
			return sAmmoNameLoadedSivler;
		case ZombieStateType::TShotType::TGold:
			return sAmmoNameLoadedGold;
		}
	}
	return sAmmoNameEmpty;
}

//constructor
ZombieGuiAmmoData::ZombieGuiAmmoData(
	GApplicationWindow& in_applicationWindow, 
	const int in_index, 
	const int in_maximumCount, 
	const float in_alpha,
	const ZombieStateType::TShotType::TEnum in_shotType									 
	)
//: mAmmoViewImage()
: mAlpha(1.0F)
, mLoaded(true)
, mShotType(in_shotType)
{
	GGuiCoordinate position;
	GGuiCoordinate size;
	LocalGetPosition(position, size, in_index, in_maximumCount);
	//mAmmoViewImage = in_applicationWindow.CreateViewComponentImage(
	//	LocalGetShotName(mShotType, mLoaded),
	//	position,
	//	size
	//	);

	//TPointerApplicationViewComponent pointerViewComponent(mAmmoViewImage);
	//in_applicationWindow.AddViewComponentFront(pointerViewComponent);

	return;
}

ZombieGuiAmmoData::~ZombieGuiAmmoData()
{
	return;
}

//public methods
void ZombieGuiAmmoData::SetShotType(const ZombieStateType::TShotType::TEnum in_shotType)
{
	if (mShotType == in_shotType)
	{
		return;
	}

	mShotType = in_shotType;
	//if (mAmmoViewImage)
	//{
	//	mAmmoViewImage->SetImage(LocalGetShotName(mShotType, mLoaded));
	//}

	return;
}

void ZombieGuiAmmoData::SetLoaded(const bool in_loaded)
{
	if (mLoaded == in_loaded)
	{
		return;
	}

	mLoaded = in_loaded;
	//if (mAmmoViewImage)
	//{
	//	mAmmoViewImage->SetImage(LocalGetShotName(mShotType, mLoaded));
	//}

	return;
}

void ZombieGuiAmmoData::SetAlpha(const float in_alpha)
{
	if (mAlpha == in_alpha)
	{
		return;
	}

	mAlpha = in_alpha;
	//if (mAmmoViewImage)
	//{
	//	mAmmoViewImage->SetAlpha(mAlpha * sAmmoAlphaConstant);
	//}

	return;
}

void ZombieGuiAmmoData::SetPosition(const int in_index, const int in_maximumCount)
{
	GGuiCoordinate position;
	GGuiCoordinate size;
	LocalGetPosition(position, size, in_index, in_maximumCount);

	//if (mAmmoViewImage)
	//{
	//	mAmmoViewImage->SetPosition(
	//		position, 
	//		size
	//		);
	//}

	return;
}

void ZombieGuiAmmoData::SetVisible(const bool in_visible)
{
	//if (!mAmmoViewImage)
	//{
	//	return;
	//}

	//if (in_visible)
	//{
	//	mAmmoViewImage->Show();
	//}
	//else
	//{
	//	mAmmoViewImage->Hide();
	//}

	return;
}
