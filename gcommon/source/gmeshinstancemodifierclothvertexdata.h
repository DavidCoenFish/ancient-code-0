//
//  GMeshInstanceModifierClothVertexData.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifierClothVertexData_h_
#define _GMeshInstanceModifierClothVertexData_h_

#include "GVector3Float.h"

/*
	for now, do cloth in world space 
		(2 matrix mul per vertex, target to world, result to object space), 
	can latter change it to object space ?
		(one matrix mul per vertex, apply world space inverse delta to prev pos)
*/
class GMeshInstanceModifierClothVertexData
{
	//typedef
private:

	//constructor
public:
	GMeshInstanceModifierClothVertexData(const GMeshInstanceModifierClothVertexData& in_src);
	GMeshInstanceModifierClothVertexData(const GVector3Float& in_startPos = GVector3Float::sZero);
	~GMeshInstanceModifierClothVertexData();

	const GMeshInstanceModifierClothVertexData& operator=(const GMeshInstanceModifierClothVertexData& in_rhs);

	//public methods
public:
	void AddAcceleration(const GVector3Float& in_acceleration);
	void MinusAcceleration(const GVector3Float& in_acceleration);
	void IntergrateAndClearAcceleration(const float in_timeDelta);

	//public accessors
public:
	void SetPosition(const GVector3Float& in_position){ mPosition = in_position; return; }
	const GVector3Float& GetPosition()const{ return mPosition; }

	void SetVelocity(const GVector3Float& in_velocity){ mVelocity = in_velocity; return; }
	const GVector3Float& GetVelocity()const{ return mVelocity; }

	//private members;
private:
	GVector3Float mPosition;
	GVector3Float mVelocity;

	GVector3Float mAccelerationSum;

};

#endif 