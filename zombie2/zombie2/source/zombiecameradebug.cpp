//
//  ZombieCameraDebug.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieCameraDebug.h"

#include <GMath.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GVector2Float.h>
#include <GVector3Float.h>
#include <GMatrix9Float.h>

#include <GComponentLerpVector3Float.h>

//constructor
ZombieCameraDebug::ZombieCameraDebug()
: mPosition()
, mAt(-GVector3Float::sUnitZ)
, mUp(GVector3Float::sUnitY)
{
	return;
}

ZombieCameraDebug::~ZombieCameraDebug()
{
	return;
}

//public methods
void ZombieCameraDebug::Input(
	const GInput& in_input, 
	const GVector3Float& in_joystickDirection
	)
{
	const int touchCount = in_input.GetTouchCount();
	const GVector3Float cross = CrossProduct(mAt, mUp);


	if (1 == touchCount)
	{
		const GInputTouch& touch = in_input.GetTouch(0);

		const GMatrix9Float rotateOne = GMatrix9FloatConstructAxisAngle(mUp, touch.GetTouchDelta().m_x);
		const GMatrix9Float rotateTwo = GMatrix9FloatConstructAxisAngle(cross, touch.GetTouchDelta().m_y);

		mAt = mAt * rotateOne;
		mAt = mAt * rotateTwo;
		mUp = mUp * rotateTwo;
	}
	else
	{
		GVector2Float deltaSum;
		GVector2Float firstTouchLocation;
		GVector2Float firstTouchLocationPrev;
		float zoomSum = 0.0F;
		float rollSum = 0.0F;
		for (int index = 0; index < touchCount; ++index)
		{
			const GInputTouch& touch = in_input.GetTouch(index);
			deltaSum += touch.GetTouchDelta();

			if (0 == index)
			{
				firstTouchLocation = touch.GetTouchLocation();
				firstTouchLocationPrev = firstTouchLocation - touch.GetTouchDelta();
			}
			else
			{
				const GVector2Float offset = touch.GetTouchLocation() - firstTouchLocation; 
				const GVector2Float oldOffset = touch.GetTouchLocation() - firstTouchLocationPrev - touch.GetTouchDelta();

				zoomSum += (Length(offset) - Length(oldOffset));

				rollSum += ((GMath::ATan(offset.m_x, offset.m_y) - GMath::ATan(oldOffset.m_x, oldOffset.m_y)) * 0.5F);
			}
		}

		mPosition += (mAt * (zoomSum * 100.0F));
		mPosition -= (cross * (deltaSum.m_x * 10.0F));
		mPosition += (mUp * (deltaSum.m_y * 10.0F));

		//if (rollSum)
		//{
		//	const GMatrix9Float rotateRoll = GMatrix9FloatConstructAxisAngle(mAt, rollSum);
		//	mUp = mUp * rotateRoll;
		//}

	}

	return;
}

//public accessors
void ZombieCameraDebug::SetPosition(
	const GVector3Float& in_position
	)
{
	mPosition = in_position;
	return;
}

void ZombieCameraDebug::SetRotation(
	const GVector3Float& in_at, 
	const GVector3Float& in_up
	)
{
	mAt = in_at;
	mUp = in_up;
	return;
}

void ZombieCameraDebug::GetData(
	GVector3Float& out_position,
	GVector3Float& out_at,
	GVector3Float& out_up
	)
{
	out_position = mPosition;
	out_at = mAt;
	out_up = mUp;
	return;
}