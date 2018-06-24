//
//  SceneNodeComponentClientObjectBase.h
//  Zombie
//
//  Created by David Coen on 2011 01 29
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _SceneNodeComponentClientObjectBase_h_
#define _SceneNodeComponentClientObjectBase_h_

#include "GSceneNodeComponentClient.h"
#include <boost/intrusive/list_hook.hpp>
#include <string>
#include <map>

class GVector3Float;
class GSceneNodeComponentCollisionResult;
class GAnimationPlayParam;
class GSceneNode;

class SceneNodeComponentClientObjectBase : public GSceneNodeComponentClient
{
	//typedef
public:
	typedef boost::intrusive::list_member_hook< boost::intrusive::link_mode<boost::intrusive::auto_unlink> > TListHook;

	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollisionResult> TPointerSceneNodeComponentCollisionResult;
	typedef std::pair<SceneNodeComponentClientObjectBase*, TPointerSceneNodeComponentCollisionResult> TPairClientObjectCollisionResult;
	typedef std::vector<TPairClientObjectCollisionResult> TArrayPairClientObjectCollisionResult;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//constructor
public:
	SceneNodeComponentClientObjectBase(const std::string& in_name);
	virtual ~SceneNodeComponentClientObjectBase();

	//public methods
public:
	void CollectDamageReceivers(
		TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
		const GVector3Float& in_worldSpaceConeOrigin,
		const GVector3Float& in_worldSpaceConeDirection,
		const float in_coneLength,
		const float in_coneRadiusPoint,
		const float in_coneRadiusBase
		);

	void ApplyDamage(
		float& inout_damage, 
		const std::string& in_data
		);

	void AfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot);

	void Animate(
		const std::string& in_animationName, 
		const GAnimationPlayParam& in_playParam
		);

	void Move(
		const GVector3Float& in_position,
		const float in_transitionTime,
		const int in_transitionType
		);

	void Rotate(
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const float in_transitionTime,
		const int in_transitionType
		);

	void SetAiActive(const bool in_active);
	const bool GetAiActive()const;

	const std::string& GetName()const{ return mName; }

	void Destroy();

	//interface
private:
	virtual void OnCollectDamageReceivers(
		TArrayPairClientObjectCollisionResult& out_arrayClientObjectCollisionResult,
		const GVector3Float& in_worldSpaceConeOrigin,
		const GVector3Float& in_worldSpaceConeDirection,
		const float in_coneLength,
		const float in_coneRadiusPoint,
		const float in_coneRadiusBase
		);

	virtual void OnApplyDamage(
		float& inout_damage, 
		const std::string& in_data
		);

	virtual void OnAfterDamagePass(GScene& inout_scene, TPointerSceneNode& inout_sceenRoot);

	virtual void OnAnimate(
		const std::string& in_animationName, 
		const GAnimationPlayParam& in_playParam
		);

	virtual void OnMove(
		const GVector3Float& in_position,
		const float in_transitionTime,
		const int in_transitionType
		);

	virtual void OnRotate(
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const float in_transitionTime,
		const int in_transitionType
		);

	virtual void OnSetAiActive(const bool in_active);
	virtual const bool OnGetAiActive()const;

	virtual void OnDestroy();

	//public members
public:
	TListHook mListHook;
private:
	const std::string mName;

};

#endif //_SceneNodeComponentClientObjectBase_h_
