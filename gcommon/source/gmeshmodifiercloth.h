//
//  GMeshModifierCloth.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierCloth_h_
#define _GMeshModifierCloth_h_

class GMeshModifierClothSpringData;
class GMeshModifierClothVertexData;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierCloth
{
	//constructor
public:
	GMeshModifierCloth(
		const float in_gravity,
		const float in_mass,
		const float in_springConstant,
		const float in_springDampen,
		const unsigned int in_springDataArrayCount,
		const GMeshModifierClothSpringData* const in_springDataLoadArray,
		const unsigned int in_vertexDataArrayCount,
		const GMeshModifierClothVertexData* const in_vertexDataLoadArray,
		const unsigned int* const in_vectorArrayDuplicate,
		const unsigned int in_streamTargetArrayCount,
		const unsigned int* const in_streamTargetArray
		);
	~GMeshModifierCloth();

	//public accessors
public:
	const float GetGravity()const{ return mGravity; }
	const float GetMass()const{ return mMass; }
	const float GetSpringConstant()const{ return mSpringConstant; }
	const float GetSpringDampen()const{ return mSpringDampen; }
	const unsigned int GetSpringDataArrayCount()const{ return mSpringDataArrayCount; }
	const GMeshModifierClothSpringData* const GetSpringDataLoadArray()const{ return mSpringDataLoadArray; }

	const unsigned int GetVertexDataArrayCount()const{ return mVertexDataArrayCount; }
	const GMeshModifierClothVertexData* const GetVertexDataLoadArray()const{ return mVertexDataLoadArray;  }

	const unsigned int* const GetVectorArrayDuplicate()const{ return mVectorArrayDuplicate; }

	const unsigned int GetStreamTargetArrayCount()const{ return mStreamTargetArrayCount; }
	const unsigned int* const GetStreamTargetArray()const{ return mStreamTargetArray; }

	//private members;
private:
	const float mGravity;
	const float mMass;
	const float mSpringConstant;
	const float mSpringDampen;
	const unsigned int mSpringDataArrayCount;
	const GMeshModifierClothSpringData* const mSpringDataLoadArray;

	const unsigned int mVertexDataArrayCount;
	const GMeshModifierClothVertexData* const mVertexDataLoadArray;

	const unsigned int* const mVectorArrayDuplicate; //length whatever to hold data for mVertexDataLoadArray [count, [index]]

	const unsigned int mStreamTargetArrayCount;
	const unsigned int* const mStreamTargetArray;

};

#endif 