//
//  ZombieGuiOffscreen.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiOffscreen_h_
#define _ZombieGuiOffscreen_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class ZombieGameVar;
class ZombieGuiOffscreenData;
class ApplicationTaskGame;
class GApplicationWindow;
class GComponentLerpFloat;
class GVector2Float;

/*
*/
class ZombieGuiOffscreen : public boost::noncopyable
{
	//typedef
private:
	//typedef std::pair<int, GVector2Float> TPairIntVector;
	typedef boost::shared_ptr<ZombieGuiOffscreenData> TPointerZombieGuiOffscreenData;
	typedef std::vector<TPointerZombieGuiOffscreenData> TArrayPointerZombieGuiOffscreenData;

	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieGuiOffscreen(GApplicationWindow& inout_window);
	~ZombieGuiOffscreen();

	//public methods
public:
	void ResetVisual();
	void Tick(
		const float in_timeDelta, 
		GApplicationWindow& inout_window, 
		const ZombieGameVar& in_zombieGameVar
		);
	void SetVisible(const bool in_visible, const float in_fadeDuration);

	//private methods
private:
	void UpdateAlpha();

	//private members
private:
	TPointerComponentLerpFloat mComponentFade;
	TArrayPointerZombieGuiOffscreenData mArrayData;

};

#endif //_ZombieGuiOffscreen_h_
