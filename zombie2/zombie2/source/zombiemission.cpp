//
//  ZombieMission.cpp
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieMission.h"

#include "SceneNodeComponentClientActor.h"
#include "SceneNodeComponentClientBat.h"
#include "SceneNodeComponentClientSlug.h"
#include "SceneNodeComponentClientSkeletonOne.h"
#include "SceneNodeComponentClientZombieBoss.h"
#include "SceneNodeComponentClientZombieOne.h"
#include "SceneNodeComponentClientZombieFat.h"
#include "SceneNodeComponentClientZombieFly.h"
#include "SceneNodeComponentClientZombieGirl.h"
#include "SceneNodeComponentClientDropShadow.h"
#include "SceneNodeComponentClientPickup.h"
#include "ZombieCameraMixer.h"
#include "ZombieGameVar.h"
#include "ZombieGui.h"
#include "ZombieReloadDetector.h"
#include "ZombieMissionClient.h"
#include "applicationtaskgame.h"
#include "ZombieStateType.h"
#include "ZombieSound.h"
#include "ZombieMissionVar.h"
#include "ZombieMissionVarMission00.h"
#include "ZombieFade.h"
#include "ZombieMissionAchievement.h"

#include <gassert.h>
#include <GApplicationWindow.h>
#include <GColour4Float.h>
#include <GVector2Float.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GInputButton.h>
#include <GCamera.h>
#include <GScene.h>
#include <GSceneVisitorRender.h>
#include <GFileSystem.h>
#include <GDictionary.h>
#include <GCamera.h>
#include <GVector3Float.h>
#include <GStringUtility.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentFade.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentParticle.h>
#include <GSceneNodeComponentVisual.h>
#include <GMath.h>
#include <GGuiCoordinate.h>
#include <GInputJoystick.h>
#include <GSceneNodeComponentClient.h>
#include <GScratchPad.h>
#include <GSceneNodeFactory.h>
#include <GSceneNodeComponentCollision.h>
#include <GSceneNodeComponentCollisionResult.h>
#include <GScenePostLoadCallback.h>
#include <GSceneVisitorFustrum.h>
#include <GMissionMaster.h>
#include <GAnimationPlayParam.h>
#include <GSceneNodeComponentPhysics.h>
#include <GResourceManager.h>
#include <GRender.h>
#include <GParticleManager.h>
#include <GGuiManager.h>
#include <GBuffer.h>
#include <GGuiVisitorUpdate.h>
#include <GGuiVisitorRender.h>
#include <GGui.h>

#include <string>
#include <vector>

#define NO_FIRE_WITH_DEBUG_CAMERA
#define WIN32_RELOAD_ON_SHAKE

//#define INFINITE_HEALTH
//#define DEBUG_LEVELS
#ifdef WIN32
//#define INFINITE_AMMO
//#define INFINITE_HEALTH
#endif

#ifdef WIN32
#define WIDGIT_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_WIN32
#else
#define WIDGIT_NAME_MACRO(IN_IPHONE, IN_WIN32) IN_IPHONE
#endif

static const char* const sDefaultScreen = WIDGIT_NAME_MACRO("Default.png", "win32\\Default.bmp");
static const char* const sArrow0 = WIDGIT_NAME_MACRO("guiarrow_00.png", "win32\\guiarrow_00.bmp");
static const char* const sArrow1 = WIDGIT_NAME_MACRO("guiarrow_01.png", "win32\\guiarrow_01.bmp"); 
static const char* const sArrow2 = WIDGIT_NAME_MACRO("guiarrow_02.png", "win32\\guiarrow_02.bmp");
static const char* const sArrow3 = WIDGIT_NAME_MACRO("guiarrow_03.png", "win32\\guiarrow_03.bmp");
static const char* const sArrow4 = WIDGIT_NAME_MACRO("guiarrow_04.png", "win32\\guiarrow_04.bmp");


static const char* const sPathMission02Key = "Mission02Key";
static const char* const sPathMission03Key = "Mission03Key";
static const char* const sPathMission04Key = "Mission04Key";
static const char* const sPathMission05Key = "Mission05Key";
static const char* const sPathMission06Key = "Mission06Key";
static const char* const sPathMission07Key = "Mission07Key";
static const char* const sPathMission08Key = "Mission08Key";
static const char* const sPathMission09Key = "Mission09Key";
static const char* const sPathMission10Key = "Mission10Key";
static const char* const sPathMission11Key = "Mission11Key";

static const float sCinematicHoldTextTime = 2.5F;


static const char* const sApplicationPreferenceFileName = "option.data";

static const char* const sAntiAliasKey = "AntiAliasKey";
static const char* const sNewGameEasyKey = "NewGameEasyKey";
static const char* const sNewGameNormalKey = "NewGameNormalKey"; 
static const char* const sNewGameHardKey = "NewGameHardKey"; 
static const char* const sResumeGameKey = "ResumeGameKey"; 
static const char* const sGoreParentKey = "GoreParentKey";
static const char* const sGoreKey = "GoreKey";
static const char* const sVolumeKey = "VolumeKey";
static const char* const sVolumeEffectsKey = "VolumeEffectsKey";
static const char* const sResetKey = "ResetOptionKey";
static const char* const sCreditAppStoreKey = "CreditAppStoreKey";

#ifdef DEBUG_LEVELS
static const char* const sNewGameDebug0Key = "NewGameDebug0Key";
static const char* const sNewGameDebug1Key = "NewGameDebug1Key"; 
static const char* const sNewGameDebug2Key = "NewGameDebug2Key"; 
static const char* const sNewGameDebug3Key = "NewGameDebug3Key"; 
#endif 

static const char* const sFirstBloodParentKey       = "FirstBloodParentKey";
static const char* const sFirstBloodKey				= "FirstBloodKey";			 
static const char* const sIdBeBackParentKey			= "IdBeBackParentKey";		
static const char* const sIdBeBackKey				= "IdBeBackKey";			 
static const char* const sSlugQueenParentKey        = "SlugQueenParentKey";      
static const char* const sSlugQueenKey				= "SlugQueenKey";			 
static const char* const sBulletProofParentKey      = "BulletProofParentKey";    
static const char* const sBulletProofKey			= "BulletProofKey";			 
static const char* const sNewbParentKey				= "NewbParentKey";			
static const char* const sNewbKey					= "NewbKey";				 
static const char* const sAccuracyParentKey         = "AccuracyParentKey";       
static const char* const sAccuracyKey				= "AccuracyKey";			 
static const char* const sEfficiencyParentKey       = "EfficiencyParentKey";     
static const char* const sEfficiencyKey				= "EfficiencyKey";			 
static const char* const sHeadHunterParentKey       = "HeadHunterParentKey";     
static const char* const sHeadHunterKey				= "HeadHunterKey";			 
static const char* const sGoodEffortParentKey       = "GoodEffortParentKey";     
static const char* const sGoodEffortKey				= "GoodEffortKey";			 
static const char* const sPersistanceParentKey      = "PersistanceParentKey";    
static const char* const sPersistanceKey			= "PersistanceKey";			 
static const char* const sEradicatorParentKey       = "EradicatorParentKey";     
static const char* const sEradicatorKey				= "EradicatorKey";			 
static const char* const sMissionPossibleParentKey	= "MissionPossibleParentKey";
static const char* const sMissionPossibleKey        = "MissionPossibleKey";       
static const char* const sRockParentKey				= "RockParentKey";
static const char* const sRockKey					= "RockKey";
static const char* const sUnlocksEnableParentKey    = "UnlocksEnableParentKey";   
static const char* const sUnlocksEnableKey			= "UnlocksEnableKey";   

struct TMissionList
{
	enum TEnum
	{
		TMissionIntro = 0, //missionintro
		TMissionScene00, //missionscene00
		TMissionScene01, 
		TMissionScene02, 
		TMissionScene03, 
		TMissionScene04, 
		TMissionScene05, 
		TMissionScene06, 
		TMissionScene07, 
		TMissionScene08, 
		TMissionScene09, 
		TMissionScene10, 
		TMissionScene11,
		//TGameOverComplete,
		//TGameOverDeath,
		
#ifdef DEBUG_LEVELS
		TMissionSceneDebug00,
		TMissionSceneDebug01,
		TMissionSceneDebug02,
		TMissionSceneDebug03,
#endif

		TCount
	};
};

const char* const sArrayMissionName[TMissionList::TCount] = 
{
	"missionintro",
	"missionscene00",
	"missionscene01",
	"missionscene02",
	"missionscene03",
	"missionscene04",
	"missionscene05",
	"missionscene06",
	"missionscene07",
	"missionscene08",
	"missionscene09",
	"missionscene10",
	"missionscene11"
#ifdef DEBUG_LEVELS
	,"missionscenedebug00"
	,"missionscenedebug01"
	,"missionscenedebug02"
	,"missionscenedebug03"
#endif
};

typedef boost::shared_ptr<GCamera> TPointerCamera;
typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef std::pair<int, int> TPairIntInt;
typedef std::vector<TPairIntInt> TArrayPairIntInt;
typedef std::vector<unsigned char> TArrayByte;
typedef std::vector<unsigned short> TArrayShort;
typedef boost::shared_ptr<GOptionDialogDataItem> TPointerOptionDialogDataItem;
typedef std::vector<TPointerOptionDialogDataItem> TArrayPointerOptionDialogDataItem;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	);
typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;

typedef boost::weak_ptr<GRenderMaterial> TWeakRenderMaterial;
typedef std::pair<std::string, TWeakRenderMaterial> TPairStringWeakRenderMaterial;
typedef std::vector<TPairStringWeakRenderMaterial> TArrayPairStringWeakRenderMaterial;

typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;
typedef std::map<SceneNodeComponentClientObjectBase*, int> TMapClientObjectInt;
typedef boost::shared_ptr<GMissionClient> TPointerMissionClient;
typedef std::vector<GVector2Float> TArrayVector2;
	
typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;

//public static methods
/*static*/ const int ZombieMission::GetMissionIdIntro()
{
	return TMissionList::TMissionIntro;
}
/*static*/ const int ZombieMission::GetMissionFirst()
{
	return TMissionList::TMissionScene00;
}
/*static*/ const int ZombieMission::GetMissionIdSceneSlugs()
{
	return TMissionList::TMissionScene02;
}
/*static*/ const int ZombieMission::GetMissionIdSceneBats()
{
	return TMissionList::TMissionScene06;
}
/*static*/ const int ZombieMission::GetMissionIdSceneBossFight()
{
	return TMissionList::TMissionScene11;
}
/*static*/ const bool ZombieMission::GetMissionIsPlusShotPower(const int in_index)
{
	return ((in_index == TMissionList::TMissionScene03) ||
		(in_index == TMissionList::TMissionScene09));
}
/*static*/ const bool ZombieMission::GetMissionIsPlusHealth(const int in_index)
{
	return ((in_index == TMissionList::TMissionScene04) ||
		(in_index == TMissionList::TMissionScene08));
}
/*static*/ const bool ZombieMission::GetMissionIsHeal(const int in_index)
{
	return ((in_index == TMissionList::TMissionScene01) ||
		(in_index == TMissionList::TMissionScene05) ||
		(in_index == TMissionList::TMissionScene07));
}

/*static*/ const char* const ZombieMission::GetMissionName(const int in_index)
{
	return sArrayMissionName[in_index];
}


//private static methods
/*static void ZombieMission::OnOptionDialogDataItemCallbackPath(void* const in_callbackData, GOptionDialogDataItem& inout_dataItem)
{
	GASSERT(NULL != in_callbackData, "invalide param");
	ZombieMission& localThis = *(ZombieMission*)in_callbackData;
	GApplicationWindow& applicationWindow = *localThis.mParentState.GetWindow();
	applicationWindow.DismissOptionDialog();

	TMissionList::TEnum mission = TMissionList::TCount;
	if (inout_dataItem.GetNameKey() == sPathMission02Key)
	{
		mission = TMissionList::TMissionScene02;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission03Key)
	{
		mission = TMissionList::TMissionScene03;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission04Key)
	{
		mission = TMissionList::TMissionScene04;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission05Key)
	{
		mission = TMissionList::TMissionScene05;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission06Key)
	{
		mission = TMissionList::TMissionScene06;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission07Key)
	{
		mission = TMissionList::TMissionScene07;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission08Key)
	{
		mission = TMissionList::TMissionScene08;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission09Key)
	{
		mission = TMissionList::TMissionScene09;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission10Key)
	{
		mission = TMissionList::TMissionScene10;
	}
	else if (inout_dataItem.GetNameKey() == sPathMission11Key)
	{
		mission = TMissionList::TMissionScene11;
	}

	if (TMissionList::TCount != mission)
	{
		localThis.LoadMission(mission, applicationWindow, false);
	}

	return;
}
/**/

//constructor
ZombieMission::ZombieMission(
	ApplicationTaskGame& in_parentState,
	TPointerApplicationWindow& inout_window							 
	)
	: mParentState(in_parentState)
	, mSceneNodeFactory()
	, mMissionId(TMissionList::TCount)
	, mSceneName()
	, mScene()
	, mSceneNodeRootScene()
	, mSceneNodeRootActors()
	, mMissionMaster()
	, mListClientActor()
	, mListClientObject()
	, mListClientTaraget()
	, mZombieGui()
	, mZombieGameVar()
	, mZombieReloadDetector()
	, mZombieMissionClient()
	, mZombieCameraMixer()
	, mFactoryPosition()
	, mFactoryAt(-GVector3Float::sUnitZ)
	, mFactoryUp(GVector3Float::sUnitY)
	, mPause(false)
	, mRequestMainMenu(false)
	, mRequestNextLevel(false)
	, mStopAllSequences(false)
	, mRunningMissionAchievement(false)
	, mDeathCountdown(0.0F)
{
	mZombieGameVar.reset(new ZombieGameVar(mParentState.GetFileSystem()));
	if (mZombieGameVar)
	{
		mZombieGameVar->ResetGameVar(); //before making gui
	}

	mZombieGui.reset(new ZombieGui(*this, in_parentState.GetResourceManager(), mZombieGameVar));

	mOptionDictionary.reset(new GDictionary);

	TMapStringPairFactoryClientCallbackData clientFactory;
	clientFactory["ClientActor"] = TPairFactoryClientCallbackData(SceneNodeComponentClientActor::Factory, this);
	clientFactory["ClientBat"] = TPairFactoryClientCallbackData(SceneNodeComponentClientBat::Factory, this);
	clientFactory["ClientSkeletonOne"] = TPairFactoryClientCallbackData(SceneNodeComponentClientSkeletonOne::Factory, this);
	clientFactory["ClientZombieBoss"] = TPairFactoryClientCallbackData(SceneNodeComponentClientZombieBoss::Factory, this);
	clientFactory["ClientZombieOne"] = TPairFactoryClientCallbackData(SceneNodeComponentClientZombieOne::Factory, this);
	clientFactory["ClientZombieFat"] = TPairFactoryClientCallbackData(SceneNodeComponentClientZombieFat::Factory, this);
	clientFactory["ClientZombieFly"] = TPairFactoryClientCallbackData(SceneNodeComponentClientZombieFly::Factory, this);
	clientFactory["ClientZombieGirl"] = TPairFactoryClientCallbackData(SceneNodeComponentClientZombieGirl::Factory, this);
	clientFactory["ClientSlug"] = TPairFactoryClientCallbackData(SceneNodeComponentClientSlug::Factory, this);
	clientFactory["ClientTarget"] = TPairFactoryClientCallbackData(SceneNodeComponentClientTarget::Factory, this);
	clientFactory["ClientDropShadow"] = TPairFactoryClientCallbackData(SceneNodeComponentClientDropShadow::Factory, this);
	clientFactory["ClientPickupGarbage"] = TPairFactoryClientCallbackData(SceneNodeComponentClientPickup::FactoryGarbage, this);
	clientFactory["ClientPickupAmmo"] = TPairFactoryClientCallbackData(SceneNodeComponentClientPickup::FactoryAmmo, this);
	clientFactory["ClientPickupSlugFountain"] = TPairFactoryClientCallbackData(SceneNodeComponentClientPickup::FactorySlugFountain, this);
	clientFactory["ClientPickupHeal"] = TPairFactoryClientCallbackData(SceneNodeComponentClientPickup::FactoryHeal, this);

	mParticleManager = GParticleManager::Factory(
		mParentState.GetResourceManager()->GetFileManager(),
		"particlemanager.data"
		);

	mSceneNodeFactory = GSceneNodeFactory::Factory(
		mParentState.GetResourceManager()->GetFileManager(),
		"factoryscene.data"
		);

	mScene.reset(new GScene(
		mParentState.GetResourceManager(),
		clientFactory,
		mSceneNodeFactory,
		mParticleManager
		));

	mZombieReloadDetector.reset(new ZombieReloadDetector(-GVector3Float::sUnitY));

	mZombieCameraMixer.reset(new ZombieCameraMixer);

	mZombieMissionClient.reset(new ZombieMissionClient(*this));

#ifndef WIN32
	mZombieFade.reset(new ZombieFade);
#endif

	{
		TPointerMissionClient missionClient(mZombieMissionClient);
		mMissionMaster = GMissionMaster::Factory("mission.data", mParentState.GetResourceManager(), missionClient);
	}

	//skybox
	mSceneNodeRootSkyBox = mScene->CreateSceneNode("skyboxRoot");
	mScene->LoadSceneUnderNode(
		"sceneskybox.data",
		mSceneNodeRootSkyBox
		);

	mZombieSound.reset(new ZombieSound(*this));

	mZombieMissionAchievement.reset(new ZombieMissionAchievement(*inout_window));

	return;
}

/*virtual*/ ZombieMission::~ZombieMission()
{
	return;
}

//public methods
void ZombieMission::Resize(
	const int in_width, 
	const int in_height,
	const GApplicationWindowType::TOrientation::TEnum in_screenOrientation						   
	)
{
	GApplicationWindowType::TOrientation::TEnum screenOrientation = in_screenOrientation;

	if (mZombieGui)
	{
		mZombieGui->Resize(
			in_width, 
			in_height,
			screenOrientation
			);
	}

	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->ResizeWindow(in_width, in_height);
		//screenOrientation
	}

	return;
}

void ZombieMission::Shake()
{
#ifdef WIN32
	if (mMissionId == TMissionList::TMissionIntro)
	{
		//NewGame(ZombieStateType::TGameDifficulty::TEasy, inout_window);
		RestoreGame();
	}
	else
	{
#ifdef WIN32_RELOAD_ON_SHAKE
		Reload();
#else
		LoadMission((mMissionId + 1) % TMissionList::TCount, inout_window);
#endif
	}
#else
	Reload();
#endif
	return;
}

void ZombieMission::Input(const GInput& in_input)
{
	if (mZombieGui)
	{
		mZombieGui->Input(in_input);
	}

	if (mRunningMissionAchievement && mZombieMissionAchievement)
	{
		mZombieMissionAchievement->Input(in_input);
	}

	if (mMissionVarMission00)
	{
		mMissionVarMission00->Input(in_input, *this);
	}

	if (mZombieReloadDetector)
	{
		mZombieReloadDetector->Input(in_input);

		if (mZombieReloadDetector->GetTriggerReload())
		{
			Reload();
		}
	}

	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->Input(in_input);

#ifdef NO_FIRE_WITH_DEBUG_CAMERA
		if (ZombieCameraMixer::GetCameraTypeDebug() != mZombieCameraMixer->GetCameraTypeTarget())
#endif
		{
			const int touchCount = in_input.GetTouchCount();
			for (int index = 0; index < touchCount; ++index)
			{
				const GInputTouch& touch = in_input.GetTouch(index);
				if (touch.GetTouchBegin())
				{
					Fire(touch.GetTouchLocation());
				}
			}
		}
	}

	return;
}
int sDebugSize = 0;
const bool ZombieMission::Tick(const float in_timeDelta)
{
	if (mZombieGui)
	{
		mZombieGui->Tick(in_timeDelta);
	}

	if (mZombieReloadDetector)
	{
		mZombieReloadDetector->Tick(in_timeDelta);
	}

	if (mRunningMissionAchievement && mZombieMissionAchievement && mZombieGameVar)
	{
		if (mZombieMissionAchievement->Tick(in_timeDelta, *mZombieGameVar))
		{
			return true;
		}
		mRunningMissionAchievement = false;

		if (mRequestNextLevel)
		{
			mRequestNextLevel = false;
			ProgressNextLevel();
		}
	}

	if (mStopAllSequences)
	{
		mStopAllSequences = false;
		mMissionMaster->StartMission(""); //stop everything
	}

	if (0.0F < mDeathCountdown)
	{
		mDeathCountdown -= in_timeDelta;
		if (mDeathCountdown <= 0.0F)
		{
			mDeathCountdown = 0.0F;
			mRequestMainMenu = true;
		}
	}

	if (mRequestMainMenu)
	{
		mRequestMainMenu = false;
		//RunMainMenu(*mParentState.GetWindow());
		mParentState.RunMainMenu();
	}

	//tick mission var
	if (mMissionVar && mMissionMaster)
	{
		mMissionVar->Tick(
			*mMissionMaster, //trigger sequences
			mListClientOpponent,
			mListClientActor
			);
	}

	if (mMissionVarMission00)
	{
		mMissionVarMission00->Tick(in_timeDelta, *this);
	}

	if (mRequestNextLevel)
	{
		if (mZombieMissionAchievement->CalculateAcheivementLevelEnd(*mZombieGameVar, mListClientActor.size(), false))
		{
			mRunningMissionAchievement = true;
			return true;
		}
		
		mRequestNextLevel = false;
		ProgressNextLevel();
	}

	if (mMissionMaster)
	{
		mMissionMaster->Tick(in_timeDelta);
	}

	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->Tick(in_timeDelta);

		//HACK: for now, treat the camera as the player position... based on camera type?
		if (mZombieGameVar)
		{
			ZombieGameVar& gameVar = *mZombieGameVar;
			GVector3Float position = mZombieCameraMixer->GetCamera().GetTransformPos();
			position.m_y = (0.0F);
			gameVar.SetPlayerPosition(position);

			GVector3Float at = mZombieCameraMixer->GetCamera().GetTransformAt();
			at.m_y = (0.0F);
			Normalise(at);
			gameVar.SetPlayerDirection(at);
			//gameVar.SetPlayerUp(GVector3Float::sUnitY);
		}
	}

	if (mScene)
	{
		GScene& scene = *mScene;

		float localTick = GMath::Minimum(0.1F, in_timeDelta); //0.0166667F;
		if (mPause)
		{
			localTick = 0.0F;
		}
		GSceneVisitorFustrum::Run(scene, mZombieCameraMixer->GetCamera());
		GSceneVisitorAllTick<GSceneNodeComponentPhysics>::Run(scene, localTick);
		GSceneVisitorAllTick<GSceneNodeComponentClient>::Run(scene, localTick);
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(scene, localTick);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(scene);
		GSceneVisitorAllTick<GSceneNodeComponentParticle>::Run(scene, localTick);
		GSceneVisitorAllCamera<GSceneNodeComponentParticle>::Run(scene, mZombieCameraMixer->GetCamera());
		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(scene, localTick);
		GSceneVisitorAllTick<GSceneNodeComponentFade>::Run(scene, localTick);

		for (TArrayPairVectorMatrix::iterator iterator = mArraySlugsToSpawn.begin(); iterator != mArraySlugsToSpawn.end(); ++iterator)
		{
			SpawnSlugImplement(
				(*iterator).second,
				(*iterator).first
				);
		}
		mArraySlugsToSpawn.clear();

		scene.SceneNodeProcessDeleteQueue();
	}
	
	if (mZombieSound)
	{
		mZombieSound->SetPosition(
			mZombieCameraMixer->GetCamera().GetTransformPos(),
			mZombieCameraMixer->GetCamera().GetTransformAt(),
			mZombieCameraMixer->GetCamera().GetTransformUp(),
			GVector3Float::sZero
			);
	}

	if (mZombieGui)
	{
		//update zombie client target
		TArrayVector2& arrayOffscreen = mZombieGameVar->GetArrayOffscreen();
		arrayOffscreen.clear();
		GCamera& camera = GetCamera();
		for (TListClientTarget::iterator iterator = mListClientTaraget.begin(); iterator != mListClientTaraget.end(); ++iterator)
		{
			const GVector2Float screenPos = (*iterator).GetScreenPos(camera);
			if ((GMath::Abs(screenPos.m_x) < 1.0F) &&
				(GMath::Abs(screenPos.m_y) < 1.0F))
			{
				continue;
			}

			arrayOffscreen.push_back(screenPos);
		}

		mZombieGui->Tick(in_timeDelta);
	}

	return true;
}

void ZombieMission::Draw(GRender& inout_render)
{
	GCamera& camera = GetCamera();

	if (TMissionList::TMissionIntro == mMissionId)
	{
		inout_render.RenderStart(true, GColour4Float(0.0F, 0.0F, 0.0F, 0.0F), true, 1.0F);
		if (mZombieGui)
		{
			mZombieGui->DrawSplash(inout_render);
		}
	}
	else
	{
		inout_render.RenderStart();
		GSceneVisitorRender::Run(
			mSceneNodeRootSkyBox, 
			inout_render, 
			camera
			);
	}

	if (mScene)
	{
		GSceneVisitorRender::Run(
			*mScene, 
			inout_render, 
			camera
			);
#ifdef DSC_DEBUG
		mScene->DebugDraw(
			inout_render,
			camera		
			);
		mScene->GetArrayDebugMatrix().clear();
#endif
	}

	if (mZombieGui)
	{
		mZombieGui->Draw(inout_render);
	}

	return;
}

void ZombieMission::RequestNodeDestroy(TPointerSceneNode& inout_sceneNode)
{
	if (mScene)
	{
		mScene->SceneNodeRequestDelete(inout_sceneNode);
	}
	return;
}

//private methods
void ZombieMission::Fire(const GVector2Float& in_screenPos)
{
	if ((mMissionId == TMissionList::TMissionIntro) || (0.0F != mDeathCountdown))
	{
		return;
	}

	//are we allowed to shoot
#ifndef INFINITE_AMMO
	if (mZombieGameVar)
	{
		ZombieGameVar& gameVar = *mZombieGameVar;
		const int currentAmmo = gameVar.GetAmmoCountCurrent();
		if (currentAmmo <= 0)
		{
			if (mZombieGui)
			{
				mZombieGui->RequestReloadPrompt();
			}

			if (mZombieSound && (0.0F == mDeathCountdown))
			{	
				//say reload
				mZombieSound->Play(ZombieSound::GetSoundIDPlayerReload(
					ZombieStateType::GetPlayerSelf(gameVar.GetGameDifficulty())),
					mZombieSound->GetPosition()
					);
			}

			if (mMissionVarMission00)
			{
				mMissionVarMission00->IncrementRequestReload(*this);
			}

			gameVar.SetReloadRequestCount(gameVar.GetReloadRequestCount() + 1);

			return;
		}

		gameVar.SetAmmoCountCurrent(currentAmmo - 1);
	}
#endif

	if (mZombieSound)
	{
		mZombieSound->PlaySoundGunShot();
	}

	if (mMissionVarMission00)
	{
		mMissionVarMission00->Fire(*this);
	}

	//calculate damage cone
	GVector3Float worldSpacePointNearPlane;
	GVector3Float worldSpacePointFarPlane;

	GCamera& camera = GetCamera();

	camera.ReverseProjectPoint(
		worldSpacePointNearPlane,
		worldSpacePointFarPlane,
		in_screenPos
		);

	const GVector3Float& shotOrigin = camera.GetTransformPos();
	const GVector3Float shotDirection = Normalise(worldSpacePointFarPlane - worldSpacePointNearPlane);

	const float range = mZombieGameVar->GetShotRange();
	const float radiusOne = 0.0F;
	const float radiusTwo = mZombieGameVar->GetShotRadius() * ZombieCameraMixer::GetPerspectiveRatio() * range;

	//collect what we hit
	TArrayPairClientObjectCollisionResult arrayClientObjectCollisionResult;
	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		(*iterator).CollectDamageReceivers(
			arrayClientObjectCollisionResult,
			shotOrigin,
			shotDirection,
			range,
			radiusOne,
			radiusTwo
			);
	}
	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		(*iterator).CollectDamageReceivers(
			arrayClientObjectCollisionResult,
			shotOrigin,
			shotDirection,
			range,
			radiusOne,
			radiusTwo
			);
	}
	for (TListClientObjectBase::iterator iterator = mListClientObject.begin(); iterator != mListClientObject.end(); ++iterator)
	{
		(*iterator).CollectDamageReceivers(
			arrayClientObjectCollisionResult,
			shotOrigin,
			shotDirection,
			range,
			radiusOne,
			radiusTwo
			);
	}

	if (mZombieGameVar)
	{
		ZombieGameVar& gameVar = *mZombieGameVar;
		if (arrayClientObjectCollisionResult.size()) //hit something
		{
			gameVar.SetHitCount(gameVar.GetHitCount() + 1);
		}
		else //missed
		{
			gameVar.SetMissCount(gameVar.GetMissCount() + 1);
		}
	}

	//apply damage to hit first object
	//float closest = 0.0F;
	//SceneNodeComponentClientObjectBase* firstHit = 0;
	//std::string boneData;
	float damage = -(mZombieGameVar->GetShotDamage());

	//sort hit
	typedef std::pair<std::string, SceneNodeComponentClientObjectBase*> TPairStringClientObjectBase;
	typedef std::vector<TPairStringClientObjectBase> TArrayPairStringClientObjectBase;
	typedef std::map<float, TArrayPairStringClientObjectBase> TMapFloatArrayPairStringClientObjectBase;
	TMapFloatArrayPairStringClientObjectBase mapDistanceCollision;

	for (TArrayPairClientObjectCollisionResult::iterator iterator = arrayClientObjectCollisionResult.begin(); iterator != arrayClientObjectCollisionResult.end(); ++iterator)
	{
		GSceneNodeComponentCollisionResult& collisionResult = *((*iterator).second);
		const float distance = collisionResult.GetHitRatio();
		TPointerSceneNodeComponentCollision pointerCollision = collisionResult.GetWeakComponentCollision().lock();
		const std::string& boneData = pointerCollision->GetData();

		TMapFloatArrayPairStringClientObjectBase::iterator found = mapDistanceCollision.find(distance);
		if (found == mapDistanceCollision.end())
		{
			TArrayPairStringClientObjectBase arrayString;
			arrayString.push_back(TArrayPairStringClientObjectBase::value_type(boneData, (*iterator).first));
			mapDistanceCollision.insert(TMapFloatArrayPairStringClientObjectBase::value_type(distance, arrayString));
		}
		else
		{
			(*found).second.push_back(TArrayPairStringClientObjectBase::value_type(boneData, (*iterator).first));
		}
	}

	typedef std::map<SceneNodeComponentClientObjectBase*, int> TMapClientObjectBaseInt;
	TMapClientObjectBaseInt mapHitClient;

	{
		bool first = true;
		TMapFloatArrayPairStringClientObjectBase::iterator iterator = mapDistanceCollision.begin();
		while ((damage < 0.0F) && (iterator != mapDistanceCollision.end()))
		{
			TArrayPairStringClientObjectBase& arrayData = (*iterator).second;
			TArrayPairStringClientObjectBase::iterator subIterator = arrayData.begin();
			while ((damage < 0.0F) && (subIterator != arrayData.end()))
			{
				(*subIterator).second->ApplyDamage(
					damage,
					(*subIterator).first
					);
				mapHitClient[(*subIterator).second] = 0;

				if (first)
				{
					first = false;
					TPointerSceneNode particleParent = mSceneNodeParticleParent.lock();
					TPointerSceneNodeComponentParticle pointerParticle = mSceneNodeComponentParticle.lock();
					if (pointerParticle && particleParent && GetGore())
					{
						GMatrix16Float transform(GMatrix16Float::sIdentity);
						transform.SetPosition(ProjectRay(
							shotOrigin,
							shotDirection,
							(*iterator).first
							));

						particleParent->SetParentRelativeTransform(
							transform
							);
						pointerParticle->RequestEmittor(
							2, 
							0.5F
							);
					}
				}

				++subIterator;
			}

			++iterator;
		}
	}

	TPointerSceneNode sceneNodeRoot = mSceneNodeRootActors.lock();
	GScene& scene = *mScene;
	for (TMapClientObjectBaseInt::iterator iterator = mapHitClient.begin(); iterator != mapHitClient.end(); ++iterator)
	{
		(*iterator).first->AfterDamagePass(scene, sceneNodeRoot);
	}

	return;
}

//menu triggers
void ZombieMission::NewGame(const ZombieStateType::TGameDifficulty::TEnum in_type)
{
	if (mZombieGameVar)
	{
		ZombieGameVar& gameVar = *mZombieGameVar;
		gameVar.ResetGameVar();
		gameVar.SetGameDifficulty(in_type);
		gameVar.SetUpBossHealth();
		mDeathCountdown = 0.0F;
		mRequestMainMenu = false;
		mRequestNextLevel = false;
		mStopAllSequences = false;

		//no, a game counts as played on death or beat boss
		//gameVar.SetPersistanceNewGameCount(gameVar.GetPersistanceNewGameCount() + 1);
	}
	
	mZombieSound->SetMasterVolume(GetVolume());

	LoadMission(TMissionList::TMissionScene00, false);

	//if (mZombieFade)
	//{
	//	mZombieFade->RequestFadeIn(1.0F, *mParentState.GetWindow());
	//}

	return;
}

//mission client triggers
void ZombieMission::RunMainMenu()
{
	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->UnlockDeathCamera();
	}

	mMissionVar.reset();
	mMissionVarMission00.reset();
	
	LoadMission(TMissionList::TMissionIntro, false);

	if (mZombieGui)
	{
		mZombieGui->OnSetCameraType(ZombieCameraMixer::GetCameraTypeIntro());
	}
	//if (mZombieFade)
	//{
	//	//mZombieFade->RequestFadeIn(1.0F, *mParentState.GetWindow());
	//	mZombieFade->Clear(*mParentState.GetWindow());
	//}

	return;
}

void ZombieMission::RestoreGame()
{
	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->UnlockDeathCamera();
	}

	mMissionVar.reset();
	mMissionVarMission00.reset();

	if (mZombieGameVar)
	{
		ZombieGameVar& gameVar = *mZombieGameVar;
		gameVar.RestoreGame();
		gameVar.SetUpBossHealth();
		mDeathCountdown = 0.0F;
		mRequestMainMenu = false;
		mRequestNextLevel = false;
		mStopAllSequences = false;

		const int missionId = GMath::Clamp(mZombieGameVar->GetMissionId(), TMissionList::TMissionScene00, TMissionList::TMissionScene11);

		LoadMission(missionId, true);
	}

	mZombieSound->SetMasterVolume(GetVolume());

	//if (mZombieFade)
	//{
	//	mZombieFade->Clear(*mParentState.GetWindow());
	//}
}

void ZombieMission::OptionDefault()
{
	if (!mOptionDictionary)
	{
		return;
	}

	mOptionDictionary.reset();
	mOptionDictionary.reset(new GDictionary);

	//set the defaults
	mOptionDictionary->SetValueInt(sAntiAliasKey, ZombieStateType::TSuperSampleType::T_1_5);
	mOptionDictionary->SetValueBool(sGoreKey, true);
	mOptionDictionary->SetValueFloat(sVolumeEffectsKey, 1.0F);

	return;
}

void ZombieMission::OptionLoad()
{
	if (!mOptionDictionary)
	{
		return;
	}

	//set the defaults
	OptionDefault();

	GBuffer buffer;
	const std::string filePath = mParentState.GetFileSystem()->AppendFileToPathName(mParentState.GetFileSystem()->GetWritableDir(), sApplicationPreferenceFileName);
	if (!mParentState.GetFileSystem()->LoadFile(buffer, filePath, false))
	{
		//no preferences yet?
		return;
	}

	GDictionary tempDictionary;
	GDictionary::LoadFromBuffer(tempDictionary, buffer);

	//don't union, intersect to cull garbage data...
	GDictionary::SelfUpdateIntersection(*mOptionDictionary, tempDictionary);

	return;
}

void ZombieMission::OptionSave()
{
	if ((!mOptionDictionary))
	{
		return;
	}
	GBuffer buffer;
	GDictionary::WriteToBuffer(buffer, *mOptionDictionary);

	const std::string filePath = mParentState.GetFileSystem()->AppendFileToPathName(mParentState.GetFileSystem()->GetWritableDir(), sApplicationPreferenceFileName);
	mParentState.GetFileSystem()->SaveFile(buffer, filePath);

	return;
}


void ZombieMission::RunDebugLevel(const int in_index)
{
#ifdef DEBUG_LEVELS
	if (mZombieCameraMixer)
	{
		mZombieCameraMixer->UnlockDeathCamera();
	}

	LoadMission(TMissionList::TMissionSceneDebug00 + in_index);
	if (mZombieFade)
	{
		mZombieFade->Clear(*mParentState.GetWindow());
	}
#endif
	return;
}

void ZombieMission::ClearScene()
{
	if (!mScene)
	{
		return;
	}

	{
		TPointerSceneNode sceneNode = mSceneNodeRootScene.lock();
		mScene->SceneNodeRequestDelete(sceneNode);
	}
	mScene->SceneNodeProcessDeleteQueue();

	TPointerSceneNode newSceneNode = mScene->CreateSceneNode("zombieSceneRoot");
	mScene->AddSceneRoot(newSceneNode);
	mSceneNodeRootScene = newSceneNode;


	TPointerSceneNode newSceneNodeParticleParent;
	newSceneNodeParticleParent.reset(new GSceneNode("zombieParticleParent"));
	newSceneNode->AddChildNode(newSceneNodeParticleParent);
	mSceneNodeParticleParent = newSceneNodeParticleParent;

	TPointerSceneNodeComponentParticle pointerParticle;
	TPointerSceneNodeComponentBase pointerBase;
	pointerBase = GSceneNodeComponentParticle::FactoryManual(
		*mScene,
		newSceneNodeParticleParent,
		"material_particle00a.data",
		1000.0F,
		100,
		100,
		10
		);
	newSceneNodeParticleParent->AddComponent(pointerBase);
	pointerParticle = boost::dynamic_pointer_cast<GSceneNodeComponentParticle, GSceneNodeComponentBase>(pointerBase);
	mSceneNodeComponentParticle = pointerParticle;

	return;
}

void ZombieMission::LoadScene(const std::string& in_sceneName)
{
	//TODO: this is currently broken, two levels of same name, second will be blank, something else clearing scene (LoadMission() clears scene)
	//if (mSceneName == in_sceneName)
	//{
	//	return;
	//}

	mSceneName = in_sceneName;

	ClearScene();

	if (in_sceneName.empty())
	{
		return;
	}

	TPointerSceneNode sceneRoot = mSceneNodeRootScene.lock();
	if (mScene && sceneRoot)
	{
		mScene->LoadSceneUnderNode(
			in_sceneName,
			sceneRoot
			);
	}

	return;
}

void ZombieMission::ClearActors()
{
	if (!mScene)
	{
		return;
	}

	{
		TPointerSceneNode sceneNode = mSceneNodeRootActors.lock();
		mScene->SceneNodeRequestDelete(sceneNode);
	}
	mScene->SceneNodeProcessDeleteQueue();

	TPointerSceneNode newSceneNode = mScene->CreateSceneNode("zombieActorRoot");
	mScene->AddSceneRoot(newSceneNode);
	mSceneNodeRootActors = newSceneNode;

	return;
}

void ZombieMission::SetActorPosition(
	const std::string& in_actorName,
	const GVector3Float& in_position,
	const float in_transitionTime,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Move(in_position, in_transitionTime, in_transitionType);
			return;
		}
	}
	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Move(in_position, in_transitionTime, in_transitionType);
			return;
		}
	}
	return;
}

void ZombieMission::SetActorRotation(
	const std::string& in_actorName,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Rotate(in_at, in_up, in_transitionTime, in_transitionType);
			return;
		}
	}
	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Rotate(in_at, in_up, in_transitionTime, in_transitionType);
			return;
		}
	}
	return;
}

void ZombieMission::AnimateActor(
	const std::string& in_actorName,
	const std::string& in_animationName,
	const GAnimationPlayParam& in_playParam
	)
{
	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Animate(in_animationName, in_playParam);
			return;
		}
	}
	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Animate(in_animationName, in_playParam);
			return;
		}
	}
	return;
}

void ZombieMission::SetOpponentAiActive(const std::string& in_actorName, const bool in_active)
{
	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.SetAiActive(in_active);
			return;
		}
	}

	return;
}

void ZombieMission::InvokeFactory(
	const std::string& in_actorName,
	const std::string& in_factoryStubName,
	const std::string& in_factoryData
	)
{
	if (!mScene)
	{
		return;
	}

	TPointerSceneNode parentActor = mSceneNodeRootActors.lock();
	if (!parentActor)
	{
		return;
	}
	
	TPointerSceneNode pointerActorNode;
	pointerActorNode.reset(new GSceneNode(in_actorName));
	parentActor->AddChildNode(pointerActorNode);

	pointerActorNode->SetParentRelativeTransform(GMatrix16FloatConstructAtUp(
		mFactoryAt, 
		mFactoryUp,
		GVector3Float::sUnitZ, 
		GVector3Float::sUnitY, 
		mFactoryPosition
		));

	TArrayScenePostLoadCallback arrayPostLoad;
	mScene->SceneNodeFactory(
		in_factoryStubName,
		in_factoryData,
		pointerActorNode,
		arrayPostLoad
		);
	GScene::FactoryPostLoad(arrayPostLoad,*mScene);

	return;
}

void ZombieMission::SetCameraType(
	const int in_type,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	mZombieCameraMixer->SetCameraType(
		in_type,
		in_duration,
		in_transitionType
		);

	mZombieGui->OnSetCameraType(in_type);

	return;
}

void ZombieMission::SetCameraPosition(
	const int in_type,
	const GVector3Float& in_position,
	const float in_transitionTime,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	mZombieCameraMixer->SetPosition(
		in_type,
		in_position,
		in_transitionTime,
		in_transitionType
		);

	return;
}

void ZombieMission::SetCameraRotation(
	const int in_type,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	mZombieCameraMixer->SetRotation(
		in_type,
		in_at,
		in_up,
		in_transitionTime,
		in_transitionType
		);

	return;
}

void ZombieMission::SetMissionVarConditionSequence(
	const ZombieMissionVar::TCondition::TEnum in_condition, 
	const std::string& in_data,
	const std::string& in_name
	)
{
	if (mMissionVar)
	{
		mMissionVar->SetConditionSequence(in_condition, in_data, in_name);
	}

	return;
}

void ZombieMission::SetMissionVariable(
	const ZombieMissionVar::TVariable::TEnum in_variable, 
	const bool in_value
	)
{
	if (mMissionVar)
	{
		mMissionVar->SetVariable(in_variable, in_value);
	}

	return;
}


static const char* const sBossWaveEasyMissionNames[] = {
	"sequenceWave0",
	"sequenceWave1"
};

static const char* const sBossWaveNormalMissionNames[] = {
	"sequenceWave2",
	"sequenceWave3"
};

static const char* const sBossWaveHardMissionNames[] = {
	"sequenceWave4",
	"sequenceWave5"
};

void ZombieMission::TriggerBossWave(const int in_index)
{
	if (mZombieGui)
	{
		mZombieGui->ResetBossHealthMaggot();
	}

	if (!mZombieGameVar || !mMissionMaster)
	{
		return;
	}

	ZombieGameVar& gameVar = *mZombieGameVar;
	gameVar.SetBossHealthAfterWave(in_index);

	const ZombieStateType::TGameDifficulty::TEnum difficulty = gameVar.GetGameDifficulty();
	if ((0 <= in_index) && (in_index < 2))
	{
		switch (difficulty)
		{
		default:
		case ZombieStateType::TGameDifficulty::TNormal:
			mMissionMaster->TriggerSequence(sBossWaveNormalMissionNames[in_index]);
			break;
		case ZombieStateType::TGameDifficulty::TEasy:
			mMissionMaster->TriggerSequence(sBossWaveEasyMissionNames[in_index]);
			break;
		case ZombieStateType::TGameDifficulty::THard:
			mMissionMaster->TriggerSequence(sBossWaveHardMissionNames[in_index]);
			break;
		}
	}

	return;
}


void ZombieMission::ActorCinematicDialog(const std::string& in_actorName, const std::string& in_textKey)
{
	bool actorFound = false;

	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			actorFound = true;
			break;
		}
	}

	if (!actorFound)
	{
		for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
		{
			//SceneNodeComponentClientObjectBase& objectBase = (*iterator);
			actorFound = true;
			break;
		}
	}

	if (!actorFound)
	{
		return;
	}

	if (mZombieGui)
	{
		mZombieGui->RequestCinematicText(in_textKey, sCinematicHoldTextTime);
	}

	return;
}

void ZombieMission::TakeDamage(const int in_damage, const int in_type)
{
	if (mZombieGameVar && (0.0F == mDeathCountdown))
	{
		ZombieGameVar& gameVar = *mZombieGameVar;
		const bool invunerable = (ZombieStateType::TAchieviementState::TUnlockedActive == gameVar.GetUnlockState(ZombieStateType::TAchieviement::TMissionPossible));
		if (!invunerable)
		{
#ifndef INFINITE_HEALTH
			const int currentHealth = gameVar.GetLifeCountCurrent();
			if (currentHealth <= 0)
			{
				TriggerDeath();
				if (mZombieSound)
				{	
					mZombieSound->Play(
						ZombieSound::GetSoundIDPlayerDead(ZombieStateType::GetPlayerSelf(mZombieGameVar->GetGameDifficulty())),
						mZombieSound->GetPosition()
						);
				}

				return;
			}
			else
			{
				if (mZombieSound)
				{	
					mZombieSound->Play(
						ZombieSound::GetSoundIDPlayerHurt(ZombieStateType::GetPlayerSelf(mZombieGameVar->GetGameDifficulty())),
						mZombieSound->GetPosition()
						);
				}
			}

			gameVar.SetLifeCountCurrent(currentHealth - in_damage);
#endif
			gameVar.SetDamageTaken(gameVar.GetDamageTaken() + in_damage);
		}
	}
	if (mZombieGui && GetGore())
	{
		mZombieGui->RequestDamageType(in_type);
	}

	return;
}

void ZombieMission::BossHealthBarIncrement()
{
	if (mZombieGui)
	{
		mZombieGui->RequestBossHealthMaggot();
	}

	return;
}

void ZombieMission::TriggerDeath()
{
	//TODO: death sequence, fade out, camera
	//we don't change control since this is called from within a scene query
	//RunMainMenu(*mParentState.GetWindow());
	//mRequestMainMenu = true;
	if (mDeathCountdown)
	{
		return;
	}
	mStopAllSequences = true;
	//mZombieCameraMixer->SetCameraType(ZombieCameraMixer::GetCameraTypeDeath());
	SetCameraType(ZombieCameraMixer::GetCameraTypeDeath(), 0.0F, GComponentLerp::TType::TLinear);
	mDeathCountdown = 5.0F;

	//if (mZombieFade)
	//{
	//	mZombieFade->RequestFadeOut(mDeathCountdown, *mParentState.GetWindow());
	//}

	mRunningMissionAchievement = mZombieMissionAchievement->CalculateAcheivementLevelEnd(*mZombieGameVar, mListClientActor.size(), true);

	return;
}

void ZombieMission::RequestNextMission()
{
	mRequestNextLevel = true;
	return;
}

void ZombieMission::ActorDestroy(
	const std::string& in_actorName
	)
{
	for (TListClientObjectBase::iterator iterator = mListClientActor.begin(); iterator != mListClientActor.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Destroy();
			return;
		}
	}

	for (TListClientObjectBase::iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (in_actorName == objectBase.GetName())
		{
			objectBase.Destroy();
			return;
		}
	}

	return;
}

//public accessors
//no remove, list self removes
void ZombieMission::AddClientOpponent(SceneNodeComponentClientObjectBase& inout_clientObjectBase)
{
	mListClientOpponent.push_back(inout_clientObjectBase);
	return;
}

void ZombieMission::AddClientActor(SceneNodeComponentClientObjectBase& inout_clientObjectBase)
{
	mListClientActor.push_back(inout_clientObjectBase);
	return;
}

void ZombieMission::AddClientObject(SceneNodeComponentClientObjectBase& inout_clientObjectBase)
{
	mListClientObject.push_back(inout_clientObjectBase);
	return;
}

void ZombieMission::AddClientTarget(SceneNodeComponentClientTarget& inout_clientTarget)
{
	mListClientTaraget.push_back(inout_clientTarget);
	return;
}

void ZombieMission::SpawnSlug(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_position,
	const GVector3Float& in_velocity
	)
{
	mArraySlugsToSpawn.push_back(TPairVectorMatrix(
		in_velocity,
		GMatrix16FloatConstructAtUp(
			in_at, 
			in_up,
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			in_position
		)));
	return;
}

ZombieMission::TPointerSceneNode ZombieMission::ClientFactory(
	const GMatrix16Float& in_transform,
	const std::string& in_factoryClientName,
	const std::string& in_factoryData,
	const std::string& in_nodeName
	)
{
	TPointerSceneNode pointerActorNode;
	if (!mScene)
	{
		return pointerActorNode;
	}

	TPointerSceneNode parentActor = mSceneNodeRootActors.lock();
	if (!parentActor)
	{
		return pointerActorNode;
	}
	
	pointerActorNode.reset(new GSceneNode(in_nodeName));
	parentActor->AddChildNode(pointerActorNode);

	pointerActorNode->SetParentRelativeTransform(in_transform);

	TArrayScenePostLoadCallback arrayPostLoad;
	mScene->SceneNodeFactory(
		in_factoryClientName,
		in_factoryData,
		pointerActorNode,
		arrayPostLoad
		);
	GScene::FactoryPostLoad(arrayPostLoad, *mScene);

	return pointerActorNode;
}

//private methods
//perform a reload
void ZombieMission::SpawnSlugImplement(
	const GMatrix16Float& in_transform,
	const GVector3Float& in_velocity
	)
{
	TPointerSceneNode pointerActorNode = ClientFactory(
		in_transform,
		"FactorySlug",
		"",
		"spawnedSlug"
		);

	if (!pointerActorNode)
	{
		return;
	}

	//init the slug (find the SceneNodeComponentClientSlug)
	GSceneNode& parentSceneNode = *pointerActorNode;
	const int componentCount = parentSceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		SceneNodeComponentClientSlug* const pointerClientSlug = parentSceneNode.GetComponent<SceneNodeComponentClientSlug>(index);
		if (!pointerClientSlug)
		{
			continue;
		}

		pointerClientSlug->SetFlightMode(in_velocity);
		break;
	}

	return;
}

void ZombieMission::Reload()
{
	if (TMissionList::TMissionIntro == mMissionId)
	{
		return;
	}

	if (mZombieGameVar)
	{
		const int maxAmmo = mZombieGameVar->GetAmmoCountMaximum();
		mZombieGameVar->SetAmmoCountCurrent(maxAmmo);
	}

	if (mZombieGui)
	{
		mZombieGui->HideReloadPrompt();
	}

	if (mMissionVarMission00)
	{
		mMissionVarMission00->Reload(*this);
	}


	if (mZombieSound && (0.0F == mDeathCountdown))
	{	
		//say reload
		mZombieSound->Play(
			ZombieSound::GetSoundIDReload(),
			mZombieSound->GetPosition()			
			);
	}

	return;
}

const float ZombieMission::GetVolume()
{
	if (mOptionDictionary)
	{
		return mOptionDictionary->GetValueFloat(sVolumeEffectsKey, 1.0F);
	}
	return 1.0F;
}

const bool ZombieMission::GetGore()
{
	if (mOptionDictionary)
	{
		return mOptionDictionary->GetValueBool(sGoreKey);
	}
	return false;
}

GCamera& ZombieMission::GetCamera()
{
	return mZombieCameraMixer->GetCamera();
}

void ZombieMission::LoadMission(const int in_missionId, const bool in_restore)
{
	//clear state
	mMissionVar.reset(new ZombieMissionVar);
	if ((ZombieStateType::TGameDifficulty::TEasy == mZombieGameVar->GetGameDifficulty()) &&
		(TMissionList::TMissionScene00 == in_missionId))
	{
		mMissionVarMission00.reset(new ZombieMissionVarMission00(*mParentState.GetWindow()));
	}
	else
	{
		mMissionVarMission00.reset();
	}
	mPause = false;

	mRequestNextLevel = false;

	ClearScene();
	ClearActors();

	mMissionId = in_missionId;
	if (!mMissionMaster)
	{
		return;
	}

	if (mZombieGameVar)
	{
		mZombieGameVar->SetMissionId(mMissionId, in_restore);
	}

	mMissionMaster->StartMission(sArrayMissionName[in_missionId]);
	if (mMissionVarMission00)
	{
		mMissionMaster->TriggerSequence("missionStartEasy");
	}
#ifdef TRIAL
	else if (TMissionList::TMissionScene02 == in_missionId)
	{		
		mMissionMaster->TriggerSequence("missionStartTrial");
	}
#endif
	else
	{
		mMissionMaster->TriggerSequence(GMissionMaster::GetMissionStartSequenceName());
	}
	
	//well, dont want to do it after every kill
	mZombieGameVar->SavePersistantData();

	return;
}

void ZombieMission::ProgressNextLevel()
{
#ifdef WIN32
	//win32 has no option dialog
	switch (mMissionId)
	{
	default:
			break;
	case TMissionList::TMissionScene02:
	case TMissionList::TMissionScene03:
	case TMissionList::TMissionScene04:
	case TMissionList::TMissionScene05:
	case TMissionList::TMissionScene06:
	case TMissionList::TMissionScene07:
	case TMissionList::TMissionScene08:
	case TMissionList::TMissionScene09:
	case TMissionList::TMissionScene10:
		LoadMission((mMissionId + 1) % TMissionList::TCount, false);
		return;
	}
#endif

	switch (mMissionId)
	{
	default:
		break;
	case TMissionList::TMissionScene00:
		LoadMission(TMissionList::TMissionScene01, false);
		break;
	case TMissionList::TMissionScene01:
		LoadMission(TMissionList::TMissionScene02, false);
		break;
	case TMissionList::TMissionScene02:
#ifdef TRIAL
		mStopAllSequences = true;
		mRequestMainMenu = true;
		break;
#else
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission02, false);
		break;
#endif
	case TMissionList::TMissionScene03:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission03, false);
		break;
	case TMissionList::TMissionScene04:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission04, false);
		break;
	case TMissionList::TMissionScene05:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission05, false);
		break;
	case TMissionList::TMissionScene06:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission06, false);
		break;
	case TMissionList::TMissionScene07:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission07, false);
		break;
	case TMissionList::TMissionScene08:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission08, false);
		break;
	case TMissionList::TMissionScene09:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission09, false);
		break;
	case TMissionList::TMissionScene10:
		//mParentState.GetWindow()->InvokeOptionDialog(mOptionDialogDataItemPathMission10, false);
		break;
	case TMissionList::TMissionScene11:
		mStopAllSequences = true;
		mDeathCountdown = 2.0F;

		//if (mZombieFade)
		//{
		//	mZombieFade->RequestFadeOut(mDeathCountdown, *mParentState.GetWindow());
		//}

		break;
	}
	return;
}

const int ZombieMission::GetCountOpponentCinematicControl()const
{
	int opponentUnderCinematicControl = 0;
	for (TListClientObjectBase::const_iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		const SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (!objectBase.GetAiActive())
		{
			opponentUnderCinematicControl += 1;
		}
	}
	return opponentUnderCinematicControl;
}

const int ZombieMission::GetCountOpponentAiControl()const
{
	int opponentUnderAiControl = 0;
	for (TListClientObjectBase::const_iterator iterator = mListClientOpponent.begin(); iterator != mListClientOpponent.end(); ++iterator)
	{
		const SceneNodeComponentClientObjectBase& objectBase = (*iterator);
		if (objectBase.GetAiActive())
		{
			opponentUnderAiControl += 1;
		}
	}
	return opponentUnderAiControl;
}

