//
//  GMeshModifierSkin.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierSkin_h_
#define _GMeshModifierSkin_h_

class GMeshModifierSkinBone;
class GMeshModifierSkinTargetStream;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierSkin
{
	//constructor
public:
	GMeshModifierSkin(
		const unsigned int in_arrayBoneCount,
		const GMeshModifierSkinBone* const in_arrayBone,
		const unsigned int in_arrayTargetStreamCount,
		const GMeshModifierSkinTargetStream* const in_arrayTargetStreamLoad,
		const unsigned int in_pointArrayCount,
		const unsigned int* const in_pointArrayPairIndexDuplicateOffset,
		const unsigned int* const in_pointArrayDuplicate,
		const unsigned int in_vectorArrayCount,
		const unsigned int* const in_vectorArrayPairIndexDuplicateOffset,
		const unsigned int* const in_vectorArrayDuplicate
		);
	~GMeshModifierSkin();

	//public accessors
public:
	const unsigned int GetArrayBoneCount()const{ return mArrayBoneCount; }
	const GMeshModifierSkinBone& GetBone(const int in_index)const;

	const unsigned int GetArrayTargetStreamCount()const{ return mArrayTargetStreamCount; }
	const GMeshModifierSkinTargetStream& GetArrayTargetStreamLoad(const int in_index)const;

	const unsigned int GetPointArrayCount()const{ return mPointArrayCount; }
	const unsigned int* const GetPointArrayPairIndexDuplicateOffset()const{ return mPointArrayPairIndexDuplicateOffset; }
	const unsigned int* const GetPointArrayDuplicate()const{ return mPointArrayDuplicate; }

	const unsigned int GetVectorArrayCount()const{ return mVectorArrayCount; }
	const unsigned int* const GetVectorArrayPairIndexDuplicateOffset()const{ return mVectorArrayPairIndexDuplicateOffset; }
	const unsigned int* const GetVectorArrayDuplicate()const{ return mVectorArrayDuplicate; }

	//private members;
private:
	const unsigned int mArrayBoneCount;
	const GMeshModifierSkinBone* const mArrayBone;

	const unsigned int mArrayTargetStreamCount;
	const GMeshModifierSkinTargetStream* const mArrayTargetStreamLoad;

	const unsigned int mPointArrayCount;
	const unsigned int* const mPointArrayPairIndexDuplicateOffset; 
	const unsigned int* const mPointArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]

	const unsigned int mVectorArrayCount;
	const unsigned int* const mVectorArrayPairIndexDuplicateOffset; 
	const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mArrayDuplicateOffset [count, [index]]

};

#endif 