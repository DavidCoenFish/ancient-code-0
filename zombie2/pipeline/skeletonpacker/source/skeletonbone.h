//file: skeletonbone.h
#ifndef _SkeletonBone_H_
#define _SkeletonBone_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class TiXmlElement;

/**/
class SkeletonBone : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<SkeletonBone> TPointerSkeletonBone;

	//static methods
public:
	static TPointerSkeletonBone Factory(
		TiXmlElement& in_sourceElement
		);

	//constructor
public:
	SkeletonBone(
		const std::string& in_name,
		const int in_parentIndex,
		const float* const in_parentRelativeMatrix
		);
	~SkeletonBone();

	//public methods
public:
	void WriteOutData(
		TiXmlElement& out_parentElement
		)const;

	//private members
private:
	const std::string mName;

	const int mParentIndex;

	//we include gcommon for enums, but don't want to link to it
	/*const GMatrix16Float*/ float mParentRelativeMatrix[16];

};

#endif // _SkeletonBone_H_