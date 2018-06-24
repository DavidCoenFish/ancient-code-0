//
//  BeachVolleyBallState.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallState_h_
#define _BeachVolleyBallState_h_

#include <GStateApplicationBase.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GVector2Float.h>
#include "BeachVolleyBallType.h"

class BeachVolleyBallStateWindowCallback;
class BeachVolleyBallGameVar;
class GApplicationWindow;
class GApplicationViewComponent;
class GAssetManager;
class GComponentCameraInput;
class GDictionary;
class GFileSystem;
class GInput;
class GParticleManager;
class GRenderComponent;
class GRenderMaterialManager;
class GRenderMaterial;
class GRenderMeshManual;
class GScene;
class GSceneNode;
class GSceneNodeFactory;
class GScratchPad;
class GVector2Int;
class GCamera;
class GGuiManager;
class GRenderShadow;
class BeachVolleyBallGame;
class BeachVolleyBallMenu;
class BeachVolleyBallWatch;
class BeachVolleyBallMovie;
class BeachVolleyBallSound;

//#define MOVIE

#ifdef DSC_DEBUG
	#define DEBUG_PUT_TEXT(IN_TEXT) DebugPutText(IN_TEXT);
	void DebugPutText(const std::string& in_text);

	#define DEBUG_GET_SCENE() DebugGetScene();
	GScene& DebugGetScene();
#else
#define DEBUG_PUT_TEXT(IN_TEXT)
#define DEBUG_GET_SCENE()

#endif	

class BeachVolleyBallState : public GStateApplicationBase
{
	//typedef
private:

	typedef boost::shared_ptr<BeachVolleyBallStateWindowCallback> TPointerBeachVolleyBallStateWindowCallback;
	typedef boost::shared_ptr<GApplicationWindow> TPointerApplicationWindow;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	typedef boost::shared_ptr<GAssetManager> TPointerAssetManager;
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;
	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::shared_ptr<GParticleManager> TPointerParticleManager;
	typedef boost::shared_ptr<GRenderComponent> TPointerRenderComponent;
	typedef boost::shared_ptr<GRenderMaterialManager> TPointerRenderMaterialManager;
	typedef boost::shared_ptr<GRenderMaterial> TPointerRenderMaterial;
	typedef boost::shared_ptr<GScene> TPointerScene;
	typedef boost::shared_ptr<GScratchPad> TPointerScratchPad;
	typedef boost::shared_ptr<GCamera> TPointerCamera;
	typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;
	typedef boost::shared_ptr<GGuiManager> TPointerGuiManager;
	typedef boost::shared_ptr<GRenderShadow> TPointerRenderShadow;

	typedef boost::scoped_ptr<BeachVolleyBallMenu> TPointerBeachVolleyBallMenu;
	typedef boost::scoped_ptr<BeachVolleyBallGame> TPointerBeachVolleyBallGame;
	typedef boost::scoped_ptr<BeachVolleyBallWatch> TPointerBeachVolleyBallWatch;
#ifdef MOVIE
	typedef boost::scoped_ptr<BeachVolleyBallMovie> TPointerBeachVolleyBallMovie;
#endif

	typedef boost::scoped_ptr<BeachVolleyBallGameVar> TPointerBeachVolleyBallGameVar;
	typedef boost::scoped_ptr<BeachVolleyBallSound> TPointerBeachVolleyBallSound;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	//static public methods
public:
	static const int GetStateMenu();
	static const int GetStateGame();
	static const int GetStateGameDemo();
	static const int GetStateWatch();
	static const int GetWorldFacingTypeIntro();
	static const int GetWorldFacingTypeGame(const BeachVolleyBallType::TGirl::TEnum in_girl);

	//constructor
public:
	BeachVolleyBallState(TPointerDictionary& inout_paramObject);
	virtual ~BeachVolleyBallState();

	//implement GState
private: 
	virtual const bool OnTick(const float in_timeDelta);

	//implement GStateApplicationBase
private:
	virtual void OnSetApplicationActive(const bool in_active); 

	//public methods
public:
	void ResizeWindow(const GVector2Int& in_size);
	void ShakeWindow();
	void DismissDialogWindow();
	void InputWindow(const GInput& in_input);
	const bool TickWindow(const float in_timeDelta);
	void DrawWindow();

	void PutText(const std::string& in_text);

	void SetWorldFacing(const int in_type);
	void PlaySoundMenuClick();

	//public accessors
public:
	TPointerScene& GetScene(){ return mScene; }
	TPointerGuiManager& GetGuiManager(){ return mGuiManager; }
	TPointerBeachVolleyBallGameVar& GetGameVar(){ return mPointerGameVar; }
	TPointerBeachVolleyBallSound& GetSoundManager(){ return mSoundManager; }

	void SetStateRequest(const int in_stateRequest){ mStateRequest = in_stateRequest; return; }
	void SetStateRequestData(
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);

	const BeachVolleyBallType::TGirl::TEnum GetStateRequestGirl()const{ return mStateRequestGirl; }
	const BeachVolleyBallType::TMatch::TEnum GetStateRequestMatch()const{ return mStateRequestMatch; }

	void SetDrawShadows(const bool in_drawShadows){ mDrawShadows = in_drawShadows; return; }

	//private methods
private:
	GCamera* const GetCamera(); //can be null
	void DealStateRequest();

	//private members
private:
	int mState;
	int mStateRequest;
	BeachVolleyBallType::TGirl::TEnum mStateRequestGirl;
	BeachVolleyBallType::TMatch::TEnum mStateRequestMatch;
	GVector2Float mWindowSize;
	float mCameraAspect;

	TPointerBeachVolleyBallStateWindowCallback mWindowCallback;
	TPointerApplicationWindow mWindow;
	TPointerScratchPad mScratchPad;
	TPointerApplicationViewComponent mComponentViewRender;

	TPointerAssetManager mAssetManager;
	TPointerFileSystem mFileSystem;
	TPointerRenderComponent mRenderComponent;
	TPointerRenderMaterialManager mMaterialManager;
	TPointerParticleManager mParticleManager;
	TPointerSceneNodeFactory mSceneNodeFactory;

	TPointerScene mScene;
	TPointerRenderShadow mRenderShadow;
	TPointerGuiManager mGuiManager;
	TPointerBeachVolleyBallSound mSoundManager;

	TWeakSceneNode mSceneNodeMeshSky; //SceneNodeMeshSky

	TPointerBeachVolleyBallMenu mPointerMenu;
	TPointerBeachVolleyBallGame mPointerGame;
	TPointerBeachVolleyBallWatch mPointerWatch;
#ifdef MOVIE
	TPointerBeachVolleyBallMovie mPointerMovie;
#endif

	TPointerBeachVolleyBallGameVar mPointerGameVar;

	bool mDrawShadows;

};

#endif //_BeachVolleyBallState_h_
