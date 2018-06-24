//
//  GMeshModifierClothSpringData.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierClothSpringData.h"

GMeshModifierClothSpringData::GMeshModifierClothSpringData(
	const int in_vertexIndexOne,
	const int in_vertexIndexTwo,
	const float in_length
	)
	: mVertexIndexOne(in_vertexIndexOne)
	, mVertexIndexTwo(in_vertexIndexTwo)
	, mLength(in_length)
{
	return;
}

GMeshModifierClothSpringData::~GMeshModifierClothSpringData()
{
	return;
}
