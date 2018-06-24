//
//  GMeshEdgeTwoPoly.cpp
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshEdgeTwoPoly.h"

//constructor
GMeshEdgeTwoPoly::GMeshEdgeTwoPoly(
	const int in_vertexIndexOne,
	const int in_vertexIndexTwo,
	const int in_normalIndexOne,
	const int in_normalIndexTwo
	)
	: mVertexIndexOne(in_vertexIndexOne)
	, mVertexIndexTwo(in_vertexIndexTwo)
	, mNormalIndexOne(in_normalIndexOne)
	, mNormalIndexTwo(in_normalIndexTwo)
{
	return;
}

GMeshEdgeTwoPoly::~GMeshEdgeTwoPoly()
{
	return;
}

