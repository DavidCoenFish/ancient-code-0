//file: PackSkeleton.h
#ifndef _PackSkeleton_H_
#define _PackSkeleton_H_

class TiXmlElement;
class TPackerMessage;

/**/
struct PackSkeleton
{
	// disabled
private:
	~PackSkeleton();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& in_packMessage
		);

};

#endif // _PackSkeleton_H_