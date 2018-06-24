//
//  BeachVolleyBallGameButtonCallBack.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallGameButtonCallBack_h_
#define _BeachVolleyBallGameButtonCallBack_h_

#include <GGuiNodeComponentButtonCallback.h>

class GVector2Float;
class BeachVolleyBallGame;

class BeachVolleyBallGameButtonCallBack : public GGuiNodeComponentButtonCallback
{
	//constructor
public:
	BeachVolleyBallGameButtonCallBack(BeachVolleyBallGame& inout_parent);
	virtual ~BeachVolleyBallGameButtonCallBack();

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
	BeachVolleyBallGame& mParent;

};

#endif //_BeachVolleyBallGameButtonCallBack_h_
