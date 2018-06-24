//
//  GMeshInstanceModifierClothVertexData.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifierClothVertexData.h"

//constructor
GMeshInstanceModifierClothVertexData::GMeshInstanceModifierClothVertexData(const GMeshInstanceModifierClothVertexData& in_src)
: mPosition()
, mVelocity()
, mAccelerationSum()
{
	(*this) = in_src;
	return;
}

GMeshInstanceModifierClothVertexData::GMeshInstanceModifierClothVertexData(const GVector3Float& in_startPos)
: mPosition(in_startPos)
, mVelocity()
, mAccelerationSum()
{
	return;
}

GMeshInstanceModifierClothVertexData::~GMeshInstanceModifierClothVertexData()
{
	return;
}

const GMeshInstanceModifierClothVertexData& GMeshInstanceModifierClothVertexData::operator=(const GMeshInstanceModifierClothVertexData& in_rhs)
{
	if (this != &in_rhs)
	{
		mPosition = in_rhs.mPosition;
		mVelocity = in_rhs.mVelocity;
		mAccelerationSum = in_rhs.mAccelerationSum;
	}
	return (*this);
}

//public methods
void GMeshInstanceModifierClothVertexData::AddAcceleration(const GVector3Float& in_acceleration)
{
	mAccelerationSum += in_acceleration;
	return;
}
void GMeshInstanceModifierClothVertexData::MinusAcceleration(const GVector3Float& in_acceleration)
{
	mAccelerationSum -= in_acceleration;
	return;
}
void GMeshInstanceModifierClothVertexData::IntergrateAndClearAcceleration(const float in_timeDelta)
{
	mVelocity += (mAccelerationSum * in_timeDelta);
	mPosition += mVelocity * in_timeDelta;
	mAccelerationSum = GVector3Float::sZero;
	return;
}

