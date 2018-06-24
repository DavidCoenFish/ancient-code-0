//
//  BeachVolleyBallMenuButtonCallBack.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallMenuButtonCallBack_h_
#define _BeachVolleyBallMenuButtonCallBack_h_

#include <GGuiNodeComponentButtonCallback.h>

class GVector2Float;
class BeachVolleyBallMenu;

class BeachVolleyBallMenuButtonCallBack : public GGuiNodeComponentButtonCallback
{
	//constructor
public:
	BeachVolleyBallMenuButtonCallBack(BeachVolleyBallMenu& inout_parent);
	virtual ~BeachVolleyBallMenuButtonCallBack();

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
	BeachVolleyBallMenu& mParent;

};

#endif //_BeachVolleyBallMenuButtonCallBack_h_
