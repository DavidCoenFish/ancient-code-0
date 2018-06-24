//file: ImageWrittenUV.h
#ifndef _ImageWrittenUV_H_
#define _ImageWrittenUV_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>

class Image;

/**/
class ImageWrittenUV : public boost::noncopyable
{
	// construction
public:
	ImageWrittenUV(
		const float in_topLeftU,
		const float in_topLeftV,
		const float in_bottomRightU,
		const float in_bottomRightV
		);
	~ImageWrittenUV();

	//public accessors
public:
	const float GetTopLeftU()const{ return m_topLeftU; }
	const float GetTopLeftV()const{ return m_topLeftV; }
	const float GetBottomRightU()const{ return m_bottomRightU; }
	const float GetBottomRightV()const{ return m_bottomRightV; }

	//private members
private:
	const float m_topLeftU;
	const float m_topLeftV;
	const float m_bottomRightU;
	const float m_bottomRightV;

};

#endif // _ImageWrittenUV_H_