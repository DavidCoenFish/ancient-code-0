//
//  GMeshEdgeNormalDynamic.cpp
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshEdgeNormalDynamic.h"

//constructor
GMeshEdgeNormalDynamic::GMeshEdgeNormalDynamic(
	const int in_vertexIndex0,
	const int in_vertexIndex1,
	const int in_vertexIndex2
	)
	: mVertexIndex0(in_vertexIndex0)
	, mVertexIndex1(in_vertexIndex1)
	, mVertexIndex2(in_vertexIndex2)
{
	return;
}

GMeshEdgeNormalDynamic::~GMeshEdgeNormalDynamic()
{
	return;
}
