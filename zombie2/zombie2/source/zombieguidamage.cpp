//
//  ZombieGuiDamage.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiDamage.h"

#include "ZombieGameVar.h"
#include "ZombieGuiDamageData.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>

#include <GGuiCoordinate.h>

typedef std::vector<int> TArrayInt;

typedef std::vector<GVector2Float> TArrayVector2;

struct TDamageType
{
	enum TEnum
	{
		TBiteBig = 0,
		TBiteSmall,
		TSlashRight,
		TSlashLeft,
		TCount
	};
};

#ifdef WIN32
static const char* const sDamageImage[TDamageType::TCount] = {
	"win32\\bite_00.bmp",
	"win32\\bite_01.bmp",
	"win32\\swipe_00.bmp",
	"win32\\swipe_01.bmp"
};
#else
static const char* const sDamageImage[TDamageType::TCount] = {
	"bite_00.png",
	"bite_01.png",
	"swipe_00.png",
	"swipe_01.png"
};
#endif	

/*static*/ const int ZombieGuiDamage::GetDamageTypeBiteBig()
{
	return TDamageType::TBiteBig;
}

/*static*/ const int ZombieGuiDamage::GetDamageTypeBiteSmall()
{
	return TDamageType::TBiteSmall;
}

/*static*/ const int ZombieGuiDamage::GetDamageTypeSlashRight()
{
	return TDamageType::TSlashRight;
}

/*static*/ const int ZombieGuiDamage::GetDamageTypeSlashLeft()
{
	return TDamageType::TSlashLeft;
}

//constructor
ZombieGuiDamage::ZombieGuiDamage(GApplicationWindow& inout_window)
: mComponentFade()
, mArrayData()
{
	mComponentFade.reset(new GComponentLerpFloat());
	mArrayData.resize(TDamageType::TCount);
	const GGuiCoordinate size(GVector2Float::sZero, GVector2Float(256.0F, 256.0F));
	for (int index = 0; index < TDamageType::TCount; ++index)
	{
		GGuiCoordinate origin(GVector2Float::sZero, GVector2Float(-128.0F, -128.0F));
		switch (index)
		{
		default:
			break;
		case TDamageType::TBiteBig:
			origin.GetOriginRatio().SetData(0.5F, 0.5F);
			break;
		case TDamageType::TBiteSmall:
			origin.GetOriginRatio().SetData(0.5F, 0.7F);
			break;
		case TDamageType::TSlashRight:
			origin.GetOriginRatio().SetData(0.3F, 0.6F);
			break;
		case TDamageType::TSlashLeft:
			origin.GetOriginRatio().SetData(0.7F, 0.6F);
			break;
		}
		mArrayData[index].reset(new ZombieGuiDamageData(
			inout_window,
			origin,
			size,
			sDamageImage[index]
			));
	}
	return;
}

ZombieGuiDamage::~ZombieGuiDamage()
{
	return;
}

//public methods
void ZombieGuiDamage::ResetVisual()
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(0.0F);
	}

	for (TArrayPointerZombieGuiDamageData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		ZombieGuiDamageData& damageData = *(*iterator);
		damageData.ResetVisual();
	}

	return;
}

void ZombieGuiDamage::Tick(const float in_timeDelta)
{
	float parentAlpha = 0.0F;
	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
		parentAlpha = mComponentFade->GetValue();
	}

	for (TArrayPointerZombieGuiDamageData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		ZombieGuiDamageData& damageData = *(*iterator);
		damageData.Tick(in_timeDelta, parentAlpha);
	}

	return;
}

void ZombieGuiDamage::RequestDamageType(const int in_damageType)
{
	mArrayData[in_damageType]->Activate();
	return;
}

void ZombieGuiDamage::SetVisible(const bool in_visible, const float in_fadeDuration)
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(in_visible ? 1.0F : 0.0F, in_fadeDuration);
	}

	return;
}
