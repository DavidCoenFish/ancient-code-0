//
//  GAnimationInstance.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationInstance.h"
#include "GAnimationManagerLoad.h"
#include "GAnimationStreamInfoLoad.h"
#include "GAnimationStreamDataLoad.h"
#include "GAnimationLoad.h"
#include "GAnimationType.h"
#include "GAnimationKeyLoad.h"
#include "GAssert.h"
#include "GMath.h"

static void LocalCalculateKeyHint(
	int& inout_keyIndex,
	const GAnimationStreamDataLoad& in_streamDataLoad,
	const float in_timePosition
	)
{
	const int arrayKeyCount = in_streamDataLoad.GetArrayKeyCount();
	if (arrayKeyCount <= 1)
	{
		return;
	}

	if (in_streamDataLoad.GetArrayKeyLoad()[inout_keyIndex].GetTime() <= in_timePosition)
	{
		//test advance
		while (inout_keyIndex < (arrayKeyCount - 2)) //we dont go past the last segment
		{
			if (in_timePosition < in_streamDataLoad.GetArrayKeyLoad()[inout_keyIndex + 1].GetTime())
			{
				break;
			}
			inout_keyIndex += 1;
		}
	}
	else
	{
		//test decrement
		while (0 < inout_keyIndex)
		{
			inout_keyIndex -= 1;
			if (in_streamDataLoad.GetArrayKeyLoad()[inout_keyIndex].GetTime() <= in_timePosition)
			{
				break;
			}
		}
	}

	return;
}

template<int IN_COUNT>
static void LocalReadValues(
	float* const out_arrayValue,
	const GAnimationStreamDataLoad& in_streamDataLoad,
	const int in_index
	)
{
	const float* const pSource = (const float* const)in_streamDataLoad.GetArrayKeyLoad()[in_index].GetData();
	for (int index = 0; index < IN_COUNT; ++index)
	{
		out_arrayValue[index] = pSource[index];
	}
	return;
}

template<int IN_COUNT>
static void LocalLerpValues(
	float* const inout_arrayValue,
	const int in_indexHint,
	const float in_timePosition,
	const GAnimationStreamDataLoad& in_streamDataLoad
	)
{
	GASSERT((0 <= in_indexHint) && (in_indexHint < ((int)in_streamDataLoad.GetArrayKeyCount() - 1)), "invalid param");

	const float ratio = GMath::GetRatio(
		in_timePosition,
		in_streamDataLoad.GetArrayKeyLoad()[in_indexHint].GetTime(),
		in_streamDataLoad.GetArrayKeyLoad()[in_indexHint + 1].GetTime()
		);

	if (ratio <= 0.0F)
	{
		LocalReadValues<IN_COUNT>(inout_arrayValue, in_streamDataLoad, in_indexHint);
		return;
	}
	else if (1.0F <= ratio)
	{
		LocalReadValues<IN_COUNT>(inout_arrayValue, in_streamDataLoad, in_indexHint + 1);
		return;
	}

	float valuesLhs[IN_COUNT];
	float valuesRhs[IN_COUNT];

	LocalReadValues<IN_COUNT>(valuesLhs, in_streamDataLoad, in_indexHint);
	LocalReadValues<IN_COUNT>(valuesRhs, in_streamDataLoad, in_indexHint + 1);

	for (int index = 0; index < IN_COUNT; ++index)
	{
		inout_arrayValue[index] = GMath::Lerp(ratio, valuesLhs[index], valuesRhs[index]);
	}

	return;
}

//constructor
GAnimationInstance::GAnimationInstance(
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_animationLoadIndex,
	const float in_timePosition
	)
: mAnimationLoadIndex(in_animationLoadIndex)
, mTimePosition(in_timePosition)
, mArrayStreamKeyIndexHint()
, m_loop(0 != (in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex].GetFlag() & GAnimationType::TAnimationFlag::TLoop))
{
	const int streamCount = in_animationManagerLoad.GetArrayStreamInfoCount();
	mArrayStreamKeyIndexHint.resize(streamCount);
	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];

	for (int index = 0; index < streamCount; ++index)
	{
		const GAnimationStreamDataLoad& streamDataLoad = animationLoad.GetArrayStreamDataLoad()[index];
		if (0 == streamDataLoad.GetArrayKeyCount())
		{
			mArrayStreamKeyIndexHint[index] = -1;
		}
	}

	return;
}

GAnimationInstance::~GAnimationInstance()
{
	return;
}

//public methods
//return false if non looping animation has past end or beginning
const bool GAnimationInstance::SimpleTick(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad)
{
	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];

	bool keepGoing = true;

	const bool loop = (0 != (GAnimationType::TAnimationFlag::TLoop & animationLoad.GetFlag()));
	float localTimePosition = mTimePosition + in_timeDelta;
	const float length = animationLoad.GetLength();
	if (length == 0.0F)
	{
		mTimePosition = 0.0F;
		if (!loop)
		{
			return false;
		}
		return true;
	}

	bool wrappedPastEnd = false;
	bool wrappedPastBegin = false;

	if (0.0F < in_timeDelta)
	{
		//increment
		while (length <= localTimePosition)
		{
			if (loop)
			{
				localTimePosition -= length;
				wrappedPastEnd = true;
			}
			else
			{
				mTimePosition = length;
				keepGoing = false;
				break;
			}
		}
	}
	else
	{
		while (localTimePosition < 0.0F)
		{
			if (loop)
			{
				localTimePosition += length;
				wrappedPastBegin = true;
			}
			else
			{
				mTimePosition = 0.0F;
				keepGoing = false;
				break;
			}
		}
	}

	SetTimePosition(
		in_animationManagerLoad,
		localTimePosition, 
		wrappedPastEnd, 
		wrappedPastBegin
		);

	return keepGoing;
}

const bool GAnimationInstance::HasStream(const int in_streamIndex)const
{
	return (-1 != mArrayStreamKeyIndexHint[in_streamIndex]);
}

void GAnimationInstance::ApplyToStreamPosition(
	GVector3Float& out_position,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex
	)const
{
	GASSERT(-1 != mArrayStreamKeyIndexHint[in_streamIndex], "invalid code path, call HasStream");

	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];
	const GAnimationStreamDataLoad& streamDataLoad = animationLoad.GetArrayStreamDataLoad()[in_streamIndex];
	
	float data[3];
	if (1 == streamDataLoad.GetArrayKeyCount())
	{
		LocalReadValues<3>(data, streamDataLoad, 0);
	}
	else
	{
		LocalLerpValues<3>(
			data,
			mArrayStreamKeyIndexHint[in_streamIndex],
			mTimePosition,
			streamDataLoad
			);
	}
	out_position.SetData(data[0], data[1], data[2]);

	return;
}

void GAnimationInstance::ApplyToStreamRotation(
	GVector3Float& out_at,
	GVector3Float& out_up,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex
	)const
{
	GASSERT(-1 != mArrayStreamKeyIndexHint[in_streamIndex], "invalid code path, call HasStream");

	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];
	const GAnimationStreamDataLoad& streamDataLoad = animationLoad.GetArrayStreamDataLoad()[in_streamIndex];
	
	float data[6];
	if (1 == streamDataLoad.GetArrayKeyCount())
	{
		LocalReadValues<6>(data, streamDataLoad, 0);
	}
	else
	{
		LocalLerpValues<6>(
			data,
			mArrayStreamKeyIndexHint[in_streamIndex],
			mTimePosition,
			streamDataLoad
			);
	}
	out_at.SetData(data[0], data[1], data[2]);
	out_up.SetData(data[3], data[4], data[5]);

	return;
}

void GAnimationInstance::ApplyToStreamFloat(
	float& out_scale,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex
	)const
{
	GASSERT(-1 != mArrayStreamKeyIndexHint[in_streamIndex], "invalid code path, call HasStream");

	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];
	const GAnimationStreamDataLoad& streamDataLoad = animationLoad.GetArrayStreamDataLoad()[in_streamIndex];
	
	if (1 == streamDataLoad.GetArrayKeyCount())
	{
		LocalReadValues<1>(&out_scale, streamDataLoad, 0);
	}
	else
	{
		LocalLerpValues<1>(
			&out_scale,
			mArrayStreamKeyIndexHint[in_streamIndex],
			mTimePosition,
			streamDataLoad
			);
	}

	return;
}


//public methods
void GAnimationInstance::SetTimePosition(
	const GAnimationManagerLoad& in_animationManagerLoad,
	const float in_timePosition, 
	const bool in_resetHintBegining, 
	const bool in_resetHintEnd
	)
{
	mTimePosition = in_timePosition;

	const int streamCount = in_animationManagerLoad.GetArrayStreamInfoCount();
	const GAnimationLoad& animationLoad = in_animationManagerLoad.GetArrayAnimation()[mAnimationLoadIndex];

	for (int index = 0; index < streamCount; ++index)
	{
		const GAnimationStreamDataLoad& streamDataLoad = animationLoad.GetArrayStreamDataLoad()[index];
		const int keyCount = streamDataLoad.GetArrayKeyCount();
		if (keyCount <= 1)
		{
			continue;
		}

		int& keyHint = mArrayStreamKeyIndexHint[index];

		if (in_resetHintBegining)
		{
			keyHint = 0;
		}
		else if (in_resetHintEnd)
		{
			keyHint = keyCount - 1;
		}

		LocalCalculateKeyHint(
			keyHint,
			streamDataLoad,
			mTimePosition
			);
	}

	return;
}
