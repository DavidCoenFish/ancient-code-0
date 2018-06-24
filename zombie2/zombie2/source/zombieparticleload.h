//
//  ZombieParticleLoad.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieParticleLoad_h_
#define _ZombieParticleLoad_h_

#include <boost/noncopyable.hpp>

struct ZombieParticleLoad// : public boost::noncopyable
{
	//constructor
public:
	//ZombieParticleLoad(
	//	const char* const in_particleData,
	//	const int in_particleEmittor,
	//	const int in_arrayPartAliveCount,
	//	const int* const in_arrayPartAlive,
	//	const int in_arrayPartDeadCount,
	//	const int* const in_arrayPartDead
	//	);
	//~ZombieParticleLoad();

	//public accessors
public:
	const char* const GetParticleData()const{ return mParticleData; }
	const int GetParticleEmittor()const{ return mParticleEmittor; }

	const int GetArrayPartAliveCount()const{ return mArrayPartAliveCount; }
	const int* const GetArrayPartAlive()const{ return mArrayPartAlive; }

	const int GetArrayPartDeadCount()const{ return mArrayPartDeadCount; }
	const int* const GetArrayPartDead()const{ return mArrayPartDead; }

	//private members
public:
	const char* const mParticleData;
	const int mParticleEmittor;

	const int mArrayPartAliveCount;
	const int* const mArrayPartAlive; // the part index we need to have for this particle system to play

	const int mArrayPartDeadCount;
	const int* const mArrayPartDead; //the part index that we need dead to play

};

#endif //_ZombieParticleLoad_h_
