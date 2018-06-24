//
//  ginputtouch.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GInputTouch_h_
#define _GInputTouch_h_

#include "gcommon.h"
#include "gvector2float.h"
#include <boost/noncopyable.hpp>
#include "GApplicationWindowType.h"

class GInputTouch : public boost::noncopyable
{
	//constructor
public:
	GInputTouch(
		const unsigned int in_touchId = 0,
		const GVector2Float& in_touchLocation = GVector2Float::sZero,
		const GVector2Float& in_touchDelta = GVector2Float::sZero,
		const bool in_touchBegin = true,
		const bool in_touchEnd = false,
		const bool in_touchCancel = false
		);
	~GInputTouch();

	//public methods
public:
	void UpdateLocation(const GVector2Float& in_newLocation, const GBOOL in_begin);
	void ClearDelta();

	//public accessors
public:
	const unsigned int GetTouchId()const{ return mTouchId; }
	const GVector2Float& GetTouchLocation()const{ return mTouchLocation; }
	const GVector2Float& GetTouchDelta()const{ return mTouchDelta; }
	const bool GetTouchBegin()const{ return mTouchBegin; }
	void SetTouchBegin(const bool in_value){ mTouchBegin = in_value; return; }

	const bool GetTouchCancel()const{ return mTouchCancel; }
	void SetTouchCancel(const bool in_value){ mTouchCancel = in_value; return; }

	const bool GetTouchEnd()const{ return mTouchEnd; }
	void SetTouchEnd(const bool in_value){ mTouchEnd = in_value; return; }

	const GVector2Float GetTouchLocationOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation)const;
	const GVector2Float GetTouchDeltaOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation)const;

	//private members;
private:
	const unsigned int mTouchId;
	GVector2Float mTouchLocation;
	GVector2Float mTouchDelta;
	bool mTouchBegin;
	bool mTouchCancel;
	bool mTouchEnd;

};

#endif 