//
//  GGuiNodeComponentPanelLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentPanelLoad_h_
#define _GGuiNodeComponentPanelLoad_h_

#include <boost/noncopyable.hpp>

class GColour4Byte;
class GVector2Float;
class GGuiNodeComponentPanelLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeComponentPanelLoad(
		const char* const in_name,
		const GColour4Byte* const in_arrayColour,
		const GVector2Float* const in_arrayUVPair,
		const int in_flag
		);
	~GGuiNodeComponentPanelLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; }
	const GColour4Byte* const GetArrayColour()const{ return mArrayColour; }
	const GVector2Float* const GetArrayUVPair()const{ return mArrayUVPair; }
	//const int GetFlag()const{ return mFlag; }

	const bool GetFlagColour1()const;
	const bool GetFlagColour4()const;
	const bool GetFlagUv()const;
	const bool GetFlagVisible()const;

	//private members;
private:
	const char* const mName;
	const GColour4Byte* const mArrayColour;
	const GVector2Float* const mArrayUVPair;
	const int mFlag;
	
};

#endif 