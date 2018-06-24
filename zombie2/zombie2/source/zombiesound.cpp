//
//  ZombieSound.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 11
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieSound.h"
#include "ZombieMission.h"

#include <GCommon.h>
#include <GSoundSource.h>
#include <GSoundLoad.h>
#include <GSoundManager.h>
#include <GSoundManagerLoad.h>

struct TSoundId
{
	enum TEnum
	{
		TChicoReload = 0,
		TChicoHurt,
		TChicoDead,
		TTanyaReload,
		TTanyaHurt,
		TTanyaDead,
		TSirReload,
		TSirHurt,
		TSirDead,
		TGunshot,
		TReload,
		THiss0,
		THiss1,
		THiss2,
		THiss3,
		THiss4,
		TSkeletonDeath0,
		TSlugDeath0,
		TSlugDeath1,
		TZombieFatIdle0,
		TZombieFatIdle1,
		TZombieOneIdle0,
		TZombieOneIdle1,

		TCount
	};
};

#ifdef WIN32
#define SOUND_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define SOUND_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const char* const sSoundResourceName[TSoundId::TCount] = {
	SOUND_NAME_MACRO("/chico_reload.caf"		,	"win32/chico_reload.wav"		), //TChicoReload = 0,
	SOUND_NAME_MACRO("/chico_hurt.caf"			,	"win32/chico_hurt.wav"			), //TChicoHurt,
	SOUND_NAME_MACRO("/chico_death.caf"			,	"win32/chico_death.wav"			), //TChicoDead,
	SOUND_NAME_MACRO("/tanya_reload.caf"		,	"win32/tanya_reload.wav"		), //TTanyaReload,
	SOUND_NAME_MACRO("/tanya_hurt.caf"			,	"win32/tanya_hurt.wav"			), //TTanyaHurt,
	SOUND_NAME_MACRO("/tanya_death.caf"			,	"win32/tanya_death.wav"			), //TTanyaDead,
	SOUND_NAME_MACRO("/sir_reload.caf"			,	"win32/sir_reload.wav"			), //TSirReload,
	SOUND_NAME_MACRO("/sir_hurt.caf"			,	"win32/sir_hurt.wav"			), //TSirHurt,
	SOUND_NAME_MACRO("/sir_death.caf"			,	"win32/sir_death.wav"			), //TSirDead,
	SOUND_NAME_MACRO("/gunshot.caf"				,	"win32/gunshot.wav"				), //TGunshot0,
	SOUND_NAME_MACRO("/reload.caf"				,	"win32/reload.wav"				), //TReload,
	SOUND_NAME_MACRO("/hiss0.caf"				,	"win32/hiss0.wav"				), //THiss0,
	SOUND_NAME_MACRO("/hiss1.caf"				,	"win32/hiss1.wav"				), //THiss1,
	SOUND_NAME_MACRO("/hiss2.caf"				,	"win32/hiss2.wav"				), //THiss2,
	SOUND_NAME_MACRO("/hiss3.caf"				,	"win32/hiss3.wav"				), //THiss3,
	SOUND_NAME_MACRO("/hiss4.caf"				,	"win32/hiss4.wav"				), //THiss4,
	SOUND_NAME_MACRO("/skeleton_death0.caf"		,	"win32/skeleton_death0.wav"		), //TSkeletonDeath0,
	SOUND_NAME_MACRO("/slug_death0.caf"			,	"win32/slug_death0.wav"			), //TSlugDeath0,
	SOUND_NAME_MACRO("/slug_death1.caf"			,	"win32/slug_death1.wav"			), //TSlugDeath1,
	SOUND_NAME_MACRO("/zombiefat_idle0.caf"		,	"win32/zombiefat_idle0.wav"		), //TZombieFatIdle0,
	SOUND_NAME_MACRO("/zombiefat_idle1.caf"		,	"win32/zombiefat_idle1.wav"		), //TZombieFatIdle1,
	SOUND_NAME_MACRO("/zombieone_idle0.caf"		,	"win32/zombieone_idle0.wav"		), //TZombieOneIdle0,
	SOUND_NAME_MACRO("/zombieone_idle1.caf"		,	"win32/zombieone_idle1.wav"		), //TZombieOneIdle1,
};

/**/
static const GSoundLoad sArraySoundLoad[TSoundId::TCount] = 
{
	{	sSoundResourceName[0]	},
	{	sSoundResourceName[1]	},
	{	sSoundResourceName[2]	},
	{	sSoundResourceName[3]	},
	{	sSoundResourceName[4]	},
	{	sSoundResourceName[5]	},
	{	sSoundResourceName[6]	},
	{	sSoundResourceName[7]	},
	{	sSoundResourceName[8]	},
	{	sSoundResourceName[9]	},
	{	sSoundResourceName[10]	},
	{	sSoundResourceName[11]	},
	{	sSoundResourceName[12]	},
	{	sSoundResourceName[13]	},
	{	sSoundResourceName[14]	},
	{	sSoundResourceName[15]	},
	{	sSoundResourceName[16]	},
	{	sSoundResourceName[17]	},
	{	sSoundResourceName[18]	},
	{	sSoundResourceName[19]	},
	{	sSoundResourceName[20]	},
	{	sSoundResourceName[21]	},
	{	sSoundResourceName[22]	},
};
static const int sArraySoundLoadCount = GCOMMON_ARRAY_SIZE(sArraySoundLoad);
/**/

//static public methods
/*static*/ const int ZombieSound::GetSoundIDPlayerReload(const ZombieStateType::TPlayer::TEnum in_player)
{
	switch (in_player)
	{
	default:
		break;
	case ZombieStateType::TPlayer::TChico:
		return GetSoundIDChicoReload();
	case ZombieStateType::TPlayer::TTanya:
		return GetSoundIDTanyaReload();
	case ZombieStateType::TPlayer::TSir:
		return GetSoundIDSirReload();
	}
	return TSoundId::TCount;
}

/*static*/ const int ZombieSound::GetSoundIDPlayerHurt(const ZombieStateType::TPlayer::TEnum in_player)
{
	switch (in_player)
	{
	default:
		break;
	case ZombieStateType::TPlayer::TChico:
		return GetSoundIDChicoHurt();
	case ZombieStateType::TPlayer::TTanya:
		return GetSoundIDTanyaHurt();
	case ZombieStateType::TPlayer::TSir:
		return GetSoundIDSirHurt();
	}
	return TSoundId::TCount;
}

/*static*/ const int ZombieSound::GetSoundIDPlayerDead(const ZombieStateType::TPlayer::TEnum in_player)
{
	switch (in_player)
	{
	default:
		break;
	case ZombieStateType::TPlayer::TChico:
		return GetSoundIDChicoDead();
	case ZombieStateType::TPlayer::TTanya:
		return GetSoundIDTanyaDead();
	case ZombieStateType::TPlayer::TSir:
		return GetSoundIDSirDead();
	}
	return TSoundId::TCount;
}

/*static*/ const int ZombieSound::GetSoundIDChicoReload()
{
	return TSoundId::TChicoReload;
}

/*static*/ const int ZombieSound::GetSoundIDChicoHurt()
{
	return TSoundId::TChicoHurt;
}

/*static*/ const int ZombieSound::GetSoundIDChicoDead()
{
	return TSoundId::TChicoDead;
}

/*static*/ const int ZombieSound::GetSoundIDTanyaReload()
{
	return TSoundId::TTanyaReload;
}

/*static*/ const int ZombieSound::GetSoundIDTanyaHurt()
{
	return TSoundId::TTanyaHurt;
}

/*static*/ const int ZombieSound::GetSoundIDTanyaDead()
{
	return TSoundId::TTanyaDead;
}

/*static*/ const int ZombieSound::GetSoundIDSirReload()
{
	return TSoundId::TSirReload;
}

/*static*/ const int ZombieSound::GetSoundIDSirHurt()
{
	return TSoundId::TSirHurt;
}

/*static*/ const int ZombieSound::GetSoundIDSirDead()
{
	return TSoundId::TSirDead;
}

/*static*/ const int ZombieSound::GetSoundIDGunshot()
{
	return TSoundId::TGunshot;
}

/*static*/ const int ZombieSound::GetSoundIDReload()
{
	return TSoundId::TReload;
}

/*static*/ const int ZombieSound::GetSoundIDHiss0() //long
{
	return TSoundId::THiss0;
}

/*static*/ const int ZombieSound::GetSoundIDHiss1()
{
	return TSoundId::THiss1;
}

/*static*/ const int ZombieSound::GetSoundIDHiss2() //girl?
{
	return TSoundId::THiss2;
}

/*static*/ const int ZombieSound::GetSoundIDHiss3()
{
	return TSoundId::THiss3;
}

/*static*/ const int ZombieSound::GetSoundIDHiss4() //blu, damage?
{
	return TSoundId::THiss4;
}

/*static*/ const int ZombieSound::GetSoundIDSkeletonDeath0()
{
	return TSoundId::TSkeletonDeath0;
}

/*static*/ const int ZombieSound::GetSoundIDSlugDeath0()
{
	return TSoundId::TSlugDeath0;
}

/*static*/ const int ZombieSound::GetSoundIDSlugDeath1()
{
	return TSoundId::TSlugDeath1;
}

/*static*/ const int ZombieSound::GetSoundIDZombieFatIdle0()
{
	return TSoundId::TZombieFatIdle0;
}

/*static*/ const int ZombieSound::GetSoundIDZombieFatIdle1()
{
	return TSoundId::TZombieFatIdle1;
}

/*static*/ const int ZombieSound::GetSoundIDZombieOneIdle0()
{
	return TSoundId::TZombieOneIdle0;
}

/*static*/ const int ZombieSound::GetSoundIDZombieOneIdle1()
{
	return TSoundId::TZombieOneIdle1;
}

//constructor
ZombieSound::ZombieSound(ZombieMission& inout_parentSate)
: mParentSate(inout_parentSate)
, mSoundManager()
{
	const GSoundManagerLoad soundLoad(sArraySoundLoadCount, sArraySoundLoad);
	mSoundManager.reset(new GSoundManager(soundLoad));
	return;
}

ZombieSound::~ZombieSound()
{
	return;
}

//public methods
void ZombieSound::Play(const int in_id, const GVector3Float& in_position, const float in_soundVolume)
{
	const float volume = in_soundVolume * mParentSate.GetVolume();
	if (volume <= 0.0F)
	{
		return;
	}

	TPointerSound pointerSound = GetSound(in_id).lock();
	if (pointerSound)
	{
		pointerSound->SetVolume(in_soundVolume);
		pointerSound->Play3D(in_position);
	}
	return;
}

ZombieSound::TWeakSound ZombieSound::GetSound(const int in_id)
{
	return mSoundManager->GetSoundSource(in_id);
}

void ZombieSound::PlaySoundGunShot(const float in_soundVolume)
{
	const float volume = in_soundVolume * mParentSate.GetVolume();
	if (volume <= 0.0F)
	{
		return;
	}
	//for (int index = 0; index < 4; ++index)
	//{
		//TPointerSound pointerSound = mSoundManager->GetSoundSource(TSoundId::TGunshot0 + index).lock();
		TPointerSound pointerSound = mSoundManager->GetSoundSource(TSoundId::TGunshot).lock();
		//if (!pointerSound || pointerSound->GetIsPlaying())
		//{
		//	continue;
		//}

		pointerSound->SetVolume(volume);
		pointerSound->Play3D(mPosition);
		//break;
	//}

	return;
}

void ZombieSound::SetMasterVolume(const float in_volume)
{
	mSoundManager->SetVolume(in_volume);
	return;
}
	
void ZombieSound::SetPosition(
	const GVector3Float& in_position,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_velocity
	)
{
	mSoundManager->SetListenerPosition(
		in_position,
		in_at,
		in_up,
		in_velocity
		);
	return;
}
