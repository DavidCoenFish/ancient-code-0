//file: PackAnimation.h
#ifndef _PackAnimation_H_
#define _PackAnimation_H_

#include <string>
class TiXmlElement;
class TPackerMessage;

/**/
struct PackAnimation
{
	// disabled
private:
	~PackAnimation();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		const std::string& in_masterAnimationFilePath,
		TPackerMessage& in_packMessage
		);

};

#endif // _PackAnimation_H_