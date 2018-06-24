//file: packSprite.h
#ifndef _PackSprite_H_
#define _PackSprite_H_

class TiXmlElement;
class TPackerMessage;

/**/
struct PackSprite
{
	// disabled
private:
	~PackSprite();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& inout_message
		);

};

#endif // _PackSprite_H_