//
//  GSceneNodeComponentClientLoad.h
//
//  Created by David Coen on 2010 12 28
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentClientLoad_h_
#define _GSceneNodeComponentClientLoad_h_

#include <boost/noncopyable.hpp>

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentClientLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentClientLoad(
		const char* const in_clientName
		);
	~GSceneNodeComponentClientLoad();

	//public accessors
public:
	const char* const GetClientName()const{ return mClientName; }

	//private members;
private:
	const char* const mClientName;

};

#endif 