//
//  SceneNodeComponentClientSkeletonOne.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientSkeletonOne.h"

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
static const float sSkeletonOneHealthFactor = 1.0F;

//softbinding names
struct TSkeletonOnePart
{
	enum TEnum
	{
		TSpine,
		THead,
		TBucket,
		THip,
		TLeftTorso,
		TLeftArm,
		TLeftForeArm,
		TLeftHand,
		TLeftLeg,
		TLeftCalf,
		TLeftFoot,
		TRightTorso,
		TRightArm,
		TRightForeArm,
		TRightHand,
		TRightLeg,
		TRightCalf,
		TRightFoot,
		TSheild,
		TCount
	};
};

static const char* const sSkeletonOnePartName[TSkeletonOnePart::TCount] = {
	"spine",
	"head",
	"bucket",
	"hip",
	"lefttorso",
	"leftarm",
	"leftforearm",
	"lefthand",
	"leftleg",
	"leftcalf",
	"leftfoot",
	"righttorso",
	"rightarm",
	"rightforearm",
	"righthand",
	"rightleg",
	"rightcalf",
	"rightfoot",
	"sheild"
};
/**/

struct TSkeletonOneAnimation
{
	enum TEnum
	{
		TDeath = 0,
		TIdle,
		TWalk,
		THopLeft,
		THopRight,
		TRecoil,
		TAttackLeft,
		TAttackRight,
		TAttackBite,
		TDragIdle,
		TDrag,
		TDragLeft,
		TDragRight,
		TDragRecoil,
		TDragAttackLeft,
		TDragAttackRight,
		TDragAttackBite,
		TShieldIdle,
		TShieldRecoil,
		TShieldWalk,
		TShieldAttackBite,
		TCount
	};
};
static const float sSkeletonOneAnimationRate[TSkeletonOneAnimation::TCount] = {
	1.0F, //TDeath = 0,
	0.75F, //TIdle,
	1.0F, //TWalk,
	0.75F, //THop_left,
	0.75F, //THop_right,
	1.0F, //TRecoil,
	0.75F, //TAttack_left,
	0.75F, //TAttack_right,
	0.75F, //TAttack_bite,
	0.5F, //TDrag_idle,
	0.75F, //TDrag,
	0.5F, //TDrag_left,
	0.5F, //TDrag_right,
	1.0F, //TDrag_recoil,
	0.75F, //TDrag_attack_left,
	0.75F,	//TDrag_attack_right,
	0.75F,	//TDrag_attack_bite,
	1.0F,	//TShieldIdle
	1.0F,	//TShieldRecoil
	1.0F,	//TShieldWalk
	0.75F	//TShieldAttackBite
};

static const char* const sSkeletonOneAnimationName[TSkeletonOneAnimation::TCount] = {
	"death",
	"idle",
	"walk",
	"hop_left",
	"hop_right",
	"recoil",
	"attack_left",
	"attack_right",
	"attack_bite",
	"drag_idle",
	"drag",
	"drag_left",
	"drag_right",
	"drag_recoil",
	"drag_attack_left",
	"drag_attack_right",
	"drag_attack_bite",
	"shield_idle",
	"shield_recoil",
	"shield_walk",
	"shield_attack_bite"
};

struct TSkeletonOneMovementAvaliable
{
	enum TEnum
	{
		TShield = 0,
		TBothFeet,
		TLeftFoot,
		TRightFoot,
		TDragBothArms,
		TDragLeftArm,
		TDragRightArm,
		TImobile,
		TCount
	};
};

struct TSkeletonOneAttackAvaliable
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
typedef boost::shared_ptr<ZombieSound> TPointerZombieSound;

typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

typedef std::pair<TWeakSceneNodeComponentVisual, TWeakSceneNode> TPairWeakSceneNodeComponentVisualWeakSceneNode;
typedef std::vector<TPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayPairWeakSceneNodeComponentVisualWeakSceneNode;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

static const float sSkeletonOneAnimationDefaultFadeTime = 5.0F / 30.0F;
static const float sRecoilCountDown = 5.0F / 30.0F;
static const float sAttackCountDown = 20.0F / 30.0F;
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
	//TSpine,
	static const ZombiePartDataLoad sArraySpineData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TSpine], 1.0F}
	};
	//THead,
	static const ZombiePartDataLoad sArrayHeadData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::THead], 1.0F}
	};
	static const int sArrayHeadParentData[] = { TSkeletonOnePart::TSpine };

	//TBucket,
	static const ZombiePartDataLoad sArrayBucketData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TBucket], 1.0F}
	};
	static const int sArrayBucketParentData[] = { TSkeletonOnePart::THead };

	//THip,
	static const ZombiePartDataLoad sArrayHipData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::THip], 1.0F}
	};
	static const int sArrayHipParentData[] = { TSkeletonOnePart::TSpine };

	//TLeftTorso,
	static const ZombiePartDataLoad sArrayLeftTorsoData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftTorso], 1.0F}
	};
	static const int sArrayLeftTorsoParentData[] = { TSkeletonOnePart::TSpine };

	//TLeftArm,
	static const ZombiePartDataLoad sArrayLeftArmData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftArm], 1.0F}
	};
	static const int sArrayLeftArmParentData[] = { TSkeletonOnePart::TLeftTorso };

	//TLeftForeArm,
	static const ZombiePartDataLoad sArrayLeftForeArmData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftForeArm], 1.0F}
	};
	static const int sArrayLeftForeArmParentData[] = { TSkeletonOnePart::TLeftArm };

	//TLeftHand,
	static const ZombiePartDataLoad sArrayLeftHandData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftHand], 1.0F}
	};
	static const int sArrayLeftHandParentData[] = { TSkeletonOnePart::TLeftForeArm };

	//TLeftLeg,
	static const ZombiePartDataLoad sArrayLeftLegData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftLeg], 1.0F}
	};
	static const int sArrayLeftLegParentData[] = { TSkeletonOnePart::THip };

	//TLeftCalf,
	static const ZombiePartDataLoad sArrayLeftCalfData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftCalf], 1.0F}
	};
	static const int sArrayLeftCalfParentData[] = { TSkeletonOnePart::TLeftLeg };

	//TLeftFoot,
	static const ZombiePartDataLoad sArrayLeftFootData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TLeftFoot], 1.0F}
	};
	static const int sArrayLeftFootParentData[] = { TSkeletonOnePart::TLeftCalf };

	//TRightTorso,
	static const ZombiePartDataLoad sArrayRightTorsoData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightTorso], 1.0F}
	};
	static const int sArrayRightTorsoParentData[] = { TSkeletonOnePart::TSpine };

	//TRightArm,
	static const ZombiePartDataLoad sArrayRightArmData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightArm], 1.0F}
	};
	static const int sArrayRightArmParentData[] = { TSkeletonOnePart::TRightTorso };

	//TRightForeArm,
	static const ZombiePartDataLoad sArrayRightForeArmData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightForeArm], 1.0F}
	};
	static const int sArrayRightForeArmParentData[] = { TSkeletonOnePart::TRightArm };

	//TRightHand,
	static const ZombiePartDataLoad sArrayRightHandData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightHand], 1.0F}
	};
	static const int sArrayRightHandParentData[] = { TSkeletonOnePart::TRightForeArm };

	//TRightLeg,
	static const ZombiePartDataLoad sArrayRightLegData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightLeg], 1.0F}
	};
	static const int sArrayRightLegParentData[] = { TSkeletonOnePart::THip };

	//TRightCalf,
	static const ZombiePartDataLoad sArrayRightCalfData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightCalf], 1.0F}
	};
	static const int sArrayRightCalfParentData[] = { TSkeletonOnePart::TRightLeg };

	//TRightFoot,
	static const ZombiePartDataLoad sArrayRightFootData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TRightFoot], 1.0F}
	};
	static const int sArrayRightFootParentData[] = { TSkeletonOnePart::TRightCalf };

	//TSheild,
	static const ZombiePartDataLoad sArraySheildData[] = {
		{"", 0.0F}, 
		{sSkeletonOnePartName[TSkeletonOnePart::TSheild], 1.0F}
	};
	static const int sArraySheildParentData[] = { TSkeletonOnePart::TLeftHand };

	static const ZombiePartLoad sArrayPart[TSkeletonOnePart::TCount] = 
	{
		{ sSkeletonOnePartName[TSkeletonOnePart::TSpine], GCOMMON_ARRAY_SIZE(sArraySpineData), sArraySpineData, 0, 0 },
		{ sSkeletonOnePartName[TSkeletonOnePart::THead], GCOMMON_ARRAY_SIZE(sArrayHeadData), sArrayHeadData, GCOMMON_ARRAY_SIZE(sArrayHeadParentData), sArrayHeadParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TBucket], GCOMMON_ARRAY_SIZE(sArrayBucketData), sArrayBucketData, GCOMMON_ARRAY_SIZE(sArrayBucketParentData), sArrayBucketParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::THip], GCOMMON_ARRAY_SIZE(sArrayHipData), sArrayHipData, GCOMMON_ARRAY_SIZE(sArrayHipParentData), sArrayHipParentData },
	
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftTorso], GCOMMON_ARRAY_SIZE(sArrayLeftTorsoData), sArrayLeftTorsoData, GCOMMON_ARRAY_SIZE(sArrayLeftTorsoParentData), sArrayLeftTorsoParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftArm], GCOMMON_ARRAY_SIZE(sArrayLeftArmData), sArrayLeftArmData, GCOMMON_ARRAY_SIZE(sArrayLeftArmParentData), sArrayLeftArmParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftForeArm], GCOMMON_ARRAY_SIZE(sArrayLeftForeArmData), sArrayLeftForeArmData, GCOMMON_ARRAY_SIZE(sArrayLeftForeArmParentData), sArrayLeftForeArmParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftHand], GCOMMON_ARRAY_SIZE(sArrayLeftHandData), sArrayLeftHandData, GCOMMON_ARRAY_SIZE(sArrayLeftHandParentData), sArrayLeftHandParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftLeg], GCOMMON_ARRAY_SIZE(sArrayLeftLegData), sArrayLeftLegData, GCOMMON_ARRAY_SIZE(sArrayLeftLegParentData), sArrayLeftLegParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftCalf], GCOMMON_ARRAY_SIZE(sArrayLeftCalfData), sArrayLeftCalfData, GCOMMON_ARRAY_SIZE(sArrayLeftCalfParentData), sArrayLeftCalfParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TLeftFoot], GCOMMON_ARRAY_SIZE(sArrayLeftFootData), sArrayLeftFootData, GCOMMON_ARRAY_SIZE(sArrayLeftFootParentData), sArrayLeftFootParentData },

		{ sSkeletonOnePartName[TSkeletonOnePart::TRightTorso], GCOMMON_ARRAY_SIZE(sArrayRightTorsoData), sArrayRightTorsoData, GCOMMON_ARRAY_SIZE(sArrayRightTorsoParentData), sArrayRightTorsoParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightArm], GCOMMON_ARRAY_SIZE(sArrayRightArmData), sArrayRightArmData, GCOMMON_ARRAY_SIZE(sArrayRightArmParentData), sArrayRightArmParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightForeArm], GCOMMON_ARRAY_SIZE(sArrayRightForeArmData), sArrayRightForeArmData, GCOMMON_ARRAY_SIZE(sArrayRightForeArmParentData), sArrayRightForeArmParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightHand], GCOMMON_ARRAY_SIZE(sArrayRightHandData), sArrayRightHandData, GCOMMON_ARRAY_SIZE(sArrayRightHandParentData), sArrayRightHandParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightLeg], GCOMMON_ARRAY_SIZE(sArrayRightLegData), sArrayRightLegData, GCOMMON_ARRAY_SIZE(sArrayRightLegParentData), sArrayRightLegParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightCalf], GCOMMON_ARRAY_SIZE(sArrayRightCalfData), sArrayRightCalfData, GCOMMON_ARRAY_SIZE(sArrayRightCalfParentData), sArrayRightCalfParentData },
		{ sSkeletonOnePartName[TSkeletonOnePart::TRightFoot], GCOMMON_ARRAY_SIZE(sArrayRightFootData), sArrayRightFootData, GCOMMON_ARRAY_SIZE(sArrayRightFootParentData), sArrayRightFootParentData },

		{ sSkeletonOnePartName[TSkeletonOnePart::TSheild], GCOMMON_ARRAY_SIZE(sArraySheildData), sArraySheildData, GCOMMON_ARRAY_SIZE(sArraySheildParentData), sArraySheildParentData }
	};

	static const ZombieComponentLoad sZombieComponentLoad(
		GCOMMON_ARRAY_SIZE(sArrayPart),
		sArrayPart,

		0,
		0,
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
	in_arrayZombiePart[TSkeletonOnePart::TLeftTorso]->SetHealth(in_arrayZombiePart[TSkeletonOnePart::TLeftTorso]->GetHealth() * 2.0F);
	in_arrayZombiePart[TSkeletonOnePart::TRightTorso]->SetHealth(in_arrayZombiePart[TSkeletonOnePart::TRightTorso]->GetHealth() * 2.0F);
	in_arrayZombiePart[TSkeletonOnePart::TBucket]->SetHealth(in_arrayZombiePart[TSkeletonOnePart::TBucket]->GetHealth() * 5.0F);
	in_arrayZombiePart[TSkeletonOnePart::TSheild]->SetHealth(in_arrayZombiePart[TSkeletonOnePart::TSheild]->GetHealth() * 10.0F);

	return;
}

//0.0 < health of part 
static const bool LocalPartOk(const TArrayZombiePart& in_arrayZombiePart, const TSkeletonOnePart::TEnum in_part)
{
	const bool result = (0.0F < in_arrayZombiePart[in_part]->GetHealth());
	return result;
}

static int LocalUpdateMovementAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	const bool shield = LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TSheild);
	const bool leftFoot = LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TLeftFoot);
	const bool rightFoot = LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TRightFoot);
	const bool leftArm = LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TLeftArm);
	const bool rightArm = LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TRightArm);

	if (shield)
	{
		return TSkeletonOneMovementAvaliable::TShield;
	}
	else if (leftFoot && rightFoot)
	{
		return TSkeletonOneMovementAvaliable::TBothFeet;
	} 
	else if (leftFoot)
	{
		return TSkeletonOneMovementAvaliable::TLeftFoot;
	}
	else if (rightFoot)
	{
		return TSkeletonOneMovementAvaliable::TRightFoot;
	}
	else if (leftArm && rightArm)
	{
		return TSkeletonOneMovementAvaliable::TDragBothArms;
	}
	else if (leftArm)
	{
		return TSkeletonOneMovementAvaliable::TDragLeftArm;
	}
	else if (rightArm)
	{
		return TSkeletonOneMovementAvaliable::TDragRightArm;
	}

	return TSkeletonOneMovementAvaliable::TImobile;
}

static int LocalUpdateAttackAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	int result = 0;
	if (LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::THead))
	{
		result |= TSkeletonOneAttackAvaliable::TMouth;
	}
	if ( LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TLeftArm))
	{
		result |= TSkeletonOneAttackAvaliable::TLeftHand;
	}
	if ( LocalPartOk(in_arrayZombiePart, TSkeletonOnePart::TRightArm))
	{
		result |= TSkeletonOneAttackAvaliable::TRightHand;
	}

	return result;
}


//static public methods
/*static*/ SceneNodeComponentClientSkeletonOne::TPointerSceneNodeComponentBase SceneNodeComponentClientSkeletonOne::Factory(
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
	const float healthFactor = zombieGameVar.GetZombieHealthFactor() * sSkeletonOneHealthFactor;
	const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientSkeletonOne pointerSkeletonOne;
	pointerSkeletonOne.reset(new SceneNodeComponentClientSkeletonOne(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor
		));

	pointer = pointerSkeletonOne;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientSkeletonOne::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerSkeletonOne;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientSkeletonOne::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientSkeletonOne pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientSkeletonOne>(inout_component);
	if (!pointer)
	{
		return;
	}
	SceneNodeComponentClientSkeletonOne& skeletonOne = *pointer;

	//recursivly hide all visual components. this would be better at constructor scope, but we dont have the pointer in the constructor...
	//GSceneNodeUtility::RecusiveSetVisualComponentVisible(inout_owner, false);

	skeletonOne.mZombieComponent = ZombieComponent::Factory(
		LocalGetZombieComponentLoad(),
		inout_owner,
		skeletonOne.mParentMission.GetGore(),
		skeletonOne.mHealthFactor
		);

	ZombieGameVar& zombieGameVar = *(skeletonOne.mParentMission.GetGameVar());
	TArrayZombiePart& arrayZombiePart = skeletonOne.mZombieComponent->GetArrayZombiePart();
	LocalDoctorPartArray(
		skeletonOne.mFactoryData, 
		arrayZombiePart,
		zombieGameVar
		);
	skeletonOne.mMovementAvaliable = LocalUpdateMovementAvaliable(arrayZombiePart);
	skeletonOne.mAttackAvaliable = LocalUpdateAttackAvaliable(arrayZombiePart);

	//this is a bit wack, so don't do it? damn, easy way of deleting visual is to transfer them to a local scope scene node?
	// could add to part detach something to delete or hide visual...
	TPointerSceneNode dummySceneNode;
	dummySceneNode.reset(new GSceneNode(""));
	skeletonOne.OnAfterDamagePass(inout_scene, dummySceneNode);

	return;
}

//constructor
SceneNodeComponentClientSkeletonOne::SceneNodeComponentClientSkeletonOne(
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
	, mDeathCountDown(0.0F)
	, mRecoilCountdown(0.0F)
	, mAttackCountdown(0.0F)
	, mMovementAvaliable(TSkeletonOneMovementAvaliable::TCount)
	, mAttackAvaliable(TSkeletonOneAttackAvaliable::TNone)
	, mCurrentAnimation(TSkeletonOneAnimation::TCount)
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

/*virtual*/ SceneNodeComponentClientSkeletonOne::~SceneNodeComponentClientSkeletonOne()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
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
/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnCollectDamageReceivers(
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

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnApplyDamage(
	float& inout_damage, 
	const std::string& in_data
	)
{
	bool partLost = false;

	if (mZombieComponent)
	{
		if (LocalPartOk(mZombieComponent->GetArrayZombiePart(), TSkeletonOnePart::TSheild))
		{
			partLost = mZombieComponent->ApplyDamage(
				inout_damage, 
				"sheild"
				);
		}
		else
		{
			partLost = mZombieComponent->ApplyDamage(
				inout_damage, 
				in_data	
				);
		}
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
			PlayAnimation(TSkeletonOneAnimation::TDragRecoil, true);
			break;
		case TSkeletonOneMovementAvaliable::TShield:
			PlayAnimation(TSkeletonOneAnimation::TShieldRecoil, true);
			break;
		case TSkeletonOneMovementAvaliable::TBothFeet:
		case TSkeletonOneMovementAvaliable::TLeftFoot:
		case TSkeletonOneMovementAvaliable::TRightFoot:
			PlayAnimation(TSkeletonOneAnimation::TRecoil, true);
			break;
		}
	}
	return;
}

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
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

	//trigger death?
	if ((oldMovementAvaliable != mMovementAvaliable) && 
		(mMovementAvaliable == TSkeletonOneMovementAvaliable::TImobile) &&
		//(mAttackAvaliable == TSkeletonOneAttackAvaliable::TNone) &&
		(!mDead))
	{
		mDead = true;
		mDeathCountDown = 2.5F;
		PlayAnimation(TSkeletonOneAnimation::TDeath);

		TPointerZombieSound pZombieSound = mParentMission.GetSound();
		if (pZombieSound)
		{
			pZombieSound->Play(ZombieSound::GetSoundIDSkeletonDeath0(), mLerpPosition->GetValue());
		}

		//recode death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);

		return;
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnAnimate(
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

	animationManagerInstance.StopAnimation(sSkeletonOneAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnMove(
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

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnRotate(
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

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientSkeletonOne::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientSkeletonOne::OnDestroy()
{
	TPointerSceneNode sceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(sceneNode);

	return;
}

//private methods
void SceneNodeComponentClientSkeletonOne::AiSelectAction(const float in_timeDelta)
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
		if (pZombieSound && LocalPartOk(arrayZombiePart, TSkeletonOnePart::THead))
		{
			if (GMath::RandomUnit() < 0.1F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDHiss0(), mLerpPosition->GetValue());
			}
		}

		const GVector3Float playerTarget = playerPosition + (playerAt) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
		float distanceToPlayer = 0.0F;
		GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
		//GVector3Float normalToPlayer = NormaliseAprox(playerPosition - playerTarget);
		//mLerpAt->SetValue(normalToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);

		float speed = 1.0F;

		//select animation, get speed
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TSkeletonOneMovementAvaliable::TShield:
			speed = 0.5F;
			break;
		case TSkeletonOneMovementAvaliable::TLeftFoot:
		case TSkeletonOneMovementAvaliable::TRightFoot:
			speed = 0.75F;
			break;
		case TSkeletonOneMovementAvaliable::TDragBothArms:
			speed = 0.5F;
			break;
		case TSkeletonOneMovementAvaliable::TDragRightArm:
		case TSkeletonOneMovementAvaliable::TDragLeftArm:
			speed = 0.25F;
			break;
		case TSkeletonOneMovementAvaliable::TImobile:
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
		int attackAnimation = TSkeletonOneAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if ((mAttackAvaliable & TSkeletonOneAttackAvaliable::TMouth) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			switch (mMovementAvaliable)
			{
			default:
				break;
			case TSkeletonOneMovementAvaliable::TShield:
				attackAnimation = TSkeletonOneAnimation::TShieldAttackBite;
				break;
			case TSkeletonOneMovementAvaliable::TLeftFoot:
			case TSkeletonOneMovementAvaliable::TRightFoot:
			case TSkeletonOneMovementAvaliable::TBothFeet:
				attackAnimation = TSkeletonOneAnimation::TAttackBite;
				break;
			case TSkeletonOneMovementAvaliable::TDragBothArms:
			case TSkeletonOneMovementAvaliable::TDragLeftArm:
			case TSkeletonOneMovementAvaliable::TDragRightArm:
				attackAnimation = TSkeletonOneAnimation::TDragAttackBite;
				break;
			}
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}
		else if ((mAttackAvaliable & TSkeletonOneAttackAvaliable::TLeftHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			switch (mMovementAvaliable)
			{
			default:
				break;
			case TSkeletonOneMovementAvaliable::TLeftFoot:
			case TSkeletonOneMovementAvaliable::TRightFoot:
			case TSkeletonOneMovementAvaliable::TBothFeet:
				attackAnimation = TSkeletonOneAnimation::TAttackLeft;
				break;
			case TSkeletonOneMovementAvaliable::TDragBothArms:
			case TSkeletonOneMovementAvaliable::TDragLeftArm:
			case TSkeletonOneMovementAvaliable::TDragRightArm:
				attackAnimation = TSkeletonOneAnimation::TDragAttackLeft;
				break;
			}
			damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
		}
		else if ((mAttackAvaliable & TSkeletonOneAttackAvaliable::TRightHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			switch (mMovementAvaliable)
			{
			default:
				break;
			case TSkeletonOneMovementAvaliable::TLeftFoot:
			case TSkeletonOneMovementAvaliable::TRightFoot:
			case TSkeletonOneMovementAvaliable::TBothFeet:
				attackAnimation = TSkeletonOneAnimation::TAttackRight;
				break;
			case TSkeletonOneMovementAvaliable::TDragBothArms:
			case TSkeletonOneMovementAvaliable::TDragLeftArm:
			case TSkeletonOneMovementAvaliable::TDragRightArm:
				attackAnimation = TSkeletonOneAnimation::TDragAttackRight;
				break;
			}
			damageType = ZombieGuiDamage::GetDamageTypeSlashRight();
		}

		if (TSkeletonOneAnimation::TCount != attackAnimation)
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

void SceneNodeComponentClientSkeletonOne::SelectAnimation()
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
		case TSkeletonOneMovementAvaliable::TShield:
			PlayAnimation(TSkeletonOneAnimation::TShieldWalk);
			break;
		case TSkeletonOneMovementAvaliable::TBothFeet:
			PlayAnimation(TSkeletonOneAnimation::TWalk);
			break;
		case TSkeletonOneMovementAvaliable::TLeftFoot:
			PlayAnimation(TSkeletonOneAnimation::THopLeft);
			break;
		case TSkeletonOneMovementAvaliable::TRightFoot:
			PlayAnimation(TSkeletonOneAnimation::THopRight);
			break;
		case TSkeletonOneMovementAvaliable::TDragBothArms:
			PlayAnimation(TSkeletonOneAnimation::TDrag);
			break;
		case TSkeletonOneMovementAvaliable::TDragLeftArm:
			PlayAnimation(TSkeletonOneAnimation::TDragLeft);
			break;
		case TSkeletonOneMovementAvaliable::TDragRightArm:
			PlayAnimation(TSkeletonOneAnimation::TDragRight);
			break;
		case TSkeletonOneMovementAvaliable::TImobile:
			PlayAnimation(TSkeletonOneAnimation::TDeath);
			break;
		}
	}
	else
	//select a idle animation
	{
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TSkeletonOneAnimation::TDragIdle);
			break;
		case TSkeletonOneMovementAvaliable::TShield:
			PlayAnimation(TSkeletonOneAnimation::TShieldIdle);
			break;
		case TSkeletonOneMovementAvaliable::TBothFeet:
		case TSkeletonOneMovementAvaliable::TLeftFoot:
		case TSkeletonOneMovementAvaliable::TRightFoot:
			PlayAnimation(TSkeletonOneAnimation::TIdle);
			break;
		}
	}

	return;
}

void SceneNodeComponentClientSkeletonOne::PlayAnimation(const int in_animation, const bool in_force)
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
	animationManagerInstance.StopAnimation(sSkeletonOneAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TSkeletonOneAnimation::TCount))
	{
		const GAnimationPlayParam defaultParam(
			sSkeletonOneAnimationRate[mCurrentAnimation],
			sSkeletonOneAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sSkeletonOneAnimationName[mCurrentAnimation],
			defaultParam
			);
	}

	return;
}
