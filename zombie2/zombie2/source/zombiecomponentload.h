//
//  ZombieComponentLoad.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieComponentLoad_h_
#define _ZombieComponentLoad_h_

#include <boost/noncopyable.hpp>

struct ZombiePartLoad;
struct ZombieParticleLoad;

class ZombieComponentLoad: public boost::noncopyable
{
	//constructor
public:
	ZombieComponentLoad(
		const int in_arrayPartCount, 
		const ZombiePartLoad* const in_arrayPart,
		const int in_arrayParticleCount, 
		const ZombieParticleLoad* const in_arrayParticle,

		const int in_arrayCutInHalfIndexCountDead,
		const int* const in_arrayCutInHalfIndexDead,

		const int in_arrayCutInHalfIndexCountAlive,
		const int* const in_arrayCutInHalfIndexAlive,

		const int in_arrayCutInHalfIndexPartCount,
		const int* const in_arrayCutInHalfIndexPart
		);
	~ZombieComponentLoad();

	//public accessors
public:
	const int GetArrayPartCount()const{ return mArrayPartCount; }
	const ZombiePartLoad* const GetArrayPart()const{ return mArrayPart; }

	const int GetArrayParticleCount()const{ return mArrayParticleCount; }
	const ZombieParticleLoad* const GetArrayParticle()const{ return mArrayParticle; }

	const int GetArrayCutInHalfIndexDeadCount()const{ return mArrayCutInHalfIndexDeadCount; }
	const int* const GetArrayCutInHalfIndexDead()const{ return mArrayCutInHalfIndexDead; }

	const int GetArrayCutInHalfIndexAliveCount()const{ return mArrayCutInHalfIndexAliveCount; }
	const int* const GetArrayCutInHalfIndexAlive()const{ return mArrayCutInHalfIndexAlive; }

	const int GetArrayCutInHalfIndexPartCount()const{ return mArrayCutInHalfIndexPartCount; }
	const int* const GetArrayCutInHalfIndexPart()const{ return mArrayCutInHalfIndexPart; }


	//private members
private:
	const int mArrayPartCount;
	const ZombiePartLoad* const mArrayPart;
	const int mArrayParticleCount;
	const ZombieParticleLoad* const mArrayParticle;

	const int mArrayCutInHalfIndexDeadCount;
	const int* const mArrayCutInHalfIndexDead;

	const int mArrayCutInHalfIndexAliveCount;
	const int* const mArrayCutInHalfIndexAlive;

	const int mArrayCutInHalfIndexPartCount;
	const int* const mArrayCutInHalfIndexPart;

};

#endif //_ZombieComponentLoad_h_
