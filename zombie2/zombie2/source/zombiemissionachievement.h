//
//  ZombieMissionAchievement.h
//  Zombie
//
//  Created by David Coen on 2011 03 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieMissionAchievement_h_
#define _ZombieMissionAchievement_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "ZombieStateType.h"

class GApplicationWindow;
class GInput;
class ZombieGameVar;
class ZombieMission;

/*
	display dialog to describe unlock, and apply unlock
*/
class ZombieMissionAchievement : public boost::noncopyable
{
	//typedef
private:
	//typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	//typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	//typedef boost::shared_ptr<GApplicationViewComponentText> TPointerApplicationViewComponentText;

	typedef std::vector<ZombieStateType::TAchieviement::TEnum> TArrayAchieviement;
	typedef std::vector<ZombieStateType::TBonus::TEnum> TArrayBonus;

	//constructor
public:
	ZombieMissionAchievement(GApplicationWindow& inout_applicationWindow);
	~ZombieMissionAchievement();

	//public methods
public:
	//at end of level or death
	const bool CalculateAcheivementLevelEnd(
		ZombieGameVar& in_gameVar, 
		const int in_actorCount,
		const bool in_death
		);

	void AddAchieviement(const ZombieStateType::TAchieviement::TEnum in_achieviement, ZombieGameVar& inout_gameVar);
	void AddBonus(const ZombieStateType::TBonus::TEnum in_bonus);

	//return true while we are displaying dialog
	const bool Tick(const float in_timeDelta, ZombieGameVar& in_gameVar);

	//allow dismiss dialog on touch
	void Input(const GInput& in_input);

	//private methods
private:
	void DeactivateDialog();

	//private members
private:
	GApplicationWindow& mApplicationWindow;

	float mCountdown;
	bool mActive;

	//TPointerApplicationViewComponent mBackground;
	//TPointerApplicationViewComponentImage mDialogBackground;
	//TPointerApplicationViewComponentImage mDialogImage;
	//TPointerApplicationViewComponentText mDialogText0;
	//TPointerApplicationViewComponentText mDialogText1;

	TArrayAchieviement mArrayAchieviement;
	TArrayBonus mArrayBonus;

};

#endif //_ZombieMissionAchievement_h_
