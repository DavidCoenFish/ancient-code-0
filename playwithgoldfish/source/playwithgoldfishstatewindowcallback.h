//
//  PlayWithGoldfishStateWindowCallback.h
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateWindowCallback_h_
#define _PlayWithGoldfishStateWindowCallback_h_

#include "GApplicationWindowCallback.h"

class PlayWithGoldfishState;
class GVector2Int;
//#ifdef IPHONE
//@class CAEAGLLayer;
//#endif

class PlayWithGoldfishStateWindowCallback : public GApplicationWindowCallback
{
	//constructor
public:
	PlayWithGoldfishStateWindowCallback(PlayWithGoldfishState& inout_parent);
	virtual ~PlayWithGoldfishStateWindowCallback();

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
	PlayWithGoldfishState& mParent;
};

#endif //_PlayWithGoldfishStateWindowCallback_h_
