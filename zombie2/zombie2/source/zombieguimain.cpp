//
//  ZombieGuiMain.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiMain.h"
#include "ZombieMission.h"
#include "ZombieGuiMainButtonCallback.h"

#include <GGuiManager.h>
#include <GRender.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>
#include <GGuiVisitorInput.h>
#include <GGuiVisitor.h>
#include <GGuiNodeComponentButton.h>

//constructor
ZombieGuiMain::ZombieGuiMain(
		GGuiManager& inout_guiManager,
		ZombieMission& inout_parentMission
		)							 
: mParentMission(inout_parentMission)
, mMain()
, mMaterial()
, mVisible(false)
{
	mMain = inout_guiManager.LoadGui("gui_main.data");
	mMaterial = inout_guiManager.GetMaterialColourTexture();

	boost::shared_ptr<GGui> pGui = mMain.lock();
	if (pGui)
	{
		GGui& gui = *pGui;
		std::vector< boost::weak_ptr<GGuiNodeComponentButton> > arrayButton;
		GGuiVisitor<GGuiNodeComponentButton>::Run(gui, arrayButton);
		boost::shared_ptr<GGuiNodeComponentButtonCallback> pointerButtonCallback(new ZombieGuiMainButtonCallback(*this));

		for (std::vector< boost::weak_ptr<GGuiNodeComponentButton> >::iterator iterator = arrayButton.begin(); iterator != arrayButton.end(); ++iterator)
		{
			boost::shared_ptr<GGuiNodeComponentButton> pointerButton = (*iterator).lock();
			if (!pointerButton)
			{
				continue;
			}
			GGuiNodeComponentButton& button = *pointerButton;
			button.SetCallback(pointerButtonCallback);
		}
	}

	return;
}

ZombieGuiMain::~ZombieGuiMain()
{
	return;
}

void ZombieGuiMain::Draw(GRender& inout_render)
{
	if (!mVisible)
		return;
	boost::shared_ptr<GMaterialInstance> pMaterial = mMaterial.lock();
	if (pMaterial)
		inout_render.SetMaterial(*pMaterial);

	boost::shared_ptr<GGui> pGui = mMain.lock();
	if (pGui)
	{
		GGui& gui = *pGui;
		GGuiVisitorUpdate::Run(gui);
		GGuiVisitorRender::Run(gui, inout_render);
	}
}

void ZombieGuiMain::Input(const GInput& in_input)
{
	if (!mVisible)
		return;

	boost::shared_ptr<GGui> pGui = mMain.lock();
	if (pGui)
	{
		GGui& gui = *pGui;
		GGuiVisitorInput::Run(gui, in_input);
	}
}

const bool ZombieGuiMain::Tick(const float in_timeDelta)
{
	return true;
}

void ZombieGuiMain::ClickRelease(const std::string& in_buttonName, const bool in_clickDownAndUpInsideButton)
{
	if (in_clickDownAndUpInsideButton && (in_buttonName == "Button01"))
	{
		mParentMission.NewGame(ZombieStateType::TGameDifficulty::TEasy);
	}
}

void ZombieGuiMain::SetVisible(const bool in_visible)
{
	mVisible = in_visible;
}

