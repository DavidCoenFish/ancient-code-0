//file: TXmlPack.cpp

#include "TXmlPack.h"
#include "TXml.h"
#include <stdio.h>
#include <tinyxml.h>

/*static*/ void TXmlPack::PutU8(TiXmlElement* const in_parent, const unsigned char in_value)
{
	if (!in_parent)
	{
		return;
	}

	TiXmlElement element("u8");
	TXml::SetTextInt(&element, in_value);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutU16(TiXmlElement* const in_parent, const unsigned short in_value)
{
	if (!in_parent)
	{
		return;
	}

	TiXmlElement element("u16");
	TXml::SetTextInt(&element, in_value);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutU32(TiXmlElement* const in_parent, const unsigned int in_value)
{
	if (!in_parent)
	{
		return;
	}

	TiXmlElement element("u32");
	TXml::SetTextInt(&element, in_value);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutR32(TiXmlElement* const in_parent, const float in_value)
{
	if (!in_parent)
	{
		return;
	}

	TiXmlElement element("r32");
	TXml::SetTextFloat(&element, in_value);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutString(TiXmlElement* const in_parent, const std::string& in_string)
{
	if (!in_parent)
	{
		return;
	}

	TiXmlElement element("string");
	TXml::SetTextString(&element, in_string);
	in_parent->InsertEndChild(element);

	return;
}

const char* const sArrayHex[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
template <typename IN_TYPE>
static const std::string LocalGetStringValue(const IN_TYPE in_value)
{
	std::string returnValue;
	const unsigned char *const pTrace = (const unsigned char *const)&in_value;
	const int size = sizeof(IN_TYPE);
	for (int index = 0; index < size; ++index)
	{
		returnValue += sArrayHex[(pTrace[size - index - 1] >> 4) & 0x0F];
		returnValue += sArrayHex[pTrace[size - index - 1] & 0x0F];
	}

	return returnValue;
}

/*static*/ void TXmlPack::PutArrayU8(TiXmlElement* const in_parent, const TArrayU8& in_arrayData)
{
	std::string valueString;
	for (TArrayU8::const_iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		valueString += LocalGetStringValue(*iterator);
	}

	TiXmlElement element("u8array");
	TXml::SetTextString(&element, valueString);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutArrayU16(TiXmlElement* const in_parent, const TArrayU16& in_arrayData)
{
	std::string valueString;
	for (TArrayU16::const_iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		valueString += LocalGetStringValue(*iterator);
	}

	TiXmlElement element("u16array");
	TXml::SetTextString(&element, valueString);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutArrayU32(TiXmlElement* const in_parent, const TArrayU32& in_arrayData)
{
	std::string valueString;
	for (TArrayU32::const_iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		valueString += LocalGetStringValue(*iterator);
	}

	TiXmlElement element("u32array");
	TXml::SetTextString(&element, valueString);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutArrayR32(TiXmlElement* const in_parent, const TArrayR32& in_arrayData)
{
	std::string valueString;
	for (TArrayR32::const_iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		valueString += LocalGetStringValue(*iterator);
	}

	TiXmlElement element("r32array");
	TXml::SetTextString(&element, valueString);
	in_parent->InsertEndChild(element);

	return;
}

/*static*/ void TXmlPack::PutArrayString(TiXmlElement* const in_parent, const TArrayString& in_arrayData)
{
	for (TArrayString::const_iterator iterator = in_arrayData.begin(); iterator != in_arrayData.end(); ++iterator)
	{
		PutString(in_parent, *iterator);
	}
	return;
}

static const int GetIntFromHexChar(const char in_value)
{
	for (int index = 0; index < 16; ++index)
	{
		if (in_value == sArrayHex[index][0])
		{
			return index;
		}
	}
	return 0;
}

template <typename IN_TYPE>
static void LocalGetArray(
	std::vector<IN_TYPE>& out_array,
	TiXmlElement* const in_arrayElement
	)
{
	out_array.clear();
	const int stride = sizeof(IN_TYPE);
	const std::string text = TXml::GetTextString(in_arrayElement);
	for (int index = 0; index < (int)text.size(); index += (stride * 2))
	{
		IN_TYPE value = 0;
		unsigned char* const pTrace = (unsigned char* const)&value;
		for (int subIndex = 0; subIndex < stride; ++subIndex)
		{
			const int item0 = GetIntFromHexChar(text[index + (subIndex * 2) + 0]) << 4;
			const int item1 = GetIntFromHexChar(text[index + (subIndex * 2) + 1]);
			pTrace[stride - subIndex - 1] = (item0 | item1);
		}
		out_array.push_back(value);
	}

	return;
}

/*static*/ void TXmlPack::GetArrayU8(TArrayU8& out_array, TiXmlElement* const in_arrayElement)
{
	LocalGetArray(out_array, in_arrayElement);
	return;
}

/*static*/ void TXmlPack::GetArrayU16(TArrayU16& out_array, TiXmlElement* const in_arrayElement)
{
	LocalGetArray(out_array, in_arrayElement);
	return;
}

/*static*/ void TXmlPack::GetArrayU32(TArrayU32& out_array, TiXmlElement* const in_arrayElement)
{
	LocalGetArray(out_array, in_arrayElement);
	return;
}

/*static*/ void TXmlPack::GetArrayR32(TArrayR32& out_array, TiXmlElement* const in_arrayElement)
{
	LocalGetArray(out_array, in_arrayElement);
	return;
}
