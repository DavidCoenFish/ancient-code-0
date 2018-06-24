//file: PackGeometry.h
#ifndef _PackGeometry_H_
#define _PackGeometry_H_

class TiXmlElement;
class TPackerMessage;

/**/
struct PackGeometry
{
	// disabled
private:
	~PackGeometry();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& in_packMessage
		);

};

#endif // _PackGeometry_H_