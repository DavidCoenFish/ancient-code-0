//
//  GSceneNodeComponentFactoryStubLoad.h
//
//  Created by David Coen on 2011 01 28
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentFactoryStubLoad_h_
#define _GSceneNodeComponentFactoryStubLoad_h_

#include <boost/noncopyable.hpp>

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentFactoryStubLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentFactoryStubLoad(
		const char* const in_name,
		const char* const in_data
		);
	~GSceneNodeComponentFactoryStubLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; }
	const char* const GetData()const{ return mData; }

	//private members;
private:
	const char* const mName;
	const char* const mData;

};

#endif 