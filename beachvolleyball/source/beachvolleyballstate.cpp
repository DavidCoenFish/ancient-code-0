//
//  BeachVolleyBallState.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallState.h"

#include "BeachVolleyBallStateWindowCallback.h"
#include "BeachVolleyBallMenu.h"
#include "BeachVolleyBallGame.h"
#include "BeachVolleyBallGameVar.h"
#include "BeachVolleyBallWatch.h"
#include "BeachVolleyBallSound.h"
#ifdef MOVIE
#include "BeachVolleyBallMovie.h"
#endif

#include <GApplicationWindow.h>
#include <gapplicationviewcomponent.h>
#include <GApplicationViewComponentImage.h>
#include <GRenderComponent.h>
#include <GColour4Float.h>
#include <GComponentCameraInput.h>
#include <GVector2Int.h>
#include <GVector2Float.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GInputButton.h>
#include <GCamera.h>
#include <GScene.h>
#include <GSceneVisitorRender.h>
#include <GSceneNodeComponentSpringBoneData.h>
#include <GFileSystem.h>
#include <GRenderSpriteManager.h>
#include <GRenderTextureManager.h>
#include <GDictionary.h>
#include <GComponentCameraInput.h>
#include <GCamera.h>
#include <GVector3Float.h>
#include <GStringUtility.h>
#include <GRenderMaterial.h>
#include <GRenderMaterialManager.h>
#include <GRenderMeshManual.h>
#include <GRenderMeshLoad.h>
#include <GRenderShaderManager.h>
#include <GAssetManager.h>
#include <GAsset.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentFade.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GMaterialGroupManager.h>
#include <GMath.h>
#include <GGuiCoordinate.h>
#include <GOptionDialogDataItem.h>
#include <GInputJoystick.h>
#include <GSceneNodeComponentClient.h>
#include <GScratchPad.h>
#include <GSceneNodeFactory.h>
#include <GSceneNodeComponentBoneCollisionResult.h>
#include <GScenePostLoadCallback.h>
#include <GMissionMaster.h>
#include <GAnimationPlayParam.h>
#include <GParticleManager.h>
#include <GRenderMeshManual.h>
#include <GSceneVisitorRenderShadow.h>
#include <GSceneNode.h>
#include <GSceneNodeUtility.h>
#include <GGuiManager.h>
#include <GRenderShadow.h>
#include <GRenderLight.h>

#include <string>
#include <vector>

//#define PRINT_FPS

struct TState
{
	enum TEnum
	{
		TMenu = 0,
		TGame,
		TGameDemo,
		TWatch,

#ifdef MOVIE
		TMovie,
#endif

		TCount
	};
};

struct TWorldFacingType
{
	enum TEnum
	{
		TIntro = 0,
		TGame0,
		TGame1,
		TGame2,
		TGame3,

		TCount
	};
};

static const float sWorldFacingRandian[TWorldFacingType::TCount] = 
{
	0.0F,
	0.3590F,
	1.93F,
	3.5F,
	-1.212F
};

typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;
typedef boost::shared_ptr<GApplicationWindowCallback> TPointerApplicationWindowCallback;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	);
typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;
typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;
typedef std::vector<GRenderMeshStreamInfoLoad> TArrayStreamInfoLoad;
typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;
typedef std::vector<TPointerRenderLight> TArrayPointerRenderLight;

#ifdef DSC_DEBUG
static BeachVolleyBallState* sBeachVolleyBallState = NULL;
void DebugPutText(const std::string& in_text)
{
	if (sBeachVolleyBallState)
	{
		sBeachVolleyBallState->PutText(in_text);
	}
	return;
}

GScene& DebugGetScene()
{
	return *(sBeachVolleyBallState->GetScene());
}

#endif

//static public methods
/*static*/ const int BeachVolleyBallState::GetStateMenu()
{
	return TState::TMenu;
}

/*static*/ const int BeachVolleyBallState::GetStateGame()
{
	return TState::TGame;
}

/*static*/ const int BeachVolleyBallState::GetStateGameDemo()
{
	return TState::TGameDemo;
}

/*static*/ const int BeachVolleyBallState::GetStateWatch()
{
	return TState::TWatch;
}

/*static*/ const int BeachVolleyBallState::GetWorldFacingTypeIntro()
{
	return TWorldFacingType::TIntro;
}

/*static*/ const int BeachVolleyBallState::GetWorldFacingTypeGame(const BeachVolleyBallType::TGirl::TEnum in_girl)
{
	switch (in_girl)
	{
	default:
		break;
	case BeachVolleyBallType::TGirl::TBikini:
		return TWorldFacingType::TGame0;
	case BeachVolleyBallType::TGirl::TReiko:
		return TWorldFacingType::TGame1;
	case BeachVolleyBallType::TGirl::TApril:
		return TWorldFacingType::TGame2;
	case BeachVolleyBallType::TGirl::TCuda:
		return TWorldFacingType::TGame3;
	}
	return TWorldFacingType::TGame0;
}

//constructor
BeachVolleyBallState::BeachVolleyBallState(TPointerDictionary& inout_paramObject)
: mCameraAspect(1.0F)
, mState(TState::TCount)
, mStateRequest(TState::TCount)
, mStateRequestGirl(BeachVolleyBallType::TGirl::TCount)
, mStateRequestMatch(BeachVolleyBallType::TMatch::TCount)
, mDrawShadows(true)
{
#ifdef DSC_DEBUG
	sBeachVolleyBallState = this;
#endif

	mScratchPad.reset(new GScratchPad);

	mFileSystem.reset(new GFileSystem);
	mAssetManager.reset(new GAssetManager(mFileSystem));

	mWindowCallback.reset(new BeachVolleyBallStateWindowCallback(*this));

	inout_paramObject->SetValueFloat("viewScale", 1.5F);
	TPointerApplicationWindowCallback windowCallback = mWindowCallback;
	mWindow.reset(new GApplicationWindow(windowCallback, inout_paramObject));
	if (!mWindow)
	{
		return;
	}

	mComponentViewRender = mWindow->CreateViewComponentRender(
		mRenderComponent,
		true, //in_transparentBackground, 
		GVector2Float(0.0F, 0.0F), //in_originRatio, //1.0 == 100% screen size
		GVector2Float(1.0F, 1.0F) //in_sizeRatio		
		);
	mWindow->AddViewComponentBack(mComponentViewRender);
	
	//if (mRenderComponent)
	//{
	//	TArrayPointerRenderLight arrayLight;
	//	mRenderComponent->SetLight(
	//		arrayLight,
	//		//GColour4Float(128.0F, 128.0F, 128.0F, 1.0F)
	//		GColour4Float(1.0F, 1.0F, 1.0F, 1.0F)
	//		);
	//}
	

	//mParticleManager = GParticleManager::Factory(mAssetManager, "particlemanager.data");
	mMaterialManager.reset(new GRenderMaterialManager(mAssetManager, mFileSystem));

	mSceneNodeFactory = GSceneNodeFactory::Factory(mAssetManager, "factory.data");

	TMapStringPairFactoryClientCallbackData clientComponentFactory;

	mScene = GScene::Factory(
		mRenderComponent,
		mAssetManager,
		mParticleManager,
		mMaterialManager,
		clientComponentFactory,
		mSceneNodeFactory,
		"scene.data"
		);

	if (mScene)
	{
		mSceneNodeMeshSky = GSceneNodeUtility::FindSceneNodeName(*mScene, "SceneNodeMeshSky");
	}

	mRenderShadow.reset(new GRenderShadow(
		mRenderComponent,
		*mMaterialManager,
		GVector3Float(-0.4F, -0.9F, -0.4F),
		GColour4Float(GColour4Byte(0, 9, 19, 64))
		));

	mGuiManager.reset(new GGuiManager(mAssetManager, mMaterialManager, mRenderComponent));
	mPointerGameVar.reset(new BeachVolleyBallGameVar(mFileSystem));

	mSoundManager.reset(new BeachVolleyBallSound(*mFileSystem));

	mWindow->Show();

	return;
}

/*virtual*/ BeachVolleyBallState::~BeachVolleyBallState()
{
#ifdef DSC_DEBUG
	sBeachVolleyBallState = NULL;
#endif
	return;
}

//implement GState
/*virtual*/ const bool BeachVolleyBallState::OnTick(const float in_timeDelta)
{
	bool result = true;

	if (mWindow)
	{
		result = mWindow->Tick(in_timeDelta);
	}

	return result;
}

//implement GStateApplicationBase
/*virtual*/ void BeachVolleyBallState::OnSetApplicationActive(const bool in_active)
{
	if (mWindow)
	{
		mWindow->SetApplicationActive(in_active);
	}
	return;
}

//public methods
void BeachVolleyBallState::ResizeWindow(const GVector2Int& in_size)
{
	//adjust camera to match new size
	mCameraAspect = ((float)in_size.GetX()) / ((float)in_size.GetY());
	mWindowSize = GVector2Float((float)in_size.GetX(), (float)in_size.GetY());

	if (mGuiManager)
	{
		GGuiManager& guiManager = *mGuiManager;
		guiManager.SetScreenSize(mWindowSize);
		guiManager.SetDeviceCoordinates(
			GVector2Float(-1.0F, 1.0F),
			GVector2Float(1.0F, -1.0F)
			);
	}

	switch (mState)
	{
	default:
		break;
	case TState::TGame:
	case TState::TGameDemo:
		if (mPointerGame)
		{
			mPointerGame->Resize(mCameraAspect);
		}
		break;
	case TState::TMenu:
		if (mPointerMenu)
		{
			mPointerMenu->Resize(mCameraAspect);
		}
		break;
	case TState::TWatch:
		if (mPointerWatch)
		{
			mPointerWatch->Resize(mCameraAspect);
		}
		break;
#ifdef MOVIE
	case TState::TMovie:
		if (mPointerMovie)
		{
			mPointerMovie->Resize(mCameraAspect);
		}
		break;
#endif
	}

	return;
}

void BeachVolleyBallState::ShakeWindow()
{
#ifdef DEBUG
	switch (mState)
	{
	default:
		break;
	case TState::TGame:
		if (mPointerGame)
		{
			mPointerGame->Shake();
		}
		break;
	}
#endif
	return;
}

void BeachVolleyBallState::InputWindow(const GInput& in_input)
{
#ifdef WIN32
	if (1 <= in_input.GetButtonCount())
	{
		const bool shake = in_input.GetButton(0).GetEdgeDown();
		if (shake)
		{
			ShakeWindow();
		}
	}
#endif

	//world jiggle
	if (0 < in_input.GetJoystickCount())
	{
		const GInputJoystick& joyStick = in_input.GetJoystick(0);
		static GVector3Float sAccelerometorTrack;
		static bool sInit = false;
		if (!sInit)
		{
			sInit = true;
			sAccelerometorTrack = joyStick.GetDirection();
		}
		else
		{
			const GVector3Float oldAccelerometorTrack = sAccelerometorTrack;
			sAccelerometorTrack = (0.5F * sAccelerometorTrack) + (0.5F * joyStick.GetDirection());
			GSceneNodeComponentSpringBoneData::SetWorldJiggle((oldAccelerometorTrack - sAccelerometorTrack) * 1.0F);
		}
	}

	switch (mState)
	{
	default:
		break;
	case TState::TMenu:
		if (mPointerMenu)
		{
			mPointerMenu->Input(in_input);
		}
		break;
	case TState::TGame:
	case TState::TGameDemo:
		if (mPointerGame)
		{
			mPointerGame->Input(in_input);
		}
		break;
	case TState::TWatch:
		if (mPointerWatch)
		{
			mPointerWatch->Input(in_input);
		}
		break;
	}

	return;
}

const bool BeachVolleyBallState::TickWindow(const float in_timeDelta)
{
#ifdef PRINT_FPS
	if (in_timeDelta)
	{
		std::string text("FPS: ");
		text += GStringUtility::PrintMethod(1.0F / in_timeDelta);
		PutText(text);
	}
#endif

	DealStateRequest();

	switch (mState)
	{
	default:
		break;
	case TState::TMenu:
		if (mPointerMenu)
		{
			mPointerMenu->Tick(in_timeDelta);
		}
		break;
	case TState::TGame:
	case TState::TGameDemo:
		if (mPointerGame)
		{
			mPointerGame->Tick(in_timeDelta);
		}
		break;
	case TState::TWatch:
		if (mPointerWatch)
		{
			mPointerWatch->Tick(in_timeDelta);
		}
		break;
#ifdef MOVIE
	case TState::TMovie:
		if (mPointerMovie)
		{
			mPointerMovie->Tick(in_timeDelta);
		}
		break;
#endif

	}


	return true;
}

void BeachVolleyBallState::DrawWindow()
{
	GCamera* const pointerCamera = GetCamera();
	if (mRenderComponent && pointerCamera && mScene)
	{
		GRenderComponent& renderComponent = *mRenderComponent;
		GCamera& camera = *pointerCamera;
		GScene& scene = *mScene;
		renderComponent.RenderStart();

		//renderComponent.ClearColourBuffer(GColour4Float(0.0F, 0.0F, 0.0F, 1.0F));
		//renderComponent.ClearDepthBuffer(1.0F);
		//renderComponent.ClearStencilBuffer(0);
		renderComponent.CleanState();

		GSceneVisitorRender::Run(
			scene, 
			renderComponent, 
			camera
			);

		if (mDrawShadows && mRenderShadow)
		{
			mRenderShadow->RenderShadows(
				renderComponent,
				scene,
				camera
				);
		}

	#ifdef DSC_DEBUG
		renderComponent.SetCamera(camera);
		GMatrix16Float transform(GMatrix16Float::sIdentity);
		scene.GetArrayDebugMatrix().push_back(transform);

		scene.DebugDraw(
			renderComponent,
			camera		
			);
		scene.GetArrayDebugMatrix().clear();
	#endif

		switch (mState)
		{
		default:
			break;
		case TState::TMenu:
			if (mPointerMenu)
			{
				mPointerMenu->Draw(*mRenderComponent);
			}
			break;
		case TState::TGame:
		case TState::TGameDemo:
			if (mPointerGame)
			{
				mPointerGame->Draw(*mRenderComponent);
			}
			break;
		case TState::TWatch:
			if (mPointerWatch)
			{
				mPointerWatch->Draw(*mRenderComponent);
			}
			break;
		}

#ifdef IPHONE
		renderComponent.Present();
#endif		
	}
	/**/
	return;
}

void BeachVolleyBallState::PutText(const std::string& in_text)
{
	if (mWindow)
	{
		mWindow->PutText(in_text);
	}
	return;
}

void BeachVolleyBallState::SetWorldFacing(const int in_type)
{
	TPointerSceneNode pointerSceneNode = mSceneNodeMeshSky.lock();
	if (!pointerSceneNode)
	{
		return;
	}
	GSceneNode& sceneNode = *pointerSceneNode;
	const GMatrix16Float transform = GMatrix16FloatConstructAxisAngle(GVector3Float::sUnitY, sWorldFacingRandian[in_type]);
	sceneNode.SetParentRelativeTransform(transform);
	return;
}

void BeachVolleyBallState::PlaySoundMenuClick()
{
	if (mSoundManager)
	{
		mSoundManager->PlayOmniSound(BeachVolleyBallSound::GetSoundIDClick());
	}
	return;
}


//public accessors
void BeachVolleyBallState::SetStateRequestData(
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
{
	mStateRequestGirl = in_girl;
	mStateRequestMatch = in_match;
	return;
}


//private methods
GCamera* const BeachVolleyBallState::GetCamera()
{
	switch (mState)
	{
	default:
		break;
	case TState::TMenu:
		if (mPointerMenu)
		{
			return &(mPointerMenu->GetCamera());
		}
		break;
	case TState::TGame:
	case TState::TGameDemo:
		if (mPointerGame)
		{
			return &(mPointerGame->GetCamera());
		}
		break;
	case TState::TWatch:
		if (mPointerWatch)
		{
			return &(mPointerWatch->GetCamera());
		}
		break;
#ifdef MOVIE
	case TState::TMovie:
		if (mPointerMovie)
		{
			return &(mPointerMovie->GetCamera());
		}
		break;
#endif
	}

	return NULL;
}

void BeachVolleyBallState::DealStateRequest()
{
	if ((TState::TCount == mState) ||
		(TState::TCount != mStateRequest))
	{
		mPointerMenu.reset();
		mPointerGame.reset();
		mPointerWatch.reset();
#ifdef MOVIE
		mPointerMovie.reset();
		mStateRequest = TState::TMovie;
#endif

		switch (mStateRequest)
		{
		default:
		case TState::TMenu:
			mState = TState::TMenu;
			mPointerMenu.reset(new BeachVolleyBallMenu(
				*this, 
				mCameraAspect, 
				mStateRequestGirl
				));
			break;
		case TState::TGame:
			mState = TState::TGame;
			mPointerGame.reset(new BeachVolleyBallGame(
				*this, 
				mCameraAspect, 
				mStateRequestGirl,
				mStateRequestMatch
				));
			break;
		case TState::TGameDemo:
			mState = TState::TGameDemo;
			mPointerGame.reset(new BeachVolleyBallGame(
				*this, 
				mCameraAspect, 
				mStateRequestGirl,
				mStateRequestMatch,
				true
				));
			break;
		case TState::TWatch:
			mState = TState::TWatch;
			mPointerWatch.reset(new BeachVolleyBallWatch(
				*this, 
				mCameraAspect, 
				mStateRequestGirl, 
				mStateRequestMatch
				));
			break;

#ifdef MOVIE
		case TState::TMovie:
			mState = TState::TMovie;
			mPointerMovie.reset(new BeachVolleyBallMovie(
				*this, 
				mCameraAspect
				));
			break;
#endif

		}

		mStateRequest = TState::TCount;
	}

	return;
}
