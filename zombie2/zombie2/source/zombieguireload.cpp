//
//  ZombieGuiReload.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiReload.h"
#include "ZombieMission.h"

#include <GApplicationWindow.h>
#include <GComponentLerpFloat.h>
//
#include <GVector2Float.h>
#include <GGuiCoordinate.h>
//
//
#include <GColour4Float.h>

static const float sReloadTextFadeTime = 3.0F;

#ifdef WIN32
#define WIDGIT_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define WIDGIT_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const char* const sButtonWidgit = WIDGIT_NAME_MACRO("dialogbutton_00.png", "win32\\dialogbutton_00.bmp");

/*static*/ void ZombieGuiReload::ButtonClickCallback(void* const in_data)
{
	if (!in_data)
	{
		return;
	}

	ZombieGuiReload& localThis = *(ZombieGuiReload*)in_data;
	localThis.mParentMission.Reload();

	return;
}


//constructor
ZombieGuiReload::ZombieGuiReload(
	ZombieMission& inout_parentMission,
	GApplicationWindow& inout_window
	)
: mParentMission(inout_parentMission)
, mApplicationWindow(inout_window)
, mViewComponentText()
, mComponentReloadButton()
{
//	{
//		GGuiCoordinate position(GVector2Float(0.5F, 1.0F), GVector2Float(-120, -48));
//		GGuiCoordinate size(GVector2Float(0.0F, 0.0F), GVector2Float(240, 32));
//		mViewComponentText = inout_window.CreateViewComponentText(
//#ifdef WIN32
//			"Reload",
//#else
//			"reloadkey", 
//#endif
//			position, 
//			size,
//			GColour4Float::sWhite,
//			true
//			);
//		TPointerApplicationViewComponent viewComponent = mViewComponentText;
//		mApplicationWindow.AddViewComponentFront(viewComponent);
//	}

	return;
}

ZombieGuiReload::~ZombieGuiReload()
{
	//if (mViewComponentText)
	//{
	//	TPointerApplicationViewComponent viewComponent = mViewComponentText;
	//	mApplicationWindow.RemoveViewComponent(viewComponent);		
	//}
	return;
}

//public methods
void ZombieGuiReload::ResetVisual()
{
	//if (mComponentReloadButton)
	//{
	//	mApplicationWindow.RemoveViewComponent(mComponentReloadButton);
	//	mComponentReloadButton->Hide();
	//}
	//if (mViewComponentText)
	//{
	//	mViewComponentText->Hide();
	//}
	return;
}

void ZombieGuiReload::RequestReloadPrompt()
{
	//if (mComponentReloadButton)
	//{
	//	mApplicationWindow.AddViewComponentFront(mComponentReloadButton);
	//	mComponentReloadButton->Show();
	//}
	//if (mViewComponentText)
	//{
	//	mViewComponentText->Show();
	//}

	return;
}


