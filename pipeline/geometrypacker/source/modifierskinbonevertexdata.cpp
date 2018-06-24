//file: ModifierSkinBoneVertexData.cpp

#include "ModifierSkinBoneVertexData.h"

//constructor
ModifierSkinBoneVertexData::ModifierSkinBoneVertexData(const ModifierSkinBoneVertexData& in_src)
: mVertexIndex(-1)
, mVertexWeight(0.0F)
{
	(*this) = in_src;
	return;
}

ModifierSkinBoneVertexData::ModifierSkinBoneVertexData(
	const int in_vertexIndex,
	const float in_vertexWeight
	)
: mVertexIndex(in_vertexIndex)
, mVertexWeight(in_vertexWeight)
{
	return;
}

ModifierSkinBoneVertexData::~ModifierSkinBoneVertexData()
{
	return;
}

const ModifierSkinBoneVertexData& ModifierSkinBoneVertexData::operator=(const ModifierSkinBoneVertexData& in_rhs)
{
	if (this != &in_rhs)
	{
		mVertexIndex = in_rhs.mVertexIndex;
		mVertexWeight = in_rhs.mVertexWeight;
	}
	return (*this);
}
