//
//  GSceneNodeComponentCollisionDataLoad.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionDataLoad_h_
#define _GSceneNodeComponentCollisionDataLoad_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GSceneNodeComponentCollisionDataBase;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentCollisionDataLoad : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentCollisionDataBase> TPointerSceneNodeComponentCollisionDataBase;

	//constructor
public:
	GSceneNodeComponentCollisionDataLoad(
		const int in_type,
		const void* const in_data
		);
	~GSceneNodeComponentCollisionDataLoad();

	//public methods
public:
	TPointerSceneNodeComponentCollisionDataBase Factory()const;

	//private members;
private:
	const int mType;
	const void* const mData;

};

#endif 