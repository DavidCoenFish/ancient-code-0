//
//  GSkeletonInstance.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSkeletonInstance_h_
#define _GSkeletonInstance_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>

class GSkeletonBone;
class GBuffer;
class GMatrix16Float;
class GSkeletonLoad;

/**/
class GSkeletonInstance : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GSkeletonBone> TPointerSkeletonBone;
	typedef std::vector<TPointerSkeletonBone> TArrayPointerSkeletonBone;
	typedef std::vector<float> TArrayFloat;

	//constructor
public:
	GSkeletonInstance(TPointerBuffer& inout_assetSkeletonLoad);
	~GSkeletonInstance();

	//public accessors
public:
	const int GetBoneIndex(const std::string& in_boneName)const;
	const int GetBoneCount()const;
	const char* const GetBoneName(const int in_boneIndex)const;
	const int GetBoneParentIndex(const int in_boneIndex)const;

	const GMatrix16Float& GetOriginalParentRelativeBoneMatrix(const int in_boneIndex)const;
	const GMatrix16Float& GetParentRelativeBoneMatrix(const int in_boneIndex)const;
	void SetParentRelativeBoneMatrix(const int in_boneIndex, const GMatrix16Float& in_parentRelativeMatrix);

	const GMatrix16Float& GetBoneObjectMatrix(const int in_boneIndex);

	const int GetMorphTargetIndex(const std::string& in_morphTargetName)const;
	const int GetMorphTargetCount()const;
	const char* const GetMorphTargetName(const int in_morphTargetIndex)const;
	const float GetMorphTargetWeight(const int in_morphTargetIndex)const;
	void SetMorphTargetWeight(const int in_morphTargetIndex, const float in_weight);

	//private methods
private:
	void MarkBoneChildDirty(const int in_boneIndex, const GSkeletonLoad& in_skeletonLoad);

	//private members;
private:
	TWeakBuffer mAssetSkeletonLoad;
	TArrayPointerSkeletonBone mArrayBone;
	TArrayFloat mArrayMorphTargetWeight;

};

#endif 