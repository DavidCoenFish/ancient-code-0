//
//  ZombieMissionVarMission00.cpp
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieMissionVarMission00.h"

#include "ZombieMission.h"

#include <GApplicationWindow.h>
#include <GInput.h>
#include <GInputJoystick.h>

#include <GVector2Float.h>
#include <GGuiCoordinate.h>
#include <GColour4Byte.h>
#include <GColour4Float.h>




#ifdef WIN32
#define ZOMBIE_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define ZOMBIE_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const char* const sHelpBackground = ZOMBIE_NAME_MACRO("helpbackground.png", "win32\\helpbackground.bmp");
static const char* const sHelpCamera = ZOMBIE_NAME_MACRO("helpcamera.png", "win32\\helpcamera.bmp");
static const char* const sHelpDevice = ZOMBIE_NAME_MACRO("helpdevice.png", "win32\\helpdevice.bmp");
static const char* const sHelpFinger = ZOMBIE_NAME_MACRO("helpfinger.png", "win32\\helpfinger.bmp");
static const char* const sHelpFire = ZOMBIE_NAME_MACRO("helpfire.png", "win32\\helpfire.bmp");
static const char* const sHelpReload = ZOMBIE_NAME_MACRO("helpreload.png", "win32\\helpreload.bmp");

//constructor
ZombieMissionVarMission00::ZombieMissionVarMission00(GApplicationWindow& inout_applicationWindow)
: mApplicationWindow(inout_applicationWindow)
, mTimeWatchingFeet(0.0F)
, mHasLookedUp(false)
, mTimeWithoutFire(0.0F)
, mHasFired(false)
, mReloadRequestCount(0)
, mHasReloaded(false)
, mTutorialCompleteTimer(0.0F)
, mActivatedAi(false)
, mDialogActiveLookUp(false)
, mDialogActiveFire(false)
, mDialogActiveReload(false)
, mDialogBackground()
, mDialogImageOne()
, mDialogImadeTwo()
, mDialogText()
{
	return;
}

ZombieMissionVarMission00::~ZombieMissionVarMission00()
{
	if (mDialogActiveReload ||
		mDialogActiveFire ||
		mDialogActiveLookUp
		)
	{
		DeactivateDialog();
	}
	
	return;
}

//public methods
void ZombieMissionVarMission00::Tick(
	const float in_timeDelta,
	ZombieMission& inout_mission
	)
{
	if (inout_mission.GetPause())
	{
		return;
	}

	if (!mHasLookedUp)
	{
		mTimeWatchingFeet += in_timeDelta;

		if (!mDialogActiveLookUp && (5.0F < mTimeWatchingFeet))
		{
			DeactivateDialog();
		
			inout_mission.SetPause(true);
			mDialogActiveLookUp = true;

			//mBackground = mApplicationWindow.CreateViewComponent(
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
			//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
			//	GColour4Float(0.0F, 0.0F, 0.0F, 0.5F)
			//	);
			//mApplicationWindow.AddViewComponentFront(mBackground);

			//mDialogBackground = mApplicationWindow.CreateViewComponentImage(
			//	sHelpBackground,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(256, 256))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogBackground);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogImageOne = mApplicationWindow.CreateViewComponentImage(
			//	sHelpDevice,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogImageOne);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogImadeTwo = mApplicationWindow.CreateViewComponentImage(
			//	sHelpCamera,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(0, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogImadeTwo);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogText = mApplicationWindow.CreateViewComponentText(
			//	"helpLookUpKey",
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 0)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 120)),
			//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
			//	false,
			//	true
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogText);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}
		}
	}
	if (!mHasFired && mHasLookedUp)
	{
		mTimeWithoutFire += in_timeDelta;

		if (!mDialogActiveFire && (5.0F < mTimeWithoutFire))
		{
			DeactivateDialog();
			
			inout_mission.SetPause(true);
			mDialogActiveFire = true;

			//mBackground = mApplicationWindow.CreateViewComponent(
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
			//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
			//	GColour4Float(0.0F, 0.0F, 0.0F, 0.5F)
			//	);
			//mApplicationWindow.AddViewComponentFront(mBackground);

			//mDialogBackground = mApplicationWindow.CreateViewComponentImage(
			//	sHelpBackground,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(256, 256))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogBackground);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogImageOne = mApplicationWindow.CreateViewComponentImage(
			//	sHelpFinger,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogImageOne);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogImadeTwo = mApplicationWindow.CreateViewComponentImage(
			//	sHelpFire,
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(0, -128)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogImadeTwo);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}

			//mDialogText = mApplicationWindow.CreateViewComponentText(
			//	"helpTouchKey",
			//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 0)),
			//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 120)),
			//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
			//	false,
			//	true
			//	);
			//{
			//	TPointerApplicationViewComponent downcast(mDialogText);
			//	mApplicationWindow.AddViewComponentFront(downcast);
			//}
		}
	}

	//ok, player has reloaded, fired and looked up, make sure the ai is active
	if ((!mActivatedAi) && mHasFired && mHasLookedUp && mHasReloaded)
	{
		mTutorialCompleteTimer += in_timeDelta;
		if (3.0F < mTutorialCompleteTimer)
		{
			mActivatedAi = true;
			inout_mission.SetOpponentAiActive("one", true);
		}
	}
	return;
}

void ZombieMissionVarMission00::Input(const GInput& in_input, ZombieMission& inout_mission)
{
	if (mHasLookedUp)
	{
		return;
	}
	if (0 < in_input.GetJoystickCount())
	{
		const GInputJoystick& joystick = in_input.GetJoystick(0);
		const GVector3Float norm = Normalise(joystick.GetDirection());
		if (-0.9F < norm.m_z)
		{
			LookUp(inout_mission);
		}
	}
	return;
}

void ZombieMissionVarMission00::IncrementRequestReload(
	ZombieMission& inout_mission
	)
{
	//if (mHasReloaded)
	//{
	//	return;
	//}

	mReloadRequestCount += 1;

	if ((4 < mReloadRequestCount) && (!mDialogActiveReload))
	{
		DeactivateDialog();
		
		inout_mission.SetPause(true);
		mDialogActiveReload = true;

		//mBackground = mApplicationWindow.CreateViewComponent(
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
		//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
		//	GColour4Float(0.0F, 0.0F, 0.0F, 0.5F)
		//	);
		//mApplicationWindow.AddViewComponentFront(mBackground);

		//mDialogBackground = mApplicationWindow.CreateViewComponentImage(
		//	sHelpBackground,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(256, 256))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogBackground);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogImageOne = mApplicationWindow.CreateViewComponentImage(
		//	sHelpDevice,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogImageOne);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogImadeTwo = mApplicationWindow.CreateViewComponentImage(
		//	sHelpReload,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(0, -128)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogImadeTwo);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogText = mApplicationWindow.CreateViewComponentText(
		//	"helpReloadKey",
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 0)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 120)),
		//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
		//	false,
		//	true
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogText);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

	}

	return;
}

void ZombieMissionVarMission00::LookUp(ZombieMission& inout_mission)
{
	mHasLookedUp = true;

	if (mDialogActiveLookUp)
	{
		mDialogActiveLookUp = false;
		inout_mission.SetPause(false);
		DeactivateDialog();
	}

	return;
}

void ZombieMissionVarMission00::Fire(
	ZombieMission& inout_mission
	)
{
	mHasFired = true;

	if (mDialogActiveFire)
	{
		mDialogActiveFire = false;
		inout_mission.SetPause(false);
		DeactivateDialog();
	}

	return;
}

void ZombieMissionVarMission00::Reload(
	ZombieMission& inout_mission									   
	)
{
	mHasReloaded = true;
	mReloadRequestCount = 0;

	if (mDialogActiveReload)
	{
		mDialogActiveReload = false;
		inout_mission.SetPause(false);
		DeactivateDialog();
	}

	return;
}

//private methods
void ZombieMissionVarMission00::DeactivateDialog()
{	
	//trying to catch a wierd bug where the reload dialog was not dismissed
	mDialogActiveReload = false;
	mDialogActiveFire = false;
	mDialogActiveLookUp = false;

	//if (mBackground)
	//{
	//	mApplicationWindow.RemoveViewComponent(mBackground);
	//	mBackground.reset();
	//}
	//if (mDialogBackground)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogBackground);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogBackground.reset();
	//}
	//if (mDialogImageOne)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogImageOne);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogImageOne.reset();
	//}
	//if (mDialogImadeTwo)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogImadeTwo);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogImadeTwo.reset();
	//}
	//if (mDialogText)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogText);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogText.reset();
	//}

	return;
}
