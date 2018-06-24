//
//  GMeshEdge.h
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdge_h_
#define _GMeshEdge_h_

#include <boost/noncopyable.hpp>

class GMeshEdgeNormalDynamic;
class GMeshEdgeNormalStatic;
class GMeshEdgeOnePoly;
class GMeshEdgeTwoPoly;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshEdge : public boost::noncopyable 
{
	//constructor
public:
	GMeshEdge(
		const int in_arrayOnePolyLoadCount = 0,
		const GMeshEdgeOnePoly* const in_arrayOnePolyLoad = 0,
		const int in_arrayTwoPolyLoadCount = 0,
		const GMeshEdgeTwoPoly* const in_arrayTwoPolyLoad = 0,
		const int in_arrayNormalStaticLoadCount = 0,
		const GMeshEdgeNormalStatic* const in_arrayNormalStaticLoad = 0,
		const int in_arrayNormalDynamicLoadCount = 0,
		const GMeshEdgeNormalDynamic* const in_arrayNormalDynamicLoad = 0
		);
	~GMeshEdge();

	//public accessors
public:
	const int GetArrayOnePolyLoadCount()const{ return mArrayOnePolyLoadCount; };
	const GMeshEdgeOnePoly* const GetArrayOnePolyLoad()const{ return mArrayOnePolyLoad; };

	const int GetArrayTwoPolyLoadCount()const{ return mArrayTwoPolyLoadCount; };
	const GMeshEdgeTwoPoly* const GetArrayTwoPolyLoad()const{ return mArrayTwoPolyLoad; };

	const int GetArrayNormalStaticLoadCount()const{ return mArrayNormalStaticLoadCount; };
	const GMeshEdgeNormalStatic* const GetArrayNormalStaticLoad()const{ return mArrayNormalStaticLoad; };

	const int GetArrayNormalDynamicLoadCount()const{ return mArrayNormalDynamicLoadCount; };
	const GMeshEdgeNormalDynamic* const GetArrayNormalDynamicLoad()const{ return mArrayNormalDynamicLoad; };

	//private members;
private:
	const int mArrayOnePolyLoadCount;
	const GMeshEdgeOnePoly* const mArrayOnePolyLoad;
	const int mArrayTwoPolyLoadCount;
	const GMeshEdgeTwoPoly* const mArrayTwoPolyLoad;
	const int mArrayNormalStaticLoadCount;
	const GMeshEdgeNormalStatic* const mArrayNormalStaticLoad;
	const int mArrayNormalDynamicLoadCount;
	const GMeshEdgeNormalDynamic* const mArrayNormalDynamicLoad;

};

#endif 