//
//  ZombieGuiDamageData.h
//  Zombie
//
//  Created by David Coen on 2011 02 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiDamageData_h_
#define _ZombieGuiDamageData_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

class GApplicationWindow;
class GApplicationViewComponentImage;
class GApplicationViewComponent;
class GGuiCoordinate;
class GComponentLerpFloat;

/**/
class ZombieGuiDamageData : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GApplicationViewComponentImage> TPointerApplicationViewComponentImage;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;

	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieGuiDamageData(
		GApplicationWindow& in_applicationWindow,
		const GGuiCoordinate& in_origin,
		const GGuiCoordinate& in_size,
		const std::string& in_imageName
		);
	~ZombieGuiDamageData();

	//public methods
public:
	void ResetVisual();

	void Activate();
	void Tick(const float in_timeDelta, const float in_parentAlpha);

	//private members
private:
	TPointerApplicationViewComponentImage mViewComponentImage;
	TPointerComponentLerpFloat mComponentFade;
	float mResultAlpha;
	bool mShow;

};

#endif //_ZombieGuiDamageData_h_
