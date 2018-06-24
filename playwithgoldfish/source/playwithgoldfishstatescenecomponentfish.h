//
//  PlayWithGoldfishStateSceneComponentFish.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateSceneComponentFish_h_
#define _PlayWithGoldfishStateSceneComponentFish_h_

#include <GSceneNodeComponentClient.h>
#include <GVector3Float.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "PlayWithGoldfishStateType.h"

class GScene;
class GSceneNode;
class GCamera;
class GInput;
class GScenePostLoadCallback;
class PlayWithKittyState;
class GVector3Float;
class GRenderMeshInstance;
class GDictionary;
class PlayWithGoldfishState;
class PlayWithGoldfishStateComponentAi;
class PlayWithGoldfishStateComponentAnimation;

class PlayWithGoldfishStateSceneComponentFish : public GSceneNodeComponentClient
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<PlayWithGoldfishStateSceneComponentFish> TPointerPlayWithGoldfishStateSceneComponentFish;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::scoped_ptr<GDictionary> TPointerDictionary;
	typedef boost::scoped_ptr<PlayWithGoldfishStateComponentAi> TPointerPlayWithGoldfishStateComponentAi;
	typedef boost::scoped_ptr<PlayWithGoldfishStateComponentAnimation> TPointerPlayWithGoldfishStateComponentAnimation;

	typedef boost::shared_ptr<GScene> TPointerScene;

	//static public methods
public:
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene,
		GSceneNode& inout_owner,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		void* const in_data //address of PlayWithKittyState
		);

	static const GVector3Float& GetPosition(GDictionary& inout_dictionary);
	static void SetPosition(GDictionary& inout_dictionary, const GVector3Float& in_value);

	static const float GetFacing(GDictionary& inout_dictionary);
	static void SetFacing(GDictionary& inout_dictionary, const float in_value);

	static const bool GetTouchBegin(GDictionary& inout_dictionary);
	static void SetTouchBegin(GDictionary& inout_dictionary, const bool in_value);

	static const bool GetTouchActive(GDictionary& inout_dictionary);
	static void SetTouchActive(GDictionary& inout_dictionary, const bool in_value);

	static const GVector3Float& GetTouchWorldPoint(GDictionary& inout_dictionary);
	static void SetTouchWorldPoint(GDictionary& inout_dictionary, const GVector3Float& in_value);

	static const GVector3Float& GetCameraTarget(GDictionary& inout_dictionary);
	static void SetCameraTarget(GDictionary& inout_dictionary, const GVector3Float& in_value);

private:
	static void PostLoadCallback(
		GScene& inout_scene, //access to mesh/ material manager
		GSceneNode& inout_owner,
		TPointerSceneNodeComponentBase& inout_component,
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
		);

	//constructor
public:
	PlayWithGoldfishStateSceneComponentFish(
		PlayWithGoldfishState& inout_parentState
		);
	virtual ~PlayWithGoldfishStateSceneComponentFish();

	//implement GSceneNodeComponentClient
private:
	virtual void OnTick(GSceneNode& inout_sceneNode, const float in_timeDelta); 

	//public methods
public:
	void ShakeWindow();
	void SetTouch(const GVector3Float& in_touchWorldPoint, const bool in_touchBegin);
	void SetCameraTarget(const GVector3Float& in_cameraTarget);

	void SetMovementType(const PlayWithGoldfishStateType::TMovementType::TEnum in_movementType);
	void SetAnimationType(const PlayWithGoldfishStateType::TAnimationType::TEnum in_animationType);

	//private members
private:
	PlayWithGoldfishState& mParentState;
	//GVector3Float mTouchWorldPoint;
	bool mTeleport;

	TPointerDictionary mDictionary;
	TPointerPlayWithGoldfishStateComponentAi mComponentAi;
	TPointerPlayWithGoldfishStateComponentAnimation mComponentAnimation;

	PlayWithGoldfishStateType::TMovementType::TEnum mMovementType;
	PlayWithGoldfishStateType::TAnimationType::TEnum mAnimationType;

};

#endif //_PlayWithGoldfishStateSceneComponentFish_h_