//
//  GGuiFont.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiFont_h_
#define _GGuiFont_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class GBuffer;
class GGuiFontDataLoad;
class GGuiFontLoad;

/*
	want an instance of this class to hold a map of character id to array index
*/
class GGuiFont : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerFile;
	typedef boost::weak_ptr<GBuffer> TWeakFile;

	typedef std::map<char, int> TMapCharInt;
	typedef std::map<int, int> TMapIntInt;

	//constructor
public:
	GGuiFont(
		TPointerFile& inout_assetFontLoad
		);
	~GGuiFont();

	//public accessors
public:
	const int GetIndexForChar(
		const char in_value
		)const;

	const int GetIndexForChar(
		const unsigned int in_value
		)const;

	//can return NULL
	const GGuiFontDataLoad* const GetIndexGliph(
		const int in_index
		)const;

	const float GetLineVerticalPad()const;

	//private methods
private:
	const GGuiFontLoad* const GetGuiFontLoad()const;

	//private members;
private:
	TWeakFile mAssetFontLoad;

	TMapCharInt mMapAnsii;
	TMapIntInt mMapInt;

};

#endif 