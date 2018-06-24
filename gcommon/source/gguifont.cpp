//
//  GGuiFont.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiFont.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GGuiFontLoad.h"
#include "GGuiFontDataLoad.h"

//constructor
GGuiFont::GGuiFont(
	TPointerFile& inout_assetFontLoad
	)
	: mAssetFontLoad(inout_assetFontLoad)
{
	const GGuiFontLoad* const pFontLoad = GetGuiFontLoad();
	if (pFontLoad)
	{
		int count = pFontLoad->GetDataCount();
		for (int index = 0; index < count; ++index)
		{
			const GGuiFontDataLoad& fontDataLoad = (pFontLoad->GetArrayData()[index]);
			const int id = fontDataLoad.GetID();

			if (fontDataLoad.GetFlagSupportAnsii())
			{
				mMapAnsii[(char)(id & 0xFF)] = index;
			}

			mMapInt[id] = index;
		}
	}

	return;
}

GGuiFont::~GGuiFont()
{
	return;
}

//public accessors
const int GGuiFont::GetIndexForChar(
	const char in_value
	)const
{
	TMapCharInt::const_iterator iterator = mMapAnsii.find(in_value);
	if (iterator != mMapAnsii.end())
	{
		return (*iterator).second;
	}
	return GCOMMON_INVALID_INDEX;
}

const int GGuiFont::GetIndexForChar(
	const unsigned int in_value
	)const
{
	TMapIntInt::const_iterator iterator = mMapInt.find(in_value);
	if (iterator != mMapInt.end())
	{
		return (*iterator).second;
	}
	return GCOMMON_INVALID_INDEX;
}

const GGuiFontDataLoad* const GGuiFont::GetIndexGliph(
	const int in_index
	)const
{
	if (GCOMMON_INVALID_INDEX == in_index)
	{
		return NULL;
	}

	const GGuiFontLoad* const pFont = GetGuiFontLoad();
	if (!pFont)
	{
		return NULL;
	}

	if (pFont->GetDataCount() <= (int)in_index)
	{
		return NULL;
	}

	return &(pFont->GetArrayData()[in_index]);
}

const float GGuiFont::GetLineVerticalPad()const
{
	const GGuiFontLoad* const pFontLoad = GetGuiFontLoad();
	if (!pFontLoad)
	{
		return 0.0F;
	}
	return pFontLoad->GetLineVerticalPad();
}

//private methods
const GGuiFontLoad* const GGuiFont::GetGuiFontLoad()const
{
	TPointerFile pointerAsset = mAssetFontLoad.lock();
	if (!pointerAsset)
	{
		return NULL;
	}
	const GGuiFontLoad& fontLoad = GBufferUtility::GetLoadBufferRoot<GGuiFontLoad>(*pointerAsset);

	return &fontLoad;
}
