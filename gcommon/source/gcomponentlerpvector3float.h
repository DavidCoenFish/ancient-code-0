//
//  GComponentLerpVector3Float.h
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _GComponentLerpVector3Float_h_
#define _GComponentLerpVector3Float_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "GComponentLerp.h"
#include "GVector3Float.h"

/*
	component to manage fade between visible and not visible
*/
class GComponentLerpVector3Float : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerp> TPointerComponentLerp;

	//constructor
public:
	GComponentLerpVector3Float(
		const GVector3Float& in_value = GVector3Float::sZero,
		const float in_duration = 0.0F,
		const GVector3Float& in_targetValue = GVector3Float::sZero		
		);
	~GComponentLerpVector3Float();

	//public methods
public:
	void Tick(const float in_timeDelta);

	//public accessors
public:
	void SetValue(
		const GVector3Float& in_targetValue, 
		const float in_duration = 0.0F, 
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);
	
	const GVector3Float& GetValue()const{ return mValue; }
	const bool GetActive()const;

	//private members
private:
	GVector3Float mValue;
	GVector3Float mValueLerpOld;
	GVector3Float mValueLerpTarget;
	TPointerComponentLerp mLerp;

};

#endif //_GComponentLerpVector3Float_h_
