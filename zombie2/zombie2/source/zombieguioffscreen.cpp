//
//  ZombieGuiOffscreen.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiOffscreen.h"

#include "ZombieGameVar.h"
#include "ZombieGuiOffscreenData.h"
//#include "ZombieCamera.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>

typedef std::vector<GVector2Float> TArrayVector2;

static const int sZombieGuiOffscreenDefaultCount = 0;

//constructor
ZombieGuiOffscreen::ZombieGuiOffscreen(GApplicationWindow& inout_window)
: mComponentFade()
, mArrayData()
{
	mComponentFade.reset(new GComponentLerpFloat());
	mArrayData.resize(sZombieGuiOffscreenDefaultCount);
	for (int index = 0; index < sZombieGuiOffscreenDefaultCount; ++index)
	{
		mArrayData[index].reset(new ZombieGuiOffscreenData(inout_window));
	}
	return;
}

ZombieGuiOffscreen::~ZombieGuiOffscreen()
{
	return;
}

//public methods
void ZombieGuiOffscreen::ResetVisual()
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(0.0F);
	}
	UpdateAlpha();
}

void ZombieGuiOffscreen::Tick(
	const float in_timeDelta, 
	GApplicationWindow& inout_window, 
	const ZombieGameVar& in_zombieGameVar
	)
{
	const TArrayVector2& arrayOffscreen = in_zombieGameVar.GetArrayOffscreen();
	const int countOffscreen = arrayOffscreen.size();
	while ((int)mArrayData.size() < countOffscreen)
	{
		TPointerZombieGuiOffscreenData pointer;
		pointer.reset(new ZombieGuiOffscreenData(inout_window));
		mArrayData.push_back(pointer);
	}

	for (int index = 0; index < (int)mArrayData.size(); ++index)
	{
		ZombieGuiOffscreenData& data = *mArrayData[index];
		if (index < countOffscreen)
		{
			data.SetVisiblie(true);
			data.SetPosition(
				arrayOffscreen[index]
				);
		}
		else
		{
			data.SetVisiblie(false);
		}
	}

	if (mComponentFade)
	{
		mComponentFade->Tick(in_timeDelta);
	}

	UpdateAlpha();

	return;
}

void ZombieGuiOffscreen::SetVisible(const bool in_visible, const float in_fadeDuration)
{
	if (mComponentFade)
	{
		mComponentFade->SetValue(in_visible ? 1.0F : 0.0F, in_fadeDuration);
	}
	UpdateAlpha();

	return;
}

//private methods
void ZombieGuiOffscreen::UpdateAlpha()
{
	if (!mComponentFade)
	{
		return;
	}
	float alpha = mComponentFade->GetValue();
	for (TArrayPointerZombieGuiOffscreenData::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->SetAlpha(alpha);
	}
	return;
}
