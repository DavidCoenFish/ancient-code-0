//
//  ZombieGuiHealthData.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiHealthData.h"
#include <GApplicationWindow.h>
#include <GGuiCoordinate.h>


//typedef
typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

#ifdef WIN32
static const char* const sHealthNameEmpty = "win32\\health_01.bmp";
static const char* const sHealthNameHalf = "win32\\health_02.bmp";
static const char* const sHealthNameFull = "win32\\health_00.bmp";
#else
static const char* const sHealthNameEmpty = "health_01.png";
static const char* const sHealthNameHalf = "health_02.png";
static const char* const sHealthNameFull = "health_00.png";
#endif

static const float sHealthAlphaConstant = 0.75F;

struct TZombieGuiHealthDataState
{
	enum TEnum
	{
		TEmpty = 0,
		THalfFull,
		TFull,
		TCount
	};
};

static void LocalGetPosition(
	GGuiCoordinate& out_position,
	GGuiCoordinate& out_size,
	const int in_index					 
	)
{
	out_position.GetOriginRatio().SetData(0.5F, 0.0F);
	out_position.GetPixelOffset().SetData(16.0F + (32.0F * in_index) - 160.0F, 16.0F);
	out_size.GetPixelOffset().SetData(32.0F, 32.0F);

	return;
}

//public static methods
/*static*/ const int ZombieGuiHealthData::GetStateEmpty()
{
	return TZombieGuiHealthDataState::TEmpty;
}

/*static*/ const int ZombieGuiHealthData::GetStateHalf()
{
	return TZombieGuiHealthDataState::THalfFull;
}

/*static*/ const int ZombieGuiHealthData::GetStateFull()
{
	return TZombieGuiHealthDataState::TFull;
}

//constructor
ZombieGuiHealthData::ZombieGuiHealthData(GApplicationWindow& in_applicationWindow, const int in_index, const float in_alpha)
: mHealthViewImage()
, mAlpha(1.0F)
, mState(-1)
{
	GGuiCoordinate position;
	GGuiCoordinate size;
	LocalGetPosition(position, size, in_index);
	//mHealthViewImage = in_applicationWindow.CreateViewComponentImage(
	//	sHealthNameFull,
	//	position,
	//	size
	//	);

	//TPointerApplicationViewComponent pointerViewComponent(mHealthViewImage);
	//in_applicationWindow.AddViewComponentFront(pointerViewComponent);

	return;
}

ZombieGuiHealthData::~ZombieGuiHealthData()
{
	return;
}

//public methods
void ZombieGuiHealthData::SetState(const int in_state)
{
	if (mState == in_state)
	{
		return;
	}

	mState = in_state;
	if (mHealthViewImage)
	{
		switch (mState)
		{
		default:
			break;
		case TZombieGuiHealthDataState::TEmpty:
			//mHealthViewImage->SetImage(sHealthNameEmpty);
			break;
		case TZombieGuiHealthDataState::THalfFull:
			//mHealthViewImage->SetImage(sHealthNameHalf);
			break;
		case TZombieGuiHealthDataState::TFull:
			//mHealthViewImage->SetImage(sHealthNameFull);
			break;
		}
	}

	return;
}

void ZombieGuiHealthData::SetAlpha(const float in_alpha)
{
	if (mAlpha == in_alpha)
	{
		return;
	}

	mAlpha = in_alpha;
	//if (mHealthViewImage)
	//{
	//	mHealthViewImage->SetAlpha(mAlpha * sHealthAlphaConstant);
	//}

	return;
}

void ZombieGuiHealthData::SetPosition(const int in_index)
{
	GGuiCoordinate position;
	GGuiCoordinate size;
	LocalGetPosition(position, size, in_index);

	//if (mHealthViewImage)
	//{
	//	mHealthViewImage->SetPosition(
	//		position, 
	//		size
	//		);
	//}

	return;
}
