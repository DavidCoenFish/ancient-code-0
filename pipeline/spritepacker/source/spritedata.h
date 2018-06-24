//file: SpriteData.h
#ifndef _SpriteData_H_
#define _SpriteData_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>

class Image;
class TiXmlElement;
class ImageWrittenUV;

/**/
class SpriteData : public boost::noncopyable
{
public:
	struct TOrientation
	{
		enum TEnum
		{
			T12 = 0,
			T12Flip,
			T03,
			T03Flip,
			T06,
			T06Flip,
			T09,
			T09Flip,
			TCount
		};
	};
	// typedefs
private:
	typedef boost::shared_ptr<Image> TPointerImage;
	typedef boost::weak_ptr<Image> TWeakImage;

	//typedef boost::shared_ptr<ImageWrittenUV> TPointerImageWrittenUV;

	// construction
public:
	SpriteData(
		const std::string in_name,
		const TOrientation::TEnum in_orientation,
		TPointerImage& inout_image
		);
	~SpriteData();

	//public methods
public:
	void Serialise(TiXmlElement& inout_data, const ImageWrittenUV& in_writtenUV)const;

	//public accessors
public:
	const std::string& GetName()const{ return m_name; }
	const TOrientation::TEnum GetOrientation()const{ return m_orientation; }
	const TWeakImage& GetImage()const{ return m_image; }
	//TWeakImage& GetImage(){ return m_image; }

	//const TPointerImageWrittenUV& GetWrittenUV()const{ return m_writtenUV; }
	//TPointerImageWrittenUV& GetWrittenUV(){ return m_writtenUV; }

	//private members
private:
	const std::string m_name;
	const TOrientation::TEnum m_orientation;
	TWeakImage m_image;

	//TPointerImageWrittenUV m_writtenUV;

};

#endif // _SpriteData_H_