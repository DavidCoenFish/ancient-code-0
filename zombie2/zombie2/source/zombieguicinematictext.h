//
//  ZombieGuiCinematicText.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiCinematicText_h_
#define _ZombieGuiCinematicText_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class GApplicationWindow;
class GComponentLerpFloat;
//class GApplicationViewComponent;
//class GApplicationViewComponentText;


class ZombieGuiCinematicText : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;
	//typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	//typedef boost::shared_ptr<GApplicationViewComponentText> TPointerApplicationViewComponentText;

	//constructor
public:
	ZombieGuiCinematicText(GApplicationWindow& inout_window);
	~ZombieGuiCinematicText();

	//public methods
public:
	void ResetVisual();
	void Tick(const float in_timeDelta);
	void RequestText(const std::string& in_textKey, const float in_holdTextTime);

	//private members
private:
	//TPointerApplicationViewComponentText mViewComponentText;
	TPointerComponentLerpFloat mComponentFade;
	float mHoldTextCountdown;
	float mAlpha;

};

#endif //_ZombieGuiCinematicText_h_
