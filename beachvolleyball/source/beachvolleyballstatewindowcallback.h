//
//  BeachVolleyBallStateWindowCallback.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallStateWindowCallback_h_
#define _BeachVolleyBallStateWindowCallback_h_

#include "GApplicationWindowCallback.h"

class BeachVolleyBallState;
class GVector2Int;

class BeachVolleyBallStateWindowCallback : public GApplicationWindowCallback
{
	//constructor
public:
	BeachVolleyBallStateWindowCallback(BeachVolleyBallState& inout_parent);
	virtual ~BeachVolleyBallStateWindowCallback();

	//implement GApplicationWindowCallback
private:
	virtual void OnResize(const GVector2Int& in_size);
	virtual void OnShake();
	virtual void OnDismissDialog();

	virtual void OnInput(const GInput& in_input);
	virtual const bool OnTick(const float in_timeDelta);
	virtual void OnDraw();

	//private members
private:
	BeachVolleyBallState& mParent;
};

#endif //_BeachVolleyBallStateWindowCallback_h_
