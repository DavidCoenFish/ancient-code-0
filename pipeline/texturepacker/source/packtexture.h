//file: packTexture.h
#ifndef _packTexture_H_
#define _packTexture_H_

#include <string>

class TiXmlElement;
class TPackerMessage;

/**/
struct PackTexture
{
	// disabled
private:
	~PackTexture();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		const std::string& in_dataName,
		const std::string& in_platform,
		TPackerMessage& inout_message
		);

};

#endif // _packTexture_H_