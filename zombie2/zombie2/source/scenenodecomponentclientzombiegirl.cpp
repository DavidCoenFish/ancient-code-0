//
//  SceneNodeComponentClientZombieGirl.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientZombieGirl.h"

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
static const float sZombieGirlHealthFactor = 2.0F;

//softbinding names
struct TZombieGirlPart
{
	enum TEnum
	{
		TLeftTorso,
		TLeftArm,
		TLeftForearm,
		TLeftHand,
		TLeftHip,
		TLeftLeg,
		TLeftCalf,
		TLeftFoot,
		TRightTorso,
		TRightArm,
		TRightForearm,
		TRightHand,
		TRightHip,
		TRightLeg,
		TRightCalf,
		TRightFoot,
		TNeck,
		THead,
		TCount
	};
};

static const char* const sZombieGirlPartName[TZombieGirlPart::TCount] = {
	"lefttorso",
	"leftarm",
	"leftforearm",
	"lefthand",
	"lefthip",
	"leftleg",
	"leftcalf",
	"leftfoot",
	"righttorso",
	"rightarm",
	"rightforearm",
	"righthand",
	"righthip",
	"rightleg",
	"rightcalf",
	"rightfoot",
	"neck",
	"head"
};
/**/

struct TZombieGirlAnimation
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
static const float sZombieGirlAnimationRate[TZombieGirlAnimation::TCount] = {
	1.0F, //TDeath = 0,
	0.75F, //TIdle,
	1.0F, //TWalk,
	1.0F, //TRecoil,
	0.75F, //TAttack_left,
	0.75F, //TAttack_right,
	0.75F, //TAttack_bite,
	0.5F, //TDrag_idle,
	0.75F, //TDrag,
	1.0F, //TDragRecoil,
	0.75F, //TDrag_attack_left,
	0.75F, //TDrag_attack_right,
	0.75F  //TDrag_attack_bite,
};

static const char* const sZombieGirlAnimationName[TZombieGirlAnimation::TCount] = {
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

struct TZombieGirlMovementAvaliable
{
	enum TEnum
	{
		TFeet = 0,
		TDrag,
		TImobile,
		TCount
	};
};

struct TZombieGirlAttackAvaliable
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

typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;
typedef std::vector<TWeakSceneNodeComponentParticle> TArrayWeakSceneNodeComponentParticle;
typedef std::map<std::string, TWeakSceneNodeComponentParticle> TMapStringWeakSceneNodeComponentParticle;
typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;

static const float sZombieGirlAnimationDefaultFadeTime = 5.0F / 30.0F;
static const float sRecoilCountDown = 5.0F / 30.0F;
static const float sAttackCountDown = 15.0F / 30.0F;
static const float sChanceMouthAttack = 1.0F / 4.0F;
static const float sChanceHandAttack = 1.0F / 2.0F;


//////////////////////////////////////////////////////////////////
// Local static methods
//////////////////////////////////////////////////////////////////
static const ZombieComponentLoad& LocalGetZombieComponentLoad()
{
	//TLeftTorso,
	static const ZombiePartDataLoad sArrayLeftTorsoData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftTorso], 1.0F}
	};
	//TLeftArm,
	static const ZombiePartDataLoad sArrayLeftArmData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftArm], 1.0F}
	};
	static const int sArrayLeftArmParentData[] = { TZombieGirlPart::TLeftTorso };
	//TLeftForearm,
	static const ZombiePartDataLoad sArrayLeftForearmData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftForearm], 1.0F}
	};
	static const int sArrayLeftForearmParentData[] = { TZombieGirlPart::TLeftArm };
	//TLeftHand,
	static const ZombiePartDataLoad sArrayLeftHandData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftHand], 1.0F}
	};
	static const int sArrayLeftHandParentData[] = { TZombieGirlPart::TLeftForearm };
	//TLeftHip,
	static const ZombiePartDataLoad sArrayLeftHipData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftHip], 1.0F}
	};
	//TLeftLeg,
	static const ZombiePartDataLoad sArrayLeftLegData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftLeg], 1.0F}
	};
	static const int sArrayLeftLegParentData[] = { TZombieGirlPart::TLeftHip };
	//TLeftCalf,
	static const ZombiePartDataLoad sArrayLeftCalfData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftCalf], 1.0F}
	};
	static const int sArrayLeftCalfParentData[] = { TZombieGirlPart::TLeftLeg };
	//TLeftFoot,
	static const ZombiePartDataLoad sArrayLeftFootData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TLeftFoot], 1.0F}
	};
	static const int sArrayLeftFootParentData[] = { TZombieGirlPart::TLeftCalf };


	//TRightTorso,
	static const ZombiePartDataLoad sArrayRightTorsoData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightTorso], 1.0F}
	};
	//TRightArm,
	static const ZombiePartDataLoad sArrayRightArmData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightArm], 1.0F}
	};
	static const int sArrayRightArmParentData[] = { TZombieGirlPart::TRightTorso };
	//TRightForearm,
	static const ZombiePartDataLoad sArrayRightForearmData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightForearm], 1.0F}
	};
	static const int sArrayRightForearmParentData[] = { TZombieGirlPart::TRightArm };
	//TRightHand,
	static const ZombiePartDataLoad sArrayRightHandData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightHand], 1.0F}
	};
	static const int sArrayRightHandParentData[] = { TZombieGirlPart::TRightForearm };
	//TRightHip,
	static const ZombiePartDataLoad sArrayRightHipData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightHip], 1.0F}
	};
	//TRightLeg,
	static const ZombiePartDataLoad sArrayRightLegData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightLeg], 1.0F}
	};
	static const int sArrayRightLegParentData[] = { TZombieGirlPart::TRightHip };
	//TRightCalf,
	static const ZombiePartDataLoad sArrayRightCalfData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightCalf], 1.0F}
	};
	static const int sArrayRightCalfParentData[] = { TZombieGirlPart::TRightLeg };
	//TRightFoot,
	static const ZombiePartDataLoad sArrayRightFootData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TRightFoot], 1.0F}
	};
	static const int sArrayRightFootParentData[] = { TZombieGirlPart::TRightCalf };


	//TNeck,
	static const ZombiePartDataLoad sArrayNeckData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::TNeck], 1.0F}
	};
	static const int sArrayNeckParentData[] = { TZombieGirlPart::TLeftTorso, TZombieGirlPart::TRightTorso };
	//THead,
	static const ZombiePartDataLoad sArrayHeadData[] = {
		{"", 0.0F}, 
		{sZombieGirlPartName[TZombieGirlPart::THead], 1.0F}
	};
	static const int sArrayHeadParentData[] = { TZombieGirlPart::TNeck };

	static const ZombiePartLoad sArrayPart[TZombieGirlPart::TCount] = 
	{
		{ sZombieGirlPartName[TZombieGirlPart::TLeftTorso], GCOMMON_ARRAY_SIZE(sArrayLeftTorsoData), sArrayLeftTorsoData, 0, 0 },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftArm], GCOMMON_ARRAY_SIZE(sArrayLeftArmData), sArrayLeftArmData, GCOMMON_ARRAY_SIZE(sArrayLeftArmParentData), sArrayLeftArmParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftForearm], GCOMMON_ARRAY_SIZE(sArrayLeftForearmData), sArrayLeftForearmData, GCOMMON_ARRAY_SIZE(sArrayLeftForearmParentData), sArrayLeftForearmParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftHand], GCOMMON_ARRAY_SIZE(sArrayLeftHandData), sArrayLeftHandData, GCOMMON_ARRAY_SIZE(sArrayLeftHandParentData), sArrayLeftHandParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftHip], GCOMMON_ARRAY_SIZE(sArrayLeftHipData), sArrayLeftHipData, 0, 0 },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftLeg], GCOMMON_ARRAY_SIZE(sArrayLeftLegData), sArrayLeftLegData, GCOMMON_ARRAY_SIZE(sArrayLeftLegParentData), sArrayLeftLegParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftCalf], GCOMMON_ARRAY_SIZE(sArrayLeftCalfData), sArrayLeftCalfData, GCOMMON_ARRAY_SIZE(sArrayLeftCalfParentData), sArrayLeftCalfParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TLeftFoot], GCOMMON_ARRAY_SIZE(sArrayLeftFootData), sArrayLeftFootData, GCOMMON_ARRAY_SIZE(sArrayLeftFootParentData), sArrayLeftFootParentData },

		{ sZombieGirlPartName[TZombieGirlPart::TRightTorso], GCOMMON_ARRAY_SIZE(sArrayRightTorsoData), sArrayRightTorsoData, 0, 0 },
		{ sZombieGirlPartName[TZombieGirlPart::TRightArm], GCOMMON_ARRAY_SIZE(sArrayRightArmData), sArrayRightArmData, GCOMMON_ARRAY_SIZE(sArrayRightArmParentData), sArrayRightArmParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TRightForearm], GCOMMON_ARRAY_SIZE(sArrayRightForearmData), sArrayRightForearmData, GCOMMON_ARRAY_SIZE(sArrayRightForearmParentData), sArrayRightForearmParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TRightHand], GCOMMON_ARRAY_SIZE(sArrayRightHandData), sArrayRightHandData, GCOMMON_ARRAY_SIZE(sArrayRightHandParentData), sArrayRightHandParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TRightHip], GCOMMON_ARRAY_SIZE(sArrayRightHipData), sArrayRightHipData, 0, 0 },
		{ sZombieGirlPartName[TZombieGirlPart::TRightLeg], GCOMMON_ARRAY_SIZE(sArrayRightLegData), sArrayRightLegData, GCOMMON_ARRAY_SIZE(sArrayRightLegParentData), sArrayRightLegParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TRightCalf], GCOMMON_ARRAY_SIZE(sArrayRightCalfData), sArrayRightCalfData, GCOMMON_ARRAY_SIZE(sArrayRightCalfParentData), sArrayRightCalfParentData },
		{ sZombieGirlPartName[TZombieGirlPart::TRightFoot], GCOMMON_ARRAY_SIZE(sArrayRightFootData), sArrayRightFootData, GCOMMON_ARRAY_SIZE(sArrayRightFootParentData), sArrayRightFootParentData },

		{ sZombieGirlPartName[TZombieGirlPart::TNeck], GCOMMON_ARRAY_SIZE(sArrayNeckData), sArrayNeckData, GCOMMON_ARRAY_SIZE(sArrayNeckParentData), sArrayNeckParentData },
		{ sZombieGirlPartName[TZombieGirlPart::THead], GCOMMON_ARRAY_SIZE(sArrayHeadData), sArrayHeadData, GCOMMON_ARRAY_SIZE(sArrayHeadParentData), sArrayHeadParentData },
	};

	static const int sArrayParticleHeadAlive[] = { TZombieGirlPart::TNeck};
	static const int sArrayParticleHeadDead[] = { TZombieGirlPart::THead};
	static const int sArrayParticleNeckAlive[] = { TZombieGirlPart::TLeftTorso, TZombieGirlPart::TRightTorso };
	static const int sArrayParticleNeckDead[] = { TZombieGirlPart::TNeck};
	static const int sArrayParticleChest[] = { TZombieGirlPart::TLeftTorso, TZombieGirlPart::TRightTorso, TZombieGirlPart::TLeftHip, TZombieGirlPart::TRightHip };
	static const int sArrayParticleHip[] = { TZombieGirlPart::TLeftHip, TZombieGirlPart::TRightHip };
	static const int sArrayParticleLeftLegAlive[] = { TZombieGirlPart::TLeftHip };
	static const int sArrayParticleLeftLegDead[] = { TZombieGirlPart::TLeftLeg };
	static const int sArrayParticleRightLegAlive[] = { TZombieGirlPart::TRightHip };
	static const int sArrayParticleRightLegDead[] = { TZombieGirlPart::TRightLeg };

	static const int sArrayParticleLeftArmAlive[] = { TZombieGirlPart::TLeftTorso };
	static const int sArrayParticleLeftArmDead[] = { TZombieGirlPart::TLeftArm };
	static const int sArrayParticleRightArmAlive[] = { TZombieGirlPart::TRightTorso };
	static const int sArrayParticleRightArmDead[] = { TZombieGirlPart::TRightArm };

	static const int sArrayParticleLeftForeArmAlive[] = { TZombieGirlPart::TLeftArm };
	static const int sArrayParticleLeftForeArmDead[] = { TZombieGirlPart::TLeftHand };
	static const int sArrayParticleRightForeArmAlive[] = { TZombieGirlPart::TRightArm };
	static const int sArrayParticleRightForeArmDead[] = { TZombieGirlPart::TRightHand };

	static const ZombieParticleLoad sArrayParticle[] = 
	{
		{ "head", 0, GCOMMON_ARRAY_SIZE(sArrayParticleHeadAlive), sArrayParticleHeadAlive, GCOMMON_ARRAY_SIZE(sArrayParticleHeadDead), sArrayParticleHeadDead },
		{ "neck", 0, GCOMMON_ARRAY_SIZE(sArrayParticleNeckAlive), sArrayParticleNeckAlive, GCOMMON_ARRAY_SIZE(sArrayParticleNeckDead), sArrayParticleNeckDead },
		{ "chest", 1, GCOMMON_ARRAY_SIZE(sArrayParticleChest), sArrayParticleChest, GCOMMON_ARRAY_SIZE(sArrayParticleChest), sArrayParticleChest },
		{ "hip", 1, GCOMMON_ARRAY_SIZE(sArrayParticleHip), sArrayParticleHip, GCOMMON_ARRAY_SIZE(sArrayParticleHip), sArrayParticleHip },
		{ "leftleg", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftLegAlive), sArrayParticleLeftLegAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftLegDead), sArrayParticleLeftLegDead },
		{ "rightleg", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightLegAlive), sArrayParticleRightLegAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightLegDead), sArrayParticleRightLegDead },
		{ "leftarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmAlive), sArrayParticleLeftArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmDead), sArrayParticleLeftArmDead },
		{ "rightarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmAlive), sArrayParticleRightArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmDead), sArrayParticleRightArmDead },
		{ "leftforearm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftForeArmAlive), sArrayParticleLeftForeArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftForeArmDead), sArrayParticleLeftForeArmDead },
		{ "rightforearm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightForeArmAlive), sArrayParticleRightForeArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightForeArmDead), sArrayParticleRightForeArmDead }
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
	in_arrayZombiePart[TZombieGirlPart::TLeftTorso]->SetHealth(in_arrayZombiePart[TZombieGirlPart::TLeftTorso]->GetHealth() * 2.0F);
	in_arrayZombiePart[TZombieGirlPart::TRightTorso]->SetHealth(in_arrayZombiePart[TZombieGirlPart::TRightTorso]->GetHealth() * 2.0F);

	return;
}

//0.0 < health of part 
static const bool LocalPartOk(const TArrayZombiePart& in_arrayZombiePart, const TZombieGirlPart::TEnum in_part)
{
	const bool result = (0.0F < in_arrayZombiePart[in_part]->GetHealth());
	return result;
}

static int LocalUpdateMovementAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	const bool leftFoot = LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TLeftFoot);
	const bool rightFoot = LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TRightFoot);
	const bool leftArm = LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TLeftArm);
	const bool rightArm = LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TRightArm);

	if (leftFoot && rightFoot)
	{
		return TZombieGirlMovementAvaliable::TFeet;
	} 
	else if (leftFoot || rightFoot || leftArm || rightArm)
	{
		return TZombieGirlMovementAvaliable::TDrag;
	}

	return TZombieGirlMovementAvaliable::TImobile;
}

static int LocalUpdateAttackAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	int result = 0;
	if (LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TNeck))
	{
		result |= TZombieGirlAttackAvaliable::TMouth;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TLeftForearm))
	{
		result |= TZombieGirlAttackAvaliable::TLeftHand;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieGirlPart::TRightForearm))
	{
		result |= TZombieGirlAttackAvaliable::TRightHand;
	}

	return result;
}


//static public methods
/*static*/ SceneNodeComponentClientZombieGirl::TPointerSceneNodeComponentBase SceneNodeComponentClientZombieGirl::Factory(
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
	const float healthFactor = zombieGameVar.GetZombieHealthFactor() * sZombieGirlHealthFactor;
	const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientZombieGirl pointerZombieGirl;
	pointerZombieGirl.reset(new SceneNodeComponentClientZombieGirl(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor
		));

	pointer = pointerZombieGirl;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientZombieGirl::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieGirl;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientZombieGirl::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientZombieGirl pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientZombieGirl>(inout_component);
	if (!pointer)
	{
		return;
	}
	SceneNodeComponentClientZombieGirl& zombieGirl = *pointer;

	//recursivly hide all visual components. this would be better at constructor scope, but we dont have the pointer in the constructor...
	//GSceneNodeUtility::RecusiveSetVisualComponentVisible(inout_owner, false);

	zombieGirl.mZombieComponent = ZombieComponent::Factory(
		LocalGetZombieComponentLoad(),
		inout_owner,
		zombieGirl.mParentMission.GetGore(),
		zombieGirl.mHealthFactor
		);

	ZombieGameVar& zombieGameVar = *(zombieGirl.mParentMission.GetGameVar());
	TArrayZombiePart& arrayZombiePart = zombieGirl.mZombieComponent->GetArrayZombiePart();
	LocalDoctorPartArray(
		zombieGirl.mFactoryData, 
		arrayZombiePart,
		zombieGameVar
		);
	zombieGirl.mMovementAvaliable = LocalUpdateMovementAvaliable(arrayZombiePart);
	zombieGirl.mAttackAvaliable = LocalUpdateAttackAvaliable(arrayZombiePart);

	//this is a bit wack, so don't do it? damn, easy way of deleting visual is to transfer them to a local scope scene node?
	// could add to part detach something to delete or hide visual...
	TPointerSceneNode dummySceneNode;
	dummySceneNode.reset(new GSceneNode(""));
	zombieGirl.OnAfterDamagePass(inout_scene, dummySceneNode);

	return;
}

//constructor
SceneNodeComponentClientZombieGirl::SceneNodeComponentClientZombieGirl(
	ZombieMission& in_parent,
	TPointerSceneNode& in_parentSceneNode,
	const std::string& in_factoryData,
	const float in_healthFactor
	)
	: SceneNodeComponentClientObjectBase(in_parentSceneNode->GetName())
	, mParentMission(in_parent)
	, mParentSceneNode(in_parentSceneNode)
	//, mWeakParent(in_parentSceneNode)
	, mFactoryData(in_factoryData)
	, mHealthFactor(in_healthFactor)
	, mDead(false)
	, mTransformDirty(true)
	, mAiControl(false)
	, mFirstHit(true)
	, mDeathCountDown(0.0F)
	, mRecoilCountdown(0.0F)
	, mAttackCountdown(0.0F)
	, mMovementAvaliable(TZombieGirlMovementAvaliable::TCount)
	, mAttackAvaliable(TZombieGirlAttackAvaliable::TNone)
	, mCurrentAnimation(TZombieGirlAnimation::TCount)
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

/*virtual*/ SceneNodeComponentClientZombieGirl::~SceneNodeComponentClientZombieGirl()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientZombieGirl::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
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
/*virtual*/ void SceneNodeComponentClientZombieGirl::OnCollectDamageReceivers(
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

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnApplyDamage(
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
			PlayAnimation(TZombieGirlAnimation::TDragRecoil, true);
			break;
		case TZombieGirlMovementAvaliable::TFeet:
			PlayAnimation(TZombieGirlAnimation::TRecoil, true);
			break;
		}
	}
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
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

	//check movement status
	const int oldMovementAvaliable = mMovementAvaliable;
	mMovementAvaliable = LocalUpdateMovementAvaliable(mZombieComponent->GetArrayZombiePart());
	mAttackAvaliable = LocalUpdateAttackAvaliable(mZombieComponent->GetArrayZombiePart());

	//head hunter
	if (mFirstHit)
	{
		mFirstHit = false;
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		if (LocalPartOk(mZombieComponent->GetArrayZombiePart(), TZombieGirlPart::THead))
		{
			gameVar.SetMissHeadFirstCount(gameVar.GetMissHeadFirstCount() + 1);
		}
		else
		{
			gameVar.SetHitHeadFirstCount(gameVar.GetHitHeadFirstCount() + 1);
		}
	}

	//trigger death?
	if ((oldMovementAvaliable != mMovementAvaliable) && 
		(mMovementAvaliable == TZombieGirlMovementAvaliable::TImobile) &&
		//(mAttackAvaliable == TZombieGirlAttackAvaliable::TNone) &&
		(!mDead))
	{
		mDead = true;
		mDeathCountDown = 2.5F;
		PlayAnimation(TZombieGirlAnimation::TDeath);

		//recorde death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);

		return;
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnAnimate(
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

	animationManagerInstance.StopAnimation(sZombieGirlAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnMove(
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

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnRotate(
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

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientZombieGirl::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientZombieGirl::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
void SceneNodeComponentClientZombieGirl::AiSelectAction(const float in_timeDelta)
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
		if (pZombieSound && LocalPartOk(arrayZombiePart, TZombieGirlPart::THead))
		{
			if (GMath::RandomUnit() < 0.3F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDHiss2(), zombiePosition);
			}
		}

		const GVector3Float playerTarget = playerPosition + (playerAt) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
		float distanceToPlayer = 0.0F;
		GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
		//GVector3Float normalToPlayer = NormaliseAprox(playerPosition - playerTarget);
		//mLerpAt->SetValue(normalToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);

		float speed = 1.5F;

		//select animation, get speed
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TZombieGirlMovementAvaliable::TDrag:
			speed = 0.5F;
			break;
		case TZombieGirlMovementAvaliable::TImobile:
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
		bool drag = (TZombieGirlMovementAvaliable::TDrag == mMovementAvaliable);

		int attackAnimation = TZombieGirlAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if ((mAttackAvaliable & TZombieGirlAttackAvaliable::TMouth) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieGirlAnimation::TDragAttackBite : TZombieGirlAnimation::TAttackBite;
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}
		else if ((mAttackAvaliable & TZombieGirlAttackAvaliable::TLeftHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieGirlAnimation::TDragAttackLeft : TZombieGirlAnimation::TAttackLeft;
			damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
		}
		else if ((mAttackAvaliable & TZombieGirlAttackAvaliable::TRightHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieGirlAnimation::TDragAttackRight : TZombieGirlAnimation::TAttackRight;
			damageType = ZombieGuiDamage::GetDamageTypeSlashRight();
		}

		if (TZombieGirlAnimation::TCount != attackAnimation)
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

void SceneNodeComponentClientZombieGirl::SelectAnimation()
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
		case TZombieGirlMovementAvaliable::TFeet:
			PlayAnimation(TZombieGirlAnimation::TWalk);
			break;
		case TZombieGirlMovementAvaliable::TDrag:
			PlayAnimation(TZombieGirlAnimation::TDrag);
			break;
		case TZombieGirlMovementAvaliable::TImobile:
			PlayAnimation(TZombieGirlAnimation::TDeath);
			break;
		}
	}
	else
	//select a idle animation
	{
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TZombieGirlAnimation::TDragIdle);
			break;
		case TZombieGirlMovementAvaliable::TFeet:
			PlayAnimation(TZombieGirlAnimation::TIdle);
			break;
		}
	}

	return;
}

void SceneNodeComponentClientZombieGirl::PlayAnimation(const int in_animation, const bool in_force)
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
	animationManagerInstance.StopAnimation(sZombieGirlAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TZombieGirlAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sZombieGirlAnimationRate[mCurrentAnimation],
			sZombieGirlAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sZombieGirlAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
