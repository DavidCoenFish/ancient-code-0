//
//  GAnimationManagerInstance.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationManagerInstance.h"

#include "GAnimationLoad.h"
#include "GAnimationManagerLoad.h"
#include "GAnimationStreamInfoLoad.h"
#include "GAnimationInstance.h"
#include "GAnimationWrapper.h"
#include "GAnimationPlayParam.h"
#include "GBuffer.h"
#include "GBufferUtility.h"
#include <vector>

typedef boost::shared_ptr<GAnimationInstance> TPointerAnimationInstance;
typedef boost::weak_ptr<GAnimationInstance> TWeakAnimationInstance;

//constructor
GAnimationManagerInstance::GAnimationManagerInstance(TPointerBuffer& inout_assetAnimationManagerLoad)
: mAssetAnimationManagerLoad(inout_assetAnimationManagerLoad)
, mMapAnimationNameIndex()
, mMapStreamTypeIndex()
, mArrayPointerAnimationWrapper()
{
	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*inout_assetAnimationManagerLoad);

	const int animationCount = animationManagerLoad.GetArrayAnimationCount();
	for (int index = 0; index < animationCount; ++index)
	{
		mMapAnimationNameIndex[std::string(animationManagerLoad.GetArrayAnimation()[index].GetName())] = index;
	}

	const int streamCount = animationManagerLoad.GetArrayStreamInfoCount();
	for (int index = 0; index < streamCount; ++index)
	{
		const GAnimationStreamInfoLoad& streamInfo = animationManagerLoad.GetArrayStreamInfo()[index];
		mMapStreamTypeIndex.insert(TMapPairStringIntInt::value_type(			
			TPairStringInt(streamInfo.GetTargetName(), streamInfo.GetTargetType()),
			index
			));
	}

	return;
}

GAnimationManagerInstance::~GAnimationManagerInstance()
{
	return;
}

//public methods
void GAnimationManagerInstance::Tick(const float in_timeDelta)
{
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if (!pointerAsset)
	{
		return;
	}

	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);
	TArrayPointerAnimationWrapper::iterator iterator = mArrayPointerAnimationWrapper.begin();
	while (iterator != mArrayPointerAnimationWrapper.end())
	{
		const TPointerAnimationWrapper& animationWrapper = (*iterator);
	
		//if (!animationWrapper->GetFinished())
		//{
			animationWrapper->SimpleTick(
				in_timeDelta,
				animationManagerLoad
				);
		//}

		//if (animationWrapper->GetFinished())
		//{
		//	iterator = mArrayPointerAnimationWrapper.erase(iterator);
		//}
		//else
		{	
			++iterator;
		}
	}

	return;
}

//return -1 if not found
const int GAnimationManagerInstance::GetStreamIndex(
	const std::string& in_streamTarget,
	const GAnimationType::TStreamType::TEnum in_type
	)
{
	TMapPairStringIntInt::const_iterator iterator = mMapStreamTypeIndex.find(TPairStringInt(in_streamTarget, in_type));
	if (iterator != mMapStreamTypeIndex.end())
	{
		return (*iterator).second;
	}
	return GCOMMON_INVALID_INDEX;
}

GAnimationManagerInstance::TWeakAnimationWrapper GAnimationManagerInstance::PlayAnimation(
	const std::string& in_animationName,
	const GAnimationPlayParam& in_playParam
	)
{
	TPointerAnimationWrapper pointer;
	TMapStringInt::const_iterator iterator = mMapAnimationNameIndex.find(in_animationName);
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if ((iterator != mMapAnimationNameIndex.end()) && pointerAsset)
	{
		const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);

		TPointerAnimationInstance animationInstance;
		animationInstance.reset(new GAnimationInstance(
			animationManagerLoad,
			(*iterator).second,
			in_playParam.GetStartTime()
			));

		pointer.reset(new GAnimationWrapper(
			animationInstance,
			in_playParam.GetWeight(),
			in_playParam.GetTickRate(),
			in_playParam.GetFadeRate(),
			in_playParam.GetFadeInDuration(),
			in_playParam.GetManualTick(),
			in_playParam.GetManualFade()
			));
		mArrayPointerAnimationWrapper.push_back(pointer);
	}

	return pointer;
}

void GAnimationManagerInstance::StopAnimation(
	const float in_fadeInDuration,
	const float in_fadeRate
	)
{
	if (0.0F == in_fadeInDuration)
	{
		mArrayPointerAnimationWrapper.clear();
	}

	for (TArrayPointerAnimationWrapper::iterator iterator = mArrayPointerAnimationWrapper.begin(); iterator != mArrayPointerAnimationWrapper.end(); ++iterator)
	{
		GAnimationWrapper& animationWrapper = *(*iterator);
		animationWrapper.FadeOut(in_fadeInDuration);
		animationWrapper.SetFadeRate(in_fadeRate);
	}

	return;
}

void GAnimationManagerInstance::RemoveAnimation(
	TWeakAnimationWrapper& inout_animationWrapper
	)
{
	TPointerAnimationWrapper animationWrapper = inout_animationWrapper.lock();

	if (!animationWrapper)
	{
		return;
	}

	TArrayPointerAnimationWrapper::iterator iterator = mArrayPointerAnimationWrapper.begin();
	while (iterator != mArrayPointerAnimationWrapper.end())
	{
		const TPointerAnimationWrapper& testAnimationWrapper = (*iterator);
		if (animationWrapper == testAnimationWrapper)
		{
			iterator = mArrayPointerAnimationWrapper.erase(iterator);
		}
		else
		{	
			++iterator;
		}
	}

	return;
}

void GAnimationManagerInstance::ApplyToStreamPosition(
	GVector3Float& out_position,
	const int in_streamIndex
	)const
{
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if (!pointerAsset)
	{
		return;
	}
	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);
	for (TArrayPointerAnimationWrapper::const_iterator iterator = mArrayPointerAnimationWrapper.begin(); iterator != mArrayPointerAnimationWrapper.end(); ++iterator)
	{
		const TPointerAnimationWrapper& animationWrapper = (*iterator);
		//if (animationWrapper->GetFinished())
		//{
		//	continue;
		//}
		animationWrapper->ApplyToStreamPosition(
			out_position,
			animationManagerLoad,
			in_streamIndex
			);
	}

	return;
}

void GAnimationManagerInstance::ApplyToStreamRotation(
	GVector3Float& out_at,
	GVector3Float& out_up,
	const int in_streamIndex
	)const
{
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if (!pointerAsset)
	{
		return;
	}
	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);
	for (TArrayPointerAnimationWrapper::const_iterator iterator = mArrayPointerAnimationWrapper.begin(); iterator != mArrayPointerAnimationWrapper.end(); ++iterator)
	{
		const TPointerAnimationWrapper& animationWrapper = (*iterator);
		//if (animationWrapper->GetFinished())
		//{
		//	continue;
		//}
		animationWrapper->ApplyToStreamRotation(
			out_at,
			out_up,
			animationManagerLoad,
			in_streamIndex
			);
	}

	return;
}
void GAnimationManagerInstance::ApplyToStreamScale(
	float& out_scale,
	const int in_streamIndex
	)const
{
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if (!pointerAsset)
	{
		return;
	}
	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);
	for (TArrayPointerAnimationWrapper::const_iterator iterator = mArrayPointerAnimationWrapper.begin(); iterator != mArrayPointerAnimationWrapper.end(); ++iterator)
	{
		const TPointerAnimationWrapper& animationWrapper = (*iterator);
		//if (animationWrapper->GetFinished())
		//{
		//	continue;
		//}
		animationWrapper->ApplyToStreamFloat(
			out_scale,
			animationManagerLoad,
			in_streamIndex,
			1.0F
			);
	}

	return;
}
void GAnimationManagerInstance::ApplyToStreamMorphTargetWeight(
	float& out_weight,
	const int in_streamIndex
	)const
{
	TPointerBuffer pointerAsset = mAssetAnimationManagerLoad.lock();
	if (!pointerAsset)
	{
		return;
	}
	const GAnimationManagerLoad& animationManagerLoad = GBufferUtility::GetLoadBufferRoot<GAnimationManagerLoad>(*pointerAsset);
	for (TArrayPointerAnimationWrapper::const_iterator iterator = mArrayPointerAnimationWrapper.begin(); iterator != mArrayPointerAnimationWrapper.end(); ++iterator)
	{
		const TPointerAnimationWrapper& animationWrapper = (*iterator);
		//if (animationWrapper->GetFinished())
		//{
		//	continue;
		//}
		animationWrapper->ApplyToStreamFloat(
			out_weight,
			animationManagerLoad,
			in_streamIndex,
			0.0F
			);
	}

	return;
}
