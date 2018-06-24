//
//  BeachVolleyBallSound.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallSound.h"

#include <GCommon.h>
#include <GSoundContext.h>
#include <GSoundSource.h>
#include <GSoundLoad.h>
#include <GSoundManager.h>
#include <GSoundManagerLoad.h>
#include <GMath.h>

typedef std::vector<int> TArrayInt;

struct TContextType
{
	enum TEnum
	{
		TMenu = 0,
		TBikini0,
		TBikini1,
		TBikini2,
		TReiko0,
		TReiko1,
		TReiko2,
		TApril0,
		TApril1,
		TApril2,
		TCuda0,
		TCuda1,
		TCuda2,

		TCount
	};
};

struct TBufferId
{
	enum TEnum
	{
		TClick = 0,
		THit,
		THitHard,

		TG00Fall,
		TG00FallBack,
		TG00GiveUp,
		TG00HitHigh,	
		TG00HitJump,	
		TG00HitLow,	
		TG00HitMed,	
		TG00M00LooseGame,
		TG00M00LoosePoint,
		TG00M00LooseServe,
		TG00M00WinGame,
		TG00M00WinPoint,
		TG00M00WinServe,
		TG00M01LooseGame,
		TG00M01LoosePoint,
		TG00M01LooseServe,
		TG00M01WinGame,
		TG00M01WinPoint,
		TG00M01WinServe,
		TG00M02LooseGame,
		TG00M02LoosePoint,
		TG00M02LooseServe,
		TG00M02WinGame,
		TG00M02WinPoint,
		TG00M02WinServe,
		TG00Serve,	

		//TG01Fall,
		//TG01FallBack,
		//TG01GiveUp,
		//TG01HitHigh,	
		//TG01HitJump,	
		//TG01HitLow,	
		//TG01HitMed,	
		//TG01M00LooseGame,
		//TG01M00LoosePoint,
		//TG01M00LooseServe,
		//TG01M00WinGame,
		//TG01M00WinPoint,
		//TG01M00WinServe,
		//TG01M01LooseGame,
		//TG01M01LoosePoint,
		//TG01M01LooseServe,
		//TG01M01WinGame,
		//TG01M01WinPoint,
		//TG01M01WinServe,
		//TG01M02LooseGame,
		//TG01M02LoosePoint,
		//TG01M02LooseServe,
		//TG01M02WinGame,
		//TG01M02WinPoint,
		//TG01M02WinServe,
		//TG01Serve,	

		TG02Fall,
		TG02FallBack,
		TG02GiveUp,
		TG02HitHigh,	
		TG02HitJump,	
		TG02HitLow,	
		TG02HitMed,	
		TG02M00LooseGame,
		TG02M00LoosePoint,
		TG02M00LooseServe,
		TG02M00WinGame,
		TG02M00WinPoint,
		TG02M00WinServe,
		TG02M01LooseGame,
		TG02M01LoosePoint,
		TG02M01LooseServe,
		TG02M01WinGame,
		TG02M01WinPoint,
		TG02M01WinServe,
		TG02M02LooseGame,
		TG02M02LoosePoint,
		TG02M02LooseServe,
		TG02M02WinGame,
		TG02M02WinPoint,
		TG02M02WinServe,
		TG02Serve,	

		TG03Fall,
		TG03FallBack,
		TG03GiveUp,
		TG03HitHigh,	
		TG03HitJump,	
		TG03HitLow,	
		TG03HitMed,	
		TG03M00LooseGame,
		TG03M00LoosePoint,
		TG03M00LooseServe,
		TG03M00WinGame,
		TG03M00WinPoint,
		TG03M00WinServe,
		TG03M01LooseGame,
		TG03M01LoosePoint,
		TG03M01LooseServe,
		TG03M01WinGame,
		TG03M01WinPoint,
		TG03M01WinServe,
		TG03M02LooseGame,
		TG03M02LoosePoint,
		TG03M02LooseServe,
		TG03M02WinGame,
		TG03M02WinPoint,
		TG03M02WinServe,
		//TG03Serve,	

		TCount
	};
};

#ifdef WIN32
#define SOUND_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define SOUND_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const GSoundLoad sArraySoundLoad[TBufferId::TCount] = 
{
	{	SOUND_NAME_MACRO("/click.caf"				,	"win32/click.wav"		)	},
	{	SOUND_NAME_MACRO("/hit.caf"					,	"win32/hit.wav"			)	},
	{	SOUND_NAME_MACRO("/hithard.caf"				,	"win32/hithard.wav"			)	},

	{	SOUND_NAME_MACRO("/g00fall.caf"				,	"win32/g00fall.wav"			)	},
	{	SOUND_NAME_MACRO("/g00fallback.caf"			,	"win32/g00fallback.wav"			)	},
	{	SOUND_NAME_MACRO("/g00giveup.caf"			,	"win32/g00giveup.wav"			)	},
	{	SOUND_NAME_MACRO("/g00hithigh.caf"			,	"win32/g00hithigh.wav"			)	},
	{	SOUND_NAME_MACRO("/g00hitjump.caf"			,	"win32/g00hitjump.wav"			)	},
	{	SOUND_NAME_MACRO("/g00hitlow.caf"			,	"win32/g00hitlow.wav"			)	},
	{	SOUND_NAME_MACRO("/g00hitmed.caf"			,	"win32/g00hitmed.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00loosegame.caf"		,	"win32/g00m00loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00loosepoint.caf"	,	"win32/g00m00loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00looseserve.caf"	,	"win32/g00m00looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00wingame.caf"		,	"win32/g00m00wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00winpoint.caf"		,	"win32/g00m00winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m00winserve.caf"		,	"win32/g00m00winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01loosegame.caf"		,	"win32/g00m01loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01loosepoint.caf"	,	"win32/g00m01loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01looseserve.caf"	,	"win32/g00m01looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01wingame.caf"		,	"win32/g00m01wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01winpoint.caf"		,	"win32/g00m01winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m01winserve.caf"		,	"win32/g00m01winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02loosegame.caf"		,	"win32/g00m02loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02loosepoint.caf"	,	"win32/g00m02loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02looseserve.caf"	,	"win32/g00m02looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02wingame.caf"		,	"win32/g00m02wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02winpoint.caf"		,	"win32/g00m02winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g00m02winserve.caf"		,	"win32/g00m02winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g00serve.caf"			,	"win32/g00serve.wav"			)	},

	//{	SOUND_NAME_MACRO("/g01fall.caf"				,	"win32/g01fall.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01fallback.caf"			,	"win32/g01fallback.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01giveup.caf"			,	"win32/g01giveup.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01hithigh.caf"			,	"win32/g01hithigh.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01hitjump.caf"			,	"win32/g01hitjump.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01hitlow.caf"			,	"win32/g01hitlow.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01hitmed.caf"			,	"win32/g01hitmed.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00loosegame.caf"		,	"win32/g01m00loosegame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00loosepoint.caf"	,	"win32/g01m00loosepoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00looseserve.caf"	,	"win32/g01m00looseserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00wingame.caf"		,	"win32/g01m00wingame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00winpoint.caf"		,	"win32/g01m00winpoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m00winserve.caf"		,	"win32/g01m00winserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01loosegame.caf"		,	"win32/g01m01loosegame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01loosepoint.caf"	,	"win32/g01m01loosepoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01looseserve.caf"	,	"win32/g01m01looseserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01wingame.caf"		,	"win32/g01m01wingame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01winpoint.caf"		,	"win32/g01m01winpoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m01winserve.caf"		,	"win32/g01m01winserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02loosegame.caf"		,	"win32/g01m02loosegame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02loosepoint.caf"	,	"win32/g01m02loosepoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02looseserve.caf"	,	"win32/g01m02looseserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02wingame.caf"		,	"win32/g01m02wingame.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02winpoint.caf"		,	"win32/g01m02winpoint.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01m02winserve.caf"		,	"win32/g01m02winserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g01serve.caf"			,	"win32/g01serve.wav"			)	},

	{	SOUND_NAME_MACRO("/g02fall.caf"				,	"win32/g02fall.wav"			)	},
	{	SOUND_NAME_MACRO("/g02fallback.caf"			,	"win32/g02fallback.wav"			)	},
	{	SOUND_NAME_MACRO("/g02giveup.caf"			,	"win32/g02giveup.wav"			)	},
	{	SOUND_NAME_MACRO("/g02hithigh.caf"			,	"win32/g02hithigh.wav"			)	},
	{	SOUND_NAME_MACRO("/g02hitjump.caf"			,	"win32/g02hitjump.wav"			)	},
	{	SOUND_NAME_MACRO("/g02hitlow.caf"			,	"win32/g02hitlow.wav"			)	},
	{	SOUND_NAME_MACRO("/g02hitmed.caf"			,	"win32/g02hitmed.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00loosegame.caf"		,	"win32/g02m00loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00loosepoint.caf"	,	"win32/g02m00loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00looseserve.caf"	,	"win32/g02m00looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00wingame.caf"		,	"win32/g02m00wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00winpoint.caf"		,	"win32/g02m00winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m00winserve.caf"		,	"win32/g02m00winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01loosegame.caf"		,	"win32/g02m01loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01loosepoint.caf"	,	"win32/g02m01loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01looseserve.caf"	,	"win32/g02m01looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01wingame.caf"		,	"win32/g02m01wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01winpoint.caf"		,	"win32/g02m01winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m01winserve.caf"		,	"win32/g02m01winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02loosegame.caf"		,	"win32/g02m02loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02loosepoint.caf"	,	"win32/g02m02loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02looseserve.caf"	,	"win32/g02m02looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02wingame.caf"		,	"win32/g02m02wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02winpoint.caf"		,	"win32/g02m02winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g02m02winserve.caf"		,	"win32/g02m02winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g02serve.caf"			,	"win32/g02serve.wav"			)	},

	{	SOUND_NAME_MACRO("/g03fall.caf"				,	"win32/g03fall.wav"			)	},
	{	SOUND_NAME_MACRO("/g03fallback.caf"			,	"win32/g03fallback.wav"			)	},
	{	SOUND_NAME_MACRO("/g03giveup.caf"			,	"win32/g03giveup.wav"			)	},
	{	SOUND_NAME_MACRO("/g03hithigh.caf"			,	"win32/g03hithigh.wav"			)	},
	{	SOUND_NAME_MACRO("/g03hitjump.caf"			,	"win32/g03hitjump.wav"			)	},
	{	SOUND_NAME_MACRO("/g03hitlow.caf"			,	"win32/g03hitlow.wav"			)	},
	{	SOUND_NAME_MACRO("/g03hitmed.caf"			,	"win32/g03hitmed.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00loosegame.caf"		,	"win32/g03m00loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00loosepoint.caf"	,	"win32/g03m00loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00looseserve.caf"	,	"win32/g03m00looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00wingame.caf"		,	"win32/g03m00wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00winpoint.caf"		,	"win32/g03m00winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m00winserve.caf"		,	"win32/g03m00winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01loosegame.caf"		,	"win32/g03m01loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01loosepoint.caf"	,	"win32/g03m01loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01looseserve.caf"	,	"win32/g03m01looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01wingame.caf"		,	"win32/g03m01wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01winpoint.caf"		,	"win32/g03m01winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m01winserve.caf"		,	"win32/g03m01winserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02loosegame.caf"		,	"win32/g03m02loosegame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02loosepoint.caf"	,	"win32/g03m02loosepoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02looseserve.caf"	,	"win32/g03m02looseserve.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02wingame.caf"		,	"win32/g03m02wingame.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02winpoint.caf"		,	"win32/g03m02winpoint.wav"			)	},
	{	SOUND_NAME_MACRO("/g03m02winserve.caf"		,	"win32/g03m02winserve.wav"			)	},
	//{	SOUND_NAME_MACRO("/g03serve.caf"			,	"win32/g03serve.wav"			)	}


};
static const int sArraySoundLoadCount = GCOMMON_ARRAY_SIZE(sArraySoundLoad);

struct TSoundId
{
	enum TEnum
	{
		TClick = 0,
		THit,
		THitHard,

		//dynamic sounds
		TFall,		
		TFallBack,
		TGiveUp,
		THitHigh,
		THitJump,
		THitLow,
		THitMed,	
		TLooseGame,
		TLoosePoint0,
		TLoosePoint1,
		TLoosePoint2,
		TLooseServe0,
		TLooseServe1,
		TLooseServe2,
		TWinGame,
		TWinPoint0,
		TWinPoint1,
		TWinPoint2,
		TWinServe0,
		TWinServe1,
		TWinServe2,
		TServe,

		TCount
	};
};

//static public methods
/*static*/ const int BeachVolleyBallSound::GetContextMenu()
{
	return TContextType::TMenu;
}

/*static*/ const int BeachVolleyBallSound::GetContextMatch(
	const BeachVolleyBallType::TGirl::TEnum in_againstGirl,
	const BeachVolleyBallType::TMatch::TEnum in_match		
	)
{
	switch (in_againstGirl)
	{
	default:
		break;
	case BeachVolleyBallType::TGirl::TBikini:
		return TContextType::TBikini0 + in_match;
	case BeachVolleyBallType::TGirl::TReiko:
		return TContextType::TReiko0 + in_match;
	case BeachVolleyBallType::TGirl::TApril:
		return TContextType::TApril0 + in_match;
	case BeachVolleyBallType::TGirl::TCuda:
		return TContextType::TCuda0 + in_match;
	}
	return TContextType::TCount;
}

/*static*/ const int BeachVolleyBallSound::GetSoundIDClick()
{
	return TSoundId::TClick;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHit()
{
	return TSoundId::THit;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHitHard()
{
	return TSoundId::THitHard;
}

/*static*/ const int BeachVolleyBallSound::GetSoundIDFall()
{
	return TSoundId::TFall;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDFallBack()
{
	return TSoundId::TFallBack;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDGiveUp()
{
	return TSoundId::TGiveUp;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHitHigh()
{
	return TSoundId::THitHigh;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHitJump()
{
	return TSoundId::THitJump;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHitLow()
{
	return TSoundId::THitLow;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDHitMed()
{
	return TSoundId::THitMed;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDLooseGame()
{
	return TSoundId::TLooseGame;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDLoosePoint()
{
	if (GMath::RandomUnit() < 0.25F)
	{
		return TSoundId::TGiveUp;
	}
	return TSoundId::TLoosePoint0 + (GMath::RandomInt() % 3);
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDLooseServe()
{
	return TSoundId::TLooseServe0 + (GMath::RandomInt() % 3);
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDWinGame()
{
	return TSoundId::TWinGame;
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDWinPoint()
{
	return TSoundId::TWinPoint0 + (GMath::RandomInt() % 3);
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDWinServe()
{
	return TSoundId::TWinServe0 + (GMath::RandomInt() % 3);
}
/*static*/ const int BeachVolleyBallSound::GetSoundIDServe()
{
	return TSoundId::TServe;
}

//constructor
BeachVolleyBallSound::BeachVolleyBallSound(
	GFileSystem& in_fileSystem
	)
: mSoundManager()
, mGilrSound()
, mGirlSoundForce(false)
{
	const GSoundManagerLoad soundLoad(sArraySoundLoadCount, sArraySoundLoad);

	TArrayInt arraySoundBufferIndex;
	arraySoundBufferIndex.push_back(TBufferId::TClick);
	arraySoundBufferIndex.push_back(TBufferId::THit);
	arraySoundBufferIndex.push_back(TBufferId::THitHard);

	mSoundManager.reset(new GSoundManager(in_fileSystem, soundLoad, arraySoundBufferIndex));

	return;
}

BeachVolleyBallSound::~BeachVolleyBallSound()
{
	return;
}

//public methods
void BeachVolleyBallSound::SetContext(const int in_contextType)
{
	switch (in_contextType)
	{
	default:
		break;
	case TContextType::TBikini0:
	case TContextType::TReiko0:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG00Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG00FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG00GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TBikini1:
	case TContextType::TReiko1:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG00Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG00FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG00GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TBikini2:
	case TContextType::TReiko2:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG00Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG00FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG00GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG00HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG00M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG00M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG00M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG00Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	//case TContextType::TReiko0:
	//	if (mSoundManager)
	//	{
	//		TArrayInt arraySoundBufferIndex;
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Fall);			//TFall,		
	//		arraySoundBufferIndex.push_back(TBufferId::TG01FallBack);			//TFallBack,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01GiveUp);			//TGiveUp,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitHigh);			//THitHigh,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitJump);			//THitJump,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitLow);			//THitLow,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitMed);			//THitMed,	
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LooseGame);			//TLooseGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LoosePoint);			//TLoosePoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LoosePoint);			//TLoosePoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LoosePoint);			//TLoosePoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LooseServe);			//TLooseServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LooseServe);			//TLooseServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LooseServe);			//TLooseServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinGame);			//TWinGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinPoint);			//TWinPoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinPoint);			//TWinPoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinPoint);			//TWinPoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinServe);			//TWinServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinServe);			//TWinServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinServe);			//TWinServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Serve);			//TServe,
	//		mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
	//	}
	//	break;
	//case TContextType::TReiko1:
	//	if (mSoundManager)
	//	{
	//		TArrayInt arraySoundBufferIndex;
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Fall);			//TFall,		
	//		arraySoundBufferIndex.push_back(TBufferId::TG01FallBack);			//TFallBack,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01GiveUp);			//TGiveUp,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitHigh);			//THitHigh,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitJump);			//THitJump,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitLow);			//THitLow,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitMed);			//THitMed,	
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LooseGame);			//TLooseGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LoosePoint);			//TLoosePoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LoosePoint);			//TLoosePoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LoosePoint);			//TLoosePoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LooseServe);			//TLooseServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LooseServe);			//TLooseServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LooseServe);			//TLooseServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinGame);			//TWinGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinPoint);			//TWinPoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinPoint);			//TWinPoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinPoint);			//TWinPoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinServe);			//TWinServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinServe);			//TWinServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinServe);			//TWinServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Serve);			//TServe,
	//		mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
	//	}
	//	break;
	//case TContextType::TReiko2:
	//	if (mSoundManager)
	//	{
	//		TArrayInt arraySoundBufferIndex;
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Fall);			//TFall,		
	//		arraySoundBufferIndex.push_back(TBufferId::TG01FallBack);			//TFallBack,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01GiveUp);			//TGiveUp,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitHigh);			//THitHigh,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitJump);			//THitJump,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitLow);			//THitLow,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01HitMed);			//THitMed,	
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LooseGame);			//TLooseGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LoosePoint);			//TLoosePoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LoosePoint);			//TLoosePoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LoosePoint);			//TLoosePoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00LooseServe);			//TLooseServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01LooseServe);			//TLooseServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02LooseServe);			//TLooseServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinGame);			//TWinGame,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinPoint);			//TWinPoint0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinPoint);			//TWinPoint1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinPoint);			//TWinPoint2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M00WinServe);			//TWinServe0,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M01WinServe);			//TWinServe1,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01M02WinServe);			//TWinServe2,
	//		arraySoundBufferIndex.push_back(TBufferId::TG01Serve);			//TServe,
	//		mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
	//	}
	//	break;
	case TContextType::TApril0:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG02Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG02FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG02GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TApril1:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG02Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG02FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG02GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TApril2:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG02Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG02FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG02GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG02HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG02M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG02M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG02M02WinServe);			//TWinServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG02Serve);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TCuda0:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG03Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG03FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG03GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinServe);			//TWinServe2,
			//arraySoundBufferIndex.push_back(TBufferId::TG03Serve);			//TServe,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TCuda1:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG03Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG03FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG03GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinServe);			//TWinServe2,
			//arraySoundBufferIndex.push_back(TBufferId::TG03Serve);			//TServe,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	case TContextType::TCuda2:
		if (mSoundManager)
		{
			TArrayInt arraySoundBufferIndex;
			arraySoundBufferIndex.push_back(TBufferId::TG03Fall);			//TFall,		
			arraySoundBufferIndex.push_back(TBufferId::TG03FallBack);			//TFallBack,
			arraySoundBufferIndex.push_back(TBufferId::TG03GiveUp);			//TGiveUp,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitHigh);			//THitHigh,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//THitJump,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitLow);			//THitLow,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitMed);			//THitMed,	
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LooseGame);			//TLooseGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LoosePoint);			//TLoosePoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LoosePoint);			//TLoosePoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LoosePoint);			//TLoosePoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00LooseServe);			//TLooseServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01LooseServe);			//TLooseServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02LooseServe);			//TLooseServe2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinGame);			//TWinGame,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinPoint);			//TWinPoint0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinPoint);			//TWinPoint1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinPoint);			//TWinPoint2,
			arraySoundBufferIndex.push_back(TBufferId::TG03M00WinServe);			//TWinServe0,
			arraySoundBufferIndex.push_back(TBufferId::TG03M01WinServe);			//TWinServe1,
			arraySoundBufferIndex.push_back(TBufferId::TG03M02WinServe);			//TWinServe2,
			//arraySoundBufferIndex.push_back(TBufferId::TG03Serve);			//TServe,
			arraySoundBufferIndex.push_back(TBufferId::TG03HitJump);			//TServe,
			mSoundManager->CreateSoundSourceDynamicGroup(arraySoundBufferIndex);
		}
		break;
	}

	return;
}

void BeachVolleyBallSound::PlayOmniSound(const int in_id, const float in_soundVolume)
{
	TWeakSoundSource weakSoundSoure = GetSound(in_id);
	TPointerSoundSource pointerSoundSource = weakSoundSoure.lock();
	if (pointerSoundSource)
	{
		GSoundSource& soundSource = *pointerSoundSource;
		soundSource.SetOmni(true);
		soundSource.SetVolume(in_soundVolume);
		soundSource.Play();
	}

	return;
}

void BeachVolleyBallSound::PlayPositionSound(const int in_id, const GVector3Float& in_position, const float in_soundVolume)
{
	TWeakSoundSource weakSoundSoure = GetSound(in_id);
	TPointerSoundSource pointerSoundSource = weakSoundSoure.lock();
	if (pointerSoundSource)
	{
		GSoundSource& soundSource = *pointerSoundSource;
		soundSource.SetOmni(false);
		soundSource.SetPosition(in_position);
		soundSource.SetVolume(in_soundVolume);
		soundSource.Play();
	}

	return;
}

void BeachVolleyBallSound::PlayGirlSound(const int in_id, const GVector3Float& in_position, const bool in_force)
{
	//turn off force if finished
	if (mGirlSoundForce)
	{
		TPointerSoundSource girlSoundSource = mGilrSound.lock();
		if (!girlSoundSource ||
			!girlSoundSource->GetIsPlaying())
		{
			mGirlSoundForce = false;
			mGilrSound.reset();
		}
	}

	//bail if force sound playing and we are not forced
	if (mGirlSoundForce &&
		!in_force)
	{
		return;
	}

	TWeakSoundSource weakSoundSource = mSoundManager->GetSoundSource(in_id);
	TPointerSoundSource pointerSoundSource = weakSoundSource.lock();
	if (!pointerSoundSource)
	{
		return;
	}
	
	//stop old sound
	{
		TPointerSoundSource girlSoundSource = mGilrSound.lock();
		if (girlSoundSource)
		{
			girlSoundSource->Stop();
		}
	}

	mGilrSound = weakSoundSource;
	mGirlSoundForce = in_force;

	GSoundSource& soundSource = *pointerSoundSource;
	soundSource.SetOmni(false);
	soundSource.SetPosition(in_position);
	soundSource.SetVolume(1.0F);
	soundSource.Play();


	return;
}

BeachVolleyBallSound::TWeakSoundSource BeachVolleyBallSound::GetSound(const int in_id)
{
	if (mSoundManager)
	{
		return mSoundManager->GetSoundSource(in_id);
	}

	return TWeakSoundSource();
}

void BeachVolleyBallSound::SetContextPosition(
	const GVector3Float& in_position,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_velocity
	)
{
	if (mSoundManager)
	{
		mSoundManager->SetListenerPosition(
			in_position,
			in_at,
			in_up,
			in_velocity
			);
	}
	return;
}