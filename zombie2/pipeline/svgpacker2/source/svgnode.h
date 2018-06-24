//file: svgnode.h
#ifndef _svgnode_H_
#define _svgnode_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GMatrix9Float.h>

class TiXmlElement;

/**/
class SvgNode : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<SvgNode> TPointerSvgNode;
	typedef boost::weak_ptr<SvgNode> TWeakSvgNode;

	//static methods
public:
	static TPointerSvgNode Factory(
		TPointerSvgNode& in_parentNode,
		TiXmlElement& in_sourceElement
		);

	//constructor
public:
	SvgNode(
		TPointerSvgNode& in_parentNode,
		const GMatrix9Float& in_transform
		);
	~SvgNode();

	//public methods
public:
	const GMatrix9Float GetAbsoluteTransform()const;

	//private members
private:
	TWeakSvgNode m_parentNode;
	const GMatrix9Float m_transform;

};

#endif // _svgnode_H_