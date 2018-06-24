//
//  ZombieGuiOffscreenData.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiOffscreenData_h_
#define _ZombieGuiOffscreenData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

class GApplicationWindow;
class GApplicationViewComponentImage;
class GApplicationViewComponent;
class GVector2Float;

/**/
class ZombieGuiOffscreenData : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

	//constructor
public:
	ZombieGuiOffscreenData(GApplicationWindow& in_applicationWindow);
	~ZombieGuiOffscreenData();

	//public methods
public:
	void SetVisiblie(const bool in_visible);
	void SetAlpha(const float in_alpha);
	void SetPosition(const GVector2Float in_screenPos);

	//private members
private:
	TPointerApplicationViewComponentImage mViewComponentImage;
	float mAlpha;
	bool mVisible;
	std::string mImageName;

};

#endif //_ZombieGuiOffscreenData_h_
