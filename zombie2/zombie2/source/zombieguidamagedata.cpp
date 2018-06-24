//
//  ZombieGuiDamageData.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiDamageData.h"

#include <GApplicationWindow.h>
//
//
#include <GVector2Float.h>
#include <GGuiCoordinate.h>
#include <GMath.h>
#include <GComponentLerpFloat.h>

static const float sDamageDisplayTime = 3.0F;

//constructor
ZombieGuiDamageData::ZombieGuiDamageData(
		GApplicationWindow& in_applicationWindow,
		const GGuiCoordinate& in_origin,
		const GGuiCoordinate& in_size,
		const std::string& in_imageName
		)
	: mViewComponentImage()
	, mComponentFade()
	, mResultAlpha(0.0F)
	, mShow(false)
{
	mComponentFade.reset(new GComponentLerpFloat(0.0F));
	//mViewComponentImage = in_applicationWindow.CreateViewComponentImage(
	//	in_imageName,
	//	in_origin,
	//	in_size
	//	);
	//mViewComponentImage->SetAlpha(0.0F);
	//mViewComponentImage->Hide();

	//TPointerApplicationViewComponent pointerViewComponent(mViewComponentImage);
	//in_applicationWindow.AddViewComponentFront(pointerViewComponent);

	return;
}

ZombieGuiDamageData::~ZombieGuiDamageData()
{
	//TODO
	//LEAK:, need to remove view from window
	return;
}

//public methods
void ZombieGuiDamageData::ResetVisual()
{
	GComponentLerpFloat& componentLerp = *mComponentFade;
	componentLerp.SetValue(0.0F);
	Tick(0.0F, 0.0F);
}

void ZombieGuiDamageData::Activate()
{
	GComponentLerpFloat& componentLerp = *mComponentFade;
	componentLerp.SetValue(1.0F);
	componentLerp.SetValue(0.0F, sDamageDisplayTime);
	return;
}

void ZombieGuiDamageData::Tick(const float in_timeDelta, const float in_parentAlpha)
{
	GComponentLerpFloat& componentLerp = *mComponentFade;
	componentLerp.Tick(in_timeDelta);

	//GApplicationViewComponentImage& componentImage = *mViewComponentImage;

	const float result = componentLerp.GetValue() * in_parentAlpha;
	if (result != mResultAlpha)
	{
		const bool show = (0.0F < result);
		mResultAlpha = result;
		if (show)
		{
			if (mShow != show)
			{
				//componentImage.Show();
				mShow = show;
			}
			//componentImage.SetAlpha(mResultAlpha);
		}
		else if (mShow != show)
		{
			//componentImage.Hide();
			mShow = show;
		}
	}

	return;
}
