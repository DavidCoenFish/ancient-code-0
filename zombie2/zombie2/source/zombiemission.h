//
//  ZombieMission.h
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieMission_h_
#define _ZombieMission_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive/list.hpp>
#include <vector>
#include <map>
#include "ZombieStateType.h"
#include "ZombieMissionVar.h"
#include "SceneNodeComponentClientTarget.h"
#include "SceneNodeComponentClientObjectBase.h"
#include <GVector3Float.h>
#include <GComponentLerp.h>
#include <GApplicationWindowType.h>

class ApplicationTaskGame;
class ZombieSound;
class ZombieFade;
class GState;
class GDictionary;
class GApplicationWindow;
class GRender;
class GVector2Float;
class GInput;
class GComponentCameraInput;
class GFileSystem;
class GRenderTextureManager;
class GRenderMeshManager;
class GVector3Float;
class GMatrix16Float;
class ZombieMissionWindowCallback;
class GApplicationViewComponent;
class GApplicationViewComponentImage;
class GRenderMaterial;
class GRenderMaterialManager;
class GMeshManual;
class GScene;
class GOptionDialogDataItem;
class GScratchPad;
class GSceneNodeFactory;
class SceneNodeComponentClientObjectBase;
class GCamera;
class GVector2Float;
class GSceneNode;
class ZombieGui;
class ZombieGameVar;
class ZombieReloadDetector;
class ZombieCamera;
class ZombieMissionClient;
class GMissionMaster;
class GAnimationPlayParam;
class ZombieCameraMixer;
class ZombieMissionVar;
class ZombieMissionVarMission00;
class GSceneNodeComponentParticle;
class ZombieMissionAchievement;
class GParticleManager;
class GDictionary;
class GGuiManager;
class GGui;

#ifdef DSC_DEBUG
#define DEBUG_PUT_TEXT(IN_TEXT) DebugPutText(IN_TEXT);
void DebugPutText(const std::string& in_text);
#else
#define DEBUG_PUT_TEXT(IN_TEXT)
#endif	

class ZombieMission : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GApplicationWindow> TPointerApplicationWindow;
	typedef boost::shared_ptr<GRender> TPointerComponentRender;

	typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;
	typedef boost::shared_ptr<GScene> TPointerScene;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;
	typedef boost::shared_ptr<GMissionMaster> TPointerMissionMaster;

	typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
	typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;

	typedef boost::shared_ptr<ZombieMissionVar> TPointerZombieMissionVar;
	typedef boost::shared_ptr<ZombieMissionVarMission00> TPointerZombieMissionVarMission00;

	typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
	typedef boost::weak_ptr<SceneNodeComponentClientObjectBase> TWeakSceneNodeComponentClientObjectBase;
	typedef std::vector<TWeakSceneNodeComponentClientObjectBase> TArrayWeakSceneNodeComponentClientObjectBase;
	typedef std::map<std::string, TWeakSceneNodeComponentClientObjectBase> TMapStringWeakSceneNodeComponentClientObjectBase;

	typedef boost::shared_ptr<ZombieGui> TPointerZombieGui;
	typedef boost::shared_ptr<ZombieGameVar> TPointerZombieGameVar;
	typedef boost::shared_ptr<ZombieReloadDetector> TPointerZombieReloadDetector;
	typedef boost::shared_ptr<ZombieCamera> TPointerZombieCamera;
	typedef boost::shared_ptr<ZombieMissionClient> TPointerZombieMissionClient;
	typedef boost::shared_ptr<ZombieCameraMixer> TPointerZombieCameraMixer;
	typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;
	typedef boost::shared_ptr<ZombieFade> TPointerZombieFade;
	typedef boost::shared_ptr<ZombieMissionAchievement> TPointerZombieMissionAchievement;

	typedef boost::shared_ptr<GOptionDialogDataItem> TPointerOptionDialogDataItem;

	typedef boost::intrusive::list< 
		SceneNodeComponentClientObjectBase, 
		boost::intrusive::member_hook<SceneNodeComponentClientObjectBase, SceneNodeComponentClientObjectBase::TListHook, &SceneNodeComponentClientObjectBase::mListHook>,
		boost::intrusive::constant_time_size<false>
		> TListClientObjectBase;

	typedef boost::intrusive::list< 
		SceneNodeComponentClientTarget, 
		boost::intrusive::member_hook<SceneNodeComponentClientTarget, SceneNodeComponentClientTarget::TListHook, &SceneNodeComponentClientTarget::mListHook>,
		boost::intrusive::constant_time_size<false>
		> TListClientTarget;

	typedef std::pair<GVector3Float, GMatrix16Float> TPairVectorMatrix;
	typedef std::vector<TPairVectorMatrix> TArrayPairVectorMatrix;

	//public static methods
public:
	static const int GetMissionIdIntro();
	static const int GetMissionFirst();
	static const int GetMissionIdSceneSlugs();
	static const int GetMissionIdSceneBats();
	static const int GetMissionIdSceneBossFight();
	static const bool GetMissionIsPlusShotPower(const int in_index);
	static const bool GetMissionIsPlusHealth(const int in_index);
	static const bool GetMissionIsHeal(const int in_index);
	static const char* const GetMissionName(const int in_index);

	//private static methods
private:
	static void OnOptionDialogDataItemCallbackPath(void* const in_callbackData, GOptionDialogDataItem& inout_dataItem);

	//constructor
public:
	ZombieMission(
		ApplicationTaskGame& in_parentState,
		TPointerApplicationWindow& inout_window
		);
	virtual ~ZombieMission();

	//public methods
public:
	void Resize(
		const int in_width
		, const int in_height
		, const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
		);
	void Shake();
	void Input(const GInput& in_input);
	const bool Tick(const float in_timeDelta);
	void Draw(GRender& inout_renderComponent);
	void Fire(const GVector2Float& in_screenPos);

	void NewGame(const ZombieStateType::TGameDifficulty::TEnum in_type);
	void RunMainMenu();

	void RestoreGame();

	void RunDebugLevel(const int in_index);

	GCamera& GetCamera(); //slug attach to camera wanted access

	//mission client triggers
	void ClearScene();
	void LoadScene(const std::string& in_sceneName);
	void ClearActors();
	void InvokeFactory(
		const std::string& in_actorName,
		const std::string& in_factoryStubName,
		const std::string& in_factoryData
		);
	void SetActorPosition(
		const std::string& in_actorName,
		const GVector3Float& in_position,
		const float in_transitionTime = 0.0F,
		const GComponentLerp::TType::TEnum in_transitionType = GComponentLerp::TType::TLinear
		);
	void SetActorRotation(
		const std::string& in_actorName,
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const float in_transitionTime = 0.0F,
		const GComponentLerp::TType::TEnum in_transitionType = GComponentLerp::TType::TLinear
		);
	void AnimateActor(
		const std::string& in_actorName,
		const std::string& in_animationName,
		const GAnimationPlayParam& in_playParam
		);
	void SetOpponentAiActive(const std::string& in_actorName, const bool in_active);

	void SetCameraType(
		const int in_type,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);
	void SetCameraPosition(
		const int in_type,
		const GVector3Float& in_position,
		const float in_transitionTime = 0.0F,
		const GComponentLerp::TType::TEnum in_transitionType = GComponentLerp::TType::TLinear
		);
	void SetCameraRotation(
		const int in_type,
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const float in_transitionTime = 0.0F,
		const GComponentLerp::TType::TEnum in_transitionType = GComponentLerp::TType::TLinear
		);

	void ActorCinematicDialog(const std::string& in_actorName, const std::string& in_textKey);

	void SetMissionVarConditionSequence(
		const ZombieMissionVar::TCondition::TEnum in_condition, 
		const std::string& in_data,
		const std::string& in_name = ""
		);

	void SetMissionVariable(
		const ZombieMissionVar::TVariable::TEnum in_variable, 
		const bool in_value
		);

	void TriggerBossWave(const int in_index);


	//player has taken damage
	void TakeDamage(const int in_damage, const int in_type);

	void BossHealthBarIncrement();
	void BossHealthBarClear();

	//player has died
	void TriggerDeath();
	void RequestNextMission();

	void ActorDestroy(
		const std::string& in_actorName
		);

	//public accessors
public:
	//const GVector3Float& GetFactoryPosition()const{ return mFactoryPosition; }
	void SetFactoryPosition(const GVector3Float& in_value){ mFactoryPosition = in_value; return; }

	//const GVector3Float& GetFactoryAt()const{ return mFactoryAt; }
	void SetFactoryAt(const GVector3Float& in_value){ mFactoryAt = in_value; return; }

	//const GVector3Float& GetFactoryUp()const{ return mFactoryUp; }
	void SetFactoryUp(const GVector3Float& in_value){ mFactoryUp = in_value; return; }

	//TPointerScene& GetScene(){ return mScene; }

	const TPointerZombieGameVar& GetGameVar()const{ return mZombieGameVar; }
	TPointerZombieGameVar& GetGameVar(){ return mZombieGameVar; }

	const TPointerZombieSound& GetSound()const{ return mZombieSound; }
	TPointerZombieSound& GetSound(){ return mZombieSound; }

	const TPointerZombieMissionAchievement& GetMissionAchievement()const{ return mZombieMissionAchievement; }
	TPointerZombieMissionAchievement& GetMissionAchievement(){ return mZombieMissionAchievement; }

	//no remove, list self removes
	void AddClientOpponent(SceneNodeComponentClientObjectBase& inout_clientObjectBase);
	void AddClientActor(SceneNodeComponentClientObjectBase& inout_clientObjectBase); //npc (rescue), actor in cinematic
	void AddClientObject(SceneNodeComponentClientObjectBase& inout_clientObjectBase); //things we can shoot

	void AddClientTarget(SceneNodeComponentClientTarget& inout_clientTarget); //client targets are a hit to user that a zombie is offscreen

	void SpawnSlug(
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const GVector3Float& in_position,
		const GVector3Float& in_velocity
		);

	TPointerSceneNode ClientFactory(
		const GMatrix16Float& in_transform,
		const std::string& in_factoryClientName,
		const std::string& in_factoryData,
		const std::string& in_nodeName
		);

	void RequestNodeDestroy(TPointerSceneNode& inout_sceneNode);

	const float GetVolume();
	const bool GetGore();

	void Reload();

	const bool GetPause()const{ return mPause; }
	void SetPause(const bool in_value){ mPause = in_value; return; }

	void OptionDefault();
	void OptionLoad();
	void OptionSave();	
	void ApplyOptionToMenuData();//TPointerOptionDialogDataItem& inout_dataItem);
	void ApplyUnlockToMenuData();//TPointerOptionDialogDataItem& inout_dataItem);

	void MakeMainMenu();
	void PutText(const std::string& in_text);

	//private methods
private:
	void SpawnSlugImplement(
		const GMatrix16Float& in_transform,
		const GVector3Float& in_velocity
		);

	void Fire(
		const GCamera& in_camera,
		const GVector2Float& in_screenPos 
		);

	void LoadMission(const int in_missionId, const bool in_restore);
	void ProgressNextLevel();

	const int GetCountOpponentCinematicControl()const;
	const int GetCountOpponentAiControl()const;

	//private members
private:
	ApplicationTaskGame& mParentState;

	boost::shared_ptr<GDictionary> mOptionDictionary;

	boost::shared_ptr<ZombieGui> mZombieGui;
	boost::shared_ptr<GParticleManager> mParticleManager;
	boost::shared_ptr<GSceneNodeFactory> mSceneNodeFactory;
	std::string mSceneName;
	boost::shared_ptr<GScene> mScene;
	boost::weak_ptr<GSceneNode> mSceneNodeRootScene;
	boost::weak_ptr<GSceneNode> mSceneNodeRootActors;
	boost::weak_ptr<GSceneNode> mSceneNodeParticleParent;
	boost::weak_ptr<GSceneNodeComponentParticle> mSceneNodeComponentParticle;
	boost::shared_ptr<GSceneNode> mSceneNodeRootSkyBox;
	boost::shared_ptr<GMissionMaster> mMissionMaster;
	int mMissionId;

	TListClientObjectBase mListClientOpponent; //things that move (receive animation) opponents
	TListClientObjectBase mListClientActor; //things that move (receive animation) players and npcs
	TListClientObjectBase mListClientObject; //things that can be shot
	TListClientTarget mListClientTaraget;

	boost::shared_ptr<ZombieGameVar> mZombieGameVar;
	boost::shared_ptr<ZombieReloadDetector> mZombieReloadDetector;
	boost::shared_ptr<ZombieMissionClient> mZombieMissionClient;
	boost::shared_ptr<ZombieCameraMixer> mZombieCameraMixer;
	boost::shared_ptr<ZombieSound> mZombieSound;
	boost::shared_ptr<ZombieMissionAchievement> mZombieMissionAchievement;

	GVector3Float mFactoryPosition;
	GVector3Float mFactoryAt;
	GVector3Float mFactoryUp;

	boost::shared_ptr<ZombieMissionVar> mMissionVar;
	boost::shared_ptr<ZombieMissionVarMission00> mMissionVarMission00;

	bool mPause; //don't tick animation or client?
	bool mRequestMainMenu;
	bool mRequestNextLevel;
	bool mStopAllSequences;
	bool mRunningMissionAchievement;
	float mDeathCountdown;

	TArrayPairVectorMatrix mArraySlugsToSpawn;

};

#endif //_ZombieMission_h_
