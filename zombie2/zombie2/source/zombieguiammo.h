//
//  ZombieGuiAmmo.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiAmmo_h_
#define _ZombieGuiAmmo_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "ZombieStateType.h"

class GApplicationWindow;
class GComponentLerpFloat;
class ZombieGameVar;
class ZombieGuiAmmoData;

class ZombieGuiAmmo : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<ZombieGuiAmmoData> TPointerZombieGuiAmmoData;
	typedef std::vector<TPointerZombieGuiAmmoData> TArrayPointerZombieGuiAmmoData;
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieGuiAmmo(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	~ZombieGuiAmmo();

	//public methods
public:
	void ResetVisual();

	void Tick(const float in_timeDelta, GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	void SetVisible(const bool in_visible, const float in_fadeDuration);

	//private methods
private:
	void UpdateArrayData(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	void UpdateLoaded(const ZombieGameVar& in_zombieGameVar);
	void UpdateAlpha();

	//private members
private:
	TArrayPointerZombieGuiAmmoData mArrayData;
	int mAmmoDisplayCount; //this is the current state of what we are displaying, ZombieGameVar owns the current ammo count
	ZombieStateType::TShotType::TEnum mShotType;

	TPointerComponentLerpFloat mComponentFade;

};

#endif //_ZombieGuiAmmo_h_
