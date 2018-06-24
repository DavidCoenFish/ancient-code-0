//
//  GGuiFontLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiFontLoad_h_
#define _GGuiFontLoad_h_

#include <boost/noncopyable.hpp>

class GGuiFontDataLoad;

/**/
class GGuiFontLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiFontLoad(
		//const float in_characterHorizontalPad,
		const float in_lineVerticalPad,
		const int in_dataCount,
		const GGuiFontDataLoad* const in_arrayData
		);
	~GGuiFontLoad();

	//public accessors
public:
	//const float GetCharacterHorizontalPad()const{ return mCharacterHorizontalPad; }
	const float GetLineVerticalPad()const{ return mLineVerticalPad; }
	const int GetDataCount()const{ return mDataCount; }
	const GGuiFontDataLoad* const GetArrayData()const{ return mArrayData; }

	//private members;
private:
	//const float mCharacterHorizontalPad;
	const float mLineVerticalPad;
	const int mDataCount;
	const GGuiFontDataLoad* const mArrayData;	

};

#endif 