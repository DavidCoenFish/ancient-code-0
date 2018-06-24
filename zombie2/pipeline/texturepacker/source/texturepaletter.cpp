//file: texturePaletter.cpp

#include "texturePaletter.h"
#include "texturePaletterBucket.h"

#include <map>
#include <limits>
#include <FreeImage.h>

class TexturePaletterPixelData;

typedef std::pair<unsigned int, unsigned int> TPairInt;
typedef std::vector<TexturePaletterPixelData> TArrayTexturePaletterPixelData;
typedef std::vector<RGBQUAD> TArrayRGBQUAD;
typedef std::vector<unsigned char> TArrayByte;

static void LocalDataGet(
	TArrayRGBQUAD& out_arrayPixelColour,
	FIBITMAP& in_image
	)
{
	FIBITMAP* const localImage32 = FreeImage_ConvertTo32Bits(&in_image);
	const int width = FreeImage_GetWidth(&in_image);
	const int height = FreeImage_GetHeight(&in_image);

	out_arrayPixelColour.resize(height * width);
	for (int indexY = 0; indexY < height; ++indexY)
	{
		for (int indexX = 0; indexX < width; ++indexX)
		{
			FreeImage_GetPixelColor(
				localImage32, 
				indexX, 
				indexY, 
				&out_arrayPixelColour[(indexY * width) + indexX]
				);
		}
	}

	FreeImage_Unload(localImage32);
}



static void LocalPolulatePalette(
	TArrayRGBQUAD& out_palette,
	const int in_palletColourCount,
	const TArrayRGBQUAD& in_arrayPixelColour
	)
{
	TexturePaletterBucket::QuantaziseData(
		out_palette,
		in_palletColourCount,
		in_arrayPixelColour
		);

	return;
}

static void LocalDitherImage(
	TArrayByte& out_pixelIndex,
	const TArrayRGBQUAD& in_arrayPixelColour,
	const TArrayRGBQUAD& in_palette
	)
{
	const unsigned int size = in_arrayPixelColour.size();
	out_pixelIndex.resize(size);
	//int ditherError[4];
	//ditherError[0] = 0;
	//ditherError[1] = 0;
	//ditherError[2] = 0;
	//ditherError[3] = 0;
	for (unsigned int index = 0; index < size; ++index)
	{
		//find closest match for this pixel
		int thisPixel[4];
		const RGBQUAD& thisPixelQuad = in_arrayPixelColour[index];
		//thisPixel[0] = ((int)thisPixelQuad.rgbRed) + ditherError[0];
		//thisPixel[1] = ((int)thisPixelQuad.rgbGreen) + ditherError[1];
		//thisPixel[2] = ((int)thisPixelQuad.rgbBlue) + ditherError[2];
		//thisPixel[3] = ((int)thisPixelQuad.rgbReserved) + ditherError[3];
		thisPixel[0] = (int)thisPixelQuad.rgbRed;// + ditherError[0];
		thisPixel[1] = (int)thisPixelQuad.rgbGreen;// + ditherError[1];
		thisPixel[2] = (int)thisPixelQuad.rgbBlue;// + ditherError[2];
		thisPixel[3] = (int)thisPixelQuad.rgbReserved;// + ditherError[3];

		int bestDifference = 1024;
		unsigned char bestPalletIndex = 0;
		for (unsigned int subIndex = 0; subIndex < in_palette.size(); ++subIndex)
		{
			const RGBQUAD& paletteColour = in_palette[subIndex];
			const int difference = abs(thisPixel[0] - ((int)paletteColour.rgbRed)) + 
				abs(thisPixel[1] - ((int)paletteColour.rgbGreen)) + 
				abs(thisPixel[2] - ((int)paletteColour.rgbBlue)) + 
				abs(thisPixel[3] - ((int)paletteColour.rgbReserved));
			if (difference < bestDifference)
			{
				bestPalletIndex = (unsigned char)subIndex;
				bestDifference = difference;
			}
		}

		out_pixelIndex[index] = bestPalletIndex;

		//update dither error
		//const RGBQUAD& selectedPaletteColour = in_palette[bestPalletIndex];
		//ditherError[0] = ((int)selectedPaletteColour.rgbRed) - thisPixel[0];
		//ditherError[1] = ((int)selectedPaletteColour.rgbGreen) - thisPixel[1];
		//ditherError[2] = ((int)selectedPaletteColour.rgbBlue) - thisPixel[2];
		//ditherError[3] = ((int)selectedPaletteColour.rgbReserved) - thisPixel[3];
	}

	return;
}

/*
for (arrayPixelColour)
	get each pixels average distance to all the other pixels
	pick the lowest average distance
	remove ((1 / arrayPixelColour) * pixel count) pixels closest to the removed pixel and remove them
do a 'dither' pass to put each pixel into closest bucket

*/

// static public methods
/*static*/ void texturePaletter::Run(
	TArrayRGBQUAD& out_palette,
	TArrayByte& out_pixelIndex,
	FIBITMAP& in_image,
	const int in_palletColourCount 
	)
{
	TArrayRGBQUAD arrayPixelColour;
	LocalDataGet(
		arrayPixelColour,
		in_image
		);

	LocalPolulatePalette(
		out_palette,
		in_palletColourCount,
		arrayPixelColour
		);

	LocalDitherImage(
		out_pixelIndex,
		arrayPixelColour,
		out_palette
		);

	return;
}