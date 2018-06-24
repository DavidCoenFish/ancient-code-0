//file: ModifierClothVertexData.cpp

#include "ModifierClothVertexData.h"

//constructor
ModifierClothVertexData::ModifierClothVertexData(const ModifierClothVertexData& in_src)
: mVertexWeight(0.0F)
, mDuplicateTableOffset(-1)
{
	(*this) = in_src;
	return;
}

ModifierClothVertexData::ModifierClothVertexData(
	const float in_vertexWeight,
	const int in_duplicateTableOffset
	)
: mVertexWeight(in_vertexWeight)
, mDuplicateTableOffset(in_duplicateTableOffset)
{
	return;
}

ModifierClothVertexData::~ModifierClothVertexData()
{
	return;
}

const ModifierClothVertexData& ModifierClothVertexData::operator=(const ModifierClothVertexData& in_rhs)
{
	if (this != &in_rhs)
	{
		mVertexWeight = in_rhs.mVertexWeight;
		mDuplicateTableOffset = in_rhs.mDuplicateTableOffset;
	}
	return (*this);
}
