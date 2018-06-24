//
//  BeachVolleyBallWatchButtonCallBack.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallWatchButtonCallBack_h_
#define _BeachVolleyBallWatchButtonCallBack_h_

#include <GGuiNodeComponentButtonCallback.h>

class GVector2Float;
class BeachVolleyBallWatch;

class BeachVolleyBallWatchButtonCallBack : public GGuiNodeComponentButtonCallback
{
	//constructor
public:
	BeachVolleyBallWatchButtonCallBack(BeachVolleyBallWatch& inout_parent);
	virtual ~BeachVolleyBallWatchButtonCallBack();

	//implement GGuiNodeComponentButtonCallback
private:
	virtual void OnClickDown(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos		
		);
	virtual void OnClickDrag(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos		
		);
	virtual void OnClickRelease(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const bool in_activate
		);

	//private members
private:
	BeachVolleyBallWatch& mParent;

};

#endif //_BeachVolleyBallWatchButtonCallBack_h_
