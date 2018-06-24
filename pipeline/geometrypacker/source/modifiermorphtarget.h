//file: ModifierMorphTarget.h
#ifndef _ModifierMorphTarget_H_
#define _ModifierMorphTarget_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class TiXmlElement;
class DscVertexPool;
class StreamInfo;
class TPackerMessage;
class ModifierMorphTargetStream;

/**/
class ModifierMorphTarget : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<ModifierMorphTarget> TPointerModifierMorphTarget;
	typedef boost::shared_ptr<ModifierMorphTargetStream> TPointerModifierMorphTargetStream;
	typedef std::vector<TPointerModifierMorphTargetStream> TArrayPointerModifierMorphTargetStream;

	typedef std::vector<int> TArrayInt;
	typedef std::vector<unsigned int> TArrayU32;

	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;
	typedef std::vector<bool> TArrayBool;

	//static methods
public:
	static TPointerModifierMorphTarget Factory(
		TiXmlElement& in_sourceElement,
		const TArrayPointerStreamInfo& in_arrayStreamInfo, 
		const DscVertexPool& in_vertexPool,
		const TArrayU32& in_arrayWritenVertexIndex,
		const TArrayBool& in_arrayStreamSupported,
		TPackerMessage& in_packMessage
		);

	//constructor
public:
	ModifierMorphTarget(
		const std::string& in_name,
		const TArrayPointerModifierMorphTargetStream& in_arrayStream, //WARN: transfer ownership
		const TArrayU32& in_duplicateTable
		);
	~ModifierMorphTarget();

	//public methods
public:
	void WriteOutData(
		TiXmlElement& out_parentElement
		)const;

	//private members
private:
	const std::string mName;
	const TArrayPointerModifierMorphTargetStream mArrayStream;
	const TArrayU32 mDuplicateTable;

};

#endif // _ModifierMorphTarget_H_