//
//  ZombieComponent.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieComponent_h_
#define _ZombieComponent_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <map>

class ZombieMission;
class ZombiePart;
class ZombieComponentLoad;
class GSceneNodeComponentAnimation;
class GSceneNodeComponentCollision;
class GSceneNodeComponentCollisionResult;
class SceneNodeComponentClientObjectBase;
class GComponentLerpVector3Float;
class GSceneNodeComponentParticle;
class GSceneNodeComponentBase;
class GScene;
class GSceneNode;
class GVector3Float;

/*
	there is currently no provision to detach body parts which are not skinned, ie, if the part
	is not skinned (like most heads) then they will not have the correct offset when detached (ie, will appear to fall
	from the creatures origin/ feet, rather than where it should detach from)
	//i'm going to live with this, else need to flag the attached parts and get the offset, and make a child node for them off the detach node
	//or fix it as the skeletonOne is mostly not skinned....
	fixed
*/

class ZombieComponent : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<ZombiePart> TPointerZombiePart;
	typedef std::vector<TPointerZombiePart> TArrayZombiePart;

	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<ZombieComponent> TPointerZombieComponent;

	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
	typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;
	typedef std::vector<TWeakSceneNodeComponentCollision> TArrayWeakSceneNodeComponentCollision;
	typedef std::vector<TArrayWeakSceneNodeComponentCollision> TArrayArrayWeakSceneNodeComponentCollision;

	typedef boost::shared_ptr<GSceneNodeComponentAnimation> TPointerSceneNodeComponentAnimation;
	typedef boost::weak_ptr<GSceneNodeComponentAnimation> TWeakSceneNodeComponentAnimation;

	typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
	typedef boost::weak_ptr<GSceneNodeComponentParticle> TWeakSceneNodeComponentParticle;
	typedef std::vector<TWeakSceneNodeComponentParticle> TArrayWeakSceneNodeComponentParticle;
	typedef std::map<std::string, TWeakSceneNodeComponentParticle> TMapStringWeakSceneNodeComponentParticle;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	//static public methods
public:
	static TPointerZombieComponent Factory(
		const ZombieComponentLoad& in_zombieComponentLoad,
		TPointerSceneNode& in_sceneNodeParent,
		const bool in_gore,
		const float in_health
		);

	//constructor
public:
	ZombieComponent(
		const ZombieComponentLoad& in_zombieComponentLoad,
		TArrayZombiePart& in_arrayZombiePart, //WARN: transfer ownership,
		TArrayArrayWeakSceneNodeComponentCollision& in_arrayCollision,
		TWeakSceneNodeComponentAnimation& in_animation,
		TMapStringWeakSceneNodeComponentParticle& in_mapNameParticle
		);
	~ZombieComponent();

	//public methods
public:
	void CollectDamageReceivers(
		SceneNodeComponentClientObjectBase* const in_parent,
		TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
		const GVector3Float& in_worldSpaceConeOrigin,
		const GVector3Float& in_worldSpaceConeDirection,
		const float in_coneLength,
		const float in_coneRadiusPoint,
		const float in_coneRadiusBase
		);

	//return true if part is destroyed
	const bool ApplyDamage(
		float& inout_damage, 
		const std::string& in_data
		);

	void AfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot, TPointerSceneNode& in_zombieParentSceneNode);

	TArrayZombiePart& GetArrayZombiePart(){ return mArrayZombiePart; }
	TWeakSceneNodeComponentAnimation& GetAnimation(){ return mAnimation; }

	//private members
private:
	const ZombieComponentLoad& mZombieComponentLoad;
	//GSceneNode& mParentSceneNode; //to detach limbs, need to know scene node world pos, 
	TArrayZombiePart mArrayZombiePart;

	TArrayArrayWeakSceneNodeComponentCollision mArrayCollision;
	TWeakSceneNodeComponentAnimation mAnimation;
	TMapStringWeakSceneNodeComponentParticle mMapNameParticle;

};

#endif //_ZombieComponent_h_
