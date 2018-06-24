//file: Image.h
#ifndef _Image_H_
#define _Image_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

struct FIBITMAP;

/*
	Image thinks origin is top left
	FreeImage thinks origin is bottom left
*/
class Image: public boost::noncopyable
{
	// typedef
private:
	typedef std::vector<unsigned char> TArrayByte;
	typedef std::vector<bool> TArrayBool;
	typedef boost::shared_ptr<Image> TPointerImage;

	//static public methods
public:
	static TPointerImage FactoryBlank(
		const int in_width,
		const int in_height
		);

	static TPointerImage FactoryFreeImage(
		FIBITMAP& in_image
		);

	static void Blit(
		Image& inout_destinationImage,
		const Image& in_stampImage,
		const int in_originX,
		const int in_originY,
		const int in_writeWidth,
		const int in_writeHeight
		);
	static void BlitFreeImage(
		FIBITMAP& inout_destinationImage,
		const Image& in_stampImage,
		const int in_originX,
		const int in_originY,
		const int in_writeWidth,
		const int in_writeHeight
		);

	static const bool TestWritten(
		const Image& in_image,
		const int in_originX,
		const int in_originY,
		const int in_writeWidth,
		const int in_writeHeight
		);

	// construction
public:
	Image(
		const int in_width,
		const int in_height,
		const TArrayByte& in_arrayImageData = TArrayByte()
		);
	~Image();

	// accessors
public:
	const bool GetPixel(
		unsigned char& out_red, 
		unsigned char& out_green, 
		unsigned char& out_blue, 
		unsigned char& out_alpha, 
		const int in_x, 
		const int in_y
		)const;

	void SetPixel(
		const int in_x, 
		const int in_y,
		const unsigned char in_red, 
		const unsigned char in_green, 
		const unsigned char in_blue, 
		const unsigned char in_alpha, 
		const bool in_markWritten = true
		);
	void SetWritten(
		const int in_x, 
		const int in_y,
		const bool in_written
		);
	const bool GetWritten(
		const int in_x, 
		const int in_y
		)const;

	// public accessors
public:
	const int GetWidth()const{ return mWidth; }
	const int GetHeight()const{ return mHeight; }

	// private members
private:
	const int mWidth;
	const int mHeight;

	TArrayByte mArrayData;
	TArrayBool mArrayWritten;

};

#endif // _Image_H_