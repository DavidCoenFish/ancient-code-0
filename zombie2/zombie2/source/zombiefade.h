//
//  ZombieFade.h
//  Zombie
//
//  Created by David Coen on 2011 03 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieFade_h_
#define _ZombieFade_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationWindow;

class ZombieFade : public boost::noncopyable 
{
	//constructor
public:
	ZombieFade();
	~ZombieFade();

	//public methods
public:
	void Tick(
		const float in_timeDelta, 
		GApplicationWindow& inout_applicationWindow
		);

	//black to transparent
	void RequestFadeIn(
		const float in_duration,
		GApplicationWindow& inout_applicationWindow
		);

	//transparent to black
	void RequestFadeOut(
		const float in_duration,
		GApplicationWindow& inout_applicationWindow
		);

	void Clear(GApplicationWindow& inout_applicationWindow);

	//private members
private:
	bool mFadeIn;
	bool mFadeOut;
	float mDuration;
	float mTime;

};

#endif //_ZombieFade_h_
