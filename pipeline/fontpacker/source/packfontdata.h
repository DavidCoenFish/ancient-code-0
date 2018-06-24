//file: PackFontData.h
#ifndef _PackFontData_H_
#define _PackFontData_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class TiXmlElement;
class TPackerMessage;
class Image;

/**/
class PackFontData: public boost::noncopyable
{
	// typedef
private:
	typedef std::vector<unsigned char> TArrayByte;
	typedef boost::shared_ptr<PackFontData> TPointerPackFontData;
	typedef boost::shared_ptr<TPackerMessage> TPointerPackerMessage;
	typedef boost::shared_ptr<Image> TPointerImage;

	// static public methods
public:
	static TPointerPackFontData FactorySpace(
		const int in_spaceWidth
		);

	static TPointerPackFontData Factory(
		TiXmlElement& in_element,
		const std::string in_sourceFilePath,
		TPackerMessage& inout_message
		);

	// construction
public:
	PackFontData(
		const unsigned int in_ID,
		const float in_baseLine,
		const int in_flag,
		const TPointerImage& in_image,
		const int in_width,
		const int in_height
		);
	~PackFontData();

	// accessors
public:
	const unsigned int GetID()const{ return mID; }
	const float GetBaseLine()const{ return mBaseLine; }
	const int GetFlag()const{ return mFlag; }
	const TPointerImage& GetImage()const{ return mImage; }
	const int GetWidth()const{ return mWidth; }
	const int GetHeight()const{ return mHeight; }

	const int& GetUVTop()const{ return mUVTop; }
	int& GetUVTop(){ return mUVTop; }

	const int& GetUVLeft()const{ return mUVLeft; }
	int& GetUVLeft(){ return mUVLeft; }

	const int& GetUVBottom()const{ return mUVBottom; }
	int& GetUVBottom(){ return mUVBottom; }

	const int& GetUVRight()const{ return mUVRight; }
	int& GetUVRight(){ return mUVRight; }

	const bool GetIsVisible()const;

	// private members
private:
	const unsigned int mID;
	const float mBaseLine; //distance from top to common line (waterline) of font
	const int mFlag;
	TPointerImage mImage;
	const int mWidth;
	const int mHeight;

	//where did this glyph end up being written
	int mUVTop;
	int mUVLeft;
	int mUVBottom;
	int mUVRight;

};

#endif // _PackFontData_H_