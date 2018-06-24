//
//  ZombieGuiAmmo.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiMain_h_
#define _ZombieGuiMain_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <GMaterialInstance.h>

class GGui;
class GGuiManager;
class GInput;
class GRender;
class ZombieMission;

class ZombieGuiMain : public boost::noncopyable
{
	//constructor
public:
	ZombieGuiMain(
		GGuiManager& inout_guiManager,
		ZombieMission& inout_parentMission
		);
	~ZombieGuiMain();

	//public methods
public:
	void Draw(GRender& inout_renderComponent);
	void Input(const GInput& in_input);
	const bool Tick(const float in_timeDelta);
	
	void ClickRelease(const std::string& in_buttonName, const bool in_clickDownAndUpInsideButton);

	void SetVisible(const bool in_visible);

	//private members
private:
	ZombieMission& mParentMission;

	boost::weak_ptr<GGui> mMain;
	boost::weak_ptr<GMaterialInstance> mMaterial;

	bool mVisible;

};

#endif //_ZombieGuiAmmo_h_
