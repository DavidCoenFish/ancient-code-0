//
//  GSceneNodeComponentBase.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentBase_h_
#define _GSceneNodeComponentBase_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

/*
add base flag "support render", "support tick"? visitor shortcut flag?
*/
class GSceneNodeComponentBase : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentBase(const unsigned int in_componentFlag);
	virtual ~GSceneNodeComponentBase();

	//public accessors
public:
	//ideally this would be virtual, but virtual function can be expencive
	const unsigned int GetComponentFlag()const{ return mComponentFlag; }

	//private members
private:
	const unsigned int mComponentFlag;

};

#endif 