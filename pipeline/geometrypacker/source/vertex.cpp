//file: Vertex.cpp

#include "Vertex.h"

//construction
Vertex::Vertex(const int in_keyIndex)
: mKeyIndex(in_keyIndex)
, mArrayByte()
{
	return;
}

Vertex::Vertex(const Vertex& in_src)
: mKeyIndex(-1)
, mArrayByte()
{
	(*this) = in_src;
	return;
}

Vertex::~Vertex()
{
	return;
}

//operators
const Vertex& Vertex::operator=(const Vertex& in_rhs)
{
	if (this != &in_rhs)
	{
		mKeyIndex = in_rhs.mKeyIndex;
		mArrayByte = in_rhs.mArrayByte;
	}
	return (*this);
}

const bool Vertex::operator==(const Vertex& in_rhs)const
{
	if (mKeyIndex != in_rhs.mKeyIndex)
	{
		return false;
	}
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

const bool Vertex::operator<(const Vertex& in_rhs)const
{
	if (mKeyIndex != in_rhs.mKeyIndex)
	{
		return (mKeyIndex < in_rhs.mKeyIndex);
	}
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
