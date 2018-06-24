//
//  BeachVolleyBallMenuButtonCallBack.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallMenuButtonCallBack.h"
#include "BeachVolleyBallMenu.h"

//constructor
BeachVolleyBallMenuButtonCallBack::BeachVolleyBallMenuButtonCallBack(BeachVolleyBallMenu& inout_parent)
: mParent(inout_parent)
{
	return;
}

/*virtual*/ BeachVolleyBallMenuButtonCallBack::~BeachVolleyBallMenuButtonCallBack()
{
	return;
}

//implement GGuiNodeComponentButtonCallback
/*virtual*/ void BeachVolleyBallMenuButtonCallBack::OnClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallMenuButtonCallBack::OnClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallMenuButtonCallBack::OnClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const bool in_activate
	)
{
	mParent.ClickButton(in_buttonName);
	return;
}

