//
//  ZombieGuiHealthData.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiHealthData_h_
#define _ZombieGuiHealthData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationViewComponentImage;
class GApplicationViewComponent;
class GApplicationWindow;

class ZombieGuiHealthData : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

	//public static methods
public:
	static const int GetStateEmpty();
	static const int GetStateHalf();
	static const int GetStateFull();

	//constructor
public:
	ZombieGuiHealthData(GApplicationWindow& in_applicationWindow, const int in_index, const float in_alpha);
	~ZombieGuiHealthData();

	//public methods
public:
	void SetState(const int in_state);
	void SetAlpha(const float in_alpha);
	void SetPosition(const int in_index);

	//private members
private:
	TPointerApplicationViewComponentImage mHealthViewImage;
	//TPointerApplicationViewComponent mAmmoViewImage;

	float mAlpha;
	int mState;

};

#endif //_ZombieGuiHealthData_h_
