//
//  ZombieGuiAmmoData.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiAmmoData_h_
#define _ZombieGuiAmmoData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ZombieStateType.h"

class GApplicationViewComponentImage;
class GApplicationViewComponent;
class GApplicationWindow;

class ZombieGuiAmmoData : public boost::noncopyable
{
	//typedef
private:
	//typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	//typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

	//constructor
public:
	ZombieGuiAmmoData(
		GApplicationWindow& in_applicationWindow, 
		const int in_index, 
		const int in_maximumCount, 
		const float in_alpha,
		const ZombieStateType::TShotType::TEnum in_shotType
		);
	~ZombieGuiAmmoData();

	//public methods
public:
	void SetShotType(const ZombieStateType::TShotType::TEnum in_shotType);
	void SetLoaded(const bool in_loaded);
	void SetAlpha(const float in_alpha);
	void SetPosition(const int in_index, const int in_maximumCount);
	void SetVisible(const bool in_visible);

	//private members
private:
	//TPointerApplicationViewComponentImage mAmmoViewImage;
	//TPointerApplicationViewComponent mAmmoViewImage;

	float mAlpha;
	bool mLoaded;
	ZombieStateType::TShotType::TEnum mShotType;

};

#endif //_ZombieGuiAmmoData_h_
