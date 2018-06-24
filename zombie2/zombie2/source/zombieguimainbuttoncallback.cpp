//
//  ZombieGuiMainButtonCallback.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieGuiMainButtonCallback.h"
#include "ZombieGuiMain.h"

//constructor
ZombieGuiMainButtonCallback::ZombieGuiMainButtonCallback(ZombieGuiMain& inout_parent)
: m_parent(inout_parent)
{
	return;
}

/*virtual*/ ZombieGuiMainButtonCallback::~ZombieGuiMainButtonCallback()
{
	return;
}

//implement GGuiNodeComponentButtonCallback
/*virtual*/ void ZombieGuiMainButtonCallback::OnClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId		
	)
{
	m_parent.ClickRelease(in_buttonName, true);
	//m_parent.ClickButton(in_buttonName);
	return;
}

/*virtual*/ void ZombieGuiMainButtonCallback::OnClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId
	)
{
	//m_parent.ClickDrag(in_buttonName);
	return;
}

/*virtual*/ void ZombieGuiMainButtonCallback::OnClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId,
	const bool in_activate
	)
{
	//m_parent.ClickRelease(in_buttonName, in_activate);
	return;
}
