//
//  ZombieStateType.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 02 04
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieStateType.h"

//public static methods
/*static*/ const std::string ZombieStateType::GetSuperSampleTypeString(const TSuperSampleType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TSuperSampleType::T_1_0:
		return "TSuperSample10";
	case TSuperSampleType::T_1_5:
		return "TSuperSample15";
	case TSuperSampleType::T_2_0:
		return "TSuperSample20";
//#ifdef DSC_DEBUG
//	case TSuperSampleType::T_4_0:
//		return "TSuperSample40";		
//#endif
	}
	return "";
}

/*static*/ const float ZombieStateType::GetSuperSampleScale(const TSuperSampleType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case ZombieStateType::TSuperSampleType::T_1_0:
		return 1.0F;
	case ZombieStateType::TSuperSampleType::T_1_5:
		return 1.5F;
	case ZombieStateType::TSuperSampleType::T_2_0:
		return 2.0F;
//#ifdef DSC_DEBUG
//	case ZombieStateType::TSuperSampleType::T_4_0:
//		return 4.0F;
//#endif
	}
	return 1.0F;
}

/*static*/ const ZombieStateType::TPlayer::TEnum ZombieStateType::GetPlayerSelf(const TGameDifficulty::TEnum in_difficulty)
{
	switch (in_difficulty)
	{
	default:
		break;
	case TGameDifficulty::TEasy:
		return TPlayer::TChico;
	case TGameDifficulty::TNormal:
		return TPlayer::TTanya;
	case TGameDifficulty::THard:
		return TPlayer::TSir;
	}
	return TPlayer::TCount;
}

/*static*/ const ZombieStateType::TPlayer::TEnum ZombieStateType::GetPlayerRescue(const TGameDifficulty::TEnum in_difficulty)
{
	switch (in_difficulty)
	{
	default:
		break;
	case TGameDifficulty::TEasy:
		return TPlayer::TTanya;
	case TGameDifficulty::TNormal:
		return TPlayer::TSir;
	case TGameDifficulty::THard:
		return TPlayer::TChico;
	}
	return TPlayer::TCount;
}

/*static*/ const ZombieStateType::TPlayer::TEnum ZombieStateType::GetPlayerOther(const TGameDifficulty::TEnum in_difficulty){
	switch (in_difficulty)
	{
	default:
		break;
	case TGameDifficulty::TEasy:
		return TPlayer::TSir;
	case TGameDifficulty::TNormal:
		return TPlayer::TChico;
	case TGameDifficulty::THard:
		return TPlayer::TTanya;
	}
	return TPlayer::TCount;
}

/*static*/ const char* const ZombieStateType::GetAchieviementString(const TAchieviement::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TAchieviement::TFirstBlood:
		return "TAchieviementFirstBlood";
	case TAchieviement::TIdBeBack:         
		return "TAchieviementIdBeBack";
	case TAchieviement::TSlugQueen:        
		return "TAchieviementSlugQueen";
	case TAchieviement::TBulletProof:      
		return "TAchieviementBulletProof";
	case TAchieviement::TNewb:             
		return "TAchieviementNewb";
	case TAchieviement::TAccuracy:       
		return "TAchieviementAccuracy";
	case TAchieviement::TEfficiency:       
		return "TAchieviementEfficiency";
	case TAchieviement::THeadHunter:       
		return "TAchieviementHeadHunter";
	case TAchieviement::TGoodWork:         
		return "TAchieviementGoodWork";
	case TAchieviement::TPercistance:      
		return "TAchieviementPercistance";
	case TAchieviement::TEradicator: 
		return "TAchieviementEradicator";
	case TAchieviement::TRock:
		return "TAchieviementRock";
	case TAchieviement::TMissionPossible:
		return "TAchieviementMissionPossible";
	}
	return "";
}

/*static*/ const char* const ZombieStateType::GetBonusString(const TBonus::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TBonus::TAddAmmo:
		return "TBonusAddAmmo";
	case TBonus::TAddHealth:
		return "TBonusAddHealth";
	case TBonus::THeal:
		return "TBonusHeal";
	case TBonus::TAddShotPower:
		return "TBonusAddShotPower";
	case TBonus::TFinishedGame:
		return "TBonusFinishedGame";
	//case TBonus::TGameOver:
	//	return "TBonusGameOver";
	}
	return "";
}
