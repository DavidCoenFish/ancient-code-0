//
//  GMeshModifierSkinBone.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierSkinBone_h_
#define _GMeshModifierSkinBone_h_

#include "GMatrix16Float.h"

class GMeshModifierSkinBoneVertexData;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierSkinBone
{
	//constructor
public:
	GMeshModifierSkinBone(
		const char* const in_boneName,
		const GMatrix16Float& in_invertBindMatrix,
		const unsigned int in_pointArrayVertexDataLoadCount,
		const GMeshModifierSkinBoneVertexData* const in_pointArrayVertexDataLoad,
		const unsigned int in_vectorArrayVertexDataLoadCount,
		const GMeshModifierSkinBoneVertexData* const in_vectorArrayVertexDataLoad
		);
	~GMeshModifierSkinBone();

	//public accessors
public:
	const char* const GetBoneName()const{ return mBoneName; }

	const GMatrix16Float& GetInvertBindMatrix()const{ return mInvertBindMatrix; }

	const unsigned int GetPointArrayVertexDataLoadCount()const{ return mPointArrayVertexDataLoadCount; }
	const GMeshModifierSkinBoneVertexData* const GetPointArrayVertexDataLoad()const{ return mPointArrayVertexDataLoad; }

	const unsigned int GetVectorArrayVertexDataLoadCount()const{ return mVectorArrayVertexDataLoadCount; }
	const GMeshModifierSkinBoneVertexData* const GetVectorArrayVertexDataLoad()const{ return mVectorArrayVertexDataLoad; }


	//private members
private:
	const char* const mBoneName;

	//move to skeleton instance
	//const unsigned int mParentIndex;
	//const GMatrix16Float mParentRelativeMatrix;

	const GMatrix16Float mInvertBindMatrix;

	const unsigned int mPointArrayVertexDataLoadCount;
	const GMeshModifierSkinBoneVertexData* const mPointArrayVertexDataLoad;

	const unsigned int mVectorArrayVertexDataLoadCount;
	const GMeshModifierSkinBoneVertexData* const mVectorArrayVertexDataLoad;

};

#endif 