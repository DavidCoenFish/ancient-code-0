//
//  ZombieMissionVarMission00.h
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieMissionVarMission00_h_
#define _ZombieMissionVarMission00_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationWindow;
class GApplicationViewComponent;
class GApplicationViewComponentImage;
class GApplicationViewComponentText;
class GInput;
class ZombieMission;

/*
	decide when to trigger the reload 
*/
class ZombieMissionVarMission00 : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	typedef boost::shared_ptr<GApplicationViewComponentText> TPointerApplicationViewComponentText;

	//constructor
public:
	ZombieMissionVarMission00(GApplicationWindow& inout_applicationWindow);
	~ZombieMissionVarMission00();

	//public methods
public:
	void Tick(
		const float in_timeDelta,
		ZombieMission& inout_mission
		);

	void Input(const GInput& in_input, ZombieMission& inout_mission);

	void IncrementRequestReload(
		ZombieMission& inout_mission
		);

	void LookUp(ZombieMission& inout_mission);
	void Fire(ZombieMission& inout_mission);
	void Reload(ZombieMission& inout_mission);

	//private methods
private:
	void DeactivateDialog();

	//private members
private:
	GApplicationWindow& mApplicationWindow;

	float mTimeWatchingFeet;
	bool mHasLookedUp;

	float mTimeWithoutFire;
	bool mHasFired;

	int mReloadRequestCount;
	bool mHasReloaded;

	float mTutorialCompleteTimer;
	bool mActivatedAi;

	//the view components for the messages?
	bool mDialogActiveLookUp;
	bool mDialogActiveFire;
	bool mDialogActiveReload;

	TPointerApplicationViewComponent mBackground;
	TPointerApplicationViewComponentImage mDialogBackground;
	TPointerApplicationViewComponentImage mDialogImageOne;
	TPointerApplicationViewComponentImage mDialogImadeTwo;
	TPointerApplicationViewComponentText mDialogText;

};

#endif //_ZombieMissionVarMission00_h_
