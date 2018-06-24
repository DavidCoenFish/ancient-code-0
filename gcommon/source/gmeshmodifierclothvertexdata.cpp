//
//  GMeshModifierClothVertexData.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierClothVertexData.h"

//constructor
GMeshModifierClothVertexData::GMeshModifierClothVertexData(
	//const int in_vertexIndex,
	const float in_weight,
	const int in_duplicateTableOffset
	)
	//: mVertexIndex(in_vertexIndex)
	: mWeight(in_weight)
	, mDuplicateTableOffset(in_duplicateTableOffset)
{
	return;
}

GMeshModifierClothVertexData::~GMeshModifierClothVertexData()
{
	return;
}

//public accessors
const int GMeshModifierClothVertexData::GetVertexIndex(const unsigned int* const in_duplicateTable)const
{
	//const int count = in_duplicateTable[mDuplicateTableOffset];
	return in_duplicateTable[mDuplicateTableOffset + 1];
}
