//
//  applicationtask.cpp
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "applicationtaskgame.h"
#include "ZombieMission.h"
#include "ZombieMissionClient.h"
#include "ZombieGameVar.h"

#include <gapplicationwindow.h>
#include <grender.h>
#include <gresourcemanager.h>
#include <gfilesystem.h>
#include <gcamera.h>
#include <gmath.h>
#include <GStringUtility.h>
#include <GScratchPad.h>
#include <GGuiManager.h>
#include <GMissionMaster.h>
#include <GView.h>
#include <GDictionary.h>
#include <GBuffer.h>

#ifdef IPHONE
	#include <GIPhoneView.h>
#elif WIN32
	#include <GWin32View.h>
#endif

///////////////////////////////////////////////////////
//constructor
ApplicationTaskGame::ApplicationTaskGame()
: mApplicationWindow()
, mViewRender()
, mRender()
, mFileSystem()
, mResourceManager()
, mRequestMainMenu(false)
{
	mApplicationWindow.reset(new GApplicationWindow);

	mViewRender = mApplicationWindow->CreateViewRender(mRender, false, true, false, 1.0F);
	mApplicationWindow->PushViewFront(mViewRender);

	mFileSystem.reset(new GFileSystem);	
	mScratchPad.reset(new GScratchPad);
	mResourceManager.reset(new GResourceManager(mFileSystem, mRender));

	mZombieMission.reset(new ZombieMission(*this, mApplicationWindow));

	mZombieMission->OptionLoad();

	RunMainMenu();

	mApplicationWindow->Show();

	return;
}

/*virtual*/ ApplicationTaskGame::~ApplicationTaskGame()
{
	mApplicationWindow.reset();
	return;
}

///////////////////////////////////////////////////////
//implement GApplicationTask
/*virtual*/ const GBOOL ApplicationTaskGame::OnTick(const GR32 in_timeDelta)
{
	GBOOL result = GTRUE;
    
    GR32 localTick = in_timeDelta;
    //GR32 localTick = 0.0F;

	if (mApplicationWindow)
	{   
		mApplicationWindow->PreTick(localTick);

		result = mApplicationWindow->Tick(localTick);

		if (result && mRender)
		{
			GApplicationWindowType::TOrientation::TEnum screenOrientation = GApplicationWindowType::TOrientation::TCount;
			int width = 10;
			int height = 10;
			mApplicationWindow->GetScreenDimention(screenOrientation, width, height);

			SetScreen(width, height, screenOrientation);
			Input(mApplicationWindow->GetInput());
    		Tick(localTick);
    		Draw(*mRender);
		}
           
		mApplicationWindow->PostTick(localTick);
        
#ifdef PRINT_FPS
		if (in_timeDelta)
		{
            static bool s_first = true;
            static GR32 s_averageTick = 0.0F;
            if (s_first)
            {
                s_first = false;
                s_averageTick = 1.0F / in_timeDelta;
            }
            else
            {
                s_averageTick = ((1.0F / in_timeDelta) * 0.05F) + (s_averageTick * 0.95F);
            }
            
			std::string text("FPS: ");
			text += GStringUtility::PrintMethod(s_averageTick);
			mApplicationWindow->PutText(text);
		}
#endif
	}

	return result;
}

/*virtual*/ void ApplicationTaskGame::OnSetActive(const GBOOL in_active)
{
	return;
}

///////////////////////////////////////////////////////
//public methods
void ApplicationTaskGame::RunMainMenu()
{
	if (mZombieMission && mApplicationWindow)
	{
		mZombieMission->RunMainMenu();
	}

	//hmmn,  what to do if you want a pause before menu comes up, let mission trigger menu?
	//if (mApplicationWindow && mOptionDialogDataItem)
	//{
	//	ApplyUnlockToMenuData();//could have changed when game was run
	//	mApplicationWindow->InvokeOptionDialog(mOptionDialogDataItem, false);
	//}
}

///////////////////////////////////////////////////////
//private methods
GVOID ApplicationTaskGame::Input(const GInput& in_input)
{
	if (mZombieMission)
	{
		mZombieMission->Input(in_input);
	}
}

GVOID ApplicationTaskGame::Tick(const GR32 in_timeDelta)
{
#ifdef PRINT_FPS
	if (in_timeDelta)
	{
		std::string text("FPS: ");
		text += GStringUtility::PrintMethod(1.0F / in_timeDelta);
		PutText(text);
	}
#endif


	if (mZombieMission)
	{
		if (!mZombieMission->Tick(in_timeDelta))
		{
			mRequestMainMenu = true;
		}
	}

	if (mRequestMainMenu)
	{
		mRequestMainMenu = false;
		RunMainMenu();
	}

	return;
}

GVOID ApplicationTaskGame::Draw(GRender& inout_render)
{
	//inout_render.RenderStart();
	
	if (mZombieMission)
	{
		mZombieMission->Draw(inout_render);
	}
#ifdef IPHONE
	inout_render.Present();
#endif		
}

GVOID ApplicationTaskGame::SetScreen(
	const GS32 in_screenWidth, 
	const GS32 in_screenHeight, 
	const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
	)
{
	if (mZombieMission)
	{
		mZombieMission->Resize(in_screenWidth, in_screenHeight, in_screenOrientation);
	}
}

//private methods

//void ApplicationTaskGame::MakeMainMenu()
//{
//	//option dialog data items
//	{
//		mOptionDialogDataItem.reset(new GOptionDialogDataItem(
//#ifdef TRIAL
//			"MainMenuTitleTrialKey",
//#else
//			"MainMenuTitleKey",
//#endif
//			"",
//			GOptionDialogDataItem::TType::TChildrenDataItem
//			));
//
//		//new game
//		{
//			TPointerOptionDialogDataItem pointerDataItem;
//			pointerDataItem.reset(new GOptionDialogDataItem(
//				"NewGameKey",
//				"NewGameFocusDescriptionKey",
//				GOptionDialogDataItem::TType::TChildrenDataItem,
//				0,
//				0,
//				true
//				));
//			//new game easy
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sNewGameEasyKey,
//					"",
//					GOptionDialogDataItem::TType::TButton,
//					OnOptionDialogDataItemCallback,
//					this,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//new game Normal
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sNewGameNormalKey,
//					"",
//					GOptionDialogDataItem::TType::TButton,
//					OnOptionDialogDataItemCallback,
//					this,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemNewGameNormal = pointerDataItemChild;
//			}
//			//new game Hard
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sNewGameHardKey,
//					"",
//					GOptionDialogDataItem::TType::TButton,
//					OnOptionDialogDataItemCallback,
//					this,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemNewGameHard = pointerDataItemChild;
//			}
//
//			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
//		}
//
//		//resume game
//		{
//			TPointerOptionDialogDataItem pointerDataItem;
//			pointerDataItem.reset(new GOptionDialogDataItem(
//				sResumeGameKey,
//				"",
//				GOptionDialogDataItem::TType::TButton,
//				OnOptionDialogDataItemCallback,
//				this,
//				false
//				));
//			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
//		}
//
//		//unlocks
//		{
//			TPointerOptionDialogDataItem pointerDataItem;
//			pointerDataItem.reset(new GOptionDialogDataItem(
//				"UnlocksKey",
//				"",
//				GOptionDialogDataItem::TType::TChildrenDataItem,
//				0,
//				0,
//				false
//				));
//			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
//			
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("GameModeUnlockKey", "", GOptionDialogDataItem::TType::TText ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//				
//			//first blood
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sFirstBloodParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TFirstBlood),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				//pointerDataItemChild->SetLocked(true);
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TFirstBlood] = pointerDataItemChild;
//
//				//first blood switch
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sFirstBloodKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//i'd be back
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sIdBeBackParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TIdBeBack),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TIdBeBack] = pointerDataItemChild;
//
//				//first blood switch
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sIdBeBackKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("HealthUnlockKey", "", GOptionDialogDataItem::TType::TText ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//
//			//slug queen
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sSlugQueenParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TSlugQueen),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TSlugQueen] = pointerDataItemChild;
//
//				//SlugQueen
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sSlugQueenKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//bullet proof
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sBulletProofParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TBulletProof),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TBulletProof] = pointerDataItemChild;
//
//				//BulletProof
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sBulletProofKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//newb
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sNewbParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TNewb),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TNewb] = pointerDataItemChild;
//
//				//newb
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sNewbKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//			
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("AmmoUnlockKey", "", GOptionDialogDataItem::TType::TText ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}			
//
//			//accuracy
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sAccuracyParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TAccuracy),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TAccuracy] = pointerDataItemChild;
//
//				//accuracy
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sAccuracyKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//efficiency
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sEfficiencyParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TEfficiency),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TEfficiency] = pointerDataItemChild;
//
//				//efficiency
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sEfficiencyKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//head hunter
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sHeadHunterParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::THeadHunter),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::THeadHunter] = pointerDataItemChild;
//
//				//HeadHunter
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sHeadHunterKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//good effort
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sGoodEffortParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TGoodWork),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TGoodWork] = pointerDataItemChild;
//
//				//GoodEffort
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sGoodEffortKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//percistance
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sPersistanceParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TPercistance),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TPercistance] = pointerDataItemChild;
//
//				//Persistance
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sPersistanceKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//eradicator
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sEradicatorParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TEradicator),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TEradicator] = pointerDataItemChild;
//
//				//Eradicator
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sEradicatorKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//			
//			
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("MiscUnlockKey", "", GOptionDialogDataItem::TType::TText ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}				
//
//			//rock
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sRockParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TRock),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					true
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TRock] = pointerDataItemChild;
//
//				//rock
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sRockKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//mission possible
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sMissionPossibleParentKey,
//					ZombieStateType::GetAchieviementString(ZombieStateType::TAchieviement::TMissionPossible),
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//				mOptionDialogDataItemUnlocks[ZombieStateType::TAchieviement::TMissionPossible] = pointerDataItemChild;
//
//				//MissionPossible
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sMissionPossibleKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//				}
//			}
//
//			//enable all
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(
//					sUnlocksEnableParentKey,
//					"UnlocksEnableParentFocusedKey",
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//
//				//enable
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sUnlocksEnableKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerDataItemChild->GetArrayChildren().push_back(pointerDataItemChildChild);
//					mOptionDialogDataUnlockEnable = pointerDataItemChildChild;
//				}
//			}
//		}
//		//options
//		{
//			TPointerOptionDialogDataItem pointerDataItem;
//			pointerDataItem.reset(new GOptionDialogDataItem(
//				"OptionsKey",
//				"",
//				GOptionDialogDataItem::TType::TChildrenDataItem,
//				0,
//				0,
//				false
//				));
//			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
//
//			//antialias	
//			{
//				TPointerOptionDialogDataItem pointerChildDataItem;
//				pointerChildDataItem.reset(new GOptionDialogDataItem(
//					sAntiAliasKey,
//					"OptionAntiAliasFocusDescriptionKey",
//					GOptionDialogDataItem::TType::TStringArray,
//					OnOptionDialogDataItemCallback,
//					this,
//					true
//					));
//					for (int index = 0; index < ZombieStateType::TSuperSampleType::TCount; ++index)
//					{
//						pointerChildDataItem->GetArrayString().push_back(ZombieStateType::GetSuperSampleTypeString(ZombieStateType::TSuperSampleType::TEnum(index)));
//					}
//					pointerChildDataItem->SetInt(ZombieStateType::TSuperSampleType::T_1_5);
//					
//				pointerDataItem->GetArrayChildren().push_back(pointerChildDataItem);
//				mOptionDialogDataItemAntiAlias = pointerChildDataItem;
//			}
//			
//			//sGoreParentKey
//			{
//				TPointerOptionDialogDataItem pointerChildDataItem;
//				pointerChildDataItem.reset(new GOptionDialogDataItem(
//					sGoreParentKey,
//					"OptionGoreFocusDescriptionKey",
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//
//				//sGoreKey
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sGoreKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSwitch, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerChildDataItem->GetArrayChildren().push_back(pointerDataItemChildChild);
//					mOptionDialogDataItemGore = pointerDataItemChildChild;
//				}
//					
//				pointerDataItem->GetArrayChildren().push_back(pointerChildDataItem);
//			}
//
//			//volume
//			{
//				TPointerOptionDialogDataItem pointerChildDataItem;
//				pointerChildDataItem.reset(new GOptionDialogDataItem(
//					sVolumeKey,
//					"VolumeEffectsFocusDescriptionKey",
//					GOptionDialogDataItem::TType::TParentSlider,
//					0,
//					0,
//					false
//					));
//					
//				//sound effect volume
//				{
//					TPointerOptionDialogDataItem pointerDataItemChildChild;
//					pointerDataItemChildChild.reset(new GOptionDialogDataItem(
//						sVolumeEffectsKey, 
//						"", 
//						GOptionDialogDataItem::TType::TSlider, 
//						OnOptionDialogDataItemCallback, 
//						this, 
//						false
//						));
//					pointerChildDataItem->GetArrayChildren().push_back(pointerDataItemChildChild);
//					mOptionDialogDataItemVolume = pointerDataItemChildChild;
//				}
//					
//				pointerDataItem->GetArrayChildren().push_back(pointerChildDataItem);
//			}
//			
//			//reset
//			{
//				TPointerOptionDialogDataItem pointerChildDataItem;
//				pointerChildDataItem.reset(new GOptionDialogDataItem(
//					sResetKey,
//					"OptionResetFocusDescriptionKey",
//					GOptionDialogDataItem::TType::TButton,
//					OnOptionDialogDataItemCallback,
//					this,
//					false
//					));
//					
//				pointerDataItem->GetArrayChildren().push_back(pointerChildDataItem);
//			}
//		}
//		//credits
//		{
//			TPointerOptionDialogDataItem pointerDataItem;
//			pointerDataItem.reset(new GOptionDialogDataItem(
//				"CreditsKey",
//				"",
//				GOptionDialogDataItem::TType::TChildrenDataItem,
//				0,
//				0,
//				false
//				));
//			mOptionDialogDataItem->GetArrayChildren().push_back(pointerDataItem);
//#ifdef TRIAL
//			//credits text 0
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("CreditText0Key", "", GOptionDialogDataItem::TType::TText));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//app store button
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(sCreditAppStoreKey, "", GOptionDialogDataItem::TType::TButton, OnOptionDialogDataItemCallback, this, false));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//#endif			
//			//credits text 1
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("CreditText1Key", "", GOptionDialogDataItem::TType::TText));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//credits text 2
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem("CreditText2Key", "", GOptionDialogDataItem::TType::TText));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//#ifdef DEBUG_LEVELS
//			//new game debug 0
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(sNewGameDebug0Key, "", GOptionDialogDataItem::TType::TButton, OnOptionDialogDataItemCallback, this, true ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//new game debug 1
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(sNewGameDebug1Key, "", GOptionDialogDataItem::TType::TButton, OnOptionDialogDataItemCallback, this, true ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//new game debug 2
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(sNewGameDebug2Key, "", GOptionDialogDataItem::TType::TButton, OnOptionDialogDataItemCallback, this, true ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//			//new game debug 3
//			{
//				TPointerOptionDialogDataItem pointerDataItemChild;
//				pointerDataItemChild.reset(new GOptionDialogDataItem(sNewGameDebug3Key, "", GOptionDialogDataItem::TType::TButton, OnOptionDialogDataItemCallback, this, false ));
//				pointerDataItem->GetArrayChildren().push_back(pointerDataItemChild);
//			}
//#endif
//		}
//	}

//	return;
//}

//#ifdef DSC_DEBUG
void ApplicationTaskGame::PutText(const std::string& in_text)
{
	if (mApplicationWindow)
	{
		mApplicationWindow->PutText(in_text);
	}
}
//#endif
//
////menu triggers
//void ApplicationTaskGame::NewGame(const ZombieStateType::TGameDifficulty::TEnum in_type)
//{
//	if (mZombieMission && mApplicationWindow)
//	{
//		mZombieMission->OptionSave();
//
//		GApplicationWindow& applicationWindow = *mApplicationWindow;
//		//applicationWindow.DismissOptionDialog();
//		mZombieMission->NewGame(in_type);
//	}
//
//	return;
//}
//
//void ApplicationTaskGame::NewGameDebug(const int in_index)
//{
//	if (mZombieMission && mApplicationWindow)
//	{
//		GApplicationWindow& applicationWindow = *mApplicationWindow;
//	//	applicationWindow.DismissOptionDialog();
//		mZombieMission->RunDebugLevel(in_index, applicationWindow);
//	}
//
//	return;
//}
//
//void ApplicationTaskGame::SetUnlock(const ZombieStateType::TAchieviement::TEnum in_acheivement, const bool in_active)
//{
//	if (mZombieMission)
//	{
//		if (in_active)
//		{
//			mZombieMission->GetGameVar()->SetUnlockState(in_acheivement, ZombieStateType::TAchieviementState::TUnlockedActive);
//		}
//		else
//		{
//			mZombieMission->GetGameVar()->SetUnlockState(in_acheivement, ZombieStateType::TAchieviementState::TUnlockedDeactive);
//		}
//		//ApplyUnlockToMenuData();
//	}
//	return;
//}
//
//
//void ApplicationTaskGame::SetSuperSampleType(const ZombieStateType::TSuperSampleType::TEnum in_type)
//{
//	const float scale = ZombieStateType::GetSuperSampleScale(in_type);
//	if (mApplicationWindow)
//	{
//		mApplicationWindow->SetSampleScale(scale);
//	}
//
//	return;
//}
//
//void ApplicationTaskGame::RestoreGame()
//{
//	if (mZombieMission && mApplicationWindow)
//	{
//		GApplicationWindow& applicationWindow = *mApplicationWindow;
//		//applicationWindow.DismissOptionDialog();
//		mZombieMission->RestoreGame(applicationWindow);
//	}
//	return;
//}
