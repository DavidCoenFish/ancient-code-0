//
//  ZombieGuiSplash.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiSplash.h"

#include <GGuiManager.h>
#include <GRender.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>

//constructor
ZombieGuiSplash::ZombieGuiSplash(GGuiManager& inout_guiManager)
: mSplash()
, mMaterial()
{
	mSplash = inout_guiManager.LoadGui("gui_splash00.data");
	mMaterial = inout_guiManager.GetMaterialColourTexture();

	return;
}

ZombieGuiSplash::~ZombieGuiSplash()
{
	return;
}

void ZombieGuiSplash::Draw(GRender& inout_render)
{
	boost::shared_ptr<GMaterialInstance> pMaterial = mMaterial.lock();
	if (pMaterial)
		inout_render.SetMaterial(*pMaterial);

	boost::shared_ptr<GGui> pSplash = mSplash.lock();
	if (pSplash)
	{
		GGui& gui = *pSplash;
		GGuiVisitorUpdate::Run(gui);
		GGuiVisitorRender::Run(gui, inout_render);
	}
}
