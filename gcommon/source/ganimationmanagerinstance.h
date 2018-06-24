//
//  GAnimationManagerInstance.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationManagerInstance_h_
#define _GAnimationManagerInstance_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include <vector>
#include "GAnimationType.h"
//#include "GAnimationPlayParam.h" //default formal param constructor? gcc is trying to cal copy constructor for default param :(

class GBuffer;
class GAnimationInstance;
class GVector3Float;
class GAnimationManagerLoad;
class GAnimationWrapper;
class GAnimationPlayParam;

/*
TODO: redesign
intent - interface - implementation are all out of step

*/

class GAnimationManagerInstance : public boost::noncopyable
{
	//typedef
private:
	typedef std::map<std::string, int> TMapStringInt;
	typedef std::pair<std::string, int> TPairStringInt;
	typedef std::map<TPairStringInt, int> TMapPairStringIntInt;

	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GAnimationWrapper> TPointerAnimationWrapper;
	typedef boost::weak_ptr<GAnimationWrapper> TWeakAnimationWrapper;

	typedef std::vector<TPointerAnimationWrapper> TArrayPointerAnimationWrapper;

	//constructor
public:
	GAnimationManagerInstance(TPointerBuffer& inout_assetAnimationManagerLoad);
	~GAnimationManagerInstance();

	//public methods
public:
	void Tick(const float in_timeDelta);

	//return -1 if not found
	const int GetStreamIndex(
		const std::string& in_streamTarget,
		const GAnimationType::TStreamType::TEnum in_type
		);

	TWeakAnimationWrapper PlayAnimation(
		const std::string& in_animationName,
		const GAnimationPlayParam& in_playParam
		);

	void StopAnimation(
		const float in_fadeInDuration = 0.0F,
		const float in_fadeRate = 1.0F
		);

	void RemoveAnimation(
		TWeakAnimationWrapper& inout_animationWrapper
		);

	void ApplyToStreamPosition(
		GVector3Float& out_position,
		const int in_streamIndex
		)const;
	void ApplyToStreamRotation(
		GVector3Float& out_at,
		GVector3Float& out_up,
		const int in_streamIndex
		)const;
	void ApplyToStreamScale(
		float& out_scale,
		const int in_streamIndex
		)const;
	void ApplyToStreamMorphTargetWeight(
		float& out_weight,
		const int in_streamIndex
		)const;

	//private members;
private:
	TWeakBuffer mAssetAnimationManagerLoad;

	TMapStringInt mMapAnimationNameIndex;
	TMapPairStringIntInt mMapStreamTypeIndex;

	TArrayPointerAnimationWrapper mArrayPointerAnimationWrapper;

};

#endif 