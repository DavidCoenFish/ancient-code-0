//
//  ZombieStateWindowCallback.h
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieStateWindowCallback_h_
#define _ZombieStateWindowCallback_h_

#include "GApplicationWindowCallback.h"

class ZombieState;
class GVector2Float;
//#ifdef IPHONE
//@class CAEAGLLayer;
//#endif

class ZombieStateWindowCallback : public GApplicationWindowCallback
{
	//constructor
public:
	ZombieStateWindowCallback(ZombieState& inout_parent);
	virtual ~ZombieStateWindowCallback();

	//implement GApplicationWindowCallback
private:
	virtual void OnResize(const int in_width, const int in_height);
	virtual void OnShake();
	virtual void OnDismissDialog();

	virtual void OnInput(const GInput& in_input);
	virtual const bool OnTick(const float in_timeDelta);
	virtual void OnDraw();

	//private members
private:
	ZombieState& mParent;
};

#endif //_ZombieStateWindowCallback_h_
