//
//  GMeshModifierClothVertexData.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierClothVertexData_h_
#define _GMeshModifierClothVertexData_h_

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierClothVertexData
{
	//constructor
public:
	GMeshModifierClothVertexData(
		//const int in_vertexIndex,
		const float in_weight,
		const int in_duplicateTableOffset
		);
	~GMeshModifierClothVertexData();

	//public accessors
public:
	const int GetVertexIndex(const unsigned int* const in_duplicateTable)const;
	//const int GetVertexIndex()const{ return mVertexIndex; }
	const float GetWeight()const{ return mWeight; }
	const int GetDuplicateTableOffset()const{ return mDuplicateTableOffset; }

	//private members;
private:
	//const int mVertexIndex;
	const float mWeight;
	const int mDuplicateTableOffset;

};

#endif 