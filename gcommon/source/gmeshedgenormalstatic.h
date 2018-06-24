//
//  GMeshEdgeNormalStatic.h
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdgeNormalStatic_h_
#define _GMeshEdgeNormalStatic_h_

#include <boost/noncopyable.hpp>
#include "GVector3Float.h"

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshEdgeNormalStatic : public boost::noncopyable 
{
	//constructor
public:
	GMeshEdgeNormalStatic(
		const GVector3Float& in_normal
		);
	~GMeshEdgeNormalStatic();

	//public accessors
public:
	const GVector3Float& GetNormal()const{ return mNormal; }

	//private members;
private:
	const GVector3Float mNormal;

};

#endif 