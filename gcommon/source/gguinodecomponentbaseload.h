//
//  GGuiNodeComponentBaseLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentBaseLoad_h_
#define _GGuiNodeComponentBaseLoad_h_

#include <boost/noncopyable.hpp>
#include "GGuiType.h"

class GGuiNodeComponentBaseLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeComponentBaseLoad(
		const GGuiType::TComponentFlag::TFlag in_type,
		const void* const in_data
		);
	~GGuiNodeComponentBaseLoad();

	//public accessors
public:
	const GGuiType::TComponentFlag::TFlag GetType()const{ return (GGuiType::TComponentFlag::TFlag)mType; }
	const void* const GetData()const{ return mData; }

	//private members;
private:
	const unsigned int mType;
	const void* const mData;
};

#endif 