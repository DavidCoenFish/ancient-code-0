//
//  GDictionary.cpp
//
//  Created by David Coen on 2010/10/14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GDictionary.h"
#include "GAssert.h"
#include "GBuffer.h"
#include "GBufferUtility.h"
#include <map>

typedef std::map<std::string, std::string> TMapStringString;

//local static methods
template <typename IN_TYPE>
static void WriteMapToBuffer(GBuffer& inout_buffer, const std::map<std::string, IN_TYPE>& in_map)
{
	typedef std::map<std::string, IN_TYPE> TMap;
	const int count = in_map.size();
	inout_buffer.AppendData(sizeof(int), (const unsigned char *const)&count);

	for (typename TMap::const_iterator iterator = in_map.begin(); iterator != in_map.end(); ++iterator)
	{
		GBufferUtility::SetString(inout_buffer, (*iterator).first);
		inout_buffer.AppendData(sizeof(IN_TYPE), (const unsigned char *const)&(*iterator).second);
	}

	return;
}

static void WriteMapToBufferString(GBuffer& inout_buffer, const TMapStringString& in_mapString)
{
	const int count = in_mapString.size();
	inout_buffer.AppendData(sizeof(int), (const unsigned char *const)&count);

	for (TMapStringString::const_iterator iterator = in_mapString.begin(); iterator != in_mapString.end(); ++iterator)
	{
		GBufferUtility::SetString(inout_buffer, (*iterator).first);
		GBufferUtility::SetString(inout_buffer, (*iterator).second);
	}

	return;
}

template <typename IN_TYPE>
static void LoadMapFromBuffer(int& inout_cursor, std::map<std::string, IN_TYPE>& out_map, const GBuffer& in_buffer)
{
	const int count = GBufferUtility::GetItemSafe<int>(inout_cursor, in_buffer);

	for (int index = 0; index < count; ++index)
	{
		const std::string key = GBufferUtility::GetStringSafe(inout_cursor, in_buffer);
		const IN_TYPE value = GBufferUtility::GetItemSafe<IN_TYPE>(inout_cursor, in_buffer);
		out_map[key] = value;
	}

	return;
}

static void LoadMapFromBufferString(int& inout_cursor, TMapStringString& out_mapString, const GBuffer& in_buffer)
{
	const int count = GBufferUtility::GetItemSafe<int>(inout_cursor, in_buffer);

	for (int index = 0; index < count; ++index)
	{
		const std::string key = GBufferUtility::GetStringSafe(inout_cursor, in_buffer);
		const std::string value = GBufferUtility::GetStringSafe(inout_cursor, in_buffer);
		out_mapString.insert(TMapStringString::value_type(key, value));
	}

	return;
}

//static public methods
/*static*/ void GDictionary::WriteToBuffer(GBuffer& inout_buffer, const GDictionary& in_dictionary)
{
	WriteMapToBuffer<bool>(inout_buffer, in_dictionary.mMapKeyBool);
	WriteMapToBuffer<float>(inout_buffer, in_dictionary.mMapKeyFloat);
	WriteMapToBuffer<GVector3Float>(inout_buffer, in_dictionary.mMapKeyVector3Float);
	WriteMapToBuffer<int>(inout_buffer, in_dictionary.mMapKeyInt);
	WriteMapToBufferString(inout_buffer, in_dictionary.mMapKeyString);

	return;
}

/*static*/ void GDictionary::LoadFromBuffer(GDictionary& out_dictionary, const GBuffer& in_buffer)
{
	int cursor = 0;
	LoadMapFromBuffer<bool>(cursor, out_dictionary.mMapKeyBool, in_buffer);
	LoadMapFromBuffer<float>(cursor, out_dictionary.mMapKeyFloat, in_buffer);
	LoadMapFromBuffer<GVector3Float>(cursor, out_dictionary.mMapKeyVector3Float, in_buffer);
	LoadMapFromBuffer<int>(cursor, out_dictionary.mMapKeyInt, in_buffer);
	LoadMapFromBufferString(cursor, out_dictionary.mMapKeyString, in_buffer);

	return;
}

template <typename IN_TYPE>
static void LoadMapSelfUnion(std::map<std::string, IN_TYPE>& inout_dest, const std::map<std::string, IN_TYPE>& in_add)
{
	for (typename std::map<std::string, IN_TYPE>::const_iterator iterator = in_add.begin(); iterator != in_add.end(); ++iterator)
	{
		inout_dest.insert(typename std::map<std::string, IN_TYPE>::value_type((*iterator).first, (*iterator).second));
	}

	return;
}

//append 'in_add' to inout_dictionary
/*static*/ void GDictionary::SelfUnion(GDictionary& inout_dictionary, const GDictionary& in_add)
{
	LoadMapSelfUnion<bool>(inout_dictionary.mMapKeyBool, in_add.mMapKeyBool);
	LoadMapSelfUnion<float>(inout_dictionary.mMapKeyFloat, in_add.mMapKeyFloat);
	LoadMapSelfUnion<GVector3Float>(inout_dictionary.mMapKeyVector3Float, in_add.mMapKeyVector3Float);
	LoadMapSelfUnion<int>(inout_dictionary.mMapKeyInt, in_add.mMapKeyInt);
	LoadMapSelfUnion<std::string>(inout_dictionary.mMapKeyString, in_add.mMapKeyString);

	return;
}

template <typename IN_TYPE>
static void LoadMapSelfUpdateIntersection(std::map<std::string, IN_TYPE>& inout_dest, const std::map<std::string, IN_TYPE>& in_add)
{
	for (typename std::map<std::string, IN_TYPE>::iterator iterator = inout_dest.begin(); iterator != inout_dest.end(); ++iterator)
	{
		typename std::map<std::string, IN_TYPE>::const_iterator found = in_add.find((*iterator).first);
		if (found != in_add.end())
		{
			(*iterator).second = (*found).second;
		}
	}

	return;
}

// update values in inout_dictionary from in_add if found in in_add
/*static*/ void GDictionary::SelfUpdateIntersection(GDictionary& inout_dictionary, const GDictionary& in_add)
{
	LoadMapSelfUpdateIntersection<bool>(inout_dictionary.mMapKeyBool, in_add.mMapKeyBool);
	LoadMapSelfUpdateIntersection<float>(inout_dictionary.mMapKeyFloat, in_add.mMapKeyFloat);
	LoadMapSelfUpdateIntersection<GVector3Float>(inout_dictionary.mMapKeyVector3Float, in_add.mMapKeyVector3Float);
	LoadMapSelfUpdateIntersection<int>(inout_dictionary.mMapKeyInt, in_add.mMapKeyInt);
	LoadMapSelfUpdateIntersection<std::string>(inout_dictionary.mMapKeyString, in_add.mMapKeyString);

	return;
}


//constructor
GDictionary::GDictionary(const GDictionary& in_src)
: mMapKeyBool()
, mMapKeyFloat()
, mMapKeyVector3Float()
, mMapKeyInt()
, mMapKeyString()
{
	(*this) = in_src;
	return;
}

GDictionary::GDictionary()
: mMapKeyBool()
, mMapKeyFloat()
, mMapKeyVector3Float()
, mMapKeyInt()
, mMapKeyString()
{
	return;
}

GDictionary::~GDictionary()
{
	return;
}

//operators
const GDictionary& GDictionary::operator=(const GDictionary& in_rhs)
{
	if (this != &in_rhs)
	{
		mMapKeyBool = in_rhs.mMapKeyBool;
		mMapKeyFloat = in_rhs.mMapKeyFloat;
		mMapKeyVector3Float = in_rhs.mMapKeyVector3Float;
		mMapKeyInt = in_rhs.mMapKeyInt;
		mMapKeyString = in_rhs.mMapKeyString;
	}
	return (*this);
}

//public accessors
const bool GDictionary::GetValueBool(const std::string& in_key, const bool in_defaultValue)const
{
	TMapStringBool::const_iterator iterator = mMapKeyBool.find(in_key);
	if (iterator != mMapKeyBool.end())
	{
		return (*iterator).second;
	}
	return in_defaultValue;
}

void GDictionary::SetValueBool(const std::string& in_key, const bool in_value)
{
	mMapKeyBool[in_key] = in_value;
	return;
}

void GDictionary::RemoveKeyBool(const std::string& in_key)
{
	TMapStringBool::iterator iterator = mMapKeyBool.find(in_key);
	if (iterator != mMapKeyBool.end())
	{
		mMapKeyBool.erase(iterator);
	}	
	return;
}

const float GDictionary::GetValueFloat(const std::string& in_key, const float in_defaultValue)const
{
	TMapStringFloat::const_iterator iterator = mMapKeyFloat.find(in_key);
	if (iterator != mMapKeyFloat.end())
	{
		return (*iterator).second;
	}
	return in_defaultValue;
}

void GDictionary::SetValueFloat(const std::string& in_key, const float in_value)
{
	mMapKeyFloat[in_key] = in_value;
	return;
}

void GDictionary::RemoveKeyFloat(const std::string& in_key)
{
	TMapStringFloat::iterator iterator = mMapKeyFloat.find(in_key);
	if (iterator != mMapKeyFloat.end())
	{
		mMapKeyFloat.erase(iterator);
	}	
	return;
}
const GVector3Float& GDictionary::GetValueVector3Float(const std::string& in_key, const GVector3Float& in_defaultValue)const
{
	TMapStringVector3Float::const_iterator iterator = mMapKeyVector3Float.find(in_key);
	if (iterator != mMapKeyVector3Float.end())
	{
		return (*iterator).second;
	}
	return in_defaultValue;
}

void GDictionary::SetValueVector3Float(const std::string& in_key, const GVector3Float& in_value)
{
	mMapKeyVector3Float[in_key] = in_value;
	return;
}

void GDictionary::RemoveKeyVector3Float(const std::string& in_key)
{
	TMapStringVector3Float::iterator iterator = mMapKeyVector3Float.find(in_key);
	if (iterator != mMapKeyVector3Float.end())
	{
		mMapKeyVector3Float.erase(iterator);
	}	
	return;
}


const int GDictionary::GetValueInt(const std::string& in_key, const int in_defaultValue)const
{
	TMapStringInt::const_iterator iterator = mMapKeyInt.find(in_key);
	if (iterator != mMapKeyInt.end())
	{
		return (*iterator).second;
	}
	return in_defaultValue;
}

void GDictionary::SetValueInt(const std::string& in_key, const int in_value)
{
	mMapKeyInt[in_key] = in_value;
	return;
}

void GDictionary::RemoveKeyInt(const std::string& in_key)
{
	TMapStringInt::iterator iterator = mMapKeyInt.find(in_key);
	if (iterator != mMapKeyInt.end())
	{
		mMapKeyInt.erase(iterator);
	}	
	return;
}

const std::string& GDictionary::GetValueString(const std::string& in_key, const std::string& in_defaultValue)const
{
	TMapStringString::const_iterator iterator = mMapKeyString.find(in_key);
	if (iterator != mMapKeyString.end())
	{
		return (*iterator).second;
	}
	return in_defaultValue;
}

void GDictionary::SetValueString(const std::string& in_key, const std::string& in_value)
{
	mMapKeyString[in_key] = in_value;
	return;
}

void GDictionary::RemoveKeyString(const std::string& in_key)
{
	TMapStringString::iterator iterator = mMapKeyString.find(in_key);
	if (iterator != mMapKeyString.end())
	{
		mMapKeyString.erase(iterator);
	}	
	return;
}
