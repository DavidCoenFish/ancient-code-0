//
//  PlayWithGoldfishState.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishState.h"

#include "PlayWithGoldfishStateSceneComponentFish.h"
#include "PlayWithGoldfishStateType.h"
#include "PlayWithGoldfishStateWindowCallback.h"
#include "PlayWithGoldfishMaterialGroupQuery.h"

#include <GApplicationWindow.h>
#include <gapplicationviewcomponent.h>
#include <GRenderComponent.h>
#include <GColour4Float.h>
#include <GVector2Int.h>
#include <GVector2Float.h>
#include <GInput.h>
#include <GInputTouch.h>
#include <GInputButton.h>
#include <GCamera.h>
#include <GScene.h>
#include <GSceneVisitorRender.h>
#include <GFileSystem.h>
#include <GRenderSpriteManager.h>
#include <GRenderTextureManager.h>
#include <GDictionary.h>
#include <GComponentCameraInput.h>
#include <GCamera.h>
#include <GVector3Float.h>
#include <GStringUtility.h>
#include <GRenderMaterial.h>
#include <GRenderMaterialLoad.h>
#include <GRenderMaterialBasic.h>
#include <GRenderMaterialBasicLoad.h>
#include <GRenderMaterialShader.h>
#include <GRenderMaterialShaderLoad.h>
#include <GRenderMaterialTextureDataLoad.h>
#include <GRenderMaterialManager.h>
#include <GRenderMeshManual.h>
#include <GRenderMeshLoad.h>
#include <GAssetManager.h>
#include <GAsset.h>
#include <GSceneVisitorAll.h>
#include <GSceneNodeComponentAnimation.h>
#include <GSceneNodeComponentNodeAttach.h>
#include <GSceneNodeComponentVisual.h>
#include <GMaterialGroupManager.h>
#include <GMath.h>
#include <GGuiCoordinate.h>
#include <GOptionDialogDataItem.h>
#include <GInputJoystick.h>

#include <string>
#include <vector>

typedef boost::shared_ptr<GApplicationWindowCallback> TPointerApplicationWindowCallback;
typedef boost::shared_ptr<GCamera> TPointerCamera;
typedef std::pair<int, int> TPairIntInt;
typedef std::vector<TPairIntInt> TArrayPairIntInt;
typedef std::vector<unsigned char> TArrayByte;
typedef std::vector<unsigned short> TArrayShort;

typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
typedef TPointerSceneNodeComponentBase (*TComponentFactoryClientCallback)(
	GScene& inout_scene, //access to mesh/ material manager
	GSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	void* const in_data
	);
typedef std::pair<TComponentFactoryClientCallback, void*> TPairFactoryClientCallbackData;
typedef std::map<std::string, TPairFactoryClientCallbackData> TMapStringPairFactoryClientCallbackData;

typedef boost::weak_ptr<GRenderMaterial> TWeakRenderMaterial;
typedef std::pair<std::string, TWeakRenderMaterial> TPairStringWeakRenderMaterial;
typedef std::vector<TPairStringWeakRenderMaterial> TArrayPairStringWeakRenderMaterial;

#define YOUTUBE_PREVIEW

static const float sSmoothJoystickRatio = 0.1F;
static const float sDefaultCameraPerspectiveBounds = 0.2F;
static const float sDefaultCameraPerspectiveNear = 1.0F;
static const float sDefaultCameraPerspectiveFar = 10000.0F;
static const char* const sOptionMaterialKey = "OptionMaterial";
static const char* const sOptionAnimationTypeKey = "OptionAnimation";
static const char* const sOptionMovementTypeKey = "OptionMovement";
static const char* const sOptionViewBackgroundTypeKey = "OptionViewBackground";
static const char* const sOptionViewCameraTypeKey = "OptionViewCamera";
static const char* const sOptionOversampleTypeKey = "OptionOversampleTypeKey";
static const char* const sOptionResetKey = "OptionResetKey";

static const char* const sDictionaryMaterialKey = "material";
static const char* const sDictionaryMovementKey = "movement";
static const char* const sDictionaryAnimationKey = "animation";
static const char* const sDictionaryViewBackgroundKey = "viewBackground";
static const char* const sDictionaryViewCameraKey = "viewCamera";
static const char* const sDictionarySuperSampleKey = "superSample";

static const char* const sApplicationPreferenceFileName = "option.data";

static int sCameraFrameRate = 10;

#ifdef DSC_DEBUG
static PlayWithGoldfishState* sPlayWithGoldfishState = NULL;
void DebugPutText(const std::string& in_text)
{
	if (sPlayWithGoldfishState)
	{
		sPlayWithGoldfishState->PutText(in_text);
	}
	return;
}
#endif

#ifdef IPHONE
#include <UIKit/UIImagePickerController.h>
#endif

static const bool LocalGetHasCamera()
{
	bool hasCamera = true;
#ifdef IPHONE
	//hasCamera = (nil != NSClassFromString(@"UIImagePickerController"));//[UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
	hasCamera = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
#endif
	return hasCamera;
}
static const bool LocalGetHasAccelerometor()
{
	bool hasAccelerometor = true;
#ifdef IPHONE
	hasAccelerometor = (nil != NSClassFromString(@"UIAccelerometer"));
#endif
	return hasAccelerometor;
}

//static public methods
/*static*/ const int PlayWithGoldfishState::GetDictionaryMaterialIndex(const GDictionary& in_dictionary)
{
	return in_dictionary.GetValueInt(sDictionaryMaterialKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionaryMaterialIndex(GDictionary& inout_dictionary, const int in_value)
{
	inout_dictionary.SetValueInt(sDictionaryMaterialKey, in_value);
	return;
}


/*static*/ const PlayWithGoldfishStateType::TMovementType::TEnum PlayWithGoldfishState::GetDictionaryMovementType(const GDictionary& in_dictionary)
{
	return (PlayWithGoldfishStateType::TMovementType::TEnum)in_dictionary.GetValueInt(sDictionaryMovementKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionaryMovementType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TMovementType::TEnum in_value)
{
	inout_dictionary.SetValueInt(sDictionaryMovementKey, in_value);
	return;
}

/*static*/ const PlayWithGoldfishStateType::TAnimationType::TEnum PlayWithGoldfishState::GetDictionaryAnimationType(const GDictionary& in_dictionary)
{
	return (PlayWithGoldfishStateType::TAnimationType::TEnum)in_dictionary.GetValueInt(sDictionaryAnimationKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionaryAnimationType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TAnimationType::TEnum in_value)
{
	inout_dictionary.SetValueInt(sDictionaryAnimationKey, in_value);
	return;
}

/*static*/ const PlayWithGoldfishStateType::TViewBackgroundType::TEnum PlayWithGoldfishState::GetDictionaryViewBackgroundType(const GDictionary& in_dictionary)
{
	return (PlayWithGoldfishStateType::TViewBackgroundType::TEnum)in_dictionary.GetValueInt(sDictionaryViewBackgroundKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionaryViewBackgroundType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TViewBackgroundType::TEnum in_value)
{
	inout_dictionary.SetValueInt(sDictionaryViewBackgroundKey, in_value);
	return;
}

/*static*/ const PlayWithGoldfishStateType::TViewCameraType::TEnum PlayWithGoldfishState::GetDictionaryViewCameraType(const GDictionary& in_dictionary)
{
	return (PlayWithGoldfishStateType::TViewCameraType::TEnum)in_dictionary.GetValueInt(sDictionaryViewCameraKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionaryViewCameraType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TViewCameraType::TEnum in_value)
{
	inout_dictionary.SetValueInt(sDictionaryViewCameraKey, in_value);
	return;
}

/*static*/ const PlayWithGoldfishStateType::TSuperSampleType::TEnum PlayWithGoldfishState::GetDictionarySuperSampleType(const GDictionary& in_dictionary)
{
	return (PlayWithGoldfishStateType::TSuperSampleType::TEnum)in_dictionary.GetValueInt(sDictionarySuperSampleKey);
}
/*static*/ void PlayWithGoldfishState::SetDictionarySuperSampleType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TSuperSampleType::TEnum in_value)
{
	inout_dictionary.SetValueInt(sDictionarySuperSampleKey, in_value);
	return;
}

/*static*/ void PlayWithGoldfishState::OnButtonInfo(void* const in_data)
{
	GASSERT(NULL != in_data, "invalide param");
	PlayWithGoldfishState& localThis = *(PlayWithGoldfishState*)in_data;
	localThis.mWindow->InvokeAlert(
		"InfoTitle",
		"InfoMessage",
		"InfoButton"
		);

	return;
}

/*static*/ void PlayWithGoldfishState::OnButtonCamera(void* const in_data)
{
	GASSERT(NULL != in_data, "invalide param");
	PlayWithGoldfishState& localThis = *(PlayWithGoldfishState*)in_data;

	localThis.mWindow->TakeScreenShot();

	return;
}

/*static*/ void PlayWithGoldfishState::OnButtonOption(void* const in_data)
{
	GASSERT(NULL != in_data, "invalide param");
	PlayWithGoldfishState& localThis = *(PlayWithGoldfishState*)in_data;
		
	localThis.ShowButtons(false);	
	localThis.mWindow->InvokeOptionDialog(
		localThis.mOptionDialogDataItem
		);

	return;
}

/*static*/ void PlayWithGoldfishState::OnOptionDialogDataItemCallback(void* const in_callbackData, GOptionDialogDataItem& inout_dataItem)
{
	GASSERT(NULL != in_callbackData, "invalide param");
	PlayWithGoldfishState& localThis = *(PlayWithGoldfishState*)in_callbackData;

	if (inout_dataItem.GetNameKey() == sOptionMaterialKey)
	{
		localThis.SetMaterialIndex(inout_dataItem.GetInt());
	}
	else if (inout_dataItem.GetNameKey() == sOptionViewBackgroundTypeKey)
	{
		localThis.SetViewBackgroundType((PlayWithGoldfishStateType::TViewBackgroundType::TEnum)(inout_dataItem.GetInt() + (localThis.mHasCamera ? 0 : 1)));
	}
	else if (inout_dataItem.GetNameKey() == sOptionAnimationTypeKey)
	{
		localThis.SetAnimationType((PlayWithGoldfishStateType::TAnimationType::TEnum)inout_dataItem.GetInt());
	}
	else if (inout_dataItem.GetNameKey() == sOptionMovementTypeKey)
	{
		localThis.SetMovementType((PlayWithGoldfishStateType::TMovementType::TEnum)inout_dataItem.GetInt());
	}
	else if (inout_dataItem.GetNameKey() == sOptionViewCameraTypeKey)
	{
		localThis.SetViewCameraType((PlayWithGoldfishStateType::TViewCameraType::TEnum)(inout_dataItem.GetInt() + (localThis.mHasAccelerometor ? 0 : 1)));
	}
	else if (inout_dataItem.GetNameKey() == sOptionOversampleTypeKey)
	{
		localThis.SetSuperSampleType((PlayWithGoldfishStateType::TSuperSampleType::TEnum)inout_dataItem.GetInt());
	}
	else if (inout_dataItem.GetNameKey() == sOptionResetKey)
	{
		localThis.SetReset();
	}

	return;
}


//constructor
PlayWithGoldfishState::PlayWithGoldfishState(TPointerDictionary& inout_paramObject)
: mWindowCallback()
, mWindow()
, mComponentViewRender()
, mFileSystem()
, mAssetManager()
, mTextureManager()
, mMaterialManager()
, mRenderComponent()
, mComponentCameraInput()
, mScene()
, mHasCamera(LocalGetHasCamera())
, mHasAccelerometor(LocalGetHasAccelerometor())
{
#ifdef DSC_DEBUG
	sPlayWithGoldfishState = this;
#endif

	mFileSystem.reset(new GFileSystem);
	mAssetManager.reset(new GAssetManager(mFileSystem));
	mTextureManager.reset(new GRenderTextureManager(mAssetManager));

	MakeOptionDictionary();
	LoadOptionDictionary();

	mComponentCameraInput.reset(new GComponentCameraInput(
		sSmoothJoystickRatio,
		sDefaultCameraPerspectiveBounds,
		sDefaultCameraPerspectiveNear,
		sDefaultCameraPerspectiveFar,
		-GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		-750.0F,
		-GVector3Float::sUnitY,
		2,
		1000.0F
		));
		
	if (inout_paramObject && mOptionDictionary)
	{
		inout_paramObject->SetValueFloat("viewScale", PlayWithGoldfishStateType::GetSuperSampleScale(GetDictionarySuperSampleType(*mOptionDictionary)));
	}

	mWindowCallback.reset(new PlayWithGoldfishStateWindowCallback(*this));
	TPointerApplicationWindowCallback windowCallback = mWindowCallback;
	mWindow.reset(new GApplicationWindow(windowCallback, inout_paramObject));
	if (!mWindow)
	{
		return;
	}

#ifndef YOUTUBE_PREVIEW
	{
		mComponentButtonInfo = mWindow->CreateViewComponentButton(
			"buttoninfo.png",
			GGuiCoordinate(GVector2Float(1.0F, 1.0F), GVector2Int(-48, -48)),
			GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Int(32, 32)),	
			OnButtonInfo,
			this
			);
		mWindow->AddViewComponentFront(mComponentButtonInfo);
	}
	if (mHasCamera)
	{
		mComponentButtonCamera = mWindow->CreateViewComponentButton(
			"buttoncamera.png",
			GGuiCoordinate(GVector2Float(0.5F, 1.0F), GVector2Int(-16, -48)),
			GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Int(32, 32)),	
			OnButtonCamera,
			this
			);
		mWindow->AddViewComponentFront(mComponentButtonCamera);
	}
	{
		mComponentButtonOption = mWindow->CreateViewComponentButton(
			"buttonoption.png",
			GGuiCoordinate(GVector2Float(0.0F, 1.0F), GVector2Int(16, -48)),
			GGuiCoordinate(GVector2Float(0.0F, 0.0F), GVector2Int(32, 32)),	
			OnButtonOption,
			this
			);
		mWindow->AddViewComponentFront(mComponentButtonOption);
	}
#endif

	mComponentViewRender = mWindow->CreateViewComponentRender(
		mRenderComponent,
		true, //in_transparentBackground, 
		GVector2Float(0.0F, 0.0F), //in_originRatio, //1.0 == 100% screen size
		GVector2Float(1.0F, 1.0F) //in_sizeRatio		
		);
	mWindow->AddViewComponentBack(mComponentViewRender);

	//default material
	{
		const char* arrayAttributeName[] = {"a_position", "a_color" };
		const char* arrayUniformName[] = {"u_mvpMatrix" };
		const GRenderMaterialShaderLoad materialShaderLoad(
			"defaultvertexshader.txt",
			"defaultfragmentshader.txt",
			GRenderMaterialType::TState::TCullBackfacing | GRenderMaterialType::TState::TDepthRead | GRenderMaterialType::TState::TDepthWrite,
			GRenderMaterialType::TBlendMode::TOne,
			GRenderMaterialType::TBlendMode::TZero,
			GCOMMON_ARRAY_SIZE(arrayAttributeName), arrayAttributeName,
			GCOMMON_ARRAY_SIZE(arrayUniformName), arrayUniformName,
			0, NULL
			);
		const GRenderMaterialLoad materialLoad( NULL, &materialShaderLoad );
		TPointerRenderMaterial defaultMaterial = GRenderMaterial::Factory(
			&materialLoad,
			mRenderComponent,
			mFileSystem,
			mTextureManager,
			true
			);

		mRenderComponent->SetDefaultMaterial(defaultMaterial);
	}

	mMaterialManager.reset(new GRenderMaterialManager(mAssetManager, mTextureManager, mFileSystem));

	TMapStringPairFactoryClientCallbackData clientFactory;
	clientFactory["fish"] = TPairFactoryClientCallbackData(PlayWithGoldfishStateSceneComponentFish::Factory, this);

	mScene = GScene::Factory(
		mRenderComponent,
		mFileSystem,
		mAssetManager,
		mMaterialManager,
		mTextureManager,
		clientFactory,
		"defaultscene.data"
		);

	mMaterialGroupManager = GMaterialGroupManager::Factory(
		"materialgroup.data",
		mAssetManager,
		mMaterialManager,
		mRenderComponent
		);

	if (mMaterialGroupManager && mScene)
	{
		TArrayPairStringWeakRenderMaterial arrayStringMaterial;
		arrayStringMaterial.push_back(TPairStringWeakRenderMaterial(
			std::string("body"),
			mMaterialManager->GetMaterialWithoutBindingTextures("placeholdermaterialbody.data", mRenderComponent)
			));
		arrayStringMaterial.push_back(TPairStringWeakRenderMaterial(
			std::string("fin"),
			mMaterialManager->GetMaterialWithoutBindingTextures("placeholdermaterialfin.data", mRenderComponent)
			));
		arrayStringMaterial.push_back(TPairStringWeakRenderMaterial(
			std::string("eyelid"),
			mMaterialManager->GetMaterialWithoutBindingTextures("placeholdermaterialeyelid.data", mRenderComponent)
			));

		PlayWithGoldfishMaterialGroupQuery::Run(*mScene, *mMaterialGroupManager, arrayStringMaterial);
	}

	MakeOptionDialog();

	SetOptionDialogFromDictionary();
	ApplyOptionDictionary(true);

	mWindow->Show();

	return;
}

/*virtual*/ PlayWithGoldfishState::~PlayWithGoldfishState()
{
#ifdef DSC_DEBUG
	sPlayWithGoldfishState = NULL;
#endif
	return;
}

//implement GState
/*virtual*/ const bool PlayWithGoldfishState::OnTick(const float in_timeDelta)
{
	bool result = true;

	if (mWindow)
	{
		result = mWindow->Tick(in_timeDelta);
	}

	return result;
}

//implement GStateApplicationBase
/*virtual*/ void PlayWithGoldfishState::OnSetApplicationActive(const bool in_active)
{
	if (mWindow)
	{
		mWindow->SetApplicationActive(in_active);
	}
	return;
}

//public methods
void PlayWithGoldfishState::ResizeWindow(const GVector2Int& in_size)
{
	//adjust camera to match new size
	const float aspect = ((float)in_size.GetX()) / ((float)in_size.GetY());

	if (mComponentCameraInput)
	{
		TPointerCamera& camera = mComponentCameraInput->GetCamera();

		if (camera)
		{
			camera->SetTop(sDefaultCameraPerspectiveBounds);
			camera->SetBottom(-sDefaultCameraPerspectiveBounds);
			camera->SetLeft(-sDefaultCameraPerspectiveBounds * aspect);
			camera->SetRight(sDefaultCameraPerspectiveBounds * aspect);
		}
	}

	return;
}

void PlayWithGoldfishState::ShakeWindow()
{
	//reset camera
	mComponentCameraInput->Reset(
		-GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		-750.0F,
		-GVector3Float::sUnitY
		);

	for (TArrayWeakPlayWithGoldfishStateSceneComponentFish::iterator iterator = mArrayComponentFish.begin(); iterator != mArrayComponentFish.end(); ++iterator)
	{
		TPointerPlayWithGoldfishStateSceneComponentFish pointerComponentFish = (*iterator).lock();
		if (pointerComponentFish)
		{
			pointerComponentFish->ShakeWindow();
		}
	}

	return;
}

void PlayWithGoldfishState::DismissDialogWindow()
{
	ShowButtons(true);
	return;
}

void PlayWithGoldfishState::InputWindow(const GInput& in_input)
{
	//on shake, reset
	//NOTE: on IPhone, application window callback calls shakeWindow
	if (1 <= in_input.GetButtonCount())
	{
		const bool shake = in_input.GetButton(0).GetDown();
		if (shake)
		{
			ShakeWindow();
		}

#ifdef WIN32
		if (in_input.GetButton(0).GetEdgeDown())
		{
			static int sTrace = 0;
			sTrace += 1;
			if (mMaterialGroupManager && mAssetManager)
			{
				sTrace %= mMaterialGroupManager->GetMaterialGroupCount();
				//mMaterialGroupManager->SetMaterialGroup(sTrace, mAssetManager);
				SetMaterialIndex(sTrace);
			}
		}
#endif
	}

	if (mComponentCameraInput)
	{
		GVector3Float dummyAccelerometer = -GVector3Float::sUnitZ;
		if (mOptionDictionary)
		{
			switch (GetDictionaryViewCameraType(*mOptionDictionary))
			{
			default:
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TAccelerometer:
				{
					const int joystickCount = in_input.GetJoystickCount();
					if (0 < joystickCount)
					{
						dummyAccelerometer = in_input.GetJoystick(0).GetDirection();
					}
				}
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TFront:
				dummyAccelerometer = -GVector3Float::sUnitY;
				break;
			case PlayWithGoldfishStateType::TViewCameraType::THalf:
				dummyAccelerometer = GVector3Float(0.0F, -0.707F, -0.707F);
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TQuater:
				dummyAccelerometer = GVector3Float(0.0F, -0.923F, -0.383F);
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TThreeQuater:
				dummyAccelerometer = GVector3Float(0.0F, -0.383F, -0.923F);
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TTop:
				dummyAccelerometer = -GVector3Float::sUnitZ;
				break;
			case PlayWithGoldfishStateType::TViewCameraType::TUnder:
				dummyAccelerometer = GVector3Float::sUnitZ;
				break;
			}
		}
		mComponentCameraInput->Update(in_input, &dummyAccelerometer);
	}

	if (1 == in_input.GetTouchCount())
	{
		const GVector3Float worldTouchPoint = mComponentCameraInput->GetCamera()->ReverseProjectPoint(
			in_input.GetTouch(0).GetTouchLocation(),
			GVector3Float::sZero
			);

		for (TArrayWeakPlayWithGoldfishStateSceneComponentFish::iterator iterator = mArrayComponentFish.begin(); iterator != mArrayComponentFish.end(); ++iterator)
		{
			TPointerPlayWithGoldfishStateSceneComponentFish pointerComponentFish = (*iterator).lock();
			if (pointerComponentFish)
			{
				pointerComponentFish->SetTouch(worldTouchPoint, in_input.GetTouch(0).GetTouchBegin());
			}
		}
	}

	if (mComponentCameraInput)
	{
		const GVector3Float& cameraTarget = mComponentCameraInput->GetCameraPositionOrigin();
		for (TArrayWeakPlayWithGoldfishStateSceneComponentFish::iterator iterator = mArrayComponentFish.begin(); iterator != mArrayComponentFish.end(); ++iterator)
		{
			TPointerPlayWithGoldfishStateSceneComponentFish pointerComponentFish = (*iterator).lock();
			if (pointerComponentFish)
			{
				pointerComponentFish->SetCameraTarget(cameraTarget);
			}
		}
	}

	return;
}

const bool PlayWithGoldfishState::TickWindow(const float in_timeDelta)
{
#ifdef DSC_DEBUG
	if (in_timeDelta)
	{
		std::string text("FPS: ");
		text += GStringUtility::Print(1.0F / in_timeDelta);
		PutText(text);
	}
#endif

	if (mScene)
	{
		const float localTick = GMath::Minimum(0.1F, in_timeDelta); //0.0166667F;
		GSceneVisitorAllTick<GSceneNodeComponentClient>::Run(*mScene, localTick);
		GSceneVisitorAllTick<GSceneNodeComponentAnimation>::Run(*mScene, localTick);
		GSceneVisitorAllUpdate<GSceneNodeComponentNodeAttach>::Run(*mScene);
		GSceneVisitorAllTick<GSceneNodeComponentVisual>::Run(*mScene, localTick);

	}

	return true;
}

void PlayWithGoldfishState::DrawWindow()
{
	if (mRenderComponent)
	{
		mRenderComponent->RenderStart();
		
		if (mOptionDictionary)
		{
			switch (GetDictionaryViewBackgroundType(*mOptionDictionary))
			{
			default:
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TCamera:
				mRenderComponent->ClearColourBuffer(GColour4Float(0.0F, 0.0F, 0.0F, 0.0F));
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TBlack:
				mRenderComponent->ClearColourBuffer(GColour4Float(0.0F, 0.0F, 0.0F, 1.0F));
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TWhite:
				mRenderComponent->ClearColourBuffer(GColour4Float(1.0F, 1.0F, 1.0F, 1.0F));
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TGray:
				mRenderComponent->ClearColourBuffer(GColour4Float(0.5F, 0.5F, 0.5F, 1.0F));
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TLightBlue:
				mRenderComponent->ClearColourBuffer(GColour4Float(0.8F, 0.8F, 1.0F, 1.0F));
				break;
			case PlayWithGoldfishStateType::TViewBackgroundType::TDarkBlue:
				mRenderComponent->ClearColourBuffer(GColour4Float(0.0F, 0.0F, 0.5F, 1.0F));
				break;
			}
			mRenderComponent->ClearDepthBuffer(1.0F);
		}
		
		if (mScene)
		{
			GSceneVisitorRender::Run(
				*mScene, 
				*mRenderComponent, 
				*mComponentCameraInput->GetCamera()
				);
#ifdef DSC_DEBUG
			mScene->DebugDraw(
				*mRenderComponent,
				*mComponentCameraInput->GetCamera()		
				);
			mScene->GetArrayDebugMatrix().clear();
#endif
		}
#ifdef IPHONE
		mRenderComponent->Present();
#endif		
	}
	/**/
	return;
}


#ifdef DSC_DEBUG
void PlayWithGoldfishState::PutText(const std::string& in_text)
{
	if (mWindow)
	{
		mWindow->PutText(in_text);
	}
}
#endif

//public accessors
GFileSystem& PlayWithGoldfishState::GetFileSystem()
{
	return (*mFileSystem);
}

void PlayWithGoldfishState::AddComponentFish(TPointerPlayWithGoldfishStateSceneComponentFish& inout_componentFish)
{
	mArrayComponentFish.push_back(inout_componentFish);
	return;
}

//private methods
void PlayWithGoldfishState::ShowButtons(const bool in_visible)
{
	if (in_visible)
	{
		if (mComponentButtonInfo) mComponentButtonInfo->Show();
		if (mComponentButtonCamera) mComponentButtonCamera->Show();
		if (mComponentButtonOption) mComponentButtonOption->Show();
	}
	else
	{
		if (mComponentButtonInfo) mComponentButtonInfo->Hide();
		if (mComponentButtonCamera) mComponentButtonCamera->Hide();
		if (mComponentButtonOption) mComponentButtonOption->Hide();
	}
	return;
}

void PlayWithGoldfishState::MakeOptionDictionary()
{
	mOptionDictionary.reset(new GDictionary());
	ResetOptionDictionary();
	return;
}

void PlayWithGoldfishState::ResetOptionDictionary()
{
	if (!mOptionDictionary)
	{
		return;
	}
	GDictionary& optionDictionary = *mOptionDictionary;

	//damn, multiple palces are setting  option dictionary, which means we need a 'force' flag on the SetXXX functions to get back into sync 
	SetDictionaryMaterialIndex(optionDictionary, 0);
	SetDictionaryMovementType(optionDictionary, PlayWithGoldfishStateType::TMovementType::TAI);
	SetDictionaryAnimationType(optionDictionary, PlayWithGoldfishStateType::TAnimationType::TAI);
	SetDictionaryViewBackgroundType(optionDictionary, (PlayWithGoldfishStateType::TViewBackgroundType::TEnum)(PlayWithGoldfishStateType::TViewBackgroundType::TCamera + (mHasCamera ? 0 : 1)));
	SetDictionaryViewCameraType(optionDictionary, (PlayWithGoldfishStateType::TViewCameraType::TEnum)(PlayWithGoldfishStateType::TViewCameraType::TAccelerometer + (mHasAccelerometor ? 0 : 1)));
	SetDictionarySuperSampleType(optionDictionary, PlayWithGoldfishStateType::TSuperSampleType::T_1_5);
	
#ifdef YOUTUBE_PREVIEW
	SetDictionaryViewBackgroundType(optionDictionary, (PlayWithGoldfishStateType::TViewBackgroundType::TEnum)(PlayWithGoldfishStateType::TViewBackgroundType::TLightBlue + (mHasCamera ? 0 : 1)));
#endif

	return;
}

void PlayWithGoldfishState::LoadOptionDictionary()
{
	if ((!mFileSystem) || (!mOptionDictionary))
	{
		return;
	}
	GBuffer buffer;
	const std::string filePath = mFileSystem->AppendFileToPathName(mFileSystem->GetWritableDir(), sApplicationPreferenceFileName);
	if (!mFileSystem->LoadFile(buffer, filePath, false))
	{
		//no preferences yet?
		return;
	}

	GDictionary tempDictionary;
	GDictionary::LoadFromBuffer(tempDictionary, buffer);

	//don't union, cull garbage data...
	GDictionary::SelfUpdateIntersection(*mOptionDictionary, tempDictionary);

	//redundant, but i'm feeling generous
	if (!mHasAccelerometor && (PlayWithGoldfishStateType::TViewCameraType::TAccelerometer == GetDictionaryViewCameraType(*mOptionDictionary)))
	{
		SetDictionaryViewCameraType(*mOptionDictionary, PlayWithGoldfishStateType::TViewCameraType::TTop);
	}
	if (!mHasCamera && (PlayWithGoldfishStateType::TViewBackgroundType::TCamera == GetDictionaryViewBackgroundType(*mOptionDictionary)))
	{
		SetDictionaryViewBackgroundType(*mOptionDictionary, PlayWithGoldfishStateType::TViewBackgroundType::TBlack);
	}

	return;
}

void PlayWithGoldfishState::SaveOptionDictionary()
{
	if ((!mFileSystem) || (!mOptionDictionary))
	{
		return;
	}
	GBuffer buffer;
	GDictionary::WriteToBuffer(buffer, *mOptionDictionary);

	const std::string filePath = mFileSystem->AppendFileToPathName(mFileSystem->GetWritableDir(), sApplicationPreferenceFileName);
	mFileSystem->SaveFile(buffer, filePath);

	return;
}

void PlayWithGoldfishState::ApplyOptionDictionary(const bool in_load)
{
	if (!mOptionDictionary)
	{
		return;
	}

	SetMaterialIndex(GetDictionaryMaterialIndex(*mOptionDictionary), in_load);
	SetMovementType(GetDictionaryMovementType(*mOptionDictionary), in_load);
	SetAnimationType(GetDictionaryAnimationType(*mOptionDictionary), in_load);
	SetViewBackgroundType(GetDictionaryViewBackgroundType(*mOptionDictionary), in_load);
	SetViewCameraType(GetDictionaryViewCameraType(*mOptionDictionary), in_load);
	SetSuperSampleType(GetDictionarySuperSampleType(*mOptionDictionary), in_load);

	return;
}

void PlayWithGoldfishState::MakeOptionDialog()
{
	//option dialog data items
	{
		mOptionDialogDataItem.reset(new GOptionDialogDataItem(
			"OptionTitle",
			"",
			GOptionDialogDataItem::TType::TChildrenDataItem
			));

		//material
		if (mMaterialGroupManager)
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionMaterialKey,
				"OptionMaterialDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this
				));

			const int count = mMaterialGroupManager->GetMaterialGroupCount();
			for (int index = 0; index < count; ++index)
			{
				pointerDataItem->GetArrayString().push_back(mMaterialGroupManager->GetMaterialGroupName(index));
			}

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}

		//movement type
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionMovementTypeKey,
				"OptionMovementTypeDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this,
				true
				));
			for (int index = 0; index < PlayWithGoldfishStateType::TMovementType::TCount; ++index)
			{
				pointerDataItem->GetArrayString().push_back(PlayWithGoldfishStateType::GetMovementTypeString(PlayWithGoldfishStateType::TMovementType::TEnum(index)));
			}
	
			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}
		//animation type
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionAnimationTypeKey,
				"OptionAnimationTypeDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this,
				false
				));
			for (int index = 0; index < PlayWithGoldfishStateType::TAnimationType::TAnimationTurnLeft; ++index)
			{
				pointerDataItem->GetArrayString().push_back(PlayWithGoldfishStateType::GetAnimationTypeString(PlayWithGoldfishStateType::TAnimationType::TEnum(index)));
			}

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}

		//view background
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionViewBackgroundTypeKey,
				"OptionViewBackgroundTypeDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this,
				true
				));
			for (int index = (mHasCamera ? 0 : 1); index < PlayWithGoldfishStateType::TViewBackgroundType::TCount; ++index)
			{
				pointerDataItem->GetArrayString().push_back(PlayWithGoldfishStateType::GetViewBackgroundTypeString(PlayWithGoldfishStateType::TViewBackgroundType::TEnum(index)));
			}

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}

		//view camera
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionViewCameraTypeKey,
				"OptionViewCameraTypeDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this,
				false
				));

			for (int index = (mHasAccelerometor ? 0 : 1); index < PlayWithGoldfishStateType::TViewCameraType::TCount; ++index)
			{
				pointerDataItem->GetArrayString().push_back(PlayWithGoldfishStateType::GetViewCameraTypeString(PlayWithGoldfishStateType::TViewCameraType::TEnum(index)));
			}

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}

		//sample
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionOversampleTypeKey,
				"OptionOversampleTypeDescriptionKey",
				GOptionDialogDataItem::TType::TStringArray,
				OnOptionDialogDataItemCallback,
				this,
				false
				));
			for (int index = 0; index < PlayWithGoldfishStateType::TSuperSampleType::TCount; ++index)
			{
				pointerDataItem->GetArrayString().push_back(PlayWithGoldfishStateType::GetSuperSampleTypeString(PlayWithGoldfishStateType::TSuperSampleType::TEnum(index)));
			}

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}

		//Reset
		{
			TPointerOptionDialogDataItem pointerDataItem;
			pointerDataItem.reset(new GOptionDialogDataItem(
				sOptionResetKey,
				"",
				GOptionDialogDataItem::TType::TButton,
				OnOptionDialogDataItemCallback,
				this,
				false
				));

			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
		}
	}

	return;
}
void PlayWithGoldfishState::SetOptionDialogFromDictionary()
{
	if ((!mOptionDialogDataItem) || (!mOptionDictionary))
	{
		return;
	}

	TArrayPointerOptionDialogDataItem& arrayOptionDictionary = mOptionDialogDataItem->GetArrayChildren();
	for (TArrayPointerOptionDialogDataItem::iterator iterator = arrayOptionDictionary.begin(); iterator != arrayOptionDictionary.end(); ++iterator)
	{
		if (!(*iterator))
		{
			continue;
		}
		GOptionDialogDataItem& dataItem = *(*iterator);
		const std::string& keyName = dataItem.GetNameKey();
		if (keyName == sOptionMaterialKey)
		{
			dataItem.SetInt(GetDictionaryMaterialIndex(*mOptionDictionary));
		}
		else if (keyName == sOptionAnimationTypeKey)
		{
			dataItem.SetInt(GetDictionaryAnimationType(*mOptionDictionary));
		}
		else if (keyName == sOptionMovementTypeKey)
		{
			dataItem.SetInt(GetDictionaryMovementType(*mOptionDictionary));
		}
		else if (keyName == sOptionViewBackgroundTypeKey)
		{
			dataItem.SetInt(GetDictionaryViewBackgroundType(*mOptionDictionary) - (mHasCamera ? 0 : 1));
		}
		else if (keyName == sOptionViewCameraTypeKey)
		{
			dataItem.SetInt(GetDictionaryViewCameraType(*mOptionDictionary) - (mHasAccelerometor ? 0 : 1));
		}
		else if (keyName == sOptionOversampleTypeKey)
		{
			dataItem.SetInt(GetDictionarySuperSampleType(*mOptionDictionary));
		}
	}

	return;
}

void PlayWithGoldfishState::SetMaterialIndex(const int in_index, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_index != GetDictionaryMaterialIndex(*mOptionDictionary))
		{
			SetDictionaryMaterialIndex(*mOptionDictionary, in_index);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	if (mMaterialGroupManager)
	{
		mMaterialGroupManager->SetMaterialGroup(in_index, mAssetManager);
	}

	return;
}

void PlayWithGoldfishState::SetMovementType(const PlayWithGoldfishStateType::TMovementType::TEnum in_movementType, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_movementType != GetDictionaryMovementType(*mOptionDictionary))
		{
			SetDictionaryMovementType(*mOptionDictionary, in_movementType);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	for (TArrayWeakPlayWithGoldfishStateSceneComponentFish::iterator iterator = mArrayComponentFish.begin(); iterator != mArrayComponentFish.end(); ++iterator)
	{
		TPointerPlayWithGoldfishStateSceneComponentFish pointerFish = (*iterator).lock();
		if (pointerFish)
		{
			pointerFish->SetMovementType(in_movementType);
		}
	}

	return;
}
void PlayWithGoldfishState::SetAnimationType(const PlayWithGoldfishStateType::TAnimationType::TEnum in_animationType, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_animationType != GetDictionaryAnimationType(*mOptionDictionary))
		{
			SetDictionaryAnimationType(*mOptionDictionary, in_animationType);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	for (TArrayWeakPlayWithGoldfishStateSceneComponentFish::iterator iterator = mArrayComponentFish.begin(); iterator != mArrayComponentFish.end(); ++iterator)
	{
		TPointerPlayWithGoldfishStateSceneComponentFish pointerFish = (*iterator).lock();
		if (pointerFish)
		{
			pointerFish->SetAnimationType(in_animationType);
		}
	}

	return;
}
void PlayWithGoldfishState::SetViewBackgroundType(const PlayWithGoldfishStateType::TViewBackgroundType::TEnum in_viewBackground, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_viewBackground != GetDictionaryViewBackgroundType(*mOptionDictionary))
		{
			SetDictionaryViewBackgroundType(*mOptionDictionary, in_viewBackground);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	if (PlayWithGoldfishStateType::TViewBackgroundType::TCamera == in_viewBackground)
	{
		mComponentCamera.reset();
		mComponentCamera = mWindow->CreateViewComponentCamera(sCameraFrameRate);
		mWindow->AddViewComponentBack(mComponentCamera);
	}		
	else if (mComponentCamera)
	{
		mWindow->RemoveViewComponent(mComponentCamera);
		mComponentCamera.reset();
	}

	return;
}

void PlayWithGoldfishState::SetViewCameraType(const PlayWithGoldfishStateType::TViewCameraType::TEnum in_viewCameraType, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_viewCameraType != GetDictionaryViewCameraType(*mOptionDictionary))
		{
			SetDictionaryViewCameraType(*mOptionDictionary, in_viewCameraType);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	return;
}

void PlayWithGoldfishState::SetSuperSampleType(const PlayWithGoldfishStateType::TSuperSampleType::TEnum in_superSampleType, const bool in_force)
{
	if (mOptionDictionary)
	{
		if (in_superSampleType != GetDictionarySuperSampleType(*mOptionDictionary))
		{
			SetDictionarySuperSampleType(*mOptionDictionary, in_superSampleType);
			SaveOptionDictionary();
		}
		else if (!in_force)
		{
			return;
		}
	}

	const float scale = PlayWithGoldfishStateType::GetSuperSampleScale(in_superSampleType);
	if (mWindow)
	{
		mWindow->SetSampleScale(scale);
	}

	return;
}

void PlayWithGoldfishState::SetReset()
{
	ResetOptionDictionary();
	SaveOptionDictionary();
	ApplyOptionDictionary(true);
	SetOptionDialogFromDictionary();
	ShakeWindow();

	return;
}
