//
//  ZombieReloadDetector.h
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieReloadDetector_h_
#define _ZombieReloadDetector_h_

#include <boost/noncopyable.hpp>
#include <GVector3Float.h>

class GInput;

class ZombieReloadDetector : public boost::noncopyable
{
	//constructor
public:
	ZombieReloadDetector(const GVector3Float& in_accelerometorSeed);
	~ZombieReloadDetector();

	//public methods
public:
	void Input(const GInput& in_input);
	void Tick(const float in_timeDelta);

	//public accessors
public:
	const bool GetTriggerReload(){ return mTriggerReload; }

	//private members
private:
	bool mTriggerReload;
	bool mValid; 
	float mCoolDown;

	GVector3Float mAccelerometorAverage;

};

#endif //_ZombieReloadDetector_h_
