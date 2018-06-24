//file: TXml.h
#ifndef _TXml_H_
#define _TXml_H_

#include <vector>
#include <string>

class TiXmlElement;
class TiXmlDocument;

/**/
struct TXml
{
	// typedefs
private:
	typedef std::vector<unsigned char> TArrayByte;

	// disabled
private:
	~TXml();

	// static public methods
public:
	static const bool TextToBool(const std::string& in_value);
	static const std::string BoolToText(const bool in_value);

	static const bool GetAttributeBool(const TiXmlElement* const in_element, const char* const in_attributeName);
	static const int GetAttributeInt(const TiXmlElement* const in_element, const char* const in_attributeName, const int in_default);
	static const std::string GetAttributeString(const TiXmlElement* const in_element, const char* const in_attributeName);

	static const bool GetTextBool(const TiXmlElement* const in_element, const bool in_defaultValue = false);

	static const int GetTextInt(const TiXmlElement* const in_element, const int in_defaultValue = 0);
	static void SetTextInt(TiXmlElement* const in_parentElement, const int in_value);

	static const float GetTextFloat(const TiXmlElement* const in_element, const float in_default = 0.0F);
	static const float GetTextFloat(const std::string& in_string, const float in_default = 0.0F);
	static void SetTextFloat(TiXmlElement* const in_parentElement, const float in_value);

	static const std::string GetTextString(const TiXmlElement* const in_element);
	static void SetTextString(TiXmlElement* const in_parentElement, const std::string& in_value);

	static void MakeRootElement(
		TiXmlDocument& out_document, 
		const std::string& in_schema
		);
	static void MakeRootElementFudgePacker(
		TiXmlDocument& out_document
		);


};

#endif // _TXml_H_