//file: FontSpliter.h
#ifndef _FontSpliter_H_
#define _FontSpliter_H_

#include <string>

class TiXmlElement;
class TPackerMessage;

/**/
struct FontSpliter
{
	// disabled
private:
	~FontSpliter();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		const std::string& in_sourceFilePath,
		TPackerMessage& inout_message
		);

};

#endif // _FontSpliter_H_