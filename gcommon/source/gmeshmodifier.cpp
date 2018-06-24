//
//  GMeshModifier.cpp
//
//  Created by David Coen on 2010 12 16
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifier.h"
#include "GASSERT.h"

//constructor
GMeshModifier::GMeshModifier(
	const TType::TEnum in_type,
	const void* const in_data
	)
	: mType(in_type)
	, mData(in_data)
{
	return;
}

GMeshModifier::~GMeshModifier()
{
	return;
}

//public accessors
const bool GMeshModifier::GetIsSkin()const
{
	return (mType == TType::TSkin);
}

const bool GMeshModifier::GetIsCloth()const
{
	return (mType == TType::TCloth);
}

const bool GMeshModifier::GetIsMorph()const
{
	return (mType == TType::TMorph);
}

const GMeshModifierSkin& GMeshModifier::GetSkin()const
{
	GASSERT(mType == TType::TSkin, "invalid code path");
	return *((GMeshModifierSkin*)mData);
}

const GMeshModifierCloth& GMeshModifier::GetCloth()const
{
	GASSERT(mType == TType::TCloth, "invalid code path");
	return *((GMeshModifierCloth*)mData);
}

const GMeshModifierMorph& GMeshModifier::GetMorph()const
{
	GASSERT(mType == TType::TMorph, "invalid code path");
	return *((GMeshModifierMorph*)mData);
}
