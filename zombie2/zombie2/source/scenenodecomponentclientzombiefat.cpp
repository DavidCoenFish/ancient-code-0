//
//  SceneNodeComponentClientZombieFat.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientZombieFat.h"

#include "ZombieComponent.h"
#include "ZombieComponentLoad.h"
#include "ZombiePartLoad.h"
#include "ZombiePartDataLoad.h"
#include "ZombieParticleLoad.h"
#include "ZombieMission.h"
#include "ZombiePart.h"
#include "ZombieGameVar.h"
#include "ZombiePartDetach.h"
#include "ZombieGuiDamage.h"
#include "ZombieMission.h"
#include "ZombieSound.h"

#include <GCommon.h>
#include <GMath.h>
#include <GScenePostLoadCallback.h>
#include <GScene.h>
#include <GSceneNode.h>
#include <GSceneNodeUtility.h>
#include <GSceneNodeComponentVisual.h>
#include <GSceneNodeComponentCollision.h>
#include <GAnimationManagerInstance.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentParticle.h>
#include <GComponentLerpVector3Float.h>
#include <GAnimationPlayParam.h>
#include <GSceneNodeComponentFade.h>
#include <GAssert.h>
#include <GSceneVisitorCollision.h>
#include <GSceneVisitorAll.h>

/**/
static const float sZombieFatHealthFactor = 1.5F;

//softbinding names
struct TZombieFatPart
{
	enum TEnum
	{
		TLeftBack,
		TLeftLeg,
		TLeftFoot,
		TLeftNeck,
		TLeftArm,
		TLeftHand,
		TLeftChest,
		TLeftGut,
		TRightBack,
		TRightLeg,
		TRightFoot,
		TRightNeck,
		TRightArm,
		TRightHand,
		TRightChest,
		TRightGut,
		THead,

		TCount
	};
};

static const char* const sZombieFatPartName[TZombieFatPart::TCount] = {
	"leftback",
	"leftleg",
	"leftfoot",
	"leftneck",
	"leftarm",
	"lefthand",
	"leftchest",
	"leftgut",
	"rightback",
	"rightleg",
	"rightfoot",
	"rightneck",
	"rightarm",
	"righthand",
	"rightchest",
	"rightgut",
	"head"
};
/**/

struct TZombieFatAnimation
{
	enum TEnum
	{
		TDeath = 0,
		TIdle,
		TWalk,
		TRecoil,
		TAttackLeft,
		TAttackRight,
		TAttackBite,
		TDragIdle,
		TDrag,
		TDragRecoil,
		TDragAttackLeft,
		TDragAttackRight,
		TDragAttackBite,
		TCount
	};
};
static const float sZombieFatAnimationRate[TZombieFatAnimation::TCount] = {
	1.0F, //TDeath = 0,
	0.5F, //TIdle,
	0.5F, //TWalk,
	1.0F, //TRecoil,
	0.75F, //TAttack_left,
	0.75F, //TAttack_right,
	0.75F, //TAttack_bite,
	0.5F, //TDrag_idle,
	1.0F, //TDrag,
	1.0F, //TDrag_recoil,
	0.75F, //TDrag_attack_left,
	0.75F, //TDrag_attack_right,
	0.75F  //TDrag_attack_bite,
};

static const char* const sZombieFatAnimationName[TZombieFatAnimation::TCount] = {
	"death",
	"idle",
	"walk",
	"recoil",
	"attack_left",
	"attack_right",
	"attack_bite",
	"drag_idle",
	"drag",
	"drag_recoil",
	"drag_attack_left",
	"drag_attack_right",
	"drag_attack_bite"
};

struct TZombieFatMovementAvaliable
{
	enum TEnum
	{
		TFeet = 0,
		TDrag,
		TImobile,
		TCount
	};
};

struct TZombieFatAttackAvaliable
{
	enum TFlag
	{
		TNone		= 0x00,
		TMouth		= 0x01,
		TLeftHand	= 0x02,
		TRightHand	= 0x04
	};
};

//typedef
typedef std::pair<float, std::string> TPairFloatString;
typedef std::vector<TPairFloatString> TArrayPairFloatString;
typedef boost::shared_ptr<SceneNodeComponentClientObjectBase> TPointerSceneNodeComponentClientObjectBase;
typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
typedef std::vector<TPointerSceneNodeComponentCollisionResult> TArrayPointerSceneNodeComponentCollisionResult;
typedef boost::shared_ptr<ZombiePart> TPointerZombiePart;
typedef std::vector<TPointerZombiePart> TArrayZombiePart;
typedef std::map<std::string, int> TMapStringInt;
typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;
typedef boost::shared_ptr<ZombiePartDetach> TPointerZombiePartDetach;
typedef std::vector<TPointerZombiePartDetach> TArrayPointerZombiePartDetach;

typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

typedef std::pair<TWeakSceneNodeComponentVisual, TWeakSceneNode> TPairWeakSceneNodeComponentVisualWeakSceneNode;
typedef std::vector<TPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayPairWeakSceneNodeComponentVisualWeakSceneNode;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;

static const float sZombieFatAnimationDefaultFadeTime = 5.0F / 30.0F;
static const float sRecoilCountDown = 5.0F / 30.0F;
static const float sAttackCountDown = 15.0F / 30.0F; //(10 / .75) / 30
static const float sChanceMouthAttack = 1.0F / 4.0F;
static const float sChanceHandAttack = 1.0F / 2.0F;


typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;
typedef std::vector<TWeakSceneNodeComponentParticle> TArrayWeakSceneNodeComponentParticle;
typedef std::map<std::string, TWeakSceneNodeComponentParticle> TMapStringWeakSceneNodeComponentParticle;

//////////////////////////////////////////////////////////////////
// Local static methods
//////////////////////////////////////////////////////////////////
static const ZombieComponentLoad& LocalGetZombieComponentLoad()
{
	//TLeftBack,
	static const ZombiePartDataLoad sArrayLeftBackData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftBack], 1.0F}
	};
	//TLeftLeg,
	static const ZombiePartDataLoad sArrayLeftLegData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftLeg], 1.0F}
	};
	static const int sArrayLeftLegParentData[] = { TZombieFatPart::TLeftBack };
	//TLeftFoot,
	static const ZombiePartDataLoad sArrayLeftFootData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftFoot], 1.0F}
	};
	static const int sArrayLeftFootParentData[] = { TZombieFatPart::TLeftLeg };
	//TLeftNeck,
	static const ZombiePartDataLoad sArrayLeftNeckData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftNeck], 1.0F}
	};
	//static const int sArrayLeftNeckParentData[] = { TZombieFatPart::TLeftBack };
	//TLeftArm,
	static const ZombiePartDataLoad sArrayLeftArmData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftArm], 1.0F}
	};
	static const int sArrayLeftArmParentData[] = { TZombieFatPart::TLeftNeck };
	//TLeftHand,
	static const ZombiePartDataLoad sArrayLeftHandData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftHand], 1.0F}
	};
	static const int sArrayLeftHandParentData[] = { TZombieFatPart::TLeftArm };
	//TLeftChest,
	static const ZombiePartDataLoad sArrayLeftChestData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftChest], 1.0F}
	};
	static const int sArrayLeftChestParentData[] = { TZombieFatPart::TLeftBack, TZombieFatPart::TLeftNeck };

	//TLeftGut,
	static const ZombiePartDataLoad sArrayLeftGutData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TLeftGut], 1.0F}
	};
	static const int sArrayLeftGutParentData[] = { TZombieFatPart::TLeftChest, TZombieFatPart::TLeftBack };

	//TRightBack,
	static const ZombiePartDataLoad sArrayRightBackData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightBack], 1.0F}
	};
	//TRightLeg,
	static const ZombiePartDataLoad sArrayRightLegData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightLeg], 1.0F}
	};
	static const int sArrayRightLegParentData[] = { TZombieFatPart::TRightBack };
	//TRightFoot,
	static const ZombiePartDataLoad sArrayRightFootData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightFoot], 1.0F}
	};
	static const int sArrayRightFootParentData[] = { TZombieFatPart::TRightLeg };
	//TRightNeck,
	static const ZombiePartDataLoad sArrayRightNeckData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightNeck], 1.0F}
	};
	//static const int sArrayRightNeckParentData[] = { TZombieFatPart::TRightBack };
	//TRightArm,
	static const ZombiePartDataLoad sArrayRightArmData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightArm], 1.0F}
	};
	static const int sArrayRightArmParentData[] = { TZombieFatPart::TRightNeck };
	//TRightHand,
	static const ZombiePartDataLoad sArrayRightHandData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightHand], 1.0F}
	};
	static const int sArrayRightHandParentData[] = { TZombieFatPart::TRightArm };
	//TRightChest,
	static const ZombiePartDataLoad sArrayRightChestData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightChest], 1.0F}
	};
	static const int sArrayRightChestParentData[] = { TZombieFatPart::TRightBack, TZombieFatPart::TRightNeck };

	//TRightGut,
	static const ZombiePartDataLoad sArrayRightGutData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::TRightGut], 1.0F}
	};
	static const int sArrayRightGutParentData[] = { TZombieFatPart::TRightChest, TZombieFatPart::TRightBack };

	//THead,
	static const ZombiePartDataLoad sArrayHeadData[] = {
		{"", 0.0F}, 
		{sZombieFatPartName[TZombieFatPart::THead], 1.0F}
	};
	static const int sArrayHeadParentData[] = { TZombieFatPart::TLeftNeck, TZombieFatPart::TRightNeck };


	static const ZombiePartLoad sArrayPart[TZombieFatPart::TCount] = 
	{
		{ sZombieFatPartName[TZombieFatPart::TLeftBack], GCOMMON_ARRAY_SIZE(sArrayLeftBackData), sArrayLeftBackData, 0, 0 },
		{ sZombieFatPartName[TZombieFatPart::TLeftLeg], GCOMMON_ARRAY_SIZE(sArrayLeftLegData), sArrayLeftLegData, GCOMMON_ARRAY_SIZE(sArrayLeftLegParentData), sArrayLeftLegParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftFoot], GCOMMON_ARRAY_SIZE(sArrayLeftFootData), sArrayLeftFootData, GCOMMON_ARRAY_SIZE(sArrayLeftFootParentData), sArrayLeftFootParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftNeck], GCOMMON_ARRAY_SIZE(sArrayLeftNeckData), sArrayLeftNeckData, 0, 0 }, //GCOMMON_ARRAY_SIZE(sArrayLeftNeckParentData), sArrayLeftNeckParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftArm], GCOMMON_ARRAY_SIZE(sArrayLeftArmData), sArrayLeftArmData, GCOMMON_ARRAY_SIZE(sArrayLeftArmParentData), sArrayLeftArmParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftHand], GCOMMON_ARRAY_SIZE(sArrayLeftHandData), sArrayLeftHandData, GCOMMON_ARRAY_SIZE(sArrayLeftHandParentData), sArrayLeftHandParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftChest], GCOMMON_ARRAY_SIZE(sArrayLeftChestData), sArrayLeftChestData, GCOMMON_ARRAY_SIZE(sArrayLeftChestParentData), sArrayLeftChestParentData },
		{ sZombieFatPartName[TZombieFatPart::TLeftGut], GCOMMON_ARRAY_SIZE(sArrayLeftGutData), sArrayLeftGutData, GCOMMON_ARRAY_SIZE(sArrayLeftGutParentData), sArrayLeftGutParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightBack], GCOMMON_ARRAY_SIZE(sArrayRightBackData), sArrayRightBackData, 0, 0 },
		{ sZombieFatPartName[TZombieFatPart::TRightLeg], GCOMMON_ARRAY_SIZE(sArrayRightLegData), sArrayRightLegData, GCOMMON_ARRAY_SIZE(sArrayRightLegParentData), sArrayRightLegParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightFoot], GCOMMON_ARRAY_SIZE(sArrayRightFootData), sArrayRightFootData, GCOMMON_ARRAY_SIZE(sArrayRightFootParentData), sArrayRightFootParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightNeck], GCOMMON_ARRAY_SIZE(sArrayRightNeckData), sArrayRightNeckData, 0, 0 }, //GCOMMON_ARRAY_SIZE(sArrayRightNeckParentData), sArrayRightNeckParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightArm], GCOMMON_ARRAY_SIZE(sArrayRightArmData), sArrayRightArmData, GCOMMON_ARRAY_SIZE(sArrayRightArmParentData), sArrayRightArmParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightHand], GCOMMON_ARRAY_SIZE(sArrayRightHandData), sArrayRightHandData, GCOMMON_ARRAY_SIZE(sArrayRightHandParentData), sArrayRightHandParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightChest], GCOMMON_ARRAY_SIZE(sArrayRightChestData), sArrayRightChestData, GCOMMON_ARRAY_SIZE(sArrayRightChestParentData), sArrayRightChestParentData },
		{ sZombieFatPartName[TZombieFatPart::TRightGut], GCOMMON_ARRAY_SIZE(sArrayRightGutData), sArrayRightGutData, GCOMMON_ARRAY_SIZE(sArrayRightGutParentData), sArrayRightGutParentData },
		{ sZombieFatPartName[TZombieFatPart::THead], GCOMMON_ARRAY_SIZE(sArrayHeadData), sArrayHeadData, GCOMMON_ARRAY_SIZE(sArrayHeadParentData), sArrayHeadParentData },
	};

	static const int sArrayParticleHeadAlive[] = { TZombieFatPart::TLeftNeck, TZombieFatPart::TRightNeck };
	static const int sArrayParticleHeadDead[] = { TZombieFatPart::THead};
	static const int sArrayParticleNeckAlive[] = { TZombieFatPart::TLeftBack, TZombieFatPart::TRightBack };
	static const int sArrayParticleNeckDead[] = { TZombieFatPart::TLeftNeck, TZombieFatPart::TRightNeck };
	static const int sArrayParticleChest[] = { TZombieFatPart::TLeftChest, TZombieFatPart::TRightChest, TZombieFatPart::TLeftBack, TZombieFatPart::TRightBack };
	static const int sArrayParticleLeftLegAlive[] = { TZombieFatPart::TLeftBack };
	static const int sArrayParticleLeftLegDead[] = { TZombieFatPart::TLeftLeg };
	static const int sArrayParticleRightLegAlive[] = { TZombieFatPart::TRightBack };
	static const int sArrayParticleRightLegDead[] = { TZombieFatPart::TRightLeg };
	static const int sArrayParticleLeftArmAlive[] = { TZombieFatPart::TLeftNeck };
	static const int sArrayParticleLeftArmDead[] = { TZombieFatPart::TLeftArm };
	static const int sArrayParticleRightArmAlive[] = { TZombieFatPart::TRightNeck };
	static const int sArrayParticleRightArmDead[] = { TZombieFatPart::TRightArm };

	static const ZombieParticleLoad sArrayParticle[] = 
	{
		{ "head", 0, GCOMMON_ARRAY_SIZE(sArrayParticleHeadAlive), sArrayParticleHeadAlive, GCOMMON_ARRAY_SIZE(sArrayParticleHeadDead), sArrayParticleHeadDead },
		{ "neck", 0, GCOMMON_ARRAY_SIZE(sArrayParticleNeckAlive), sArrayParticleNeckAlive, GCOMMON_ARRAY_SIZE(sArrayParticleNeckDead), sArrayParticleNeckDead },
		{ "chest", 1, GCOMMON_ARRAY_SIZE(sArrayParticleChest), sArrayParticleChest, GCOMMON_ARRAY_SIZE(sArrayParticleChest), sArrayParticleChest },
		{ "leftleg", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftLegAlive), sArrayParticleLeftLegAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftLegDead), sArrayParticleLeftLegDead },
		{ "rightleg", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightLegAlive), sArrayParticleRightLegAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightLegDead), sArrayParticleRightLegDead },
		{ "leftarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmAlive), sArrayParticleLeftArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmDead), sArrayParticleLeftArmDead },
		{ "rightarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmAlive), sArrayParticleRightArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmDead), sArrayParticleRightArmDead }
	};

	static const ZombieComponentLoad sZombieComponentLoad(
		GCOMMON_ARRAY_SIZE(sArrayPart),
		sArrayPart,
		GCOMMON_ARRAY_SIZE(sArrayParticle),
		sArrayParticle,

		0,
		0,
		0,
		0,
		0,
		0
		);

	return sZombieComponentLoad;
}

static void LocalDoctorPartArray(const std::string& in_factoryData, TArrayZombiePart& in_arrayZombiePart, const ZombieGameVar& in_gameVar)
{
	in_arrayZombiePart[TZombieFatPart::TLeftBack]->SetHealth(in_arrayZombiePart[TZombieFatPart::TLeftBack]->GetHealth() * 2.0F);
	in_arrayZombiePart[TZombieFatPart::TLeftNeck]->SetHealth(in_arrayZombiePart[TZombieFatPart::TLeftNeck]->GetHealth() * 2.0F);
	in_arrayZombiePart[TZombieFatPart::TRightBack]->SetHealth(in_arrayZombiePart[TZombieFatPart::TRightBack]->GetHealth() * 2.0F);
	in_arrayZombiePart[TZombieFatPart::TRightNeck]->SetHealth(in_arrayZombiePart[TZombieFatPart::TRightNeck]->GetHealth() * 2.0F);

	return;
}

//0.0 < health of part 
static const bool LocalPartOk(const TArrayZombiePart& in_arrayZombiePart, const TZombieFatPart::TEnum in_part)
{
	const bool result = (0.0F < in_arrayZombiePart[in_part]->GetHealth());
	return result;
}

static int LocalUpdateMovementAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	const bool leftFoot = LocalPartOk(in_arrayZombiePart, TZombieFatPart::TLeftFoot);
	const bool rightFoot = LocalPartOk(in_arrayZombiePart, TZombieFatPart::TRightFoot);
	const bool leftArm = LocalPartOk(in_arrayZombiePart, TZombieFatPart::TLeftArm);
	const bool rightArm = LocalPartOk(in_arrayZombiePart, TZombieFatPart::TRightArm);

	if (leftFoot && rightFoot)
	{
		return TZombieFatMovementAvaliable::TFeet;
	} 
	else if (leftFoot || rightFoot || leftArm || rightArm)
	{
		return TZombieFatMovementAvaliable::TDrag;
	}

	return TZombieFatMovementAvaliable::TImobile;
}

static int LocalUpdateAttackAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	int result = 0;
	if ( LocalPartOk(in_arrayZombiePart, TZombieFatPart::TLeftNeck) ||
		LocalPartOk(in_arrayZombiePart, TZombieFatPart::TRightNeck))
	{
		result |= TZombieFatAttackAvaliable::TMouth;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieFatPart::TLeftArm))
	{
		result |= TZombieFatAttackAvaliable::TLeftHand;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieFatPart::TRightArm))
	{
		result |= TZombieFatAttackAvaliable::TRightHand;
	}

	return result;
}


//static public methods
/*static*/ SceneNodeComponentClientZombieFat::TPointerSceneNodeComponentBase SceneNodeComponentClientZombieFat::Factory(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data //address of ZombieMission
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_data)
	{
		return pointer;
	}

	ZombieMission& zombieMission = *((ZombieMission*)in_data);
	ZombieGameVar& zombieGameVar = *(zombieMission.GetGameVar());
	const float healthFactor = zombieGameVar.GetZombieHealthFactor() * sZombieFatHealthFactor;
	const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientZombieFat pointerZombieFat;
	pointerZombieFat.reset(new SceneNodeComponentClientZombieFat(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor
		));

	pointer = pointerZombieFat;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientZombieFat::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieFat;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientZombieFat::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientZombieFat pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientZombieFat>(inout_component);
	if (!pointer)
	{
		return;
	}
	SceneNodeComponentClientZombieFat& ZombieFat = *pointer;

	//recursivly hide all visual components. this would be better at constructor scope, but we dont have the pointer in the constructor...
	//GSceneNodeUtility::RecusiveSetVisualComponentVisible(inout_owner, false);

	ZombieFat.mZombieComponent = ZombieComponent::Factory(
		LocalGetZombieComponentLoad(),
		inout_owner,
		ZombieFat.mParentMission.GetGore(),
		ZombieFat.mHealthFactor
		);

	ZombieGameVar& zombieGameVar = *(ZombieFat.mParentMission.GetGameVar());
	TArrayZombiePart& arrayZombiePart = ZombieFat.mZombieComponent->GetArrayZombiePart();
	LocalDoctorPartArray(
		ZombieFat.mFactoryData, 
		arrayZombiePart,
		zombieGameVar
		);
	ZombieFat.mMovementAvaliable = LocalUpdateMovementAvaliable(arrayZombiePart);
	ZombieFat.mAttackAvaliable = LocalUpdateAttackAvaliable(arrayZombiePart);

	//this is a bit wack, so don't do it? damn, easy way of deleting visual is to transfer them to a local scope scene node?
	// could add to part detach something to delete or hide visual...
	TPointerSceneNode dummySceneNode;
	dummySceneNode.reset(new GSceneNode(""));
	ZombieFat.OnAfterDamagePass(inout_scene, dummySceneNode);

	return;
}

//constructor
SceneNodeComponentClientZombieFat::SceneNodeComponentClientZombieFat(
	ZombieMission& in_parent,
	TPointerSceneNode& in_parentSceneNode,
	const std::string& in_factoryData,
	const float in_healthFactor
	)
	: SceneNodeComponentClientObjectBase(in_parentSceneNode->GetName())
	, mParentMission(in_parent)
	, mParentSceneNode(in_parentSceneNode)
	, mHealthFactor(in_healthFactor)
	//, mWeakParent(in_parentSceneNode)
	, mFactoryData(in_factoryData)
	, mDead(false)
	, mTransformDirty(true)
	, mAiControl(false)
	, mFirstHit(true) //for head hunter
	, mDeathCountDown(0.0F)
	, mRecoilCountdown(0.0F)
	, mAttackCountdown(0.0F)
	, mMovementAvaliable(TZombieFatMovementAvaliable::TCount)
	, mAttackAvaliable(TZombieFatAttackAvaliable::TNone)
	, mCurrentAnimation(TZombieFatAnimation::TCount)
	, mLerpPosition()
	, mLerpAt()
	, mLerpUp()
	, mZombieComponent()
{
	const GMatrix16Float& worldTransform = in_parentSceneNode->GetWorldTransform();
	mLerpPosition.reset(new GComponentLerpVector3Float(worldTransform.GetPosition()));
	mLerpAt.reset(new GComponentLerpVector3Float(worldTransform.GetAt()));
	mLerpUp.reset(new GComponentLerpVector3Float(worldTransform.GetUp()));

	return;
}

/*virtual*/ SceneNodeComponentClientZombieFat::~SceneNodeComponentClientZombieFat()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientZombieFat::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mDead)
	{
		mDeathCountDown -= in_timeDelta;
		if (mDeathCountDown < 0.0F)
		{
			mParentMission.RequestNodeDestroy(inout_sceneNode);
		}
		return;	
	}

	if (0.0F < mRecoilCountdown)
	{
		mRecoilCountdown -= in_timeDelta;
		if (mRecoilCountdown <= 0.0F)
		{
			//at end of recoil, reconsider action
			AiSelectAction(in_timeDelta);
			SelectAnimation();
		}
	}
	else if (0.0F < mAttackCountdown)
	{
		mAttackCountdown -= in_timeDelta;
		if (mAttackCountdown <= 0.0F)
		{
			//at end of recoil, reconsider action
			AiSelectAction(in_timeDelta);
			SelectAnimation();
		}
	}
	else if ((mLerpPosition->GetActive()) ||
		(mLerpAt->GetActive()) ||
		(mLerpUp->GetActive()))
	{
		mTransformDirty = true;
		mLerpPosition->Tick(in_timeDelta);
		mLerpAt->Tick(in_timeDelta);
		mLerpUp->Tick(in_timeDelta);
	}
	else
	{
		AiSelectAction(in_timeDelta);
		SelectAnimation();
	}

	if (mTransformDirty)
	{
		mTransformDirty = false;
		GVector3Float at = mLerpAt->GetValue();
		GVector3Float up = mLerpUp->GetValue();
		if ((mLerpAt->GetActive()) ||
			(mLerpUp->GetActive()))
		{
			Orthogonalise(at, up);
		}

		const GMatrix16Float transform = GMatrix16FloatConstructAtUp(
			at, 
			up,
			GVector3Float::sUnitZ, 
			GVector3Float::sUnitY, 
			mLerpPosition->GetValue()
			);
		inout_sceneNode->SetParentRelativeTransform(transform);
	}

	return;
}
//implement SceneNodeComponentClientObjectBase
/*virtual*/ void SceneNodeComponentClientZombieFat::OnCollectDamageReceivers(
	TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
	const GVector3Float& in_worldSpaceConeOrigin,
	const GVector3Float& in_worldSpaceConeDirection,
	const float in_coneLength,
	const float in_coneRadiusPoint,
	const float in_coneRadiusBase
	)
{
	if (mZombieComponent)
	{
		mZombieComponent->CollectDamageReceivers(
			this,
			out_arrayClientObjectCollisionResult, 
			in_worldSpaceConeOrigin,
			in_worldSpaceConeDirection,
			in_coneLength,
			in_coneRadiusPoint,
			in_coneRadiusBase		
			);
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	bool partLost = false;

	if (mZombieComponent)
	{
		partLost = mZombieComponent->ApplyDamage(
			inout_damage, 
			in_data	
			);
	}

	//you just shot me, i'm now going to target you, after recoil (ie, stop following cinematic)
	mAiControl = true;

	//recoil on hit
	//if (partLost && !mDead && (sRecoilCountDown != mRecoilCountdown))
	if (!mDead && (sRecoilCountDown != mRecoilCountdown))
	{
		mAttackCountdown = 0.0F;
		mRecoilCountdown = sRecoilCountDown;
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TZombieFatAnimation::TDragRecoil, true);
			break;
		case TZombieFatMovementAvaliable::TFeet:
			PlayAnimation(TZombieFatAnimation::TRecoil, true);
			break;
		}
	}
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
{
	if (!mZombieComponent)
	{
		return;
	}
	TPointerSceneNode sceneNode = mParentSceneNode.lock();
	if (sceneNode)
	{
		mZombieComponent->AfterDamagePass(inout_scene, inout_sceenRoot, sceneNode);
	}

	//head hunter
	if (mFirstHit)
	{
		mFirstHit = false;
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		if (LocalPartOk(mZombieComponent->GetArrayZombiePart(), TZombieFatPart::THead))
		{
			gameVar.SetMissHeadFirstCount(gameVar.GetMissHeadFirstCount() + 1);
		}
		else
		{
			gameVar.SetHitHeadFirstCount(gameVar.GetHitHeadFirstCount() + 1);
		}
	}

	//check movement status
	const int oldMovementAvaliable = mMovementAvaliable;
	mMovementAvaliable = LocalUpdateMovementAvaliable(mZombieComponent->GetArrayZombiePart());
	mAttackAvaliable = LocalUpdateAttackAvaliable(mZombieComponent->GetArrayZombiePart());

	//trigger death?
	if ((oldMovementAvaliable != mMovementAvaliable) && 
		(mMovementAvaliable == TZombieFatMovementAvaliable::TImobile) &&
		//(mAttackAvaliable == TZombieFatAttackAvaliable::TNone) &&
		(!mDead))
	{
		mDead = true;
		mDeathCountDown = 2.5F;
		PlayAnimation(TZombieFatAnimation::TDeath);

		//recorde death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);

		return;
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnAnimate(
	const std::string& in_animationName, 
	const GAnimationPlayParam& in_playParam
	)
{
	if (mAiControl)
	{
		//yes, we ignore if chasing character, turn off ai on actor if needed for cinematic but could be a bad idea (since the actor could be killed)
		return;
	}
	TPointerSceneNodeComponentAnimation pointerAnimation = mZombieComponent->GetAnimation().lock();
	if (!pointerAnimation)
	{
		return;
	}

	TPointerAnimationManagerInstance& pointerAnimationManagerInstance = pointerAnimation->GetAnimationManagerInstance();
	if (!pointerAnimationManagerInstance)
	{
		return;
	}

	GAnimationManagerInstance& animationManagerInstance = *pointerAnimationManagerInstance;

	animationManagerInstance.StopAnimation(sZombieFatAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnMove(
	const GVector3Float& in_position,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mAiControl)
	{
		//i'm ignoring you now
		return;
	}

	mLerpPosition->SetValue(in_position, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnRotate(
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const float in_transitionTime,
	const int in_transitionType
	)
{
	if (mAiControl)
	{
		//i'm ignoring you now
		return;
	}

	mLerpAt->SetValue(in_at, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mLerpUp->SetValue(in_up, in_transitionTime, GComponentLerp::TType::TEnum(in_transitionType));
	mTransformDirty = true;
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientZombieFat::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientZombieFat::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
void SceneNodeComponentClientZombieFat::AiSelectAction(const float in_timeDelta)
{
	if (!mAiControl || mDead)// || !mLerpPosition || !mLerpAt || !mLerpUp)
	{
		return;
	}

	//try to move towards player, if close enough to player, attack 
	ZombieGameVar& gameVar = *mParentMission.GetGameVar();
	const GVector3Float playerPosition = gameVar.GetPlayerPosition();
	const GVector3Float playerAt = gameVar.GetPlayerDirection();
	const GVector3Float playerCross = CrossProduct(playerAt, GVector3Float::sUnitY);
	const ZombieStateType::TGameDifficulty::TEnum difficulty = gameVar.GetGameDifficulty();

	const GVector3Float zombiePosition = mLerpPosition->GetValue();

	const float distanceToPlayerSquared = LengthSquared(playerPosition - zombiePosition);

	const bool moveTowardsPlayer = (3.0F < distanceToPlayerSquared);

	if (moveTowardsPlayer)
	{
		TPointerZombieSound pZombieSound = mParentMission.GetSound();
		TArrayZombiePart& arrayZombiePart = mZombieComponent->GetArrayZombiePart();
		if (pZombieSound && LocalPartOk(arrayZombiePart, TZombieFatPart::THead))
		{
			if (GMath::RandomUnit() < 0.1F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDZombieFatIdle0(), zombiePosition);
			}
			else if (GMath::RandomUnit() < 0.1F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDZombieFatIdle1(), zombiePosition);
			}
		}

		const GVector3Float playerTarget = playerPosition + (playerAt) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
		float distanceToPlayer = 0.0F;
		GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
		//GVector3Float normalToPlayer = NormaliseAprox(playerPosition - playerTarget);
		//mLerpAt->SetValue(normalToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);

		float speed = 0.75F;

		//select animation, get speed
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TZombieFatMovementAvaliable::TDrag:
			speed = 0.25F;
			break;
		case TZombieFatMovementAvaliable::TImobile:
			speed = 0.0F;
			break;
		}

		if (speed)
		{
			float timeToPlayer = distanceToPlayer / speed;
			mLerpPosition->SetValue(playerTarget, timeToPlayer, GComponentLerp::TType::TEaseInOut);
		}
	}
	else
	{
		float chanceMul = 1.0F;
		switch (difficulty)
		{
		default:
			break;
		case ZombieStateType::TGameDifficulty::TEasy:
			chanceMul = 0.2F;
			break;
		case ZombieStateType::TGameDifficulty::TNormal:
			chanceMul = 0.5F;
			break;
		}

		//chance of attack
		bool drag = (TZombieFatMovementAvaliable::TDrag == mMovementAvaliable);

		int attackAnimation = TZombieFatAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if ((mAttackAvaliable & TZombieFatAttackAvaliable::TMouth) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieFatAnimation::TDragAttackBite : TZombieFatAnimation::TAttackBite;
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}
		else if ((mAttackAvaliable & TZombieFatAttackAvaliable::TLeftHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieFatAnimation::TDragAttackLeft : TZombieFatAnimation::TAttackLeft;
			damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
		}
		else if ((mAttackAvaliable & TZombieFatAttackAvaliable::TRightHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieFatAnimation::TDragAttackRight : TZombieFatAnimation::TAttackRight;
			damageType = ZombieGuiDamage::GetDamageTypeSlashRight();
		}

		if (TZombieFatAnimation::TCount != attackAnimation)
		{
			mParentMission.TakeDamage(2, damageType);
			mAttackCountdown = sAttackCountDown;
			PlayAnimation(attackAnimation, true);

			//and face palyer
			mLerpAt->SetValue(Normalise(playerPosition - zombiePosition), 0.25F, GComponentLerp::TType::TEaseInOut);
		}
	}

	return;
}

void SceneNodeComponentClientZombieFat::SelectAnimation()
{
	if (!mAiControl || mDead || (0.0F < mRecoilCountdown) || (0.0F < mAttackCountdown))
	{
		return;
	}

	//select a move animation
	if (mLerpPosition && mLerpPosition->GetActive())
	{
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TZombieFatMovementAvaliable::TFeet:
			PlayAnimation(TZombieFatAnimation::TWalk);
			break;
		case TZombieFatMovementAvaliable::TDrag:
			PlayAnimation(TZombieFatAnimation::TDrag);
			break;
		case TZombieFatMovementAvaliable::TImobile:
			PlayAnimation(TZombieFatAnimation::TDeath);
			break;
		}
	}
	else
	//select a idle animation
	{
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TZombieFatAnimation::TDragIdle);
			break;
		case TZombieFatMovementAvaliable::TFeet:
			PlayAnimation(TZombieFatAnimation::TIdle);
			break;
		}
	}

	return;
}

void SceneNodeComponentClientZombieFat::PlayAnimation(const int in_animation, const bool in_force)
{
	if ((in_animation == mCurrentAnimation) && (!in_force))
	{
		return;
	}
	mCurrentAnimation = in_animation;

	TPointerSceneNodeComponentAnimation pointerAnimation = mZombieComponent->GetAnimation().lock();
	if (!pointerAnimation)
	{
		return;
	}
	GAnimationManagerInstance& animationManagerInstance = *(pointerAnimation->GetAnimationManagerInstance());
	animationManagerInstance.StopAnimation(sZombieFatAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TZombieFatAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sZombieFatAnimationRate[mCurrentAnimation],
			sZombieFatAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sZombieFatAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
