//
//  ZombieMissionAchievement.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieMissionAchievement.h"

#include "ZombieMission.h"
#include "ZombieGameVar.h"

#include <GApplicationWindow.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GVector2Float.h>
#include <GGuiCoordinate.h>
#include <GColour4Byte.h>
#include <GColour4Float.h>
#include <GMath.h>

#ifdef WIN32
#define ZOMBIE_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define ZOMBIE_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const char* const sDialogBackground = ZOMBIE_NAME_MACRO("helpbackground.png", "win32\\helpbackground.bmp");
static const char* const sImageAddShotPower = ZOMBIE_NAME_MACRO("helpfire.png", "win32\\helpfire.bmp");
static const char* const sImageAddShell = ZOMBIE_NAME_MACRO("helpreload.png", "win32\\helpreload.bmp");
static const char* const sImageAddHealth = ZOMBIE_NAME_MACRO("helphealth.png", "win32\\helphealth.bmp");
static const char* const sImageHeal = ZOMBIE_NAME_MACRO("helphealth.png", "win32\\helphealth.bmp");
static const char* const sImageTrophy = ZOMBIE_NAME_MACRO("trophy.png", "win32\\trophy.bmp");

static const float sDisplayDialogDuration = 3.0F;

static const char* const LocalGetImage(const ZombieStateType::TBonus::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case ZombieStateType::TBonus::TAddAmmo:
		return sImageAddShell;
	case ZombieStateType::TBonus::TAddHealth:
		return sImageAddHealth;
	case ZombieStateType::TBonus::TAddShotPower:
		return sImageAddShotPower;
	case ZombieStateType::TBonus::THeal:
		return sImageHeal;
	case ZombieStateType::TBonus::TFinishedGame:
		return sImageTrophy;
	}

	return "";
}


//constructor
ZombieMissionAchievement::ZombieMissionAchievement(GApplicationWindow& inout_applicationWindow)
: mApplicationWindow(inout_applicationWindow)
, mCountdown(0.0F)
//, mDialogBackground()
//, mDialogImage()
//, mDialogText0()
//, mDialogText1()
, mArrayAchieviement()
, mArrayBonus()
{
	return;
}

ZombieMissionAchievement::~ZombieMissionAchievement()
{
	DeactivateDialog();
	
	return;
}

//public methods
const bool ZombieMissionAchievement::CalculateAcheivementLevelEnd(
	ZombieGameVar& in_gameVar, 
	const int in_actorCount,	
	const bool in_death
	)
{
	const ZombieStateType::TGameDifficulty::TEnum difficulty = in_gameVar.GetGameDifficulty();
	const int missionId = in_gameVar.GetMissionId();
	const int missionCount = in_gameVar.GetMissionCount(missionId);

	//////////////////////////////
	// achievements
	if (in_death)
	{
		//newb
		if ((ZombieMission::GetMissionFirst() == missionId) &&
			(ZombieStateType::TGameDifficulty::TEasy == difficulty))
		{
			const int count = in_gameVar.GetPersistanceKilledFirstZombieEasy();
			in_gameVar.SetPersistanceKilledFirstZombieEasy(count + 1);
			if ((10 <= count) &&
				(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TNewb)))
			{
				AddAchieviement(ZombieStateType::TAchieviement::TNewb, in_gameVar);
			}
		}
	}
	else
	{
		//first blood
		if ((ZombieMission::GetMissionFirst() == missionId) &&
			(ZombieStateType::TGameDifficulty::TEasy == difficulty) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TFirstBlood)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TFirstBlood, in_gameVar);
		}

		//id be back
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(ZombieStateType::TGameDifficulty::TNormal == difficulty) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TIdBeBack)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TIdBeBack, in_gameVar);
		}

		//slug queen
		if ((ZombieMission::GetMissionIdSceneSlugs() == missionId) &&
			(5 == missionCount) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TSlugQueen)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TSlugQueen, in_gameVar);
		}

		//bulletproof
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(0 == in_gameVar.GetDamageTaken()) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TSlugQueen)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TBulletProof, in_gameVar);
		}

		//accuracy
		const int hitCount = in_gameVar.GetHitCount();
		const int missCount = in_gameVar.GetMissCount();
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(((missCount) * 100) < ((hitCount + missCount) * 5)) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TAccuracy)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TAccuracy, in_gameVar);
		}

		//efficiency
		const int reloadRequestCount = in_gameVar.GetReloadRequestCount();
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(0 == reloadRequestCount) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TEfficiency)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TEfficiency, in_gameVar);
		}

		//head hunter
		const int hitHeadFirstCount = in_gameVar.GetHitHeadFirstCount();
		const int missHeadFirstCount = in_gameVar.GetMissHeadFirstCount();

		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(((missHeadFirstCount) * 100) < ((hitHeadFirstCount + missHeadFirstCount) * 20)) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TGoodWork)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TGoodWork, in_gameVar);
		}

		//good work
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(ZombieStateType::TGameDifficulty::TEasy == difficulty) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TGoodWork)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TGoodWork, in_gameVar);
		}

		//rock
		if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) &&
			(ZombieStateType::TGameDifficulty::THard == difficulty) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TRock)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TRock, in_gameVar);
		}
	}

	//Percistance
	//a game counts as played on death or beat boss, se we increment it here, not in mission::newGame
	if ((ZombieMission::GetMissionIdSceneBossFight() == missionId) || in_death)
	{
		int count = in_gameVar.GetPersistanceNewGameCount() + 1;
		if ((100 < in_gameVar.GetPersistanceNewGameCount()) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TPercistance)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TPercistance, in_gameVar);
		}

		in_gameVar.SetPersistanceNewGameCount(count);
	}

	//eradicator
	if ((1000 < in_gameVar.GetPersistanceKillCount()) &&
		(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TEradicator)))
	{
		AddAchieviement(ZombieStateType::TAchieviement::TEradicator, in_gameVar);
	}

	//mission possible
	{
		int unlockCount = 0;
		for (int index = 0; index < ZombieStateType::TAchieviement::TCount; ++index)
		{
			if (ZombieStateType::TAchieviementState::TLocked != in_gameVar.GetUnlockState((ZombieStateType::TAchieviement::TEnum)index))
			{
				unlockCount += 1;
			}
		}
		if (((unlockCount + 1) == ZombieStateType::TAchieviement::TCount) &&
			(ZombieStateType::TAchieviementState::TLocked == in_gameVar.GetUnlockState(ZombieStateType::TAchieviement::TMissionPossible)))
		{
			AddAchieviement(ZombieStateType::TAchieviement::TMissionPossible, in_gameVar);
		}
	}

	///////////////////////////////////
	//bonus
	if (in_death)
	{
		//you died, you don't get bonus
		mArrayBonus.clear();

		//if (0 == mArrayAchieviement.size())
		//{
		//	AddBonus(ZombieStateType::TBonus::TGameOver);
		//}
	}
	else
	{
		//shot power
		if ((ZombieMission::GetMissionIsPlusShotPower(missionId)) &&
			(1 == in_actorCount) && 
			(!in_gameVar.GetMissionHasBeenUsed(missionId))
			)
		{
			in_gameVar.SetMissionHasBeenUsed(missionId);
			AddBonus(ZombieStateType::TBonus::TAddShotPower);
		}

		//add health
		if ((ZombieMission::GetMissionIsPlusHealth(missionId)) &&
			(1 == in_actorCount) && 
			(!in_gameVar.GetMissionHasBeenUsed(missionId))
			)
		{
			in_gameVar.SetMissionHasBeenUsed(missionId);
			AddBonus(ZombieStateType::TBonus::TAddHealth);
		}

		//heal
		if ((ZombieMission::GetMissionIsHeal(missionId)) &&
			(1 == in_actorCount))
		{
			AddBonus(ZombieStateType::TBonus::THeal);
		}


		if ((0 == mArrayAchieviement.size()) &&
			(0 == mArrayBonus.size()) &&
			(ZombieMission::GetMissionIdSceneBossFight() == missionId))
		{
			AddBonus(ZombieStateType::TBonus::TFinishedGame);
		}
	}

	return ((0 != mArrayAchieviement.size()) || (0 != mArrayBonus.size()));
}

void ZombieMissionAchievement::AddAchieviement(const ZombieStateType::TAchieviement::TEnum in_achieviement, ZombieGameVar& inout_gameVar)
{
	mArrayAchieviement.push_back(in_achieviement);

	//apply acheivement
	inout_gameVar.SetUnlockState(in_achieviement, ZombieStateType::TAchieviementState::TUnlockedActive);

	return;
}

void ZombieMissionAchievement::AddBonus(const ZombieStateType::TBonus::TEnum in_bonus)
{
	mArrayBonus.push_back(in_bonus);
	return;
}

//return true while we are displaying dialog
const bool ZombieMissionAchievement::Tick(const float in_timeDelta, ZombieGameVar& in_gameVar)
{
	if (mActive)
	{
		mCountdown -= in_timeDelta;
		if (mCountdown <= 0.0F)
		{
			mActive = false;
			mCountdown = 0.0F;
			DeactivateDialog();
		}
	}

	if ((0.0F == mCountdown) &&
		(mArrayAchieviement.size()))
	{
		mActive = true;
		mCountdown = sDisplayDialogDuration;
		const ZombieStateType::TAchieviement::TEnum acheivement = mArrayAchieviement.front();
		mArrayAchieviement.erase(mArrayAchieviement.begin());

		////make dialog
		//mBackground = mApplicationWindow.CreateViewComponent(
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
		//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
		//	GColour4Float(0.0F, 0.0F, 0.0F, 0.5F)
		//	);
		//mApplicationWindow.AddViewComponentFront(mBackground);

		//mDialogBackground = mApplicationWindow.CreateViewComponentImage(
		//	sDialogBackground,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(256, 256))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogBackground);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogImage = mApplicationWindow.CreateViewComponentImage(
		//	sImageTrophy,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-64, -120)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogImage);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogText0 = mApplicationWindow.CreateViewComponentText(
		//	"acheivementUnlockedKey",
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 8)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 20)),
		//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
		//	false,
		//	true
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogText0);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogText1 = mApplicationWindow.CreateViewComponentText(
		//	ZombieStateType::GetAchieviementString(acheivement),
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 28)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 100)),
		//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
		//	false,
		//	true
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogText1);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}
	}

	if ((0.0F == mCountdown) &&
		(mArrayBonus.size()))
	{
		mActive = true;
		mCountdown = sDisplayDialogDuration;

		const ZombieStateType::TBonus::TEnum bonus = mArrayBonus.front();
		mArrayBonus.erase(mArrayBonus.begin());

		//apply bonus
		switch (bonus)
		{
		default:
			break;
		case ZombieStateType::TBonus::TAddAmmo:
			in_gameVar.SetPickupAmmoCount(in_gameVar.GetPickupAmmoCount() + 1);
			in_gameVar.SetAmmoCountCurrent(in_gameVar.GetAmmoCountCurrent() + 1);
			break;
		case ZombieStateType::TBonus::TAddHealth:
			in_gameVar.SetPickupHealthCount(in_gameVar.GetPickupHealthCount() + 2);
			in_gameVar.SetLifeCountCurrent(in_gameVar.GetLifeCountCurrent() + 2);
			break;
		case ZombieStateType::TBonus::TAddShotPower:
			in_gameVar.SetShotType((ZombieStateType::TShotType::TEnum)GMath::Minimum(ZombieStateType::TShotType::TCount - 1, in_gameVar.GetShotType() + 1));
			break;
		case ZombieStateType::TBonus::THeal:
			in_gameVar.SetLifeCountCurrent(in_gameVar.GetLifeCountMaximum());
			break;
		//case  ZombieStateType::TBonus::TGameOver:
		//	return true;
		}

		//make dialog
		//mBackground = mApplicationWindow.CreateViewComponent(
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(0, 0)),
		//	GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Float(0, 0)),
		//	GColour4Float(0.0F, 0.0F, 0.0F, 0.5F)
		//	);
		//mApplicationWindow.AddViewComponentFront(mBackground);

		//mDialogBackground = mApplicationWindow.CreateViewComponentImage(
		//	sDialogBackground,
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-128, -128)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(256, 256))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogBackground);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogImage = mApplicationWindow.CreateViewComponentImage(
		//	LocalGetImage(bonus),
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-64, -120)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(128, 128))
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogImage);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}

		//mDialogText1 = mApplicationWindow.CreateViewComponentText(
		//	ZombieStateType::GetBonusString(bonus),
		//	GGuiCoordinate(GVector2Float(0.5F, 0.5F), GVector2Float(-112, 8)),
		//	GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Float(224, 120)),
		//	GColour4Float(GColour4Byte(26, 28, 26, 255)),
		//	false,
		//	true
		//	);
		//{
		//	TPointerApplicationViewComponent downcast(mDialogText1);
		//	mApplicationWindow.AddViewComponentFront(downcast);
		//}
	}

	return (0.0F < mCountdown);
}

//allow dismiss dialog on touch
void ZombieMissionAchievement::Input(const GInput& in_input)
{
	if (!mActive)
	{
		return;
	}

	for (int index = 0; index < in_input.GetTouchCount(); ++index)
	{
		const GInputTouch& touch = in_input.GetTouch(index);
		if (touch.GetTouchBegin())
		{
			mCountdown = 0.0F;
			break;
		}
	}
	return;
}

//private methods
void ZombieMissionAchievement::DeactivateDialog()
{
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
	//if (mDialogImage)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogImage);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogImage.reset();
	//}
	//if (mDialogText0)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogText0);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogText0.reset();
	//}
	//if (mDialogText1)
	//{
	//	TPointerApplicationViewComponent downcast(mDialogText1);
	//	mApplicationWindow.RemoveViewComponent(downcast);
	//	mDialogText1.reset();
	//}

	return;
}
