//
//  ZombieComponentLoad.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieComponentLoad.h"

//constructor
ZombieComponentLoad::ZombieComponentLoad(
	const int in_arrayPartCount, 
	const ZombiePartLoad* const in_arrayPart,
	const int in_arrayParticleCount, 
	const ZombieParticleLoad* const in_arrayParticle,

	const int in_arrayCutInHalfIndexDeadCount,
	const int* const in_arrayCutInHalfIndexDead,

	const int in_arrayCutInHalfIndexAliveCount,
	const int* const in_arrayCutInHalfIndexAlive,

	const int in_arrayCutInHalfIndexPartCount,
	const int* const in_arrayCutInHalfIndexPart
	)
	: mArrayPartCount(in_arrayPartCount)
	, mArrayPart(in_arrayPart)
	, mArrayParticleCount(in_arrayParticleCount)
	, mArrayParticle(in_arrayParticle)

	, mArrayCutInHalfIndexDeadCount(in_arrayCutInHalfIndexDeadCount)
	, mArrayCutInHalfIndexDead(in_arrayCutInHalfIndexDead)

	, mArrayCutInHalfIndexAliveCount(in_arrayCutInHalfIndexAliveCount)
	, mArrayCutInHalfIndexAlive(in_arrayCutInHalfIndexAlive)

	, mArrayCutInHalfIndexPartCount(in_arrayCutInHalfIndexPartCount)
	, mArrayCutInHalfIndexPart(in_arrayCutInHalfIndexPart)
{
	return;
}

ZombieComponentLoad::~ZombieComponentLoad()
{
	return;
}
