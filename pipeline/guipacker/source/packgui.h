//file: PackGui.h
#ifndef _PackGui_H_
#define _PackGui_H_

class TiXmlElement;
class TPackerMessage;

/**/
struct PackGui
{
	// disabled
private:
	~PackGui();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& inout_message
		);

};

#endif // _PackGui_H_