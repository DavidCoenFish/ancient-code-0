//file: ModifierSkin.h
#ifndef _ModifierSkin_H_
#define _ModifierSkin_H_

#include <boost/shared_ptr.hpp>
#include "Modifier.h"
#include <vector>

class TiXmlElement;
class ModifierSkinBone;
class DscVertexPool;
class StreamInfo;
class TPackerMessage;

/**/
class ModifierSkin : public Modifier
{
private:
	typedef boost::shared_ptr<ModifierSkin> TPointerModifierSkin;
	typedef boost::shared_ptr<ModifierSkinBone> TPointerModifierSkinBone;
	typedef std::vector<TPointerModifierSkinBone> TArrayPointerModifierSkinBone;
	typedef std::vector<int> TArrayInt;
	typedef std::vector<unsigned int> TArrayU32;

	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

	typedef std::pair<int, int> TPairIntInt;
	typedef std::vector<TPairIntInt> TArrayPairIntInt;

	//static methods
public:
	static TPointerModifierSkin Factory(
		TiXmlElement& in_sourceElement,
		const TArrayPointerStreamInfo& in_arrayStreamInfo, 
		const DscVertexPool& in_vertexPool,
		const TArrayU32& in_arrayWritenVertexIndex,
		TPackerMessage& in_packMessage
		);

public:
	ModifierSkin(
		const TArrayInt& in_arrayStreamIndexPos,
		const TArrayInt& in_arrayStreamIndexNormal,
		const TArrayPointerModifierSkinBone& in_arrayBone,
		const TArrayPairIntInt& in_pointUsageArrayIndexOffset,
		const TArrayInt& in_pointArrayDuplicate,
		const TArrayPairIntInt& in_vectorUsageArrayIndexOffset,
		const TArrayInt& in_vectorArrayDuplicate
		);
	virtual ~ModifierSkin();

private:
	virtual void OnWriteOutData(
		TiXmlElement& inout_parentElement
		)const;

private:
	const TArrayInt mArrayStreamIndexPos;
	const TArrayInt mArrayStreamIndexNormal;
	const TArrayPointerModifierSkinBone mArrayBone;

	const TArrayPairIntInt mPointUsageArrayIndexOffset;
	const TArrayInt mPointArrayDuplicate;

	const TArrayPairIntInt mVectorUsageArrayIndexOffset;
	const TArrayInt mVectorArrayDuplicate;

};

#endif // _ModifierSkin_H_