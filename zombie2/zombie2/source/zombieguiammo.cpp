//
//  ZombieGuiAmmo.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiAmmo.h"

#include "ZombieGameVar.h"
#include "ZombieGuiAmmoData.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>

//constructor
ZombieGuiAmmo::ZombieGuiAmmo(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
: mArrayData()
, mAmmoDisplayCount(-1)
, mComponentFade()
{
	mComponentFade.reset(new GComponentLerpFloat);

	UpdateArrayData(inout_window, in_zombieGameVar);
	UpdateLoaded(in_zombieGameVar);
	UpdateAlpha();

	return;
}

ZombieGuiAmmo::~ZombieGuiAmmo()
{
	return;
}

//public methods
void ZombieGuiAmmo::ResetVisual()
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(0.0F);
	}
	UpdateAlpha();
}


void ZombieGuiAmmo::Tick(const float in_timeDelta, GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
{
	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
	}

	UpdateArrayData(inout_window, in_zombieGameVar);
	UpdateLoaded(in_zombieGameVar);
	UpdateAlpha();

	return;
}

void ZombieGuiAmmo::SetVisible(const bool in_visible, const float in_fadeDuration)
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(in_visible ? 1.0F : 0.0F, in_fadeDuration);
	}
	UpdateAlpha();
	return;
}

//private methods
void ZombieGuiAmmo::UpdateArrayData(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
{
	const ZombieStateType::TShotType::TEnum shotType = in_zombieGameVar.GetShotType();
	if (mShotType != shotType)
	{
		mShotType = shotType;
		for (TArrayPointerZombieGuiAmmoData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
		{
			(*iterator)->SetShotType(mShotType);
		}
	}

	const int maxAmmoCount = in_zombieGameVar.GetAmmoCountMaximum();
	if (maxAmmoCount == mArrayData.size())
	{
		return;
	}

	mArrayData.resize(maxAmmoCount);
	mAmmoDisplayCount = -1;

	float alpha = 1.0F;
	if (mComponentFade)
	{
		alpha = mComponentFade->GetValue();
	}

	for (int index = 0; index < maxAmmoCount; ++index)
	{
		const int reverseIndex = maxAmmoCount - index - 1;
		TPointerZombieGuiAmmoData& pointerAmmoData = mArrayData[index];
		if (pointerAmmoData)
		{
			pointerAmmoData->SetPosition(reverseIndex, maxAmmoCount);
		}
		else
		{
			pointerAmmoData.reset(new ZombieGuiAmmoData(inout_window, reverseIndex, maxAmmoCount, alpha, shotType));
		}
	}

	return;
}

void ZombieGuiAmmo::UpdateLoaded(const ZombieGameVar& in_zombieGameVar)
{
	const int loadedCount = in_zombieGameVar.GetAmmoCountCurrent();

	if (loadedCount == mAmmoDisplayCount)
	{
		return;
	}

	mAmmoDisplayCount = loadedCount;
	const int maxAmmoCount = (int)mArrayData.size();
	for (int index = 0; index < maxAmmoCount; ++index)
	{
		const int reverseIndex = maxAmmoCount - index - 1;
		mArrayData[index]->SetLoaded(reverseIndex < mAmmoDisplayCount);
	}

	return;
}

void ZombieGuiAmmo::UpdateAlpha()
{
	if (!mComponentFade)
	{
		return;
	}
	float alpha = mComponentFade->GetValue();
	for (TArrayPointerZombieGuiAmmoData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->SetAlpha(alpha);
	}
	return;
}

