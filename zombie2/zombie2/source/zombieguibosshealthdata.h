//
//  ZombieGuiBossHealthData.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiBossHealthData_h_
#define _ZombieGuiBossHealthData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationWindow;
//class GApplicationViewComponent;
//class GApplicationViewComponentImage;
	 
class ZombieGuiBossHealthData : public boost::noncopyable
{
	//typedef
private:
	//typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	//typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

	//constructor
public:
	ZombieGuiBossHealthData(GApplicationWindow& in_applicationWindow, const int in_index);
	~ZombieGuiBossHealthData();

	//public methods
public:
	void SetState(const bool in_visible);
	void SetAlpha(const float in_alpha);

	//private members
private:
	//TPointerApplicationViewComponentImage mViewImage;
	float mAlpha;

};

#endif //_ZombieGuiBossHealthData_h_
