//
//  ZombieSound.h
//  Zombie
//
//  Created by David Coen on 2011 03 11
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieSound_h_
#define _ZombieSound_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "ZombieStateType.h"
#include "GVector3Float.h"

class GSoundSource;
class GSoundManager;
class ZombieMission;

class ZombieSound : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::scoped_ptr<GSoundManager> TPointerSoundManager;

	typedef boost::shared_ptr<GSoundSource> TPointerSound;
	typedef boost::weak_ptr<GSoundSource> TWeakSound;

	//static public methods
public:
	static const int GetSoundIDPlayerReload(const ZombieStateType::TPlayer::TEnum in_player);
	static const int GetSoundIDPlayerHurt(const ZombieStateType::TPlayer::TEnum in_player);
	static const int GetSoundIDPlayerDead(const ZombieStateType::TPlayer::TEnum in_player);

	static const int GetSoundIDChicoReload();
	static const int GetSoundIDChicoHurt();
	static const int GetSoundIDChicoDead();

	static const int GetSoundIDTanyaReload();
	static const int GetSoundIDTanyaHurt();
	static const int GetSoundIDTanyaDead();

	static const int GetSoundIDSirReload();
	static const int GetSoundIDSirHurt();
	static const int GetSoundIDSirDead();

	static const int GetSoundIDGunshot();

	static const int GetSoundIDReload();

	static const int GetSoundIDHiss0(); //long
	static const int GetSoundIDHiss1();
	static const int GetSoundIDHiss2(); //girl?
	static const int GetSoundIDHiss3();
	static const int GetSoundIDHiss4(); //blu, damage?

	static const int GetSoundIDSkeletonDeath0();
	static const int GetSoundIDSlugDeath0();
	static const int GetSoundIDSlugDeath1();
	static const int GetSoundIDZombieFatIdle0();
	static const int GetSoundIDZombieFatIdle1();
	static const int GetSoundIDZombieOneIdle0();
	static const int GetSoundIDZombieOneIdle1();

	//constructor
public:
	ZombieSound(ZombieMission& inout_parentSate);
	~ZombieSound();

	//public methods
public:
	void Play(const int in_id, const GVector3Float& in_position, const float in_soundVolume = 1.0F);
	TWeakSound GetSound(const int in_id);

	void PlaySoundGunShot(const float in_soundVolume = 1.0F);

	void SetMasterVolume(const float in_volume);
	//const float GetMasterVolume()const{ return mMasterVolume; }
	
	void SetPosition(
		const GVector3Float& in_position,
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const GVector3Float& in_velocity
		);
				
	const GVector3Float& GetPosition()const{ return mPosition; };

	//private members
private:
	ZombieMission& mParentSate; //get volume
	TPointerSoundManager mSoundManager;
	//float mMasterVolume;
	GVector3Float mPosition;
	
};

#endif //_ZombieSound_h_
