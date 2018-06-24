//
//  GComponentLerpFloat.h
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _GComponentLerpFloat_h_
#define _GComponentLerpFloat_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "GComponentLerp.h"
#include "GVector3Float.h"

/*
	component to manage fade between visible and not visible
*/
class GComponentLerpFloat : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerp> TPointerComponentLerp;

	//constructor
public:
	GComponentLerpFloat(
		const float in_value = 0.0F,
		const float in_duration = 0.0F,
		const float in_targetValue = 0.0F
		);
	~GComponentLerpFloat();

	//public methods
public:
	void Tick(const float in_timeDelta);

	//public accessors
public:
	void SetValue(
		const float in_targetValue, 
		const float in_duration = 0.0F, 
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);
	//wrap old value so we take shortest path around a circle
	void SetValueRadian(
		const float in_targetValue, 
		const float in_duration = 0.0F, 
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);	

	const float GetValue()const{ return mValue; }
	const bool GetActive()const;

	//private members
private:
	float mValue;
	float mValueLerpOld;
	float mValueLerpTarget;
	TPointerComponentLerp mLerp;

};

#endif //_GComponentLerpFloat_h_
