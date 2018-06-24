//
//  BeachVolleyBallGameButtonCallBack.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallGameButtonCallBack.h"
#include "BeachVolleyBallGame.h"

//constructor
BeachVolleyBallGameButtonCallBack::BeachVolleyBallGameButtonCallBack(BeachVolleyBallGame& inout_parent)
: mParent(inout_parent)
{
	return;
}

/*virtual*/ BeachVolleyBallGameButtonCallBack::~BeachVolleyBallGameButtonCallBack()
{
	return;
}

//implement GGuiNodeComponentButtonCallback
/*virtual*/ void BeachVolleyBallGameButtonCallBack::OnClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallGameButtonCallBack::OnClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallGameButtonCallBack::OnClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const bool in_activate
	)
{
	mParent.ClickButton(in_buttonName);
	return;
}

