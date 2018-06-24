//
//  ZombieGuiHealth.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiHealth.h"

#include "ZombieGameVar.h"
#include "ZombieGuiHealthData.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>

//constructor
ZombieGuiHealth::ZombieGuiHealth(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
: mArrayData()
, mHealthDisplayCount(-1)
, mComponentFade()
{
	mComponentFade.reset(new GComponentLerpFloat);

	UpdateArrayData(inout_window, in_zombieGameVar);
	UpdateData(in_zombieGameVar);
	UpdateAlpha();

	return;
}

ZombieGuiHealth::~ZombieGuiHealth()
{
	return;
}

//public methods
void ZombieGuiHealth::ResetVisual()
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(0.0F);
	}
}

void ZombieGuiHealth::Tick(const float in_timeDelta, GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
{
	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
	}

	UpdateArrayData(inout_window, in_zombieGameVar);
	UpdateData(in_zombieGameVar);
	UpdateAlpha();

	return;
}

void ZombieGuiHealth::SetVisible(const bool in_visible, const float in_fadeDuration)
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(in_visible ? 1.0F : 0.0F, in_fadeDuration);
	}
	UpdateAlpha();
	return;
}

//private methods
void ZombieGuiHealth::UpdateArrayData(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar)
{
	int maxHealthCount = in_zombieGameVar.GetLifeCountMaximum();

	maxHealthCount = (maxHealthCount >> 1) + (maxHealthCount & 0x01);
	if (maxHealthCount == mArrayData.size())
	{
		return;
	}

	mArrayData.resize(maxHealthCount);
	mHealthDisplayCount = -1;

	float alpha = 1.0F;
	if (mComponentFade)
	{
		alpha = mComponentFade->GetValue();
	}

	for (int index = 0; index < maxHealthCount; ++index)
	{
		TPointerZombieGuiHealthData& pointerHealthData = mArrayData[index];
		if (pointerHealthData)
		{
			pointerHealthData->SetPosition(index);
		}
		else
		{
			pointerHealthData.reset(new ZombieGuiHealthData(inout_window, index, alpha));
		}
	}

	return;
}

void ZombieGuiHealth::UpdateData(const ZombieGameVar& in_zombieGameVar)
{
	const int healthCount = in_zombieGameVar.GetLifeCountCurrent();

	if (healthCount == mHealthDisplayCount)
	{
		return;
	}

	mHealthDisplayCount = healthCount;
	const int maxHealthCount = (int)mArrayData.size();
	for (int index = 0; index < maxHealthCount; ++index)
	{
		const int temp = healthCount - (index * 2);
		int state = ZombieGuiHealthData::GetStateEmpty();
		if (temp == 1)
		{
			state = ZombieGuiHealthData::GetStateHalf();
		}
		else if (1 < temp)
		{
			state = ZombieGuiHealthData::GetStateFull();
		}
		mArrayData[index]->SetState(state);
	}

	return;
}

void ZombieGuiHealth::UpdateAlpha()
{
	if (!mComponentFade)
	{
		return;
	}
	float alpha = mComponentFade->GetValue();
	for (TArrayPointerZombieGuiHealthData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->SetAlpha(alpha);
	}
	return;
}

