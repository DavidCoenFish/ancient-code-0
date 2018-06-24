//file: ModifierMorph.h
#ifndef _ModifierMorph_H_
#define _ModifierMorph_H_

#include <boost/shared_ptr.hpp>
#include "Modifier.h"
#include <vector>

class TiXmlElement;
class ModifierMorphTarget;
class DscVertexPool;
class StreamInfo;
class TPackerMessage;

/**/
class ModifierMorph : public Modifier
{
private:
	typedef boost::shared_ptr<ModifierMorph> TPointerModifierMorph;
	typedef boost::shared_ptr<ModifierMorphTarget> TPointerModifierMorphTarget;
	typedef std::vector<TPointerModifierMorphTarget> TArrayPointerModifierMorphTarget;

	typedef boost::shared_ptr<StreamInfo> TPointerStreamInfo;
	typedef std::vector<TPointerStreamInfo> TArrayPointerStreamInfo;

	typedef std::vector<unsigned int> TArrayU32;
	typedef std::vector<bool> TArrayBool;

	//static methods
public:
	static TPointerModifierMorph Factory(
		TiXmlElement& in_sourceElement,
		const TArrayPointerStreamInfo& in_arrayStreamInfo, 
		const DscVertexPool& in_vertexPool,
		const TArrayU32& in_arrayWritenVertexIndex,
		TPackerMessage& in_packMessage
		);

	//constructor
public:
	ModifierMorph(
		TArrayPointerModifierMorphTarget& in_arrayTarget, //WARN: transfer ownership
		const TArrayBool& in_arrayStreamSupported
		);
	virtual ~ModifierMorph();

	//implement Modifier
private:
	virtual void OnWriteOutData(
		TiXmlElement& inout_parentElement
		)const;

private:
	TArrayPointerModifierMorphTarget mArrayTarget;
	TArrayBool mArrayStreamSupported;

};

#endif // _ModifierMorph_H_