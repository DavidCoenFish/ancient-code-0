//file: FontSpliterData.cpp

#include "FontSpliterData.h"

// construction
FontSpliterData::FontSpliterData(
	const std::string& in_fileName,
	const int in_baseLine		
	)
	: mFileName(in_fileName)
	, mBaseLine(in_baseLine)
{
	return;
}

FontSpliterData::~FontSpliterData()
{
	return;
}
