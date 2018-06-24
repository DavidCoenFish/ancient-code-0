//file: DscVertexPool.cpp

#include "DscVertexPool.h"
#include "DscVertex.h"
#include "StreamInfo.h"
#include <map>

typedef std::map<DscVertex, int> TMapDscVertexInt;

void DscVertexPoolFindDuplicate(
	TArrayU32& out_duplicateArray, 
	const int in_newVertexIndex, 
	const TArrayU32& in_arrayWritenVertexIndex, 
	const StreamInfo& in_streamInfo,
	const DscVertexPool& in_vertexPool
	)
{
	if (in_streamInfo.GetIsPosition())
	{
		const int keyIndex = in_vertexPool.GetKeyIndex(in_newVertexIndex);
		for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
		{
			if (keyIndex == in_vertexPool.GetKeyIndex(in_arrayWritenVertexIndex[index]))
			{
				out_duplicateArray.push_back(index);
			}
		}
	}
	else
	{
		for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
		{
			if (in_newVertexIndex == in_arrayWritenVertexIndex[index])
			{
				out_duplicateArray.push_back(index);
			}
		}
	}
	return;
}

void DscVertexPoolFindDuplicateKey(
	TArrayU32& out_duplicateArray, 
	const int in_keyIndex, 
	const TArrayU32& in_arrayWritenVertexIndex, 
	const DscVertexPool& in_vertexPool
	)
{
	for (int index = 0; index < (int)in_arrayWritenVertexIndex.size(); ++index)
	{
		if (in_keyIndex == in_vertexPool.GetKeyIndex(in_arrayWritenVertexIndex[index]))
		{
			out_duplicateArray.push_back(index);
		}
	}
	return;
}

//construction
DscVertexPool::DscVertexPool()
: mArrayPointerDscVertex()
, mArrayNewIndex()
, mNewCount(0)
{
	return;
}

DscVertexPool::~DscVertexPool()
{
	return;
}

//public methods
void DscVertexPool::AddVertex(
	const int in_vertexIndex,
	const int in_keyIndex,
	const TArrayByte& in_streamData
	)
{
	if ((int)mArrayPointerDscVertex.size() <= in_vertexIndex)
	{
		mArrayPointerDscVertex.resize(in_vertexIndex + 1);
	}

	mArrayPointerDscVertex[in_vertexIndex].reset(new DscVertex(
		in_keyIndex,
		in_streamData
		));


	return;
}

//allow morph data to split vertex in main body of vertex, don by adding additional cmp data for the vertex
void DscVertexPool::AppendVertexCmpData(
	const int in_vertexIndex,
	const TArrayByte& in_additionalCmpData
	)
{
	if ((0 <= in_vertexIndex) && (in_vertexIndex < (int)mArrayPointerDscVertex.size()))
	{
		TPointerDscVertex& pointer = mArrayPointerDscVertex[in_vertexIndex];
		if (!pointer)
		{
			return;
		}

		pointer->AppendCmpData(in_additionalCmpData);
	}
	return;
}

void DscVertexPool::RemoveDuplicateVertex()
{
	const int count = mArrayPointerDscVertex.size();
	mArrayNewIndex.resize(count);
	TMapDscVertexInt mapDscVertexInt;
	for (int index = 0; index < count; ++index)
	{
		if (!mArrayPointerDscVertex[index])
		{
			continue;
		}
		const DscVertex& vertex = *mArrayPointerDscVertex[index];
		TMapDscVertexInt::iterator found = mapDscVertexInt.find(vertex);
		if (found != mapDscVertexInt.end())
		{
			mArrayNewIndex[index] = (*found).second;
		}
		else
		{
			const int newIndex = mapDscVertexInt.size();
			mapDscVertexInt.insert(TMapDscVertexInt::value_type(vertex, newIndex));
			mArrayNewIndex[index] = newIndex;
			mNewCount = mapDscVertexInt.size();

			mMapNewIndexVertex[newIndex] = mArrayPointerDscVertex[index];
		}
	}

	return;
}

const int DscVertexPool::GetNewIndex(const int in_vertexIndex)const
{
	if ((0 <= in_vertexIndex) && (in_vertexIndex < (int)mArrayNewIndex.size()))
	{
		return mArrayNewIndex[in_vertexIndex];
	}
	return -1;
}

const DscVertexPool::TArrayByte& DscVertexPool::GetVertexStreamData(const int in_newIndex)const
{
	//int count = (int)mArrayPointerDscVertex.size();
	//for (int index = 0; index < count; ++index)
	//{
	//	if (in_newIndex == GetNewIndex(index))
	//	{
	//		return mArrayPointerDscVertex[index]->GetArrayByteData();
	//	}
	//}
	TMapIntVertex::const_iterator found = mMapNewIndexVertex.find(in_newIndex);
	if (found != mMapNewIndexVertex.end())
	{
		return (*found).second->GetArrayByteData();
	}

	static const TArrayByte sData;
	return sData;
}

const int DscVertexPool::GetKeyIndex(const int in_newIndex)const
{
	TMapIntVertex::const_iterator found = mMapNewIndexVertex.find(in_newIndex);
	if (found != mMapNewIndexVertex.end())
	{
		return (*found).second->GetKeyIndex();
	}

	return -1;
}
