//
//  PlayWithGoldfishState.h
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishState_h_
#define _PlayWithGoldfishState_h_

#include <GStateApplicationBase.h>
#include <GVector3Float.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "PlayWithGoldfishStateType.h"

class GState;
class GDictionary;
class GApplicationWindow;
class GRenderComponent;
class GVector2Int;
class GInput;
class GComponentCameraInput;
class GFileSystem;
class GRenderTextureManager;
class GRenderMeshManager;
class GVector3Float;
class GMatrix16Float;
class PlayWithGoldfishStateWindowCallback;
class GApplicationViewComponent;
class GRenderMaterial;
class GRenderMaterialManager;
class GRenderMeshManual;
class GAssetManager;
class GAsset;
class GScene;
class GMaterialGroupManager;
class PlayWithGoldfishStateSceneComponentFish;
class GOptionDialogDataItem;

#ifdef DSC_DEBUG
#define DEBUG_PUT_TEXT(IN_TEXT) DebugPutText(IN_TEXT);
void DebugPutText(const std::string& in_text);
#else
#define DEBUG_PUT_TEXT(IN_TEXT)
#endif	

class PlayWithGoldfishState : public GStateApplicationBase
{
	//typedef
private:
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;
	typedef boost::shared_ptr<GApplicationWindow> TPointerApplicationWindow;
	typedef boost::shared_ptr<PlayWithGoldfishStateWindowCallback> TPointerPlayWithGoldfishStateWindowCallback;
	typedef boost::shared_ptr<GRenderComponent> TPointerComponentRender;
	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::shared_ptr<GRenderTextureManager> TPointerRenderTextureManager;

	typedef boost::shared_ptr<GComponentCameraInput> TPointerComponentCameraInput;
	typedef boost::shared_ptr<GApplicationViewComponent> TPointerApplicationViewComponent;
	typedef boost::shared_ptr<GRenderComponent> TPointerRenderComponent;
	typedef boost::shared_ptr<GRenderMaterial> TPointerRenderMaterial;
	typedef boost::shared_ptr<GRenderMaterialManager> TPointerRenderMaterialManager;
	typedef boost::shared_ptr<GOptionDialogDataItem> TPointerOptionDialogDataItem;
	typedef std::vector<TPointerOptionDialogDataItem> TArrayPointerOptionDialogDataItem;

	typedef boost::shared_ptr<GAssetManager> TPointerAssetManager;
	typedef boost::shared_ptr<GAsset> TPointerAsset;
	typedef boost::weak_ptr<GAsset> TWeakAsset;

	typedef boost::shared_ptr<PlayWithGoldfishStateSceneComponentFish> TPointerPlayWithGoldfishStateSceneComponentFish;
	typedef boost::weak_ptr<PlayWithGoldfishStateSceneComponentFish> TWeakPlayWithGoldfishStateSceneComponentFish;

	typedef std::vector<TWeakPlayWithGoldfishStateSceneComponentFish> TArrayWeakPlayWithGoldfishStateSceneComponentFish;

	typedef boost::shared_ptr<GMaterialGroupManager> TPointerMaterialGroupManager;

	typedef boost::shared_ptr<GScene> TPointerScene;

	//static public methods
public:
	static const int GetDictionaryMaterialIndex(const GDictionary& in_dictionary);
	static void SetDictionaryMaterialIndex(GDictionary& inout_dictionary, const int in_value);

	static const PlayWithGoldfishStateType::TMovementType::TEnum GetDictionaryMovementType(const GDictionary& in_dictionary);
	static void SetDictionaryMovementType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TMovementType::TEnum in_value);

	static const PlayWithGoldfishStateType::TAnimationType::TEnum GetDictionaryAnimationType(const GDictionary& in_dictionary);
	static void SetDictionaryAnimationType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TAnimationType::TEnum in_value);

	static const PlayWithGoldfishStateType::TViewBackgroundType::TEnum GetDictionaryViewBackgroundType(const GDictionary& in_dictionary);
	static void SetDictionaryViewBackgroundType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TViewBackgroundType::TEnum in_value);

	static const PlayWithGoldfishStateType::TViewCameraType::TEnum GetDictionaryViewCameraType(const GDictionary& in_dictionary);
	static void SetDictionaryViewCameraType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TViewCameraType::TEnum in_value);

	static const PlayWithGoldfishStateType::TSuperSampleType::TEnum GetDictionarySuperSampleType(const GDictionary& in_dictionary);
	static void SetDictionarySuperSampleType(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TSuperSampleType::TEnum in_value);

	static void OnButtonInfo(void* const in_data);
	static void OnButtonCamera(void* const in_data);
	static void OnButtonOption(void* const in_data);
	static void OnOptionDialogDataItemCallback(void* const in_callbackData, GOptionDialogDataItem& inout_dataItem);

	//constructor
public:
	PlayWithGoldfishState(TPointerDictionary& inout_paramObject);
	virtual ~PlayWithGoldfishState();

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

#ifdef DSC_DEBUG
	TPointerScene& GetScene(){ return mScene; }
	void PutText(const std::string& in_text);
#endif

	//public accessors
public:
	GFileSystem& GetFileSystem();

	void AddComponentFish(TPointerPlayWithGoldfishStateSceneComponentFish& inout_componentFish);

	//private methods
private:
	void ShowButtons(const bool in_visible);

	void MakeOptionDictionary();
	void ResetOptionDictionary();

	void LoadOptionDictionary();
	void SaveOptionDictionary();
	void ApplyOptionDictionary(const bool in_load); //after load or reset

	void MakeOptionDialog(); 
	void SetOptionDialogFromDictionary(); //set values in dialogOption from dictionary

	void SetMaterialIndex(const int in_index, const bool in_force = false);
	void SetMovementType(const PlayWithGoldfishStateType::TMovementType::TEnum in_movementType, const bool in_force = false);
	void SetAnimationType(const PlayWithGoldfishStateType::TAnimationType::TEnum in_movementType, const bool in_force = false);
	void SetViewBackgroundType(const PlayWithGoldfishStateType::TViewBackgroundType::TEnum in_movementType, const bool in_force = false);
	void SetViewCameraType(const PlayWithGoldfishStateType::TViewCameraType::TEnum in_movementType, const bool in_force = false);
	void SetSuperSampleType(const PlayWithGoldfishStateType::TSuperSampleType::TEnum in_movementType, const bool in_force = false);
	void SetReset();

	//private members
private:
	TPointerPlayWithGoldfishStateWindowCallback mWindowCallback;
	TPointerApplicationWindow mWindow;

	TPointerApplicationViewComponent mComponentViewRender;
	TPointerApplicationViewComponent mComponentCamera;

	TPointerApplicationViewComponent mComponentButtonInfo;
	TPointerApplicationViewComponent mComponentButtonCamera;
	TPointerApplicationViewComponent mComponentButtonOption;

	TPointerFileSystem mFileSystem;
	TPointerAssetManager mAssetManager;
	TPointerRenderTextureManager mTextureManager;
	TPointerRenderMaterialManager mMaterialManager;
	TPointerRenderComponent mRenderComponent;
	TPointerMaterialGroupManager mMaterialGroupManager;

	TPointerComponentCameraInput mComponentCameraInput;

	TPointerScene mScene;

	//allow more than one fish
	TArrayWeakPlayWithGoldfishStateSceneComponentFish mArrayComponentFish;

	TPointerOptionDialogDataItem mOptionDialogDataItem;
	TPointerDictionary mOptionDictionary;

	const bool mHasCamera;
	const bool mHasAccelerometor;

};

#endif //_PlayWithGoldfishState_h_
