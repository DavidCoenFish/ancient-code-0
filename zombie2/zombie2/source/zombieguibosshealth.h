//
//  ZombieGuiBossHealth.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiBossHealth_h_
#define _ZombieGuiBossHealth_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class ZombieGuiBossHealthData;
class GComponentLerpFloat;
class GApplicationWindow;

class ZombieGuiBossHealth : public boost::noncopyable
{
	//typedef
private:
	//typedef std::pair<int, GVector2Float> TPairIntVector;
	typedef boost::shared_ptr<ZombieGuiBossHealthData> TPointerZombieGuiBossHealthData;
	typedef std::vector<TPointerZombieGuiBossHealthData> TArrayPointerZombieGuiBossHealthData;

	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieGuiBossHealth(GApplicationWindow& inout_window);
	~ZombieGuiBossHealth();

	//public methods
public:
	void ResetVisual();

	void Tick(const float in_timeDelta);
	void SetVisible(const bool in_visible, const float in_fadeDuration);

	void MakeAnotherMaggotVisible();

	//private methods
private:
	void UpdateAlpha();
	
	//private members
private:
	TArrayPointerZombieGuiBossHealthData mArrayData;
	int mVisibleCount;
	TPointerComponentLerpFloat mComponentFade;

};

#endif //_ZombieGuiBossHealth_h_
