//
//  GGuiLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiLoad_h_
#define _GGuiLoad_h_

#include <boost/noncopyable.hpp>

class GGuiNodeLoad;

class GGuiLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiLoad(
		const unsigned int in_arrayGuiNodeCount,
		const GGuiNodeLoad* const in_arrayGuiNode		
		);
	~GGuiLoad();

	//public accessors
public:
	const unsigned int GetArrayGuiNodeCount()const{ return mArrayGuiNodeCount; }
	const GGuiNodeLoad* const GetArrayGuiNode()const{ return mArrayGuiNode; }

	//private members;
private:
	const unsigned int mArrayGuiNodeCount;
	const GGuiNodeLoad* const mArrayGuiNode;

};

#endif 