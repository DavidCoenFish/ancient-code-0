//file: packMaterial.h
#ifndef _PackMaterial_H_
#define _PackMaterial_H_

class TiXmlElement;

/**/
struct PackMaterial
{
	// disabled
private:
	~PackMaterial();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement
		);

};

#endif // _PackMaterial_H_