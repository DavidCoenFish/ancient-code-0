//
//  GComponentLerp.h
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _GComponentLerp_h_
#define _GComponentLerp_h_

#include <boost/noncopyable.hpp>

/*
	component to manage lerp between 0.0 and 1.0 over duration
*/
class GComponentLerp : public boost::noncopyable
{
	//typedef
public:
	struct TType
	{
		enum TEnum
		{
			TLinear = 0,
			TEaseInOut,

			TCount
		};
	};

	//constructor
public:
	GComponentLerp(const float in_duration = 0.0F);
	~GComponentLerp();

	//public methods
public:
	void Tick(const float in_timeDelta);

	//public accessors
public:
	void SetLerp(const float in_duration, const TType::TEnum in_type = TType::TLinear);
	const float GetRatio()const{ return mRatio; }
	const bool GetActive()const{ return (0.0F != mDuration); }
	const bool GetFinished()const{ return mFinished; }

	//private members
private:
	float mTime;
	float mDuration;
	float mRatio;
	int mType;
	bool mFinished;

};

#endif //_GComponentLerp_h_
