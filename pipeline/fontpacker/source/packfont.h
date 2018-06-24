//file: PackFont.h
#ifndef _PackFont_H_
#define _PackFont_H_

#include <string>

class TiXmlElement;
class TPackerMessage;

/**/
struct PackFont
{
	// disabled
private:
	~PackFont();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& inout_message,
		const std::string& in_sourcePath
		);

};

#endif // _PackFont_H_