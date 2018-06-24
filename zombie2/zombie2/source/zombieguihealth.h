//
//  ZombieGuiHealth.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiHealth_h_
#define _ZombieGuiHealth_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class GApplicationWindow;
class GComponentLerpFloat;
class ZombieGameVar;
class ZombieGuiHealthData;

class ZombieGuiHealth : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<ZombieGuiHealthData> TPointerZombieGuiHealthData;
	typedef std::vector<TPointerZombieGuiHealthData> TArrayPointerZombieGuiHealthData;
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieGuiHealth(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	~ZombieGuiHealth();

	//public methods
public:
	void ResetVisual();
	void Tick(const float in_timeDelta, GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	void SetVisible(const bool in_visible, const float in_fadeDuration);

	//private methods
private:
	void UpdateArrayData(GApplicationWindow& inout_window, const ZombieGameVar& in_zombieGameVar);
	void UpdateData(const ZombieGameVar& in_zombieGameVar);
	void UpdateAlpha();

	//private members
private:
	TArrayPointerZombieGuiHealthData mArrayData;
	int mHealthDisplayCount; //this is the current state of what we are displaying, ZombieGameVar owns the current ammo count

	TPointerComponentLerpFloat mComponentFade;

};

#endif //_ZombieGuiHealth_h_
