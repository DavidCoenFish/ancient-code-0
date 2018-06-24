//
//  GDictionary.h
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GDictionary_h_
#define _GDictionary_h_

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "GVector3Float.h"

class GBuffer;

class GDictionary
{
	//typedef
private:
	typedef std::map<std::string, bool> TMapStringBool;
	typedef std::map<std::string, int> TMapStringInt;
	typedef std::map<std::string, float> TMapStringFloat;
	typedef std::map<std::string, std::string> TMapStringString;
	typedef std::map<std::string, GVector3Float> TMapStringVector3Float;

	//static public methods
public:
	static void WriteToBuffer(GBuffer& inout_buffer, const GDictionary& in_dictionary);
	//WARNING: out_dictionary is cleared
	static void LoadFromBuffer(GDictionary& out_dictionary, const GBuffer& in_buffer);

	//append 'in_add' to inout_dictionary
	static void SelfUnion(GDictionary& inout_dictionary, const GDictionary& in_add);
	// update values in inout_dictionary from in_add if found in in_add
	static void SelfUpdateIntersection(GDictionary& inout_dictionary, const GDictionary& in_add);

	//constructor
public:
	GDictionary(const GDictionary& in_src);
	GDictionary();
	~GDictionary();

	//operators
public:
	const GDictionary& operator=(const GDictionary& in_rhs);

	//public accessors
public:
	const bool GetValueBool(const std::string& in_key, const bool in_defaultValue = false)const;
	void SetValueBool(const std::string& in_key, const bool in_value);
	void RemoveKeyBool(const std::string& in_key);

	const float GetValueFloat(const std::string& in_key, const float in_defaultValue = 0.0F)const;
	void SetValueFloat(const std::string& in_key, const float in_value);
	void RemoveKeyFloat(const std::string& in_key);

	const GVector3Float& GetValueVector3Float(const std::string& in_key, const GVector3Float& in_defaultValue = GVector3Float::sZero)const;
	void SetValueVector3Float(const std::string& in_key, const GVector3Float& in_value);
	void RemoveKeyVector3Float(const std::string& in_key);

	const int GetValueInt(const std::string& in_key, const int in_defaultValue = 0)const;
	void SetValueInt(const std::string& in_key, const int in_value);
	void RemoveKeyInt(const std::string& in_key);

	const std::string& GetValueString(const std::string& in_key, const std::string& in_defaultValue = std::string())const;
	void SetValueString(const std::string& in_key, const std::string& in_value);
	void RemoveKeyString(const std::string& in_key);

	//private members;
private:
	TMapStringBool mMapKeyBool;
	TMapStringFloat mMapKeyFloat;
	TMapStringVector3Float mMapKeyVector3Float;
	TMapStringInt mMapKeyInt;
	TMapStringString mMapKeyString;

};

#endif 