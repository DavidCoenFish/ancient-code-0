//
//  GSceneNodeComponentCollisionDataSphereLoad.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataSphereLoad_h_
#define _GSceneNodeComponentCollisionDataSphereLoad_h_

#include <boost/noncopyable.hpp>

class GVector3Float;

class GSceneNodeComponentCollisionDataSphereLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentCollisionDataSphereLoad(
		const float in_radius,
		const GVector3Float* const in_origin
		);
	~GSceneNodeComponentCollisionDataSphereLoad();

	//public accessors
public:
	const float GetRadius()const{ return mRadius; }
	const GVector3Float* const GetOrigin()const{ return mOrigin; }

	//private members;
private:
	const float mRadius;
	const GVector3Float* const mOrigin;

};

#endif 