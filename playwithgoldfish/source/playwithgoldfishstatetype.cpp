//
//  PlayWithGoldfishStateType.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateType.h"

//public static methods
/*static*/ const std::string PlayWithGoldfishStateType::GetMovementTypeString(const TMovementType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TMovementType::TNone:
		return "TMovementNone";
	case TMovementType::TAI:
		return "TMovementAI";
	case TMovementType::TInput:
		return "TMovementInput";		
	}
	
	return "";
}

/*static*/ const std::string PlayWithGoldfishStateType::GetAnimationTypeString(const TAnimationType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TAnimationType::TNone:
		return "TAnimationNone";
	case TAnimationType::TAI:
		return "TAnimationAI";
	case TAnimationType::TAnimationFeed:
		return "TAnimationFeed";
	case TAnimationType::TAnimationIdle:
		return "TAnimationIdle";
	case TAnimationType::TAnimationIdleAgitated:
		return "TAnimationIdleAgitated";
	case TAnimationType::TAnimationIdleSleep:
		return "TAnimationIdleSleep";
	case TAnimationType::TAnimationMove:
		return "TAnimationMove";
	case TAnimationType::TAnimationTurnLeft:
		return "TAnimationTurnLeft";
	case TAnimationType::TAnimationTurnLeftHard:
		return "TAnimationTurnLeftHard";
	case TAnimationType::TAnimationTurnRight:
		return "TAnimationTurnRight";
	case TAnimationType::TAnimationTurnRightHard:
		return "TAnimationTurnRightHard";		
	}
	
	return "";
}

/*static*/ const std::string PlayWithGoldfishStateType::GetViewBackgroundTypeString(const TViewBackgroundType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TViewBackgroundType::TCamera:
		return "TViewBackCamera";
	case TViewBackgroundType::TBlack:
		return "TViewBackBlack";
	case TViewBackgroundType::TWhite:
		return "TViewBackWhite";		
	case TViewBackgroundType::TGray:
		return "TViewBackGray";		
	case TViewBackgroundType::TLightBlue:
		return "TViewBackLightBlue";		
	case TViewBackgroundType::TDarkBlue:
		return "TViewBackDarkBlue";		
	}
	return "";
}

/*static*/ const std::string PlayWithGoldfishStateType::GetViewCameraTypeString(const TViewCameraType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case TViewCameraType::TAccelerometer:
		return "TViewCameraAccelerometer";
	case TViewCameraType::TTop:
		return "TViewCameraTop";
	case TViewCameraType::TThreeQuater:
		return "TViewCameraThreeQuater";
	case TViewCameraType::THalf:
		return "TViewCameraHalf";
	case TViewCameraType::TQuater:
		return "TViewCameraQuater";
	case TViewCameraType::TFront:
		return "TViewCameraFront";
	case TViewCameraType::TUnder:
		return "TViewCameraUnder";
	}
	return "";
}

/*static*/ const std::string PlayWithGoldfishStateType::GetSuperSampleTypeString(const TSuperSampleType::TEnum in_enum)
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
#ifdef DSC_DEBUG
	case TSuperSampleType::T_4_0:
		return "TSuperSample40";		
#endif
	}
	return "";
}

/*static*/ const float PlayWithGoldfishStateType::GetSuperSampleScale(const TSuperSampleType::TEnum in_enum)
{
	switch (in_enum)
	{
	default:
		break;
	case PlayWithGoldfishStateType::TSuperSampleType::T_1_0:
		return 1.0F;
	case PlayWithGoldfishStateType::TSuperSampleType::T_1_5:
		return 1.5F;
	case PlayWithGoldfishStateType::TSuperSampleType::T_2_0:
		return 2.0F;
#ifdef DSC_DEBUG
	case PlayWithGoldfishStateType::TSuperSampleType::T_4_0:
		return 4.0F;
#endif
	}
	return 1.0F;
}
