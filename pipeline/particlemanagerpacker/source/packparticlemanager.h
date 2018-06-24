//file: PackParticleManager.h
#ifndef _PackParticleManager_H_
#define _PackParticleManager_H_

#include <string>
class TiXmlElement;
class TPackerMessage;

/**/
struct PackParticleManager
{
	// disabled
private:
	~PackParticleManager();

	// static public methods
public:
	static const bool Pack(
		TiXmlElement* const in_sourceElement,
		TiXmlElement* const in_destinationElement,
		TPackerMessage& in_packMessage
		);

};

#endif // _PackParticleManager_H_