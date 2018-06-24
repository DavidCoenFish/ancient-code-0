//file: TXmlPack.h
#ifndef _TXmlPack_H_
#define _TXmlPack_H_

#include <vector>
#include <string>

class TiXmlElement;

/**/
struct TXmlPack
{
	// typedefs
private:
	typedef std::vector<unsigned char> TArrayU8;
	typedef std::vector<unsigned short> TArrayU16;
	typedef std::vector<unsigned int> TArrayU32;
	typedef std::vector<float> TArrayR32;
	typedef std::vector<std::string> TArrayString;

	// disabled
private:
	~TXmlPack();

	// static public methods
public:
	static void PutU8(TiXmlElement* const in_parent, const unsigned char in_value);
	static void PutU16(TiXmlElement* const in_parent, const unsigned short in_value);
	static void PutU32(TiXmlElement* const in_parent, const unsigned int in_value);
	static void PutR32(TiXmlElement* const in_parent, const float in_value);
	static void PutString(TiXmlElement* const in_parent, const std::string& in_string);

	static void PutArrayU8(TiXmlElement* const in_parent, const TArrayU8& in_arrayData);
	static void PutArrayU16(TiXmlElement* const in_parent, const TArrayU16& in_arrayData);
	static void PutArrayU32(TiXmlElement* const in_parent, const TArrayU32& in_arrayData);
	static void PutArrayR32(TiXmlElement* const in_parent, const TArrayR32& in_arrayData);
	static void PutArrayString(TiXmlElement* const in_parent, const TArrayString& in_arrayData);

	static void GetArrayU8(TArrayU8& out_array, TiXmlElement* const in_arrayElement);
	static void GetArrayU16(TArrayU16& out_array, TiXmlElement* const in_arrayElement);
	static void GetArrayU32(TArrayU32& out_array, TiXmlElement* const in_arrayElement);
	static void GetArrayR32(TArrayR32& out_array, TiXmlElement* const in_arrayElement);

};

#endif // _TXmlPack_H_