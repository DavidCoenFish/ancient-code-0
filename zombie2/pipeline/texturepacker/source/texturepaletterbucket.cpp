//file: texturePaletterBucket.cpp

#include "texturePaletterBucket.h"

#include <map>
#include <limits>

typedef std::vector<TexturePaletterBucket> TArrayTexturePaletterBucket;

/*static*/ void TexturePaletterBucket::QuantaziseData(
	TArrayRGBQUAD& out_palette,
	const int in_paletteColourCount,
	const TArrayRGBQUAD& in_pixelColourData
	)
{
	TArrayTexturePaletterBucket arrayBuckets;
	arrayBuckets.resize(in_paletteColourCount);

	//seed first bucket
	{
		unsigned char arrayRangeLow[TVar::TCount];
		unsigned char arrayRangeHigh[TVar::TCount];
		for (int index = 0; index < TVar::TCount; ++index)
		{
			arrayRangeLow[index] = 0;
			arrayRangeHigh[index] = 255;
		}
		const unsigned int size = in_pixelColourData.size();
		TArrayIndex arrayIndex;
		arrayIndex.resize(size);
		for (unsigned int index = 0; index < size; ++index)
		{
			arrayIndex[index] = index;
		}

		arrayBuckets[0] = TexturePaletterBucket(
			arrayRangeLow,
			arrayRangeHigh,
			arrayIndex,
			in_pixelColourData
			);
	}

	//split buckets
	int trace = 1;
	printf("\npalette texture progres of %d:", in_paletteColourCount);
	while (trace < in_paletteColourCount)
	{
		float largestSD = -2.0F;
		int bestIndex = -1;
		for (int index = 0; index < trace; ++index)
		{
			const float testSD = arrayBuckets[index].GetLargestStandardDeviation();
			if (largestSD < testSD)
			{
				largestSD = testSD;
				bestIndex = index;
			}
		}

		if (-1 != bestIndex)
		{
			arrayBuckets[bestIndex].SplitOnLargestStandardDeviation(
				arrayBuckets[trace],
				in_pixelColourData
				);
		}

		trace += 1;

		printf(".");
	}

	printf("\n");

	//write coulours out to palette;
	out_palette.resize(in_paletteColourCount);
	for (int index = 0; index < in_paletteColourCount; ++index)
	{
		arrayBuckets[index].ExtractColour(out_palette[index]);
	}

	return;
}

TexturePaletterBucket::TexturePaletterBucket(
	const unsigned char* const in_arrayRangeLow,
	const unsigned char* const in_arrayRangeHigh,
	const TArrayIndex& in_arrayIndex,
	const TArrayRGBQUAD& in_pixelColourData
	)
	: mLargestStandardDeveation(0.0F)
	, mLargestStandardDeveationIndex(-1)
	, mArrayIndex(in_arrayIndex)
{
	for (int index = 0; index < TVar::TCount; ++index)
	{
		mChannelData[index].mLowRange = in_arrayRangeLow[index];
		mChannelData[index].mHighRange = in_arrayRangeHigh[index];
		mChannelData[index].mAverage = 0.0F;
		mChannelData[index].mSumDifferenceAverageSquared = 0.0F;
	}
	RefreashValues(in_pixelColourData);
	return;
}

TexturePaletterBucket::TexturePaletterBucket(const TexturePaletterBucket& in_src)
{
	(*this) = in_src;
	return;
}
TexturePaletterBucket::TexturePaletterBucket()
: mLargestStandardDeveation(0.0F)
, mLargestStandardDeveationIndex(-1)
{
	return;
}
TexturePaletterBucket::~TexturePaletterBucket()
{
	return;
}

const TexturePaletterBucket& TexturePaletterBucket::operator=(const TexturePaletterBucket& in_rhs)
{
	if (this != &in_rhs)
	{
		for (int index = 0; index < TVar::TCount; ++index)
		{
			mChannelData[index] = in_rhs.mChannelData[index];
		}
		mLargestStandardDeveation = in_rhs.mLargestStandardDeveation;
		mLargestStandardDeveationIndex = in_rhs.mLargestStandardDeveationIndex;
		mArrayIndex = in_rhs.mArrayIndex;
	}
	return (*this);
}

void TexturePaletterBucket::SplitOnLargestStandardDeviation(
	TexturePaletterBucket& out_bucket,
	const TArrayRGBQUAD& in_pixelColourData
	)
{
	if (-1 == mLargestStandardDeveationIndex)
	{
		return;
	}

	unsigned char rangeALowLow[TVar::TCount];
	unsigned char rangeALowHigh[TVar::TCount];
	unsigned char rangeAHighLow[TVar::TCount];
	unsigned char rangeAHighHigh[TVar::TCount];
	unsigned char rangeBLowLow[TVar::TCount];
	unsigned char rangeBLowHigh[TVar::TCount];
	unsigned char rangeBHighLow[TVar::TCount];
	unsigned char rangeBHighHigh[TVar::TCount];
	TArrayIndex splitALow;
	TArrayIndex splitAHigh;
	TArrayIndex splitBLow;
	TArrayIndex splitBHigh;

	for (int index = 0; index < TVar::TCount; ++index)
	{
		rangeALowLow[index] = mChannelData[index].mLowRange;
		rangeBLowLow[index] = mChannelData[index].mLowRange;
		rangeAHighLow[index] = mChannelData[index].mLowRange;
		rangeBHighLow[index] = mChannelData[index].mLowRange;

		rangeALowHigh[index] = mChannelData[index].mHighRange;
		rangeBLowHigh[index] = mChannelData[index].mHighRange;
		rangeAHighHigh[index] = mChannelData[index].mHighRange;
		rangeBHighHigh[index] = mChannelData[index].mHighRange;

		if (index == mLargestStandardDeveationIndex)
		{
			unsigned char averageInt = std::max<unsigned char>(0, std::min<unsigned char>(255, (unsigned char)(mChannelData[index].mAverage + 0.5F)));

			rangeALowHigh[index] = averageInt;
			rangeAHighLow[index] = averageInt + 1;

			rangeBLowHigh[index] = averageInt - 1;
			rangeBHighLow[index] = averageInt;
		}
	}

	for (int index = 0; index < (int)mArrayIndex.size(); ++index)
	{
		const int pixelIndex = mArrayIndex[index];
		const RGBQUAD& pixelColour = in_pixelColourData[pixelIndex];
		bool inside = true;
		for (int subIndex = 0; subIndex < TVar::TCount; ++subIndex)
		{
			const unsigned char testValue = ((unsigned char*)&pixelColour)[subIndex];
			if ((testValue < rangeALowLow[subIndex]) ||
				(rangeALowHigh[subIndex] < testValue))
			{
				inside = false;
				break;
			}
		}
		if (inside)
		{
			splitALow.push_back(pixelIndex);
		}
		else
		{
			splitAHigh.push_back(pixelIndex);
		}


		inside = true;
		for (int subIndex = 0; subIndex < TVar::TCount; ++subIndex)
		{
			const unsigned char testValue = ((unsigned char*)&pixelColour)[subIndex];
			if ((testValue < rangeBLowLow[subIndex]) ||
				(rangeBLowHigh[subIndex] < testValue))
			{
				inside = false;
				break;
			}
		}
		if (inside)
		{
			splitBLow.push_back(pixelIndex);
		}
		else
		{
			splitBHigh.push_back(pixelIndex);
		}
	}

	if (std::abs((int)splitALow.size() - (int)splitAHigh.size()) < std::abs((int)splitBLow.size() - (int)splitBHigh.size()))
	{
		(*this) = TexturePaletterBucket(rangeALowLow, rangeALowHigh, splitALow, in_pixelColourData);
		(out_bucket) = TexturePaletterBucket(rangeAHighLow, rangeAHighHigh, splitAHigh, in_pixelColourData);
	}
	else
	{
		(*this) = TexturePaletterBucket(rangeBLowLow, rangeBLowHigh, splitBLow, in_pixelColourData);
		(out_bucket) = TexturePaletterBucket(rangeBHighLow, rangeBHighHigh, splitBHigh, in_pixelColourData);
	}

	return;
}

void TexturePaletterBucket::RefreashValues(const TArrayRGBQUAD& in_pixelColourData)
{
	mLargestStandardDeveation = -1.0F;
	mLargestStandardDeveationIndex = -1;
	for (int index = 0; index < TVar::TCount; ++index)
	{
		float average = 0.0F;
		for (int subIndex = 0; subIndex < (int)mArrayIndex.size(); ++subIndex)
		{
			const RGBQUAD& pixelColour = in_pixelColourData[mArrayIndex[subIndex]];
			const int intColour = (int)(((unsigned char*)&pixelColour)[index]);
			average += (float)(intColour) / ((float)mArrayIndex.size());
		}

		mChannelData[index].mAverage = average;

		float sumStandardDeviationSquared = 0.0F;
		for (int subIndex = 0; subIndex < (int)mArrayIndex.size(); ++subIndex)
		{
			const RGBQUAD& pixelColour = in_pixelColourData[mArrayIndex[subIndex]];
			const int intColour = (int)(((unsigned char*)&pixelColour)[index]);
			const float deviation = ((float)intColour) - average;
			sumStandardDeviationSquared += std::abs(deviation); //(deviation * deviation);
		}

		mChannelData[index].mSumDifferenceAverageSquared = sumStandardDeviationSquared;

		if (mLargestStandardDeveation < sumStandardDeviationSquared)
		{
			mLargestStandardDeveation = sumStandardDeviationSquared;
			mLargestStandardDeveationIndex = index;
		}
	}
	return;
}

void TexturePaletterBucket::ExtractColour(RGBQUAD& out_colour)
{
	for (int index = 0; index < TVar::TCount; ++index)
	{
		unsigned char averageInt = std::max<unsigned char>(0, std::min<unsigned char>(255, (unsigned char)(mChannelData[index].mAverage + 0.5F)));
		((unsigned char*)&out_colour)[index] = averageInt;
	}
	return;
}
