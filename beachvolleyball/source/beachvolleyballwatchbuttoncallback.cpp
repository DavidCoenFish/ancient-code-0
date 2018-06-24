//
//  BeachVolleyBallWatchButtonCallBack.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallWatchButtonCallBack.h"
#include "BeachVolleyBallWatch.h"

//constructor
BeachVolleyBallWatchButtonCallBack::BeachVolleyBallWatchButtonCallBack(BeachVolleyBallWatch& inout_parent)
: mParent(inout_parent)
{
	return;
}

/*virtual*/ BeachVolleyBallWatchButtonCallBack::~BeachVolleyBallWatchButtonCallBack()
{
	return;
}

//implement GGuiNodeComponentButtonCallback
/*virtual*/ void BeachVolleyBallWatchButtonCallBack::OnClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallWatchButtonCallBack::OnClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos		
	)
{
	return;
}

/*virtual*/ void BeachVolleyBallWatchButtonCallBack::OnClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const bool in_activate
	)
{
	mParent.ClickButton(in_buttonName);
	return;
}

