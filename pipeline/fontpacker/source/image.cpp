//file: Image.cpp

#include "Image.h"

#include <FreeImage.h>
//static public methods
/*static*/ Image::TPointerImage Image::FactoryBlank(
	const int in_width,
	const int in_height
	)
{
	TPointerImage pointerImage;
	pointerImage.reset(new Image(in_width, in_height));
	return pointerImage;
}

/*static*/ Image::TPointerImage Image::FactoryFreeImage(
	FIBITMAP& in_image
	)
{
	const int width = FreeImage_GetWidth(&in_image);
	const int height = FreeImage_GetHeight(&in_image);

	TPointerImage pointerImage;
	pointerImage.reset(new Image(width, height));
	Image& image = *pointerImage;

	for (int indexY = 0; indexY < height; ++indexY)
	{
		for (int indexX = 0; indexX < width; ++indexX)
		{
			RGBQUAD value;
			FreeImage_GetPixelColor(&in_image, indexX, height - indexY - 1, &value);
			image.SetPixel(
				indexX, 
				indexY,
				value.rgbRed,
				value.rgbGreen,
				value.rgbBlue,
				value.rgbReserved
				);
		}
	}

	return pointerImage;
}

/*static*/ void Image::Blit(
	Image& inout_destinationImage,
	const Image& in_stampImage,
	const int in_originX,
	const int in_originY,
	const int in_writeWidth,
	const int in_writeHeight
	)
{
	for (int indexY = 0; indexY < in_writeHeight; ++indexY)
	{
		for (int indexX = 0; indexX < in_writeWidth; ++indexX)
		{
			if (!in_stampImage.GetWritten(indexX, indexY))
			{
				continue;
			}
			unsigned char red = 0;
			unsigned char green = 0;
			unsigned char blue = 0;
			unsigned char alpha = 0;
			if (in_stampImage.GetPixel(red, green, blue, alpha, indexX, indexY))
			{
				inout_destinationImage.SetPixel(
					indexX + in_originX,
					indexY + in_originY,
					red,
					green,
					blue,
					alpha
					);
			}
		}
	}

	return;
}

/*static*/ void Image::BlitFreeImage(
	FIBITMAP& inout_destinationImage,
	const Image& in_stampImage,
	const int in_originX,
	const int in_originY,
	const int in_writeWidth,
	const int in_writeHeight
	)
{
	const int height = FreeImage_GetHeight(&inout_destinationImage);
	for (int indexY = 0; indexY < in_writeHeight; ++indexY)
	{
		for (int indexX = 0; indexX < in_writeWidth; ++indexX)
		{
			RGBQUAD value;
			if (in_stampImage.GetPixel(value.rgbRed, value.rgbGreen, value.rgbBlue, value.rgbReserved, indexX, indexY))
			{
				FreeImage_SetPixelColor(&inout_destinationImage, in_originX + indexX, height - (in_originY + indexY) - 1, &value);
			}
		}
	}

	return;
}

/*static*/ const bool Image::TestWritten(
	const Image& in_image,
	const int in_originX,
	const int in_originY,
	const int in_writeWidth,
	const int in_writeHeight
	)
{
	for (int indexY = 0; indexY < in_writeHeight; ++indexY)
	{
		for (int indexX = 0; indexX < in_writeWidth; ++indexX)
		{
			if (in_image.GetWritten(in_originX + indexX, in_originY + indexY))
			{
				return true;
			}
		}
	}

	return false;
}

// construction
Image::Image(
	const int in_width,
	const int in_height,
	const TArrayByte& in_arrayImageData
	)
	: mWidth(in_width)
	, mHeight(in_height)
	, mArrayData(in_arrayImageData)
	, mArrayWritten()
{
	mArrayData.resize(in_width * in_height * 4);
	mArrayWritten.resize(in_width * in_height);
	return;
}

Image::~Image()
{
	return;
}

// accessors
const bool Image::GetPixel(
	unsigned char& out_red, 
	unsigned char& out_green, 
	unsigned char& out_blue, 
	unsigned char& out_alpha, 
	const int in_x, 
	const int in_y
	)const
{
	if ((in_x < 0) ||
		(mWidth <= in_x) ||
		(in_y < 0) ||
		(mHeight <= in_y))
	{
		return false;
	}

	const int baseIndex = ((in_y * mWidth) + in_x) * 4;
	out_red = mArrayData[baseIndex + 0];
	out_green = mArrayData[baseIndex + 1];
	out_blue = mArrayData[baseIndex + 2];
	out_alpha = mArrayData[baseIndex + 3];

	return true;
}

void Image::SetPixel(
	const int in_x, 
	const int in_y,
	const unsigned char in_red, 
	const unsigned char in_green, 
	const unsigned char in_blue, 
	const unsigned char in_alpha, 
	const bool in_markWritten
	)
{
	if ((in_x < 0) ||
		(mWidth <= in_x) ||
		(in_y < 0) ||
		(mHeight <= in_y))
	{
		return;
	}

	const int baseIndex = ((in_y * mWidth) + in_x) * 4;
	mArrayData[baseIndex + 0] = in_red;
	mArrayData[baseIndex + 1] = in_green;
	mArrayData[baseIndex + 2] = in_blue;
	mArrayData[baseIndex + 3] = in_alpha;

	if (in_markWritten)
	{
		mArrayWritten[(in_y * mWidth) + in_x] = true;
	}

	return;
}

void Image::SetWritten(
	const int in_x, 
	const int in_y,
	const bool in_written
	)
{
	if ((in_x < 0) ||
		(mWidth <= in_x) ||
		(in_y < 0) ||
		(mHeight <= in_y))
	{
		return;
	}

	mArrayWritten[(in_y * mWidth) + in_x] = in_written;
	return;
}

const bool Image::GetWritten(
	const int in_x, 
	const int in_y
	)const
{
	if ((in_x < 0) ||
		(mWidth <= in_x) ||
		(in_y < 0) ||
		(mHeight <= in_y))
	{
		return false;
	}

	return mArrayWritten[(in_y * mWidth) + in_x];
}