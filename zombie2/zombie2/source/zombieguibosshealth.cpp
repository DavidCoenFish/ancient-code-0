//
//  ZombieGuiBossHealth.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiBossHealth.h"

#include "ZombieGuiBossHealthData.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>
//
//

struct TGuiBossHelathBarVar
{
	enum TEnum
	{
		TMaxMaggotCount = 9
	};
};

//constructor
ZombieGuiBossHealth::ZombieGuiBossHealth(GApplicationWindow& inout_window)
: mArrayData()
, mVisibleCount(0)
, mComponentFade()
{
	mComponentFade.reset(new GComponentLerpFloat);
	mArrayData.resize(TGuiBossHelathBarVar::TMaxMaggotCount);

	for (int index = 0; index < TGuiBossHelathBarVar::TMaxMaggotCount; ++index)
	{
		mArrayData[index].reset(new ZombieGuiBossHealthData(inout_window, index));
	}

	return;
}

ZombieGuiBossHealth::~ZombieGuiBossHealth()
{
	return;
}

//public methods
void ZombieGuiBossHealth::ResetVisual()
{
	mVisibleCount = 0;
	for (int index = 0; index < TGuiBossHelathBarVar::TMaxMaggotCount; ++index)
	{
		mArrayData[index]->SetState(false);
	}

	return;
}

void ZombieGuiBossHealth::Tick(const float in_timeDelta)
{
	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
	}
	UpdateAlpha();

	return;
}

void ZombieGuiBossHealth::SetVisible(const bool in_visible, const float in_fadeDuration)
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(in_visible ? 1.0F : 0.0F, in_fadeDuration);
	}
	UpdateAlpha();
	return;
}

void ZombieGuiBossHealth::MakeAnotherMaggotVisible()
{
	if (mVisibleCount < TGuiBossHelathBarVar::TMaxMaggotCount)
	{
		mArrayData[mVisibleCount]->SetState(true);
	}

	mVisibleCount += 1;

	return;
}


void ZombieGuiBossHealth::UpdateAlpha()
{
	if (!mComponentFade || (0 == mVisibleCount))
	{
		return;
	}
	float alpha = mComponentFade->GetValue();
	for (TArrayPointerZombieGuiBossHealthData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->SetAlpha(alpha);
	}
	return;
}
