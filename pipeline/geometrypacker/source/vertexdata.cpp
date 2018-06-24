//file: VertexData.cpp

#include "VertexData.h"

//construction
VertexData::VertexData()
: mArrayByte()
{
	return;
}

VertexData::VertexData(const VertexData& in_src)
: mArrayByte()
{
	(*this) = in_src;
	return;
}

VertexData::~VertexData()
{
	return;
}

//operators
const VertexData& VertexData::operator=(const VertexData& in_rhs)
{
	if (this != &in_rhs)
	{
		mArrayByte = in_rhs.mArrayByte;
	}
	return (*this);
}

const bool VertexData::operator==(const VertexData& in_rhs)const
{
	if (mArrayByte.size() != in_rhs.mArrayByte.size())
	{
		return false;
	}
	for (int index = 0; index < (int)mArrayByte.size(); ++index)
	{
		if (mArrayByte[index] != in_rhs.mArrayByte[index])
		{
			return false;
		}
	}

	return true;
}

const bool VertexData::operator<(const VertexData& in_rhs)const
{
	if (mArrayByte.size() != in_rhs.mArrayByte.size())
	{
		return (mArrayByte.size() < in_rhs.mArrayByte.size());
	}
	for (int index = 0; index < (int)mArrayByte.size(); ++index)
	{
		if (mArrayByte[index] != in_rhs.mArrayByte[index])
		{
			return (mArrayByte[index] < in_rhs.mArrayByte[index]);
		}
	}

	return false;
}
