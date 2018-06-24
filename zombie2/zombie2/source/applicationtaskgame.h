//
//  applicationtask.h
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _applicationtaskgame_H_
#define _applicationtaskgame_H_

#include <gcommon.h>
#include <gapplicationtask.h>
#include "ZombieStateType.h"
#include <GApplicationWindowType.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationWindow;
class GRender;
class GResourceManager;
class GFileSystem;
class GView;
class GScratchPad;
class GGuiManager;
class GMissionMaster;
class GInput;
class StateBase;
class GDictionary;
class ZombieMission;
class ZombieMissionClient;

//interface class for application tasks
class ApplicationTaskGame : public GApplicationTask
{
	///////////////////////////////////////////////////////
	//constructor
public:
	ApplicationTaskGame();
	virtual ~ApplicationTaskGame();

	boost::shared_ptr<GApplicationWindow>& GetWindow() { return mApplicationWindow; }
	boost::shared_ptr<GFileSystem>& GetFileSystem() { return mFileSystem; }
	boost::shared_ptr<GResourceManager>& GetResourceManager() { return mResourceManager; }
	boost::shared_ptr<GRender>& GetRender() { return mRender; }

	//////////////////////////////////////////////////////
	//implement GApplicationTask
private:
	virtual const GBOOL OnTick(const GR32 in_timeDelta);
	virtual GVOID OnSetActive(const GBOOL in_active);

	///////////////////////////////////////////////////////
	//public methods
public:
	void RunMainMenu();

	///////////////////////////////////////////////////////
	//private methods
private:
	GVOID Input(const GInput& in_input);
	GVOID Tick(const GR32 in_timeDelta);
	GVOID Draw(GRender& inout_render);
	GVOID SetScreen(
		const GS32 in_screenWidth, 
		const GS32 in_screenHeight, 
		const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
		);

	void PutText(const std::string& in_text);

	//menu triggers
	//void NewGame(const ZombieStateType::TGameDifficulty::TEnum in_type);
	//void SetSuperSampleType(const ZombieStateType::TSuperSampleType::TEnum in_type);
	//void RestoreGame();
	//void NewGameDebug(const int in_index);
	//void SetUnlock(const ZombieStateType::TAchieviement::TEnum in_acheivement, const bool in_active);

	///////////////////////////////////////////////////////
	//private members
private:
	boost::shared_ptr<GApplicationWindow> mApplicationWindow;
	boost::scoped_ptr<GScratchPad> mScratchPad;

	boost::shared_ptr<GView> mViewRender;
	boost::shared_ptr<GRender> mRender;
	boost::shared_ptr<GFileSystem> mFileSystem;
	boost::shared_ptr<GResourceManager> mResourceManager;

	boost::shared_ptr<ZombieMission> mZombieMission;
	
	bool mRequestMainMenu;

};

#endif //#ifndef _applicationtask_H_
