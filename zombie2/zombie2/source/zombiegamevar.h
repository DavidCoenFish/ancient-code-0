//
//  ZombieGameVar.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGameVar_h_
#define _ZombieGameVar_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <GVector2Float.h>
#include "ZombieStateType.h" //enum

class GDictionary;
class GFileSystem;
class GVector3Float;

class ZombieGameVar : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;

	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::weak_ptr<GFileSystem> TWeakFileSystem;

	typedef std::vector<GVector2Float> TArrayVector2;
	//typedef std::vector<int> TArrayInt;

	//static public methods
public:
	static const int GetPlayerIdSelf();
	static const int GetPlayerIdRescue();
	static const int GetPlayerIdOther();

	//constructor
public:
	ZombieGameVar(TPointerFileSystem& inout_fileSystem);
	~ZombieGameVar();

	//public methods
public:
	void ResetGameVar();
	void RestoreGame();

	//public accessors
public:
	const int GetPersistanceKilledFirstZombieEasy()const;
	void SetPersistanceKilledFirstZombieEasy(const int in_value);

	const int GetPersistanceNewGameCount()const;
	void SetPersistanceNewGameCount(const int in_value);

	const int GetPersistanceKillCount()const;
	void SetPersistanceKillCount(const int in_value);

	const ZombieStateType::TGameDifficulty::TEnum GetGameDifficulty()const;
	void SetGameDifficulty(const ZombieStateType::TGameDifficulty::TEnum in_value);

	const ZombieStateType::TShotType::TEnum GetShotType()const;
	void SetShotType(const ZombieStateType::TShotType::TEnum in_value);

	void SetUnlockState(
		const ZombieStateType::TAchieviement::TEnum in_achievement, 
		const ZombieStateType::TAchieviementState::TEnum in_state,
		const bool in_okToSave = true
		);

	const ZombieStateType::TAchieviementState::TEnum GetUnlockState(
		const ZombieStateType::TAchieviement::TEnum in_achievement
		);

	const std::string GetPlayerFactoryStubName(const int in_playerId)const;

	const int GetMissionId()const;
	void SetMissionId(const int in_missionID, const bool in_restore);

	const int GetMissionCount(const int in_index)const;

	//for bonus
	const bool GetMissionHasBeenUsed(const int in_index)const;
	void SetMissionHasBeenUsed(const int in_index);

	void SetUpBossHealth();
	void SetBossHealthAfterWave(const int in_waveIndex);

	const int GetMaggotCountHigh()const;
	void SetMaggotCountHigh(const int in_value);

	const int GetMaggotCountLow()const;
	void SetMaggotCountLow(const int in_value);

	const int GetPickupHealthCount()const;
	void SetPickupHealthCount(const int in_value);

	const int GetPickupAmmoCount()const;
	void SetPickupAmmoCount(const int in_value);

	//player
	const GVector3Float& GetPlayerPosition();
	void SetPlayerPosition(const GVector3Float& in_position);

	const GVector3Float& GetPlayerDirection();
	void SetPlayerDirection(const GVector3Float& in_direction);

	const int GetAmmoCountCurrent()const; //current ammo count
	void SetAmmoCountCurrent(const int in_value);

	const int GetAmmoCountMaximum()const; //max ammo count
	void SetAmmoCountMaximum(const int in_value);

	const int GetLifeCountCurrent()const; //current Life count
	void SetLifeCountCurrent(const int in_value);

	const int GetLifeCountMaximum()const; //max Life count
	void SetLifeCountMaximum(const int in_value);

	const int GetDamageTaken()const; //total of damage for the game
	void SetDamageTaken(const int in_value);

	const int GetHitCount()const; //nb shots fired that hit something
	void SetHitCount(const int in_value);

	const int GetMissCount()const; //nb shots fired that didn't hit anything
	void SetMissCount(const int in_value);

	const int GetHitHeadFirstCount()const;
	void SetHitHeadFirstCount(const int in_value);

	const int GetMissHeadFirstCount()const;
	void SetMissHeadFirstCount(const int in_value);

	const int GetReloadRequestCount()const;
	void SetReloadRequestCount(const int in_value);

	const float GetShotDamage()const; 
	void SetShotDamage(const float in_value);

	const float GetShotRadius()const; 
	//void SetShotRadius(const float in_value);

	const float GetShotRange()const; 
	//void SetShotRange(const float in_value);

	//current level var
	const float GetZombieHealthFactor()const; //modifier on the damage taken by the zombie
	void SetZombieHealthFactor(const float in_value);

	const float GetZombiePartHealthBase()const; //each part of the zombie has this much health
	void SetZombiePartHealthBase(const float in_value);

	const float GetZombiePartHealthRange()const; //random range added to each zombie part health
	void SetZombiePartHealthRange(const float in_value);

	const TArrayVector2& GetArrayScreenTouch()const{ return mArrayScreenTouch; }
	TArrayVector2& GetArrayScreenTouch(){ return mArrayScreenTouch; }

	const TArrayVector2& GetArrayOffscreen()const{ return mArrayOffscreen; }
	TArrayVector2& GetArrayOffscreen(){ return mArrayOffscreen; }

	//const TArrayInt& GetArrayDamageType()const{ return mArrayDamageType; }
	//TArrayInt& GetArrayDamageType(){ return mArrayDamageType; }

	void SavePersistantData();
	void SaveData();

	//private methods
private:
	void UpdateUnlockEffect();

	//private members
private:
	TWeakFileSystem mFileSystem;
	TPointerDictionary mDictionaryPersistant; //load/ save, progress, unlocks,
	TPointerDictionary mDictionaryGameVar; //current ammo, game session values

	//run time data (ie, for interface. not saved)
	TArrayVector2 mArrayScreenTouch;
	TArrayVector2 mArrayOffscreen;
	//TArrayInt mArrayDamageType;
};

#endif //_ZombieGameVar_h_
