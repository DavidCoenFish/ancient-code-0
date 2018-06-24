//
//  ginputbutton.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GInputButton_h_
#define _GInputButton_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>

class GInputButton : public boost::noncopyable
{
	//constructor
public:
	GInputButton();
	~GInputButton();

	//public methods
public:
	void Tick(const float in_timeDelta);

	//public accessors
public:
	void SetState(const bool in_down);

	const bool GetDown()const{ return mDown; }
	const bool GetEdgeUp()const{ return mEdgeUp; }
	const bool GetEdgeDown()const{ return mEdgeDown; }
	const bool GetTap()const{ return mTap; }
	const bool GetRepeat()const{ return mRepeat; }

	//private members;
private:
	float mTimer;

	bool mDown;
	bool mEdgeUp;
	bool mEdgeDown;
	bool mTap;
	bool mRepeat;

};

#endif //_GInputButton_h_