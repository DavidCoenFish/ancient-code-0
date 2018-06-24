//file: PackScene.h
#ifndef _PackScene_H_
#define _PackScene_H_

class TiXmlElement;
class TPackerMessage;

/**/
struct PackScene
{
	// disabled
private:
	~PackScene();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& inout_message
		);

};

#endif // _PackScene_H_