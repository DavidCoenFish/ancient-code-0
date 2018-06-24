//
//  GSceneNodeComponentCollisionDataBoxLoad.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataBoxLoad_h_
#define _GSceneNodeComponentCollisionDataBoxLoad_h_

#include <boost/noncopyable.hpp>

class GVector3Float;

/**/
class GSceneNodeComponentCollisionDataBoxLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentCollisionDataBoxLoad(
		const GVector3Float* const in_low,
		const GVector3Float* const in_high
		);
	~GSceneNodeComponentCollisionDataBoxLoad();

	//public accessors
public:
	const GVector3Float* const GetLow()const{ return mLow; }
	const GVector3Float* const GetHigh()const{ return mHigh; }

	//private members
private:
	const GVector3Float* const mLow;
	const GVector3Float* const mHigh;

};

#endif 