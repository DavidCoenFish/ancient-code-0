//file: ModifierSkinBone.h
#ifndef _ModifierSkinBone_H_
#define _ModifierSkinBone_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ModifierSkinBoneVertexData.h"
#include <string>
#include <vector>
#include <map>

class TiXmlElement;
class DscVertexPool;
class StreamInfo;
class TPackerMessage;

/**/
class ModifierSkinBone : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<ModifierSkinBone> TPointerModifierSkinBone;

	typedef std::vector<ModifierSkinBoneVertexData> TArrayModifierSkinBoneVertexData;
	typedef std::vector<int> TArrayInt;
	typedef std::vector<unsigned int> TArrayU32;

	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

	typedef std::map<int, TArrayInt> TMapIntArrayInt;
	//typedef std::map<Vertex, TArrayInt> TMapVertexArrayInt;

	//static methods
public:
	static TPointerModifierSkinBone Factory(
		TiXmlElement& in_sourceElement,
		const TArrayPointerStreamInfo& in_arrayStreamInfo, 
		const DscVertexPool& in_vertexPool,
		const TArrayU32& in_arrayWritenVertexIndex,
		const TArrayInt& in_arrayStreamIndexPosition,
		const TArrayInt& in_arrayStreamIndexNormal,
		TMapIntArrayInt& inout_mapPointUsage,
		TMapIntArrayInt& inout_mapVectorUsage,
		TPackerMessage& in_packMessage
		);

	//constructor
public:
	ModifierSkinBone(
		const std::string& in_name,
		//const int in_parentIndex,
		//const float* const in_parentRelativeMatrix,
		const float* const in_invertBindMatrix,
		const TArrayModifierSkinBoneVertexData& in_arrayModifierSkinBoneVertexDataPos,
		const TArrayModifierSkinBoneVertexData& in_arrayModifierSkinBoneVertexDataNormal
		);
	~ModifierSkinBone();

	//public methods
public:
	void WriteOutData(
		TiXmlElement& out_parentElement
		)const;
	//public accessors
public:
	const TArrayModifierSkinBoneVertexData& GetArrayModifierSkinBoneVertexDataPos()const{ return mArrayModifierSkinBoneVertexDataPos; }

	const TArrayModifierSkinBoneVertexData& GetArrayModifierSkinBoneVertexDataNormal()const{ return mArrayModifierSkinBoneVertexDataNormal; }

	//private members
private:
	const std::string mName;

	//we include gcommon for enums, but don't want to link to it
	/*const GMatrix16Float*/ float mInvertBindMatrix[16];

	const TArrayModifierSkinBoneVertexData mArrayModifierSkinBoneVertexDataPos;

	const TArrayModifierSkinBoneVertexData mArrayModifierSkinBoneVertexDataNormal;

};

#endif // _ModifierSkinBone_H_