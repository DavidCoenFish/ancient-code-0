//
//  ZombieGuiReload.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiReload_h_
#define _ZombieGuiReload_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class ZombieMission;
class GApplicationWindow;
class GComponentLerpFloat;
class GApplicationViewComponent;
class GApplicationViewComponentText;

class ZombieGuiReload : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	typedef boost::shared_ptr<GApplicationViewComponentText> TPointerApplicationViewComponentText;

	//private static methods
private:
	static void ButtonClickCallback(void* const in_data);

	//constructor
public:
	ZombieGuiReload(
		ZombieMission& inout_parentMission,
		GApplicationWindow& inout_window
		);
	~ZombieGuiReload();

	//public methods
public:
	void ResetVisual();
	void RequestReloadPrompt();

	//private members
private:
	ZombieMission& mParentMission; //reload button needs to tell parent when it is clicked
	GApplicationWindow& mApplicationWindow;

	TPointerApplicationViewComponentText mViewComponentText;
	TPointerApplicationViewComponent mComponentReloadButton;

};

#endif //_ZombieGuiReload_h_
