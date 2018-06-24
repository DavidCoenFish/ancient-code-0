//
//  PlayWithGoldfishStateType.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateType_h_
#define _PlayWithGoldfishStateType_h_

#include <boost/noncopyable.hpp>
#include <string>

class PlayWithGoldfishStateType : public boost::noncopyable
{
	//typedef
public:
	//what drives the goldfish movement
	struct TMovementType
	{
		enum TEnum
		{
			TNone = 0,
			TAI,
			TInput,
		
			TCount
		};
	};

	//what drives the goldfish animation
	struct TAnimationType
	{
		enum TEnum
		{
			TNone = 0,
			TAI,
			TAnimationFeed,
			TAnimationIdle,
			TAnimationIdleAgitated,
			TAnimationIdleSleep,
			TAnimationMove,
			TAnimationTurnLeft,
			TAnimationTurnLeftHard,
			TAnimationTurnRight,
			TAnimationTurnRightHard,
		
			TCount
		};
	};

	struct TAiState
	{
		enum TEnum
		{
			TIdle = 0,
			TMove,
			TCount
		};
	};
	
	struct TViewBackgroundType
	{
		enum TEnum
		{
			TCamera = 0,
			TBlack,
			TWhite,
			TGray,
			TLightBlue,
			TDarkBlue,
			
			TCount
		};
	};
	
	struct TViewCameraType
	{
		enum TEnum
		{
			TAccelerometer = 0,
			TTop,
			TThreeQuater,
			THalf,
			TQuater,
			TFront,
			TUnder,
			
			TCount
		};
	};

	struct TSuperSampleType
	{
		enum TEnum
		{
			T_1_0 = 0,
			T_1_5,
			T_2_0,
#ifdef DSC_DEBUG
			T_4_0,
#endif			
			TCount
		};
	};

	//disabled
private:
	~PlayWithGoldfishStateType();

	//public static methods
public:
	static const std::string GetMovementTypeString(const TMovementType::TEnum in_enum);
	static const std::string GetAnimationTypeString(const TAnimationType::TEnum in_enum);
	static const std::string GetViewBackgroundTypeString(const TViewBackgroundType::TEnum in_enum);
	static const std::string GetViewCameraTypeString(const TViewCameraType::TEnum in_enum);
	static const std::string GetSuperSampleTypeString(const TSuperSampleType::TEnum in_enum);
	static const float GetSuperSampleScale(const TSuperSampleType::TEnum in_enum);

};

#endif //_PlayWithGoldfishStateType_h_