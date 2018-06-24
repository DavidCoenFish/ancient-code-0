//file: TXml.cpp

#include "TXml.h"
#include <stdio.h>
#include <tinyxml.h>

/*static*/ const bool TXml::TextToBool(const std::string& in_value)
{
	if ((in_value == "TRUE") || 
		(in_value == "True") ||
		(in_value == "true") ||
		(in_value == "YES") ||
		(in_value == "Yes") ||
		(in_value == "yes") ||
		(in_value == "1") )
	{
		return true;
	}

	return false;
}

/*static*/ const std::string TXml::BoolToText(const bool in_value)
{
	if (0 == in_value)
	{
		return "FALSE";
	}
	return "TRUE";
}


/*static*/ const bool TXml::GetAttributeBool(const TiXmlElement* const in_element, const char* const in_attributeName)
{
	if (!in_element || !in_attributeName)
	{
		return false;
	}
	const char* const attributeValue = in_element->Attribute(in_attributeName);
	if (!attributeValue)
	{
		return false;
	}

	return TextToBool(attributeValue);
}

/*static*/ const int TXml::GetAttributeInt(const TiXmlElement* const in_element, const char* const in_attributeName, const int in_default)
{
	if (!in_element || !in_attributeName)
	{
		return in_default;
	}
	const char* const attributeValue = in_element->Attribute(in_attributeName);
	if (!attributeValue)
	{
		return in_default;
	}

	int value = in_default;
	sscanf_s(attributeValue, "%d", &value);

	return value;
}

/*static*/ const std::string TXml::GetAttributeString(const TiXmlElement* const in_element, const char* const in_attributeName)
{
	if (!in_element || !in_attributeName)
	{
		return std::string();
	}
	const char* const attributeValue = in_element->Attribute(in_attributeName);
	if (!attributeValue)
	{
		return std::string();
	}

	return std::string(attributeValue);
}

/*static*/ const bool TXml::GetTextBool(const TiXmlElement* const in_element, const bool in_defaultValue)
{
	bool value = in_defaultValue;
	if (in_element)
	{
		const char* const pText = in_element->GetText();
		if (pText)
		{	
			value = TextToBool(pText);
		}
	}
	return value;
}


/*static*/ const int TXml::GetTextInt(const TiXmlElement* const in_element, const int in_defaultValue)
{
	int value = in_defaultValue;
	if (in_element)
	{
		const char* const pText = in_element->GetText();
		if (pText)
		{	
			sscanf_s(pText, "%d", &value);
		}
	}
	return value;
}

/*static*/ void TXml::SetTextInt(TiXmlElement* const in_parentElement, const int in_value)
{
	if (!in_parentElement)
	{
		return;
	}

	char temp[256];
	sprintf_s(&temp[0], 256, "%d", in_value);
	TiXmlText xmlText(&temp[0]);

	in_parentElement->InsertEndChild(xmlText);

	return;
}


/*static*/ const float TXml::GetTextFloat(const TiXmlElement* const in_element, const float in_default)
{
	float value = in_default;
	if (in_element)
	{
		const char* const pText = in_element->GetText();
		if (pText)
		{	
			value = GetTextFloat(std::string(pText), in_default);
		}
	}
	return value;
}

/*static*/ const float TXml::GetTextFloat(const std::string& in_string, const float in_default)
{
	float value = in_default;
	if (!in_string.empty())
	{
		sscanf_s(in_string.c_str(), "%f", &value);
	}
	return value;
}


/*static*/ void TXml::SetTextFloat(TiXmlElement* const in_parentElement, const float in_value)
{
	if (!in_parentElement)
	{
		return;
	}

	char temp[256];
	sprintf_s(&temp[0], 256, "%f", in_value);
	TiXmlText xmlText(&temp[0]);

	in_parentElement->InsertEndChild(xmlText);

	return;
}

/*static*/ const std::string TXml::GetTextString(const TiXmlElement* const in_element)
{
	if (in_element)
	{
		const char* const pText = in_element->GetText();
		if (pText)
		{	
			return std::string(pText);
		}
	}
	return std::string();
}

/*static*/ void TXml::SetTextString(TiXmlElement* const in_parentElement, const std::string& in_value)
{
	if (!in_parentElement)
	{
		return;
	}

	TiXmlText xmlText(in_value.c_str());

	in_parentElement->InsertEndChild(xmlText);

	return;
}

/*static*/ void TXml::MakeRootElement(
	TiXmlDocument& out_document, 
	const std::string& in_schema
	)
{
	TiXmlElement rootElement("root");
	if (!in_schema.empty())
	{
		rootElement.SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
		rootElement.SetAttribute("xsi:noNamespaceSchemaLocation", in_schema);
	}
	out_document.InsertEndChild(rootElement);
}

/*static*/ void TXml::MakeRootElementFudgePacker(
	TiXmlDocument& out_document
	)
{
	MakeRootElement(
		out_document, 
		"Schema/FudgePacker.xsd"
		);

	//out_document.RootElement()->SetAttribute("version", in_version);
	//out_document.RootElement()->SetAttribute("usePointerDataBlock", "true");

	return;
}
