//
//  ZombieGameVar.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGameVar.h"
#include "ZombieMission.h"

#include <GBuffer.h>
#include <GDictionary.h>
#include <GFileSystem.h>
#include <GStringUtility.h>
#include <GCommon.h>
#include <GMath.h>

//#define MAX_DAMAGE

static const char* const sDictionaryPersistantFileName = "dat1.data";
static const char* const sDictionaryPersistantKilledFirstZombie = "KilledFirstZombie"; //kill by first zombie
static const char* const sDictionaryPersistantNewGameCount = "DictionaryPersistantNewGameCount";
static const char* const sDictionaryPersistantKillCount = "DictionaryPersistantKillCount";

static const char* const sDictionaryFileName = "dat0.data";
static const char* const sGameDifficulty = "GameDifficulty";
static const char* const sShotType = "ShotType";
static const char* const sMissionId = "MissionId";
static const char* const sPlayerPosition = "PlayerPosition";
static const char* const sPlayerDirection = "PlayerDirection";
static const char* const sAmmoCountCurrent = "AmmoCountCurrent";
static const char* const sAmmoCountMaximum = "AmmoCountMaximum";
static const char* const sLifeCountCurrent = "LifeCountCurrent";
static const char* const sLifeCountMaximum = "LifeCountMaximum";
static const char* const sShotDamage = "ShotDamage";
static const char* const sShotRadius = "ShotRadius";
static const char* const sShotRange = "ShotRange";
static const char* const sZombieDamageFactor = "ZombieDamageFactor";
static const char* const sZombiePartHealthBase = "ZombiePartHealthBase";
static const char* const sZombiePartHealthRange = "ZombiePartHealthRange";
static const char* const sMaggotCountHigh = "MaggotCountHigh";
static const char* const sMaggotCountLow = "MaggotCountLow";
static const char* const sPickupHealthCount = "PickupHealthCount";
static const char* const sPickupAmmoCount = "PickupAmmoCount";
static const char* const sDamageTaken = "DamageTaken";
static const char* const sHitCount = "HitCount";
static const char* const sMissCount = "MissCount";
static const char* const sReloadRequestCount = "ReloadRequestCount";
static const char* const sHitHeadFirstCount = "HitHeadFirstCount";
static const char* const sMissHeadFirstCount = "MissHeadFirstCount"; 

static const int sDefaultAmmoMaximum = 6;
static const int sDefaultLifeMaximum = 8;
static const float sDefaultShotDamage = 1.0F;
static const float sDefaultShotRadius = 1.5F / 16.0F;
static const float sDefaultShotRange = 150.0F;

struct TPlayerId
{
	enum TEnum
	{
		TSelf = 0,
		TRescue,
		TOther,
		TCount
	};
};

static const char* const sPlayerFactoryStubName[TPlayerId::TCount] = 
{
	"FactoryChico",
	"FactoryTanya",
	"FactorySir"
};

static const int LocalFibinarchie(const int in_value)
{
	if (0 < in_value)
	{
		return (LocalFibinarchie(in_value - 1) + in_value);
	}
	
	return 0;
}

/*
				easy	normal	hard
first visit		1		1.5		2.5
2nd				1.5		2.5		4
3rd				2.5		4		6
4th				4		6		8.5
5th				6		8.5		11.5
*/
static const float LocalCalculateZombieHealthFactor(
	const ZombieStateType::TGameDifficulty::TEnum in_difficulty, 
	const int in_count
	)
{
	int value = in_count;
	float mul = 0.5F;
	switch (in_difficulty)
	{
	default:
		break;
	case ZombieStateType::TGameDifficulty::TNormal:
		value += 1;
		mul = 0.4F;
		break;
	case ZombieStateType::TGameDifficulty::THard:
		value += 2;
		mul = 0.3F;
		break;
	}
	const int fib = LocalFibinarchie(value);

	float result = 1.0F + (((float)fib) * mul);
	return result;
}

static const float LocalGetShotDamage(
	const ZombieStateType::TShotType::TEnum in_shotType
	)
{
	float result = sDefaultShotDamage;
	switch (in_shotType)
	{
	default:
		break;
	case ZombieStateType::TShotType::TSilver:
		result *= 2.0F;
		break;
	case ZombieStateType::TShotType::TGold:
		result *= 4.0F;
		break;
	}
	return result;
}

//static public methods
/*static*/ const int ZombieGameVar::GetPlayerIdSelf()
{
	return TPlayerId::TSelf;
}

/*static*/ const int ZombieGameVar::GetPlayerIdRescue()
{
	return TPlayerId::TRescue;
}

/*static*/ const int ZombieGameVar::GetPlayerIdOther()
{
	return TPlayerId::TOther;
}

//constructor
ZombieGameVar::ZombieGameVar(TPointerFileSystem& inout_fileSystem)
: mFileSystem(inout_fileSystem)
, mDictionaryPersistant()
, mDictionaryGameVar()
{
	mDictionaryPersistant.reset(new GDictionary);

	mDictionaryPersistant->SetValueInt(sDictionaryPersistantKilledFirstZombie, 0);
	mDictionaryPersistant->SetValueInt(sDictionaryPersistantNewGameCount, 0);
	mDictionaryPersistant->SetValueInt(sDictionaryPersistantKillCount, 0);
	for (int index = 0; index < ZombieStateType::TAchieviement::TCount; ++index)
	{
		mDictionaryPersistant->SetValueInt(ZombieStateType::GetAchieviementString((ZombieStateType::TAchieviement::TEnum)index), 0);
	}

	if (inout_fileSystem)
	{
		GBuffer buffer;
		const std::string filePath = inout_fileSystem->AppendFileToPathName(inout_fileSystem->GetWritableDir(), sDictionaryPersistantFileName);
		if (inout_fileSystem->LoadFile(buffer, filePath, false))
		{
			GDictionary tempDictionary;
			GDictionary::LoadFromBuffer(tempDictionary, buffer);

			//don't union, cull garbage data...
			GDictionary::SelfUpdateIntersection(*mDictionaryPersistant, tempDictionary);
		}
	}

	ResetGameVar();

	return;
}

ZombieGameVar::~ZombieGameVar()
{
	return;
}

//public methods
void ZombieGameVar::ResetGameVar()
{
	mDictionaryGameVar.reset(new GDictionary);

	//some default values for the intro sceen
	SetZombieHealthFactor(1.0F);
	SetZombiePartHealthBase(0.25F);
	SetZombiePartHealthRange(0.75F);

	UpdateUnlockEffect();

	//default game play
	SetAmmoCountCurrent(GetAmmoCountMaximum());
	SetLifeCountCurrent(GetLifeCountMaximum());

	return;
}

void ZombieGameVar::RestoreGame()
{
	mDictionaryGameVar.reset(new GDictionary);

	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (fileSystem && mDictionaryGameVar)
	{
		GBuffer buffer;		
		const std::string filePath = fileSystem->AppendFileToPathName(fileSystem->GetWritableDir(), sDictionaryFileName);
		if (fileSystem->LoadFile(buffer, filePath, false))
		{
			GDictionary::LoadFromBuffer(*mDictionaryGameVar, buffer);
			UpdateUnlockEffect();
		}
		else
		{
			UpdateUnlockEffect();
			SetAmmoCountCurrent(GetAmmoCountMaximum());
			SetLifeCountCurrent(GetLifeCountMaximum());
		}
	}

	return;
}


void ZombieGameVar::SetUnlockState(
	const ZombieStateType::TAchieviement::TEnum in_achievement, 
	const ZombieStateType::TAchieviementState::TEnum in_state,
	const bool in_okToSave
	)
{
	mDictionaryPersistant->SetValueInt(
		ZombieStateType::GetAchieviementString(in_achievement), 
		in_state
		);

	UpdateUnlockEffect();
	if (in_okToSave)
	{
		SavePersistantData();
	}

	return;
}

const ZombieStateType::TAchieviementState::TEnum ZombieGameVar::GetUnlockState(
	const ZombieStateType::TAchieviement::TEnum in_achievement
	)
{
	const ZombieStateType::TAchieviementState::TEnum result = (ZombieStateType::TAchieviementState::TEnum)mDictionaryPersistant->GetValueInt(
		ZombieStateType::GetAchieviementString(in_achievement)
		);
	return result;
}

//public accessors
const int ZombieGameVar::GetPersistanceKilledFirstZombieEasy()const
{
	return mDictionaryPersistant->GetValueInt(sDictionaryPersistantKilledFirstZombie);
}

void ZombieGameVar::SetPersistanceKilledFirstZombieEasy(const int in_value)
{
	mDictionaryPersistant->SetValueInt(sDictionaryPersistantKilledFirstZombie, in_value);
	return;
}

const int ZombieGameVar::GetPersistanceNewGameCount()const
{
	return mDictionaryPersistant->GetValueInt(sDictionaryPersistantNewGameCount);
}

void ZombieGameVar::SetPersistanceNewGameCount(const int in_value)
{
	mDictionaryPersistant->SetValueInt(sDictionaryPersistantNewGameCount, in_value);
	return;
}

const int ZombieGameVar::GetPersistanceKillCount()const
{
	return mDictionaryPersistant->GetValueInt(sDictionaryPersistantKillCount);
}

void ZombieGameVar::SetPersistanceKillCount(const int in_value)
{
	mDictionaryPersistant->SetValueInt(sDictionaryPersistantKillCount, in_value);
	return;
}

const ZombieStateType::TGameDifficulty::TEnum ZombieGameVar::GetGameDifficulty()const
{
	return (ZombieStateType::TGameDifficulty::TEnum)mDictionaryGameVar->GetValueInt(sGameDifficulty);
}

void ZombieGameVar::SetGameDifficulty(const ZombieStateType::TGameDifficulty::TEnum in_value)
{
	mDictionaryGameVar->SetValueInt(sGameDifficulty, in_value);
	return;
}

const ZombieStateType::TShotType::TEnum ZombieGameVar::GetShotType()const
{
	return (ZombieStateType::TShotType::TEnum)mDictionaryGameVar->GetValueInt(sShotType);
}

void ZombieGameVar::SetShotType(const ZombieStateType::TShotType::TEnum in_value)
{
	mDictionaryGameVar->SetValueInt(sShotType, in_value);
	return;
}

const std::string ZombieGameVar::GetPlayerFactoryStubName(const int in_playerId)const
{
	const int index = (GetGameDifficulty() + in_playerId) % TPlayerId::TCount;
	return sPlayerFactoryStubName[index];
}

const int ZombieGameVar::GetMissionId()const
{
	return mDictionaryGameVar->GetValueInt(sMissionId);
}

void ZombieGameVar::SetMissionId(const int in_missionID, const bool in_restore)
{
	SavePersistantData(); //return to intro (after death), or each new level

	mDictionaryGameVar->SetValueInt(sMissionId, in_missionID);

	const char* const missionName = ZombieMission::GetMissionName(in_missionID);
	if ((ZombieMission::GetMissionIdIntro() == in_missionID) ||
		(0 == missionName))
	{
		return;
	}

	int count = mDictionaryGameVar->GetValueInt(missionName);
	if (in_restore)
	{
		count -= 1;
	}
	else
	{
		mDictionaryGameVar->SetValueInt(missionName, count + 1);
	}

	const ZombieStateType::TGameDifficulty::TEnum difficulty = GetGameDifficulty();
	const float zombieHealthFactor = LocalCalculateZombieHealthFactor(difficulty, count);

	SetZombieHealthFactor(zombieHealthFactor);
	SetZombiePartHealthBase(zombieHealthFactor * 0.25F);
	SetZombiePartHealthRange(zombieHealthFactor * 0.75F);

	//for game restore
	if (!in_restore)
	{
		SaveData();
	}

	return;
}

const int ZombieGameVar::GetMissionCount(const int in_index)const
{
	int count = 0;
	const char* const missionName = ZombieMission::GetMissionName(in_index);
	if (missionName)
	{
		mDictionaryGameVar->GetValueInt(missionName);
	}

	return count;
}

static const std::string LocalGetMissionUsedName(const int in_index)
{
	return (std::string("missionUsed") + GStringUtility::SafeString(ZombieMission::GetMissionName(in_index)));
}

//for bonus
const bool ZombieGameVar::GetMissionHasBeenUsed(const int in_index)const
{
	const std::string name = LocalGetMissionUsedName(in_index);
	const bool result = (0 != mDictionaryGameVar->GetValueInt(name));
	return result;
}

void ZombieGameVar::SetMissionHasBeenUsed(const int in_index)
{
	const std::string name = LocalGetMissionUsedName(in_index);
	mDictionaryGameVar->SetValueInt(name, 1);
	return;
}

void ZombieGameVar::SetUpBossHealth()
{
	SetBossHealthAfterWave(-1);
	return;
}
void ZombieGameVar::SetBossHealthAfterWave(const int in_waveIndex)
{
	const ZombieStateType::TGameDifficulty::TEnum difficulty = GetGameDifficulty();
	switch (difficulty)
	{
	default:
	case ZombieStateType::TGameDifficulty::TNormal:
		SetMaggotCountHigh(18 - ((in_waveIndex + 1) * 6));
		SetMaggotCountLow(12 - ((in_waveIndex + 1) * 6));
		break;
	case ZombieStateType::TGameDifficulty::TEasy:
		SetMaggotCountHigh(9 - ((in_waveIndex + 1) * 3));
		SetMaggotCountLow(6 - ((in_waveIndex + 1) * 3));
		break;
	case ZombieStateType::TGameDifficulty::THard:
		SetMaggotCountHigh(27 - ((in_waveIndex + 1) * 9));
		SetMaggotCountLow(18 - ((in_waveIndex + 1) * 9));
		break;
	}
	return;
}

const int ZombieGameVar::GetMaggotCountHigh()const
{
	return mDictionaryGameVar->GetValueInt(sMaggotCountHigh);
}

void ZombieGameVar::SetMaggotCountHigh(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sMaggotCountHigh, in_value);
	return;
}

const int ZombieGameVar::GetMaggotCountLow()const
{
	return mDictionaryGameVar->GetValueInt(sMaggotCountLow);
}

void ZombieGameVar::SetMaggotCountLow(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sMaggotCountLow, in_value);
	return;
}

const int ZombieGameVar::GetPickupHealthCount()const
{
	return mDictionaryGameVar->GetValueInt(sPickupHealthCount);
}

void ZombieGameVar::SetPickupHealthCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sPickupHealthCount, in_value);
	UpdateUnlockEffect();
	return;
}
const int ZombieGameVar::GetPickupAmmoCount()const
{
	return mDictionaryGameVar->GetValueInt(sPickupAmmoCount);
}

void ZombieGameVar::SetPickupAmmoCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sPickupAmmoCount, in_value);
	UpdateUnlockEffect();
	return;
}

const GVector3Float& ZombieGameVar::GetPlayerPosition()
{
	return mDictionaryGameVar->GetValueVector3Float(sPlayerPosition);
}

void ZombieGameVar::SetPlayerPosition(const GVector3Float& in_position)
{
	mDictionaryGameVar->SetValueVector3Float(sPlayerPosition, in_position);
	return;
}

const GVector3Float& ZombieGameVar::GetPlayerDirection()
{
	return mDictionaryGameVar->GetValueVector3Float(sPlayerDirection);
}

void ZombieGameVar::SetPlayerDirection(const GVector3Float& in_direction)
{
	return mDictionaryGameVar->SetValueVector3Float(sPlayerDirection, in_direction);
}

const int ZombieGameVar::GetAmmoCountCurrent()const
{
	return mDictionaryGameVar->GetValueInt(sAmmoCountCurrent);
}

void ZombieGameVar::SetAmmoCountCurrent(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sAmmoCountCurrent, in_value);
	return;
}

const int ZombieGameVar::GetAmmoCountMaximum()const
{
	return mDictionaryGameVar->GetValueInt(sAmmoCountMaximum);
}

void ZombieGameVar::SetAmmoCountMaximum(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sAmmoCountMaximum, in_value);
	return;
}

const int ZombieGameVar::GetLifeCountCurrent()const
{
	return mDictionaryGameVar->GetValueInt(sLifeCountCurrent);
}
void ZombieGameVar::SetLifeCountCurrent(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sLifeCountCurrent, in_value);
	return;
}

const int ZombieGameVar::GetLifeCountMaximum()const
{
	return mDictionaryGameVar->GetValueInt(sLifeCountMaximum);
}
void ZombieGameVar::SetLifeCountMaximum(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sLifeCountMaximum, in_value);
	return;
}

const int ZombieGameVar::GetDamageTaken()const
{
	return mDictionaryGameVar->GetValueInt(sDamageTaken);
}

void ZombieGameVar::SetDamageTaken(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sDamageTaken, in_value);
	return;
}

const int ZombieGameVar::GetHitCount()const
{
	return mDictionaryGameVar->GetValueInt(sHitCount);
}

void ZombieGameVar::SetHitCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sHitCount, in_value);
	return;
}

const int ZombieGameVar::GetMissCount()const
{
	return mDictionaryGameVar->GetValueInt(sMissCount);
}

void ZombieGameVar::SetMissCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sMissCount, in_value);
	return;
}

const int ZombieGameVar::GetHitHeadFirstCount()const
{
	return mDictionaryGameVar->GetValueInt(sHitHeadFirstCount);
}

void ZombieGameVar::SetHitHeadFirstCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sHitHeadFirstCount, in_value);
	return;
}

const int ZombieGameVar::GetMissHeadFirstCount()const
{
	return mDictionaryGameVar->GetValueInt(sMissHeadFirstCount);
}

void ZombieGameVar::SetMissHeadFirstCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sMissHeadFirstCount, in_value);
	return;
}

const int ZombieGameVar::GetReloadRequestCount()const
{
	return mDictionaryGameVar->GetValueInt(sReloadRequestCount);
}

void ZombieGameVar::SetReloadRequestCount(const int in_value)
{
	mDictionaryGameVar->SetValueInt(sReloadRequestCount, in_value);
	return;
}

const float ZombieGameVar::GetShotDamage()const
{
	return mDictionaryGameVar->GetValueFloat(sShotDamage);
}

void ZombieGameVar::SetShotDamage(const float in_value)
{
	mDictionaryGameVar->SetValueFloat(sShotDamage, in_value);
	return;
}

const float ZombieGameVar::GetShotRadius()const
{
	return sDefaultShotRadius;
	//return mDictionaryGameVar->GetValueFloat(sShotRadius);
}

//void ZombieGameVar::SetShotRadius(const float in_value)
//{
//	mDictionaryGameVar->SetValueFloat(sShotRadius, in_value);
//	return;
//}

const float ZombieGameVar::GetShotRange()const
{
	return sDefaultShotRange;
	//return mDictionaryGameVar->GetValueFloat(sShotRange);
}

//void ZombieGameVar::SetShotRange(const float in_value)
//{
//	mDictionaryGameVar->SetValueFloat(sShotRange, in_value);
//	return;
//}

const float ZombieGameVar::GetZombieHealthFactor()const
{
	return mDictionaryGameVar->GetValueFloat(sZombieDamageFactor);
}

void ZombieGameVar::SetZombieHealthFactor(const float in_value)
{
	mDictionaryGameVar->SetValueFloat(sZombieDamageFactor, in_value);
	return;
}

const float ZombieGameVar::GetZombiePartHealthBase()const
{
	return mDictionaryGameVar->GetValueFloat(sZombiePartHealthBase);
}

void ZombieGameVar::SetZombiePartHealthBase(const float in_value)
{
	mDictionaryGameVar->SetValueFloat(sZombiePartHealthBase, in_value);
	return;
}

const float ZombieGameVar::GetZombiePartHealthRange()const
{
	return mDictionaryGameVar->GetValueFloat(sZombiePartHealthRange);
}

void ZombieGameVar::SetZombiePartHealthRange(const float in_value)
{
	mDictionaryGameVar->SetValueFloat(sZombiePartHealthRange, in_value);
	return;
}

//private methods
void ZombieGameVar::SavePersistantData()
{
	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (fileSystem && mDictionaryPersistant)
	{
		GBuffer buffer;
		GDictionary::WriteToBuffer(buffer, *mDictionaryPersistant);
		
		const std::string filePath = fileSystem->AppendFileToPathName(fileSystem->GetWritableDir(), sDictionaryPersistantFileName);
		fileSystem->SaveFile(buffer, filePath);
	}
	return;
}

void ZombieGameVar::SaveData()
{
	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (fileSystem && mDictionaryGameVar)
	{
		GBuffer buffer;
		GDictionary::WriteToBuffer(buffer, *mDictionaryGameVar);
		
		const std::string filePath = fileSystem->AppendFileToPathName(fileSystem->GetWritableDir(), sDictionaryFileName);
		fileSystem->SaveFile(buffer, filePath);
	}
	return;
}

void ZombieGameVar::UpdateUnlockEffect()
{
	int ammoPlus = GetPickupAmmoCount();
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TAccuracy))
	{
		ammoPlus += 1;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TEfficiency))
	{
		ammoPlus += 1;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::THeadHunter))
	{
		ammoPlus += 1;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TGoodWork))
	{
		ammoPlus += 1;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TPercistance))
	{
		ammoPlus += 1;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TEradicator))
	{
		ammoPlus += 1;
	}
	SetAmmoCountMaximum(GMath::Minimum(18, sDefaultAmmoMaximum + ammoPlus));

	int healthPlus = GetPickupHealthCount();
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TSlugQueen))
	{
		healthPlus += 2;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TBulletProof))
	{
		healthPlus += 2;
	}
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TNewb))
	{
		healthPlus += 2;
	}
	SetLifeCountMaximum(GMath::Minimum(18, sDefaultLifeMaximum + healthPlus));

#ifdef MAX_DAMAGE
	SetShotDamage(8.0F);
#else
	float mul = 1.0F;
	if (ZombieStateType::TAchieviementState::TUnlockedActive == GetUnlockState(ZombieStateType::TAchieviement::TRock))
	{
		mul = 2.0F;
	}
	SetShotDamage(GMath::Minimum(8.0F, LocalGetShotDamage(GetShotType()) * mul));
#endif

	return;
}
