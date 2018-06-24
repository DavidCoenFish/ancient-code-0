//
//  ZombieGuiDamage.h
//  Zombie
//
//  Created by David Coen on 2011 02 17
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiDamage_h_
#define _ZombieGuiDamage_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class ZombieGameVar;
class ZombieGuiDamageData;
class ApplicationTaskGame;
class GApplicationWindow;
class GComponentLerpFloat;
class GVector2Float;

/*
*/
class ZombieGuiDamage : public boost::noncopyable
{
	//typedef
private:
	//typedef std::pair<int, GVector2Float> TPairIntVector;
	typedef boost::shared_ptr<ZombieGuiDamageData> TPointerZombieGuiDamageData;
	typedef std::vector<TPointerZombieGuiDamageData> TArrayPointerZombieGuiDamageData;

	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//static public methods
public:
	static const int GetDamageTypeBiteBig();
	static const int GetDamageTypeBiteSmall();
	static const int GetDamageTypeSlashRight();
	static const int GetDamageTypeSlashLeft();

	//constructor
public:
	ZombieGuiDamage(GApplicationWindow& inout_window);
	~ZombieGuiDamage();

	//public methods
public:
	void ResetVisual();

	void Tick(const float in_timeDelta);
	void SetVisible(const bool in_visible, const float in_fadeDuration);
	void RequestDamageType(const int in_damageType);

	//private members
private:
	TPointerComponentLerpFloat mComponentFade;
	TArrayPointerZombieGuiDamageData mArrayData;

};

#endif //_ZombieGuiDamage_h_
