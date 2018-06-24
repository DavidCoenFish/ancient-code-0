//file: ModifierCloth.h
#ifndef _ModifierCloth_H_
#define _ModifierCloth_H_

#include <boost/shared_ptr.hpp>
#include "Modifier.h"
#include <vector>

class TiXmlElement;
class ModifierClothSpringData;
class ModifierClothVertexData;
class StreamInfo;
class DscVertexPool;
class TPackerMessage;

/**/
class ModifierCloth : public Modifier
{
private:
	typedef boost::shared_ptr<ModifierCloth> TPointerModifierCloth;

	typedef std::vector<unsigned int> TArrayU32;
	typedef std::vector<int> TArrayInt;
	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

	typedef std::vector<ModifierClothSpringData> TArrayModifierClothSpringData;
	typedef std::vector<ModifierClothVertexData> TArrayModifierClothVertexData;

	//static methods
public:
	static TPointerModifierCloth Factory(
		TiXmlElement& in_sourceElement,
		const TArrayPointerStreamInfo& in_arrayStreamInfo, 
		const DscVertexPool& in_vertexPool,
		const TArrayU32& in_arrayWritenVertexIndex,
		TPackerMessage& in_packMessage
		);

public:
	ModifierCloth(
		const float in_gravity,
		const float in_mass,
		const float in_springConstant,
		const float in_springDampen,
		const TArrayModifierClothSpringData& in_arraySpringData,
		const TArrayModifierClothVertexData& in_arrayVertexData,
		const TArrayInt& in_arrayDuplicate,
		const TArrayInt& in_arrayStreamTarget
		);
	virtual ~ModifierCloth();

private:
	virtual void OnWriteOutData(
		TiXmlElement& inout_parentElement
		)const;

private:
	const float mGravity;
	const float mMass;
	const float mSpringConstant;
	const float mSpringDampen;
	const TArrayModifierClothSpringData mArraySpringData;
	const TArrayModifierClothVertexData mArrayVertexData;
	const TArrayInt mArrayDuplicate;
	const TArrayInt mArrayStreamTarget;

};

#endif // _ModifierCloth_H_