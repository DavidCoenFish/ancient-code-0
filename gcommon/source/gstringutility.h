//
//  gstringutility.h
//  GCommon
//
//  Created by David Coen on 2011 05 29
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GStringUtility_H_
#define _GStringUtility_H_

#include <string>
#include <vector>
#include "gcommon.h"

/**/
struct GStringUtility
{
	////////////////////////////////////////////////////
	// typedefs
private:
	typedef std::vector<std::string> TArrayString;
	typedef std::vector<GCHAR> TArrayCharacters;

	typedef std::pair<std::string, GS32> TPairStringInt;
	typedef std::vector<TPairStringInt> TArrayPairStringInt;

	////////////////////////////////////////////////////
	// disabled
private:
	~GStringUtility();

	////////////////////////////////////////////////////
	// static public methods
public:
	static std::string SafeString(const char* const in_char = NULL);
	
	static const int StringToEnum(const TArrayPairStringInt& in_arrayStringEnum, const std::string& in_enumString, const int in_default = -1);

	//spit a string into tokens whenever any delimiter is hit
	static void Split(const std::string& in_string, const TArrayCharacters& in_delimiters, TArrayString& out_tokens);
	static void Split(const std::string& in_string, const char in_delimiter, TArrayString& out_tokens);
	static void Split(const std::string& in_string, const std::string& in_delimiterString, TArrayString& out_tokens);

	static void PathSplitExtention(
		std::string& out_pathFileName,
		std::string& out_extention, //what is on the right of the last "." or empty if no "."
		const std::string& in_string
		);
	static void PathSplitLeaf(
		std::string& out_subDir, //what is on the left of the last "\" or "/" else empty
		std::string& out_fileName, 
		const std::string& in_string
		);

	static std::string PrintMethod(const float in_value);
	static std::string PrintMethod(const int in_value);
	static std::string PrintMethod02(const int in_value);

	static const std::string sEmpty;
};

#endif // _GStringUtility_H_