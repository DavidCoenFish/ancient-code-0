//
//  GMeshEdgeOnePoly.cpp
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshEdgeOnePoly.h"

//constructor
GMeshEdgeOnePoly::GMeshEdgeOnePoly(
	const int in_vertexIndexOne,
	const int in_vertexIndexTwo,
	const int in_normalIndex
	)
	: mVertexIndexOne(in_vertexIndexOne)
	, mVertexIndexTwo(in_vertexIndexTwo)
	, mNormalIndex(in_normalIndex)
{
	return;
}

GMeshEdgeOnePoly::~GMeshEdgeOnePoly()
{
	return;
}

