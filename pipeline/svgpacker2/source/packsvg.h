//file: PackSvg.h
#ifndef _PackSvg_H_
#define _PackSvg_H_

class TiXmlElement;
class TPackerMessage;
#include <string>

/**/
struct PackSvg
{
	// disabled
private:
	~PackSvg();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		//TiXmlElement* const in_destinationElement,
		std::string& inout_string,
		TPackerMessage& in_packMessage,
		const int in_accuracy = 6
		);

};

#endif // _PackSvg_H_