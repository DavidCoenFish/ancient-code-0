//
//  BeachVolleyBallSound.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallSound_h_
#define _BeachVolleyBallSound_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "BeachVolleyBallType.h"
#include "GVector3Float.h"

class GSoundContext;
class GSoundManager;
class GSoundSource;
class GFileSystem;

class BeachVolleyBallSound : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::scoped_ptr<GSoundManager> TPointerSoundManager;

	typedef boost::shared_ptr<GSoundContext> TPointerSoundContext;
	typedef boost::weak_ptr<GSoundContext> TWeakSoundContext;

	typedef boost::shared_ptr<GSoundSource> TPointerSoundSource;
	typedef boost::weak_ptr<GSoundSource> TWeakSoundSource;

	//static public methods
public:
	static const int GetContextMenu();
	static const int GetContextMatch(
		const BeachVolleyBallType::TGirl::TEnum in_againstGirl,
		const BeachVolleyBallType::TMatch::TEnum in_match		
		);

	static const int GetSoundIDClick();
	static const int GetSoundIDHit();
	static const int GetSoundIDHitHard();

	static const int GetSoundIDFall();		
	static const int GetSoundIDFallBack();
	static const int GetSoundIDGiveUp();
	static const int GetSoundIDHitHigh();
	static const int GetSoundIDHitJump();
	static const int GetSoundIDHitLow();
	static const int GetSoundIDHitMed();	
	static const int GetSoundIDLooseGame();
	static const int GetSoundIDLoosePoint();
	static const int GetSoundIDLooseServe();
	static const int GetSoundIDWinGame();
	static const int GetSoundIDWinPoint();
	static const int GetSoundIDWinServe();
	static const int GetSoundIDServe();

	//constructor
public:
	BeachVolleyBallSound(
		GFileSystem& in_fileSystem
		);
	~BeachVolleyBallSound();

	//public methods
public:
	void SetContext(const int in_contextType);

	void PlayOmniSound(const int in_id, const float in_soundVolume = 1.0F);
	void PlayPositionSound(const int in_id, const GVector3Float& in_position, const float in_soundVolume = 1.0F);
	//if force is true, replace current sound with forced, and wait for finish. replace forced with new forced
	void PlayGirlSound(const int in_id, const GVector3Float& in_position, const bool in_force);
	TWeakSoundSource GetSound(const int in_id);

	void SetContextPosition(
		const GVector3Float& in_position,
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const GVector3Float& in_velocity
		);

	//private members
private:
	TPointerSoundManager mSoundManager;

	//girl only has one voice
	TWeakSoundSource mGilrSound;
	bool mGirlSoundForce;

};

#endif //_BeachVolleyBallSound_h_
