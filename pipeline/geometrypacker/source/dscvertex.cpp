//file: DscVertex.cpp

#include "DscVertex.h"

//construction
DscVertex::DscVertex(const int in_keyIndex, const TArrayByte& in_arrayByte)
: mKeyIndex(in_keyIndex)
, mArrayByteData(in_arrayByte)
, mArrayByteCmp(in_arrayByte)
{
	return;
}

DscVertex::DscVertex(const DscVertex& in_src)
: mKeyIndex(-1)
, mArrayByteData()
, mArrayByteCmp()
{
	(*this) = in_src;
	return;
}

DscVertex::~DscVertex()
{
	return;
}

//operators
const DscVertex& DscVertex::operator=(const DscVertex& in_rhs)
{
	if (this != &in_rhs)
	{
		mKeyIndex = in_rhs.mKeyIndex;
		mArrayByteData = in_rhs.mArrayByteData;
		mArrayByteCmp = in_rhs.mArrayByteCmp;
	}
	return (*this);
}

const bool DscVertex::operator==(const DscVertex& in_rhs)const
{
	if (mKeyIndex != in_rhs.mKeyIndex)
	{
		return false;
	}
	if (mArrayByteData.size() != in_rhs.mArrayByteData.size())
	{
		return false;
	}
	if (mArrayByteCmp.size() != in_rhs.mArrayByteCmp.size())
	{
		return false;
	}
	for (int index = 0; index < (int)mArrayByteData.size(); ++index)
	{
		if (mArrayByteData[index] != in_rhs.mArrayByteData[index])
		{
			return false;
		}
	}
	for (int index = 0; index < (int)mArrayByteCmp.size(); ++index)
	{
		if (mArrayByteCmp[index] != in_rhs.mArrayByteCmp[index])
		{
			return false;
		}
	}
	return true;
}

const bool DscVertex::operator<(const DscVertex& in_rhs)const
{
	if (mKeyIndex != in_rhs.mKeyIndex)
	{
		return (mKeyIndex < in_rhs.mKeyIndex);
	}
	if (mArrayByteData.size() != in_rhs.mArrayByteData.size())
	{
		return (mArrayByteData.size() < in_rhs.mArrayByteData.size());
	}
	if (mArrayByteCmp.size() != in_rhs.mArrayByteCmp.size())
	{
		return (mArrayByteCmp.size() < in_rhs.mArrayByteCmp.size());
	}
	for (int index = 0; index < (int)mArrayByteData.size(); ++index)
	{
		if (mArrayByteData[index] != in_rhs.mArrayByteData[index])
		{
			return (mArrayByteData[index] < in_rhs.mArrayByteData[index]);
		}
	}
	for (int index = 0; index < (int)mArrayByteCmp.size(); ++index)
	{
		if (mArrayByteCmp[index] != in_rhs.mArrayByteCmp[index])
		{
			return (mArrayByteCmp[index] < in_rhs.mArrayByteCmp[index]);
		}
	}

	return false;
}

//public methods
void DscVertex::AppendCmpData(const TArrayByte& in_arrayByte)
{
	mArrayByteCmp.insert(mArrayByteCmp.end(), in_arrayByte.begin(), in_arrayByte.end());
	return;
}
