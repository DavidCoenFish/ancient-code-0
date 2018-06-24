//file: texturePaletterBucket.h
#ifndef _TexturePaletterBucket_H_
#define _TexturePaletterBucket_H_

#include <vector>
#include <FreeImage.h>

class TexturePaletterBucket
{
private:
	struct TVar
	{
		enum TEnum
		{
			TCount = 4
		};

	};
	typedef std::vector<RGBQUAD> TArrayRGBQUAD;
	typedef std::vector<unsigned char> TArrayByte;
	typedef std::vector<unsigned int> TArrayIndex;

	struct TChannelData
	{
		unsigned char mLowRange;
		unsigned char mHighRange;
		float mAverage;
		float mSumDifferenceAverageSquared;
	};

public:
	static void QuantaziseData(
		TArrayRGBQUAD& out_palette,
		const int in_paletteColourCount,
		const TArrayRGBQUAD& in_pixelColourData
		);

	TexturePaletterBucket(
		const unsigned char* const in_arrayRangeLow,
		const unsigned char* const in_arrayRangeHigh,
		const TArrayIndex& in_arrayIndex,
		const TArrayRGBQUAD& in_pixelColourData
		);
	TexturePaletterBucket(const TexturePaletterBucket& in_src);
	TexturePaletterBucket();
	~TexturePaletterBucket();

	const TexturePaletterBucket& operator=(const TexturePaletterBucket& in_rhs);

	const float GetLargestStandardDeviation()const{ return mLargestStandardDeveation; }
	void SplitOnLargestStandardDeviation(
		TexturePaletterBucket& out_bucket,
		const TArrayRGBQUAD& in_pixelColourData
		);

	void ExtractColour(RGBQUAD& out_colour);

private:
	void RefreashValues(const TArrayRGBQUAD& in_pixelColourData);

private:
	float mLargestStandardDeveation;
	int mLargestStandardDeveationIndex;
	TChannelData mChannelData[TVar::TCount];
	TArrayIndex mArrayIndex;

};

#endif // _TexturePaletterBucket_H_