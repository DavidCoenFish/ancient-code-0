//
//  gmesh.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GMesh_h_
#define _GMesh_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>

class GMeshStreamInfo;
class GMeshModifier;
class GMeshEdge;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMesh : public boost::noncopyable 
{
	/////////////////////////////////////////////////////////////////////
	//typedef
private:

	/////////////////////////////////////////////////////////////////////
	//constructor
public:
	GMesh(
		const GS32 in_topologyType = 0,
		const GS32 in_elementCount = 0, //vertex count can be greater than 255 and still use u8 index, as long as there are less than 255 vertexDataCount
		const GS32 in_flag = 0,
		const GVOID* const in_vertexIndexArray = 0,
		const GS32 in_vertexDataCount = 0,
		const GS32 in_vertexDataByteStride = 0,
		const GVOID* const in_vertexData = 0,
		const GS32 in_streamInfoCount = 0,
		const GMeshStreamInfo* const in_arrayStreamInfo = 0,
		const GMeshEdge* const in_edge = 0,
		const GS32 in_modifierCount = 0,
		const GMeshModifier* const in_arrayModifier = 0,
		const GCHAR* const in_skeletonName = 0
		);
	~GMesh();

	/////////////////////////////////////////////////////////////////////
	//public accessors
public:
	const GS32 GetTopologyType()const{ return mTopologyType; }
	const GS32 GetElementCount()const{ return mElementCount; }//nb of vertex to draw, may be number of vertex data if not indexs
	void SetElementCount(const int in_elementCount){ mElementCount = in_elementCount; return; }//for example, particles want this as don't always use ALL the vertex data
	const GS32 GetVertexDataCount()const{ return mVertexDataCount; } //nb of vertex in vertex data

	const GBOOL HasIndexU8()const;
	const GBOOL HasIndexU16()const;
	const GU8* const GetVertexIndexU8()const;
	const GU16* const GetVertexIndexU16()const;

	const GS32 GetVertexIndexType()const;
	const GVOID* const GetVertexIndexArray()const{ return mVertexIndexArray; }

	const GS32 GetVertexByteStride()const{ return mVertexDataByteStride; };

	const GS32 GetVertexDataByteSize()const;
	const GVOID* const GetVertexData()const{ return mVertexData; }

	const GS32 GetStreamInfoCount()const{ return mStreamInfoCount; };
	const GMeshStreamInfo* const GetArrayStreamInfo()const{ return mArrayStreamInfo; }

	const GMeshEdge* const GetEdge()const{ return mEdge; }

	const int GetModifierCount()const{ return mModifierCount; };
	const GMeshModifier* const GetArrayModifier()const{ return mArrayModifier; }

	const char* const GetSkeletonName()const{ return mSkeletonName; }

	/////////////////////////////////////////////////////////////////////
	//private members
private:
	const GS32 mTopologyType;

	/*const*/ GS32 mElementCount; //not const, particles may want to set this lower
	const GS32 mFlag;
	const GVOID* const mVertexIndexArray; //u8 or u16 depending on flag, can be null if not an indexed topology, else has mElementCount items

	const GS32 mVertexDataCount; //same as vertex count if not indexed geometry
	const GS32 mVertexDataByteStride;
	//mVertexDataSize = mVertexDataCount * mVertexDataByteStride
	const GVOID* const mVertexData;

	const GS32 mStreamInfoCount;
	const GMeshStreamInfo* const mArrayStreamInfo;

	const GMeshEdge* const mEdge;

	const int mModifierCount;
	const GMeshModifier* const mArrayModifier;

	const char* const mSkeletonName;

};

#endif //_GMesh_h_