//
//  GSceneNodeComponentCollisionDataCapsualLoad.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataCapsualLoad_h_
#define _GSceneNodeComponentCollisionDataCapsualLoad_h_

#include <boost/noncopyable.hpp>

class GVector3Float;

/**/
class GSceneNodeComponentCollisionDataCapsualLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentCollisionDataCapsualLoad(
		const GVector3Float* const in_origin,
		const GVector3Float* const in_direction,
		const float in_radius,
		const float in_length
		);
	~GSceneNodeComponentCollisionDataCapsualLoad();

	//public accessors
public:
	const GVector3Float* const GetOrigin()const{ return mOrigin; }
	const GVector3Float* const GetDirection()const{ return mDirection; }
	const float GetRadius()const{ return mRadius; }
	const float GetLength()const{ return mLength; }

	//private members;
private:
	const GVector3Float* const mOrigin;
	const GVector3Float* const mDirection;
	const float mRadius;
	const float mLength;

};

#endif 