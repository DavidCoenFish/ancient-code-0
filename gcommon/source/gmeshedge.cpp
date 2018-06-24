//
//  GMeshEdge.cpp
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshEdge.h"

//constructor
GMeshEdge::GMeshEdge(
	const int in_arrayOnePolyLoadCount,
	const GMeshEdgeOnePoly* const in_arrayOnePolyLoad,
	const int in_arrayTwoPolyLoadCount,
	const GMeshEdgeTwoPoly* const in_arrayTwoPolyLoad,
	const int in_arrayNormalStaticLoadCount,
	const GMeshEdgeNormalStatic* const in_arrayNormalStaticLoad,
	const int in_arrayNormalDynamicLoadCount,
	const GMeshEdgeNormalDynamic* const in_arrayNormalDynamicLoad
	)
	: mArrayOnePolyLoadCount(in_arrayOnePolyLoadCount)
	, mArrayOnePolyLoad(in_arrayOnePolyLoad)
	, mArrayTwoPolyLoadCount(in_arrayTwoPolyLoadCount)
	, mArrayTwoPolyLoad(in_arrayTwoPolyLoad)
	, mArrayNormalStaticLoadCount(in_arrayNormalStaticLoadCount)
	, mArrayNormalStaticLoad(in_arrayNormalStaticLoad)
	, mArrayNormalDynamicLoadCount(in_arrayNormalDynamicLoadCount)
	, mArrayNormalDynamicLoad(in_arrayNormalDynamicLoad)
{
	return;
}

GMeshEdge::~GMeshEdge()
{
	return;
}