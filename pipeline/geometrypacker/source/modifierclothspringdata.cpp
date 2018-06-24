//file: ModifierClothSpringData.cpp

#include "ModifierClothSpringData.h"

//constructor
ModifierClothSpringData::ModifierClothSpringData(const ModifierClothSpringData& in_src)
: mVertexIndexOne(-1)
, mVertexIndexTwo(-1)
, mLength(0.0F)
{
	(*this) = in_src;
	return;
}

ModifierClothSpringData::ModifierClothSpringData(
	const int in_vertexIndexOne,
	const int in_vertexIndexTwo,
	const float in_length
	)
: mVertexIndexOne(in_vertexIndexOne)
, mVertexIndexTwo(in_vertexIndexTwo)
, mLength(in_length)
{
	return;
}

ModifierClothSpringData::~ModifierClothSpringData()
{
	return;
}

const ModifierClothSpringData& ModifierClothSpringData::operator=(const ModifierClothSpringData& in_rhs)
{
	if (this != &in_rhs)
	{
		mVertexIndexOne = in_rhs.mVertexIndexOne;
		mVertexIndexTwo = in_rhs.mVertexIndexTwo;
		mLength = in_rhs.mLength;
	}
	return (*this);
}
