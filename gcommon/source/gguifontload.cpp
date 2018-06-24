//
//  GGuiFontLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiFontLoad.h"

#include "GGuiFontDataLoad.h"

//constructor
GGuiFontLoad::GGuiFontLoad(
	//const float in_characterHorizontalPad,
	const float in_lineVerticalPad,
	const int in_dataCount,
	const GGuiFontDataLoad* const in_arrayData
	)
	//: mCharacterHorizontalPad(in_characterHorizontalPad)
	: mLineVerticalPad(in_lineVerticalPad)
	, mDataCount(in_dataCount)
	, mArrayData(in_arrayData)
{
	return;
}

GGuiFontLoad::~GGuiFontLoad()
{
	return;
}
