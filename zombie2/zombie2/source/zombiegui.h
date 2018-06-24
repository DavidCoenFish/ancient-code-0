//
//  ZombieGui.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGui_h_
#define _ZombieGui_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <GApplicationWindowType.h>

class GApplicationWindow;
class GGuiManager;
class GInput;
class GRender;
class GResourceManager;
class GVector2Float;
class ZombieGameVar;
class ZombieGuiAmmo;
class ZombieGuiHealth;
class ZombieGuiOffscreen;
class ZombieGuiReload;
class ZombieGuiDamage;
class ZombieGuiCinematicText;
class ZombieGuiBossHealth;
class ZombieGuiSplash;
class ZombieMission;
class ZombieGuiMain;

/*
just in game menu, 
no options or splash screen?
*/

class ZombieGui : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<ZombieGameVar> TPointerZombieGameVar;
	typedef boost::weak_ptr<ZombieGameVar> TWeakZombieGameVar;

	typedef boost::shared_ptr<GApplicationWindow> TPointerApplicationWindow;
	typedef boost::weak_ptr<GApplicationWindow> TWeakApplicationWindow;

	typedef boost::shared_ptr<ZombieGuiAmmo> TPointerZombieGuiAmmo;
	typedef boost::shared_ptr<ZombieGuiHealth> TPointerZombieGuiHealth;
	typedef boost::shared_ptr<ZombieGuiOffscreen> TPointerZombieGuiOffscreen;
	typedef boost::shared_ptr<ZombieGuiReload> TPointerZombieGuiReload;
	typedef boost::shared_ptr<ZombieGuiDamage> TPointerZombieGuiDamage;
	typedef boost::shared_ptr<ZombieGuiCinematicText> TPointerZombieGuiCinematicText;
	typedef boost::shared_ptr<ZombieGuiBossHealth> TPointerZombieGuiBossHealth;

	//constructor
public:
	ZombieGui(
		ZombieMission& inout_parentMission,
		boost::shared_ptr<GResourceManager>& inout_resourceManager,
		boost::shared_ptr<ZombieGameVar>& inout_gameVar
		);
	~ZombieGui();

	//public methods
public:
	void Resize(
		const int in_width
		, const int in_height
		, GApplicationWindowType::TOrientation::TEnum& inout_screenOrientation
		);
	void Input(const GInput& in_input);
	const bool Tick(const float in_timeDelta);
	void DrawSplash(GRender& inout_render);
	void Draw(GRender& inout_render);

	//void RunMainMenu();
	void OnSetCameraType(const int in_cameraType);

	void RequestDamageType(const int in_damageType);
	void RequestReloadPrompt();
	void HideReloadPrompt();
	void RequestCinematicText(const std::string& in_textKey, const float in_holdTextTime);

	void RequestBossHealthMaggot();
	void ResetBossHealthMaggot();

	//private members
private:
	boost::shared_ptr<GGuiManager> mGuiManager;
	boost::weak_ptr<ZombieGameVar> mGameVar;

	boost::shared_ptr<ZombieGuiSplash> mSplash;
	boost::shared_ptr<ZombieGuiMain> mMain;

	TPointerZombieGuiAmmo mGuiAmmo;
	TPointerZombieGuiHealth mGuiHealth;
	TPointerZombieGuiOffscreen mGuiOffscreen;
	TPointerZombieGuiReload mGuiReload;
	TPointerZombieGuiDamage mGuiDamage;
	TPointerZombieGuiCinematicText mGuiCinematicText;
	TPointerZombieGuiBossHealth mGuiBossHealth;

};

#endif //_ZombieGui_h_
