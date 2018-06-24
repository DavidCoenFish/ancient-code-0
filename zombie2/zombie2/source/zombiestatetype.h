//
//  ZombieStateType.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 02 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieStateType_h_
#define _ZombieStateType_h_

#include <boost/noncopyable.hpp>
#include <string>

class ZombieStateType : public boost::noncopyable
{
	//typedef
public:
	struct TSuperSampleType
	{
		enum TEnum
		{
			T_1_0 = 0,
			T_1_5,
			T_2_0,
//#ifdef DSC_DEBUG
//			T_4_0,
//#endif			
			TCount
		};
	};

	struct TGameDifficulty
	{
		enum TEnum
		{
			TEasy = 0,
			TNormal,
			THard,

			TCount
		};
	};

	struct TPlayer
	{
		enum TEnum
		{
			TChico = 0,
			TTanya,
			TSir,

			TCount
		};
	};

	struct TShotType
	{
		enum TEnum
		{
			TBronze = 0,
			TSilver,
			TGold,

			TCount
		};
	};

	struct TAchieviementState
	{
		enum TEnum
		{
			TLocked = 0,
			TUnlockedActive, //client can unlock something but deactivate it
			TUnlockedDeactive,
			TCount
		};
	};

	struct TAchieviement
	{
		enum TEnum
		{
			TFirstBlood = 0,   
			TIdBeBack,         
			TSlugQueen,        
			TBulletProof,      
			TNewb,             
			TAccuracy,       
			TEfficiency,       
			THeadHunter,       
			TGoodWork,         
			TPercistance,      
			TEradicator, 
			TRock,
			TMissionPossible,

			TCount
		};
	};

	struct TBonus
	{
		enum TEnum
		{
			TAddAmmo = 0,
			TAddHealth,
			THeal,
			TAddShotPower,
			TFinishedGame,
			//TGameOver,

			TCount
		};
	};

	//disabled
private:
	~ZombieStateType();

	//public static methods
public:
	static const std::string GetSuperSampleTypeString(const TSuperSampleType::TEnum in_enum);
	static const float GetSuperSampleScale(const TSuperSampleType::TEnum in_enum);

	static const TPlayer::TEnum GetPlayerSelf(const TGameDifficulty::TEnum in_difficulty);
	static const TPlayer::TEnum GetPlayerRescue(const TGameDifficulty::TEnum in_difficulty);
	static const TPlayer::TEnum GetPlayerOther(const TGameDifficulty::TEnum in_difficulty);

	static const char* const GetAchieviementString(const TAchieviement::TEnum in_enum);
	static const char* const GetBonusString(const TBonus::TEnum in_enum);

};

#endif //_ZombieStateType_h_