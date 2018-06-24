//
//  SceneNodeComponentClientZombieOne.cpp
//  Zombie
//
//  Created by David Coen on 2011 01 29
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "SceneNodeComponentClientZombieOne.h"

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
//softbinding names
struct TZombieOnePart
{
	enum TEnum
	{
		TNeck, //				neck		
		THead, //				head
		TLeftTorso, //			lefttorso
		TLeftArm, //			leftarm			
		TLeftHand, //			lefthand		
		//TLefthanddamaged		lefthanddamaged	
		TLeftWaist, //			leftwaist
		TLeftHip, //			lefthip			
		TLeftKnee, //			leftknee		
		TLeftFoot, //			leftfoot		
		//TLeftfootdamaged		leftfootdamaged	
		TRightTorso, //			righttorso
		TRightArm, //			rightarm	
		TRightHand, //			righthand		
		//TRighthanddamaged		righthanddamaged
		TRightWaist, //			rightwaist
		TRightHip, //			righthip	
		TRightKnee, //			rightknee		
		TRightFoot, //			rightfoot		
		//TRightfootdamaged		rightfootdamaged
		TCount
	};
};

static const char* const sZombieOnePartName[TZombieOnePart::TCount] = {
	"neck",
	"head",
	"lefttorso",
	"leftarm",			
	"lefthand",
	//"lefthanddamaged",
	"leftwaist",
	"lefthip",			
	"leftknee",		
	"leftfoot",
	//"leftfootdamaged",
	"righttorso",
	"rightarm",	
	"righthand",		
	//"righthanddamaged",
	"rightwaist",
	"righthip",
	"rightknee",		
	"rightfoot",
	//"rightfootdamaged"
};
/**/

struct TZombieOneAnimation
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
		TCount
	};
};
static const float sZombieOneAnimationRate[TZombieOneAnimation::TCount] = {
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
	0.75F, //TDrag_attack_right,
	0.75F  //TDrag_attack_bite,
};

static const char* const sZombieOneAnimationName[TZombieOneAnimation::TCount] = {
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
	"drag_attack_bite"
};

struct TZombieOneMovementAvaliable
{
	enum TEnum
	{
		TImobile = 0,
		TBothFeet,
		TLeftFoot,
		TRightFoot,
		TDragBothArms,
		TDragLeftArm,
		TDragRightArm,
		TCount
	};
};

struct TZombieOneAttackAvaliable
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

static const float sZombieOneAnimationDefaultFadeTime = 5.0F / 30.0F;
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
	//head
	static const ZombiePartDataLoad sArrayHeadData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::THead], 1.0F}
	};
	static const int sArrayHeadParentData[] = { TZombieOnePart::TNeck };
	//neck
	static const ZombiePartDataLoad sArrayNeckData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::TNeck], 1.0F}
	};
	static const int sArrayNeckParentData[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso };
	//LeftTorso
	static const ZombiePartDataLoad sArrayLeftTorsoData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::TLeftTorso], 1.0F}
	};
	//leftarm
	static const ZombiePartDataLoad sArrayLeftArmData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::TLeftArm], 1.0F}
	};
	static const int sArrayLeftArmParentData[] = { TZombieOnePart::TLeftTorso };
	//lefthand
	static const ZombiePartDataLoad sArrayLeftHandData[] = {
		{"", 0.0F}, 		
		{"lefthanddamaged", 0.5F}, 
		{sZombieOnePartName[TZombieOnePart::TLeftHand], 1.0F}
	};
	static const int sArrayLeftHandParentData[] = { TZombieOnePart::TLeftArm };
	//leftwaist
	static const ZombiePartDataLoad sArrayLeftWaistData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TLeftWaist], 1.0F}
	};
	static const int sArrayLeftWaistParentData[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso, TZombieOnePart::TLeftHip, TZombieOnePart::TLeftHip };
	//lefthip
	static const ZombiePartDataLoad sArrayLeftHipData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TLeftHip], 1.0F}
	};
	//leftknee
	static const ZombiePartDataLoad sArrayLeftKneeData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TLeftKnee], 1.0F}
	};
	static const int sArrayLeftKneeParentData[] = { TZombieOnePart::TLeftHip };
	//leftfoot
	static const ZombiePartDataLoad sArrayLeftFootData[] = {
		{"", 0.0F}, 		
		{"leftfootdamaged", 0.5F}, 		
		{sZombieOnePartName[TZombieOnePart::TLeftFoot], 1.0F}
	};
	static const int sArrayLeftFootParentData[] = { TZombieOnePart::TLeftKnee };

	//rightTorso
	static const ZombiePartDataLoad sArrayRightTorsoData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::TRightTorso], 1.0F}
	};
	//rightarm
	static const ZombiePartDataLoad sArrayRightArmData[] = {
		{"", 0.0F}, 
		{sZombieOnePartName[TZombieOnePart::TRightArm], 1.0F}
	};
	static const int sArrayRightArmParentData[] = { TZombieOnePart::TRightTorso };
	//righthand
	static const ZombiePartDataLoad sArrayRightHandData[] = {
		{"", 0.0F}, 		
		{"righthanddamaged", 0.5F}, 
		{sZombieOnePartName[TZombieOnePart::TRightHand], 1.0F}
	};
	static const int sArrayRightHandParentData[] = { TZombieOnePart::TRightArm };
	//rightwaist
	static const ZombiePartDataLoad sArrayRightWaistData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TRightWaist], 1.0F}
	};
	static const int sArrayRightWaistParentData[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso, TZombieOnePart::TLeftHip, TZombieOnePart::TLeftHip };
	//righthip
	static const ZombiePartDataLoad sArrayRightHipData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TRightHip], 1.0F}
	};
	//rightknee
	static const ZombiePartDataLoad sArrayRightKneeData[] = {
		{"", 0.0F}, 		
		{sZombieOnePartName[TZombieOnePart::TRightKnee], 1.0F}
	};
	static const int sArrayRightKneeParentData[] = { TZombieOnePart::TRightHip };
	//rightfoot
	static const ZombiePartDataLoad sArrayRightFootData[] = {
		{"", 0.0F}, 		
		{"rightfootdamaged", 0.5F}, 		
		{sZombieOnePartName[TZombieOnePart::TRightFoot], 1.0F}
	};
	static const int sArrayRightFootParentData[] = { TZombieOnePart::TRightKnee };



	static const ZombiePartLoad sArrayPart[TZombieOnePart::TCount] = 
	{
		{ sZombieOnePartName[TZombieOnePart::TNeck], GCOMMON_ARRAY_SIZE(sArrayNeckData), sArrayNeckData, GCOMMON_ARRAY_SIZE(sArrayNeckParentData), sArrayNeckParentData },
		{ sZombieOnePartName[TZombieOnePart::THead], GCOMMON_ARRAY_SIZE(sArrayHeadData), sArrayHeadData, GCOMMON_ARRAY_SIZE(sArrayHeadParentData), sArrayHeadParentData },
		{ sZombieOnePartName[TZombieOnePart::TLeftTorso], GCOMMON_ARRAY_SIZE(sArrayLeftTorsoData), sArrayLeftTorsoData, 0, 0 },
		{ sZombieOnePartName[TZombieOnePart::TLeftArm], GCOMMON_ARRAY_SIZE(sArrayLeftArmData), sArrayLeftArmData, GCOMMON_ARRAY_SIZE(sArrayLeftArmParentData), sArrayLeftArmParentData },
		{ sZombieOnePartName[TZombieOnePart::TLeftHand], GCOMMON_ARRAY_SIZE(sArrayLeftHandData), sArrayLeftHandData, GCOMMON_ARRAY_SIZE(sArrayLeftHandParentData), sArrayLeftHandParentData },
		{ sZombieOnePartName[TZombieOnePart::TLeftWaist], GCOMMON_ARRAY_SIZE(sArrayLeftWaistData), sArrayLeftWaistData, GCOMMON_ARRAY_SIZE(sArrayLeftWaistParentData), sArrayLeftWaistParentData },
		{ sZombieOnePartName[TZombieOnePart::TLeftHip], GCOMMON_ARRAY_SIZE(sArrayLeftHipData), sArrayLeftHipData, 0, 0 },
		{ sZombieOnePartName[TZombieOnePart::TLeftKnee], GCOMMON_ARRAY_SIZE(sArrayLeftKneeData), sArrayLeftKneeData, GCOMMON_ARRAY_SIZE(sArrayLeftKneeParentData), sArrayLeftKneeParentData },
		{ sZombieOnePartName[TZombieOnePart::TLeftFoot], GCOMMON_ARRAY_SIZE(sArrayLeftFootData), sArrayLeftFootData, GCOMMON_ARRAY_SIZE(sArrayLeftFootParentData), sArrayLeftFootParentData },
		{ sZombieOnePartName[TZombieOnePart::TRightTorso], GCOMMON_ARRAY_SIZE(sArrayRightTorsoData), sArrayRightTorsoData, 0, 0 },
		{ sZombieOnePartName[TZombieOnePart::TRightArm], GCOMMON_ARRAY_SIZE(sArrayRightArmData), sArrayRightArmData, GCOMMON_ARRAY_SIZE(sArrayRightArmParentData), sArrayRightArmParentData },
		{ sZombieOnePartName[TZombieOnePart::TRightHand], GCOMMON_ARRAY_SIZE(sArrayRightHandData), sArrayRightHandData, GCOMMON_ARRAY_SIZE(sArrayRightHandParentData), sArrayRightHandParentData },
		{ sZombieOnePartName[TZombieOnePart::TRightWaist], GCOMMON_ARRAY_SIZE(sArrayRightWaistData), sArrayRightWaistData, GCOMMON_ARRAY_SIZE(sArrayRightWaistParentData), sArrayRightWaistParentData },
		{ sZombieOnePartName[TZombieOnePart::TRightHip], GCOMMON_ARRAY_SIZE(sArrayRightHipData), sArrayRightHipData, 0, 0 },
		{ sZombieOnePartName[TZombieOnePart::TRightKnee], GCOMMON_ARRAY_SIZE(sArrayRightKneeData), sArrayRightKneeData, GCOMMON_ARRAY_SIZE(sArrayRightKneeParentData), sArrayRightKneeParentData },
		{ sZombieOnePartName[TZombieOnePart::TRightFoot], GCOMMON_ARRAY_SIZE(sArrayRightFootData), sArrayRightFootData, GCOMMON_ARRAY_SIZE(sArrayRightFootParentData), sArrayRightFootParentData }
	};

	static const int sArrayParticleHeadAlive[] = { TZombieOnePart::TNeck };
	static const int sArrayParticleHeadDead[] = { TZombieOnePart::THead};
	static const int sArrayParticleNeckAlive[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso };
	static const int sArrayParticleNeckDead[] = { TZombieOnePart::TNeck};
	static const int sArrayParticleTorso[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso, TZombieOnePart::TLeftWaist, TZombieOnePart::TRightWaist };
	static const int sArrayParticleWaist[] = { TZombieOnePart::TLeftWaist, TZombieOnePart::TRightWaist, TZombieOnePart::TLeftHip, TZombieOnePart::TRightHip };
	static const int sArrayParticleLeftKneeAlive[] = { TZombieOnePart::TLeftHip };
	static const int sArrayParticleLeftKneeDead[] = { TZombieOnePart::TLeftKnee };
	static const int sArrayParticleRightKneeAlive[] = { TZombieOnePart::TRightHip };
	static const int sArrayParticleRightKneeDead[] = { TZombieOnePart::TRightKnee };
	static const int sArrayParticleLeftArmAlive[] = { TZombieOnePart::TLeftTorso };
	static const int sArrayParticleLeftArmDead[] = { TZombieOnePart::TLeftArm };
	static const int sArrayParticleRightArmAlive[] = { TZombieOnePart::TRightTorso };
	static const int sArrayParticleRightArmDead[] = { TZombieOnePart::TRightArm };

	static const ZombieParticleLoad sArrayParticle[] = 
	{
		{ "head", 0, GCOMMON_ARRAY_SIZE(sArrayParticleHeadAlive), sArrayParticleHeadAlive, GCOMMON_ARRAY_SIZE(sArrayParticleHeadDead), sArrayParticleHeadDead },
		{ "neck", 0, GCOMMON_ARRAY_SIZE(sArrayParticleNeckAlive), sArrayParticleNeckAlive, GCOMMON_ARRAY_SIZE(sArrayParticleNeckDead), sArrayParticleNeckDead },
		{ "torso", 1, GCOMMON_ARRAY_SIZE(sArrayParticleTorso), sArrayParticleTorso, GCOMMON_ARRAY_SIZE(sArrayParticleTorso), sArrayParticleTorso },
		{ "waist", 1, GCOMMON_ARRAY_SIZE(sArrayParticleWaist), sArrayParticleWaist, GCOMMON_ARRAY_SIZE(sArrayParticleWaist), sArrayParticleWaist },
		{ "leftknee", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftKneeAlive), sArrayParticleLeftKneeAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftKneeDead), sArrayParticleLeftKneeDead },
		{ "rightknee", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightKneeAlive), sArrayParticleRightKneeAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightKneeDead), sArrayParticleRightKneeDead },
		{ "leftarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmAlive), sArrayParticleLeftArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleLeftArmDead), sArrayParticleLeftArmDead },
		{ "rightarm", 1, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmAlive), sArrayParticleRightArmAlive, GCOMMON_ARRAY_SIZE(sArrayParticleRightArmDead), sArrayParticleRightArmDead }
	};


	static const int sArrayCutInHalfDead[] = { TZombieOnePart::TLeftWaist, TZombieOnePart::TRightWaist };
	static const int sArrayCutInHalfAlive[] = { TZombieOnePart::TLeftTorso, TZombieOnePart::TRightTorso };
	static const int sArrayCutInHalfPart[] = { TZombieOnePart::TLeftHip, TZombieOnePart::TLeftKnee, TZombieOnePart::TLeftFoot, TZombieOnePart::TRightHip, TZombieOnePart::TRightKnee, TZombieOnePart::TRightFoot };

	static const ZombieComponentLoad sZombieComponentLoad(
		GCOMMON_ARRAY_SIZE(sArrayPart),
		sArrayPart,
		GCOMMON_ARRAY_SIZE(sArrayParticle),
		sArrayParticle,

		GCOMMON_ARRAY_SIZE(sArrayCutInHalfDead),
		sArrayCutInHalfDead,
		GCOMMON_ARRAY_SIZE(sArrayCutInHalfAlive),
		sArrayCutInHalfAlive,
		GCOMMON_ARRAY_SIZE(sArrayCutInHalfPart),
		sArrayCutInHalfPart
		);

	return sZombieComponentLoad;
}

static void LocalDoctorPartArray(const std::string& in_factoryData, TArrayZombiePart& in_arrayZombiePart, const ZombieGameVar& in_gameVar)
{
	if (in_factoryData == "random")
	{
		const float healthBase = in_gameVar.GetZombiePartHealthBase();
		const float healthRange = in_gameVar.GetZombiePartHealthRange();

		//make the arrayZombiePart
		for (int index = 0; index < TZombieOnePart::TCount; ++index)
		{
			const float health = healthBase + (GMath::RandomUnit() * healthRange);
			in_arrayZombiePart[index]->SetHealth(health);
		}
	}
	if (in_factoryData == "standard")
	{
		in_arrayZombiePart[TZombieOnePart::TLeftHand]->SetHealth(0.25F);
		in_arrayZombiePart[TZombieOnePart::TRightFoot]->SetHealth(0.25F);
	}
	else if (in_factoryData == "half")
	{
		in_arrayZombiePart[TZombieOnePart::THead]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TLeftHip]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TRightHand]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TLeftTorso]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TRightFoot]->SetHealth(0.25F);
	}
	else if (in_factoryData == "torso")
	{
		in_arrayZombiePart[TZombieOnePart::THead]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TRightWaist]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TLeftHip]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TRightHip]->SetHealth(0.0F);
		in_arrayZombiePart[TZombieOnePart::TLeftHand]->SetHealth(0.25F);
	}

	in_arrayZombiePart[TZombieOnePart::TLeftTorso]->SetHealth(in_arrayZombiePart[TZombieOnePart::TLeftTorso]->GetHealth() * 2.0F);
	in_arrayZombiePart[TZombieOnePart::TRightTorso]->SetHealth(in_arrayZombiePart[TZombieOnePart::TRightTorso]->GetHealth() * 2.0F);

	return;
}

//0.0 < health of part 
static const bool LocalPartOk(const TArrayZombiePart& in_arrayZombiePart, const TZombieOnePart::TEnum in_part)
{
	const bool result = (0.0F < in_arrayZombiePart[in_part]->GetHealth());
	return result;
}

static int LocalUpdateMovementAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	const bool leftFoot = LocalPartOk(in_arrayZombiePart, TZombieOnePart::TLeftFoot);
	const bool rightFoot = LocalPartOk(in_arrayZombiePart, TZombieOnePart::TRightFoot);
	const bool leftArm = LocalPartOk(in_arrayZombiePart, TZombieOnePart::TLeftArm);
	const bool rightArm = LocalPartOk(in_arrayZombiePart, TZombieOnePart::TRightArm);

	if (leftFoot && rightFoot)
	{
		return TZombieOneMovementAvaliable::TBothFeet;
	} 
	else if (leftFoot)
	{
		return TZombieOneMovementAvaliable::TLeftFoot;
	}
	else if (rightFoot)
	{
		return TZombieOneMovementAvaliable::TRightFoot;
	}
	else if (leftArm && rightArm)
	{
		return TZombieOneMovementAvaliable::TDragBothArms;
	}
	else if (leftArm)
	{
		return TZombieOneMovementAvaliable::TDragLeftArm;
	}
	else if (rightArm)
	{
		return TZombieOneMovementAvaliable::TDragRightArm;
	}

	return TZombieOneMovementAvaliable::TImobile;
}

static int LocalUpdateAttackAvaliable(const TArrayZombiePart& in_arrayZombiePart)
{
	int result = 0;
	if ( LocalPartOk(in_arrayZombiePart, TZombieOnePart::TNeck))
	{
		result |= TZombieOneAttackAvaliable::TMouth;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieOnePart::TLeftArm))
	{
		result |= TZombieOneAttackAvaliable::TLeftHand;
	}
	if ( LocalPartOk(in_arrayZombiePart, TZombieOnePart::TRightArm))
	{
		result |= TZombieOneAttackAvaliable::TRightHand;
	}

	return result;
}


//static public methods
/*static*/ SceneNodeComponentClientZombieOne::TPointerSceneNodeComponentBase SceneNodeComponentClientZombieOne::Factory(
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
	const float healthFactor = zombieGameVar.GetZombieHealthFactor();
	const std::string& factoryData = inout_scene.GetFactoryData();

	TPointerSceneNodeComponentClientZombieOne pointerZombieOne;
	pointerZombieOne.reset(new SceneNodeComponentClientZombieOne(
		zombieMission,
		inout_owner,
		factoryData,
		healthFactor
		));

	pointer = pointerZombieOne;

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		SceneNodeComponentClientZombieOne::PostLoadCallback
		));

	TPointerSceneNodeComponentClientObjectBase pointerObjectBase = pointerZombieOne;
	zombieMission.AddClientOpponent(*pointerObjectBase);

	return pointer;
}

//static private methods
/*static*/ void SceneNodeComponentClientZombieOne::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentClientZombieOne pointer = boost::dynamic_pointer_cast<SceneNodeComponentClientZombieOne>(inout_component);
	if (!pointer)
	{
		return;
	}
	SceneNodeComponentClientZombieOne& zombieOne = *pointer;

	//recursivly hide all visual components. this would be better at constructor scope, but we dont have the pointer in the constructor...
	//GSceneNodeUtility::RecusiveSetVisualComponentVisible(inout_owner, false);

	zombieOne.mZombieComponent = ZombieComponent::Factory(
		LocalGetZombieComponentLoad(),
		inout_owner,
		zombieOne.mParentMission.GetGore(),
		zombieOne.mHealthFactor
		);

	ZombieGameVar& zombieGameVar = *(zombieOne.mParentMission.GetGameVar());
	TArrayZombiePart& arrayZombiePart = zombieOne.mZombieComponent->GetArrayZombiePart();
	LocalDoctorPartArray(
		zombieOne.mFactoryData, 
		arrayZombiePart,
		zombieGameVar
		);
	zombieOne.mMovementAvaliable = LocalUpdateMovementAvaliable(arrayZombiePart);
	zombieOne.mAttackAvaliable = LocalUpdateAttackAvaliable(arrayZombiePart);

	//this is a bit wack, so don't do it? damn, easy way of deleting visual is to transfer them to a local scope scene node?
	// could add to part detach something to delete or hide visual...
	TPointerSceneNode dummySceneNode;
	//TPointerSceneNode dummySceneNodeParent;
	//dummySceneNode.reset(new GSceneNode(dummySceneNodeParent, ""));
	zombieOne.OnAfterDamagePass(inout_scene, dummySceneNode);

	return;
}

//constructor
SceneNodeComponentClientZombieOne::SceneNodeComponentClientZombieOne(
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
	, mMovementAvaliable(TZombieOneMovementAvaliable::TCount)
	, mAttackAvaliable(TZombieOneAttackAvaliable::TNone)
	, mCurrentAnimation(TZombieOneAnimation::TCount)
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

/*virtual*/ SceneNodeComponentClientZombieOne::~SceneNodeComponentClientZombieOne()
{
	return;
}

//implement GSceneNodeComponentClient
/*virtual*/ void SceneNodeComponentClientZombieOne::OnTick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
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
/*virtual*/ void SceneNodeComponentClientZombieOne::OnCollectDamageReceivers(
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

/*virtual*/ void SceneNodeComponentClientZombieOne::OnApplyDamage(
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

	//head hunter
	if (mFirstHit)
	{
		mFirstHit = false;
		ZombieGameVar& gameVar = *mParentMission.GetGameVar();
		if (LocalPartOk(mZombieComponent->GetArrayZombiePart(), TZombieOnePart::THead))
		{
			gameVar.SetMissHeadFirstCount(gameVar.GetMissHeadFirstCount() + 1);
		}
		else
		{
			gameVar.SetHitHeadFirstCount(gameVar.GetHitHeadFirstCount() + 1);
		}
	}

	//recoil on hit
	//if (partLost && !mDead && (sRecoilCountDown != mRecoilCountdown))
	if (!mDead && (sRecoilCountDown != mRecoilCountdown))
	{
		mAttackCountdown = 0.0F;
		mRecoilCountdown = sRecoilCountDown;
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TZombieOneAnimation::TDragRecoil, true);
			break;
		case TZombieOneMovementAvaliable::TBothFeet:
		case TZombieOneMovementAvaliable::TLeftFoot:
		case TZombieOneMovementAvaliable::TRightFoot:
			PlayAnimation(TZombieOneAnimation::TRecoil, true);
			break;
		}
	}
	return;
}

/*virtual*/ void SceneNodeComponentClientZombieOne::OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot)
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
		(mMovementAvaliable == TZombieOneMovementAvaliable::TImobile) &&
		//(mAttackAvaliable == TZombieOneAttackAvaliable::TNone) &&
		(!mDead))
	{
		mDead = true;
		mDeathCountDown = 2.5F;
		PlayAnimation(TZombieOneAnimation::TDeath);

		//recorde death
		ZombieGameVar& gameVar = *(mParentMission.GetGameVar());
		gameVar.SetPersistanceKillCount(gameVar.GetPersistanceKillCount() + 1);

		return;
	}

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieOne::OnAnimate(
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

	animationManagerInstance.StopAnimation(sZombieOneAnimationDefaultFadeTime);
	animationManagerInstance.PlayAnimation(
		in_animationName,
		in_playParam
		);

	return;
}

/*virtual*/ void SceneNodeComponentClientZombieOne::OnMove(
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

/*virtual*/ void SceneNodeComponentClientZombieOne::OnRotate(
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

/*virtual*/ void SceneNodeComponentClientZombieOne::OnSetAiActive(const bool in_active)
{
	mAiControl = in_active;
	return;
}

/*virtual*/ const bool SceneNodeComponentClientZombieOne::OnGetAiActive()const
{
	return mAiControl;
}

/*virtual*/ void SceneNodeComponentClientZombieOne::OnDestroy()
{
	TPointerSceneNode pointerSceneNode = mParentSceneNode.lock();
	mParentMission.RequestNodeDestroy(pointerSceneNode);
}

//private methods
void SceneNodeComponentClientZombieOne::AiSelectAction(const float in_timeDelta)
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
		if (pZombieSound && LocalPartOk(arrayZombiePart, TZombieOnePart::THead))
		{
			if (GMath::RandomUnit() < 0.1F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDZombieOneIdle0(), zombiePosition);
			}
			else if (GMath::RandomUnit() < 0.1F)
			{
				pZombieSound->Play(ZombieSound::GetSoundIDZombieOneIdle1(), zombiePosition);
			}
		}

		const GVector3Float playerTarget = playerPosition + (playerAt) + ((0.33F * GMath::RandomUnitPlusMinius()) * playerCross);
		float distanceToPlayer = 0.0F;
		GVector3Float normalToPlayerTarget = Normalise(playerTarget - zombiePosition, distanceToPlayer);
		//GVector3Float normalToPlayer = NormaliseAprox(playerPosition - playerTarget);
		//mLerpAt->SetValue(normalToPlayer, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpAt->SetValue(normalToPlayerTarget, 0.25F, GComponentLerp::TType::TEaseInOut);
		mLerpUp->SetValue(GVector3Float::sUnitY, 0.25F, GComponentLerp::TType::TEaseInOut);

		float speed = 0.0F;

		//select animation, get speed
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TZombieOneMovementAvaliable::TBothFeet:
			speed = 1.0F;
			break;
		case TZombieOneMovementAvaliable::TLeftFoot:
		case TZombieOneMovementAvaliable::TRightFoot:
			speed = 0.6F;
			break;
		case TZombieOneMovementAvaliable::TDragBothArms:
			speed = 0.4F;
			break;
		case TZombieOneMovementAvaliable::TDragRightArm:
		case TZombieOneMovementAvaliable::TDragLeftArm:
			speed = 0.2F;
			break;
		case TZombieOneMovementAvaliable::TImobile:
			speed = 0.0F;
			break;
		}

		if (speed)
		{
			float timeToPlayer = distanceToPlayer / speed;
			mLerpPosition->SetValue(playerTarget, timeToPlayer, GComponentLerp::TType::TLinear);
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
		bool drag = true;
		switch (mMovementAvaliable)
		{
		default:
			break;
		case TZombieOneMovementAvaliable::TBothFeet:
		case TZombieOneMovementAvaliable::TLeftFoot:
		case TZombieOneMovementAvaliable::TRightFoot:
			drag = false;
			break;
		}

		int attackAnimation = TZombieOneAnimation::TCount;
		int damageType = GCOMMON_INVALID_INDEX;
		if ((mAttackAvaliable & TZombieOneAttackAvaliable::TMouth) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieOneAnimation::TDragAttackBite : TZombieOneAnimation::TAttackBite;
			damageType = ZombieGuiDamage::GetDamageTypeBiteBig();
		}
		else if ((mAttackAvaliable & TZombieOneAttackAvaliable::TLeftHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieOneAnimation::TDragAttackLeft : TZombieOneAnimation::TAttackLeft;
			damageType = ZombieGuiDamage::GetDamageTypeSlashLeft();
		}
		else if ((mAttackAvaliable & TZombieOneAttackAvaliable::TRightHand) && (GMath::RandomUnit() < sChanceMouthAttack * in_timeDelta * chanceMul))
		{
			attackAnimation = drag ? TZombieOneAnimation::TDragAttackRight : TZombieOneAnimation::TAttackRight;
			damageType = ZombieGuiDamage::GetDamageTypeSlashRight();
		}

		if (TZombieOneAnimation::TCount != attackAnimation)
		{
			mParentMission.TakeDamage(2, damageType);
			mAttackCountdown = sAttackCountDown;
			PlayAnimation(attackAnimation, true);

			//and face palyer
			mLerpAt->SetValue(Normalise(playerPosition - zombiePosition), 0.25F, GComponentLerp::TType::TEaseInOut);
		}

		//TPointerZombieSound pZombieSound = mParentMission.GetSound();
		//if (pZombieSound)
		//{
		//	pZombieSound->Play(ZombieSound::GetSoundIDHiss4());
		//}
	}

	return;
}

void SceneNodeComponentClientZombieOne::SelectAnimation()
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
		case TZombieOneMovementAvaliable::TBothFeet:
			PlayAnimation(TZombieOneAnimation::TWalk);
			break;
		case TZombieOneMovementAvaliable::TLeftFoot:
			PlayAnimation(TZombieOneAnimation::THopLeft);
			break;
		case TZombieOneMovementAvaliable::TRightFoot:
			PlayAnimation(TZombieOneAnimation::THopRight);
			break;
		case TZombieOneMovementAvaliable::TDragBothArms:
			PlayAnimation(TZombieOneAnimation::TDrag);
			break;
		case TZombieOneMovementAvaliable::TDragLeftArm:
			PlayAnimation(TZombieOneAnimation::TDragLeft);
			break;
		case TZombieOneMovementAvaliable::TDragRightArm:
			PlayAnimation(TZombieOneAnimation::TDragRight);
			break;
		case TZombieOneMovementAvaliable::TImobile:
			PlayAnimation(TZombieOneAnimation::TDeath);
			break;
		}
	}
	else
	//select a idle animation
	{
		switch (mMovementAvaliable)
		{
		default:
			PlayAnimation(TZombieOneAnimation::TDragIdle);
			break;
		case TZombieOneMovementAvaliable::TBothFeet:
		case TZombieOneMovementAvaliable::TLeftFoot:
		case TZombieOneMovementAvaliable::TRightFoot:
			PlayAnimation(TZombieOneAnimation::TIdle);
			break;
		}
	}

	return;
}

void SceneNodeComponentClientZombieOne::PlayAnimation(const int in_animation, const bool in_force)
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
	animationManagerInstance.StopAnimation(sZombieOneAnimationDefaultFadeTime);

	if ((0 <= mCurrentAnimation) &&
		(mCurrentAnimation < TZombieOneAnimation::TCount))
	{
#if 0
		animationManagerInstance.StopAnimation();
#else
		const GAnimationPlayParam defaultParam(
			sZombieOneAnimationRate[mCurrentAnimation],
			sZombieOneAnimationDefaultFadeTime
			);
		animationManagerInstance.PlayAnimation(
			sZombieOneAnimationName[mCurrentAnimation],
			defaultParam
			);
#endif
	}

	return;
}
