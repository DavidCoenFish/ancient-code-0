//
//  ZombieGui.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGui.h"

#include "ZombieCameraMixer.h"
#include "ZombieGameVar.h"
#include "ZombieGuiAmmo.h"
#include "ZombieGuiHealth.h"
#include "ZombieGuiOffscreen.h"
#include "ZombieGuiReload.h"
#include "ZombieGuiDamage.h"
#include "ZombieGuiCinematicText.h"
#include "ZombieGuiBossHealth.h"
#include "ZombieGuiSplash.h"
#include "ZombieGuiMain.h"

#include <GApplicationWindow.h>
#include <GGuiManager.h>

const float sFadeDuration = 0.16666F;

//constructor
ZombieGui::ZombieGui(
	ZombieMission& inout_parentMission,
	boost::shared_ptr<GResourceManager>& inout_resourceManager,
	boost::shared_ptr<ZombieGameVar>& inout_gameVar
	)
: mGuiManager()
, mGameVar(inout_gameVar)
, mSplash()
, mGuiAmmo()
, mGuiHealth()
, mGuiOffscreen()
, mGuiReload()
, mGuiDamage()
{
	mGuiManager.reset(new GGuiManager(inout_resourceManager, "texture_01.data"));
	if (mGuiManager)
	{
		GGuiManager& guiManager = *mGuiManager;

		mSplash.reset(new ZombieGuiSplash(guiManager));
		mMain.reset(new ZombieGuiMain(guiManager, inout_parentMission));
	}

	return;
}

ZombieGui::~ZombieGui()
{
	return;
}

//public methods
void ZombieGui::Resize(
	const int in_width
	, const int in_height
	, GApplicationWindowType::TOrientation::TEnum& inout_screenOrientation
	)
{
	if (mGuiManager)
	{
		mGuiManager->SetScreen(
			GVector2Float((float)in_width, (float)in_height),
			inout_screenOrientation
			);
            
        inout_screenOrientation = mGuiManager->GetScreenOrientation();
	}
}

void ZombieGui::Input(const GInput& in_input)
{
	if (mMain)
	{
		mMain->Input(in_input);
	}
}

const bool ZombieGui::Tick(const float in_timeDelta)
{
	if (mMain)
	{
		mMain->Tick(in_timeDelta);
	}
	return true;
}
void ZombieGui::DrawSplash(GRender& inout_render)
{
	if (mSplash)
	{
		mSplash->Draw(inout_render);
	}
}
void ZombieGui::Draw(GRender& inout_render)
{
	if (mMain)
	{
		mMain->Draw(inout_render);
	}
}


//void ZombieGui::ResetVisual()
//{
//	if (mGuiAmmo)
//	{
//		mGuiAmmo->ResetVisual();
//	}
//
//	if (mGuiHealth)
//	{
//		mGuiHealth->ResetVisual();
//	}
//
//	if (mGuiOffscreen)
//	{
//		mGuiOffscreen->ResetVisual();
//	}
//
//	if (mGuiDamage)
//	{
//		mGuiDamage->ResetVisual();
//	}
//
//	if (mGuiReload)
//	{
//		mGuiReload->ResetVisual();
//	}
//
//	if (mGuiCinematicText)
//	{
//		mGuiCinematicText->ResetVisual();
//	}
//
//	return;
//}

void ZombieGui::OnSetCameraType(const int in_cameraType)
{
	if (mMain)
	{
		mMain->SetVisible(in_cameraType == ZombieCameraMixer::GetCameraTypeIntro());
	}


	//if (in_cameraType == ZombieCameraMixer::GetCameraTypeDeath())
	//{
	//	mGuiAmmo->SetVisible(false, sFadeDuration);
	//	mGuiHealth->SetVisible(false, sFadeDuration);
	//	mGuiOffscreen->SetVisible(false, sFadeDuration);
	//	mGuiCinematicText->ResetVisual();
	//	mGuiBossHealth->SetVisible(false, sFadeDuration);
	//}
	//else if (in_cameraType == ZombieCameraMixer::GetCameraTypeGame())
	//{
	//	mGuiAmmo->SetVisible(true, sFadeDuration);
	//	mGuiHealth->SetVisible(true, sFadeDuration);
	//	mGuiOffscreen->SetVisible(true, sFadeDuration);
	//	mGuiReload->ResetVisual();
	//	mGuiDamage->SetVisible(true, sFadeDuration);
	//	mGuiCinematicText->ResetVisual();
	//	mGuiBossHealth->SetVisible(true, sFadeDuration);
	//}
	//else if (in_cameraType == ZombieCameraMixer::GetCameraTypeCinematic())
	//{
	//	mGuiAmmo->SetVisible(false, sFadeDuration);
	//	mGuiHealth->SetVisible(false, sFadeDuration);
	//	mGuiOffscreen->SetVisible(false, sFadeDuration);
	//	mGuiReload->ResetVisual();
	//	mGuiDamage->ResetVisual();
	//	mGuiBossHealth->SetVisible(false, sFadeDuration);
	//}
	//else if (in_cameraType == ZombieCameraMixer::GetCameraTypeCinematicNoRoll())
	//{
	//	mGuiAmmo->SetVisible(false, sFadeDuration);
	//	mGuiHealth->SetVisible(false, sFadeDuration);
	//	mGuiOffscreen->SetVisible(false, sFadeDuration);
	//	mGuiReload->ResetVisual();
	//	mGuiDamage->ResetVisual();
	//	mGuiCinematicText->ResetVisual();
	//	mGuiBossHealth->ResetVisual();
	//}

	return;
}

void ZombieGui::RequestDamageType(const int in_damageType)
{
	if (mGuiDamage)
	{
		mGuiDamage->RequestDamageType(in_damageType);
	}

	return;
}

void ZombieGui::RequestReloadPrompt()
{
	if (mGuiReload)
	{
		mGuiReload->RequestReloadPrompt();
	}

	return;
}

void ZombieGui::HideReloadPrompt()
{
	if (mGuiReload)
	{
		mGuiReload->ResetVisual();
	}

	return;
}

void ZombieGui::RequestCinematicText(const std::string& in_textKey, const float in_holdTextTime)
{
	if (mGuiCinematicText)
	{
		mGuiCinematicText->RequestText(in_textKey, in_holdTextTime);
	}

	return;
}

void ZombieGui::RequestBossHealthMaggot()
{
	if (mGuiBossHealth)
	{
		mGuiBossHealth->MakeAnotherMaggotVisible();
	}
	return;
}

void ZombieGui::ResetBossHealthMaggot()
{
	if (mGuiBossHealth)
	{
		mGuiBossHealth->ResetVisual();
	}

	return;
}
