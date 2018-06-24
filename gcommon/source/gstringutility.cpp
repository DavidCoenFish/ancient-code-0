//
//  gstringutility.h
//  GCommon
//
//  Created by David Coen on 2011 05 29
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gstringutility.h"

/*static*/ const std::string GStringUtility::sEmpty;


/*static*/ std::string GStringUtility::SafeString(const char* const in_char)
{
	if (in_char)
	{
		return std::string(in_char);
	}
	return sEmpty;
}

/*static*/ const int GStringUtility::StringToEnum(const TArrayPairStringInt& in_arrayStringEnum, const std::string& in_enumString, const int in_default)
{
	for (TArrayPairStringInt::const_iterator iterator = in_arrayStringEnum.begin(); iterator != in_arrayStringEnum.end(); ++iterator)
	{
		if ((*iterator).first == in_enumString)
		{
			return (*iterator).second;
		}
	}
	return in_default;
}

/*static*/ void GStringUtility::Split( const std::string& in_string, const TArrayCharacters& in_delimiters, TArrayString& out_tokens )
{
	out_tokens.clear();

	std::string token;
	for (int trace = 0; trace < (int)in_string.length(); ++trace)
	{
		const char value = in_string[ trace ];
		bool found = false;
		for (TArrayCharacters::const_iterator iterator = in_delimiters.begin(); iterator != in_delimiters.end(); ++iterator)
		{
			if (value == (*iterator))
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			if( 0 != token.length() )
			{
				out_tokens.push_back( token );
				token.clear();
			}
		}
		else
		{
			token += value;
		}
	}

	if( 0 != token.length() )
	{
		out_tokens.push_back( token );
	}

	return;
}

/**/
void GStringUtility::Split( const std::string& in_string, const char in_delimiter, TArrayString& out_tokens )
{
	TArrayCharacters characterArray;
	characterArray.push_back( in_delimiter );

	Split( in_string, characterArray, out_tokens );

	return;
}

/**/
void GStringUtility::Split( const std::string& in_string, const std::string& in_delimiter, TArrayString& out_tokens )
{
	TArrayCharacters characterArray;
	for( int index = 0; index < (int)in_delimiter.length(); ++index )
	{
		characterArray.push_back( in_delimiter[ index ] );
	}

	Split( in_string, characterArray, out_tokens );

	return;
}

/*static*/ void GStringUtility::PathSplitExtention(
	std::string& out_pathFileName,
	std::string& out_extention, //what is on the right of the last "." or empty if no "."
	const std::string& in_string
	)
{
	const std::string::size_type pos = in_string.find_last_of(".");
	if (pos != std::string::npos)
	{
		out_pathFileName = in_string.substr(0, pos);
		out_extention = in_string.substr(pos + 1);
	}
	else
	{
		out_pathFileName = in_string;
	}

	return;
}

/*static*/ void GStringUtility::PathSplitLeaf(
											  std::string& out_subDir, //what is on the left of the last "\" or "/" else empty
											  std::string& out_fileName,
											  const std::string& in_string
											  )
{
	const std::string::size_type posA = in_string.find_last_of("\\");
	const std::string::size_type posB = in_string.find_last_of("//");
	std::string::size_type pos = std::string::npos;
	
	if ((std::string::npos != posA) && 
		(std::string::npos != posB))
	{
		//pos = std::max(posA, posB);
		if (posA < posB)
		{
			pos = posB;
		}
		else 
		{
			pos = posA;
		}
	}
	else if (std::string::npos != posA)
	{
		pos = posA;
	}
	else if (std::string::npos != posB)
	{
		pos = posB;
	}
	
	
	if (pos != std::string::npos)
	{
		out_subDir = in_string.substr(0, pos);
		out_fileName = in_string.substr(pos + 1);
	}
	else
	{
		out_fileName = in_string;
	}
	
	return;
}

#define TEXT_BUFFER_SIZE 64
/*static*/ std::string GStringUtility::PrintMethod(const float in_value)
{
	char temp[TEXT_BUFFER_SIZE];
#ifdef IPHONE
	sprintf(&temp[0], "%.2f", in_value);
#elif WIN32
	sprintf_s(&temp[0], TEXT_BUFFER_SIZE, "%.2f", in_value);
#endif
	return &temp[0];
}

/*static*/ std::string GStringUtility::PrintMethod(const int in_value)
{
	char temp[TEXT_BUFFER_SIZE];
#ifdef IPHONE
	sprintf(&temp[0], "%d", in_value);
#elif WIN32
	sprintf_s(&temp[0], TEXT_BUFFER_SIZE, "%d", in_value);
#endif
	return &temp[0];
}
/*static*/ std::string GStringUtility::PrintMethod02(const int in_value)
{
	char temp[TEXT_BUFFER_SIZE];
#ifdef IPHONE
	sprintf(&temp[0], "%02d", in_value);
#elif WIN32
	sprintf_s(&temp[0], TEXT_BUFFER_SIZE, "%02d", in_value);
#endif
	return &temp[0];
}
