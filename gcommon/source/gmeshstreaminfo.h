//
//  gmeshstreaminfo.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GMeshStreamInfo_h_
#define _GMeshStreamInfo_h_

#include "gcommon.h"
#include <vector>
#include "gmeshtype.h"

/*
the 'image' of data to cast memory as after loading from tools output file
has assignment/ copy to allow GMeshManual to make a dynamic array of stream info
*/
class GMeshStreamInfo //: public boost::noncopyable 
{
	//typedef
private:
	typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

	//static public methods
public:
	//help client construct a array of stream info, the vertexDataByteOffset is calculatated based on contents of array
	static GVOID AppendStreamInfo(
		TArrayStreamInfo& inout_arrayStreamInfo,
		const GMeshType::TStreamType::TEnum in_type = GMeshType::TStreamType::TFloat,
		const GS32 in_count = 0,
		const GMeshType::TStreamUsage::TEnum in_usage = GMeshType::TStreamUsage::TCount
		);
	static const GS32 CalculateVertexDataByteSize(
		const TArrayStreamInfo& in_arrayStreamInfo
		);

	//constructor
public:
	GMeshStreamInfo(
		const GMeshType::TStreamType::TEnum in_type = GMeshType::TStreamType::TFloat,
		const GS32 in_count = 0,
		const GMeshType::TStreamUsage::TEnum in_usage = GMeshType::TStreamUsage::TCount,
		const GS32 in_vertexDataByteOffset = 0
		);
	GMeshStreamInfo(const GMeshStreamInfo& in_src);
	~GMeshStreamInfo();

	//operator
public:
	const GMeshStreamInfo& operator=(const GMeshStreamInfo& in_rhs);

	//public accessors
public:
	const GMeshType::TStreamType::TEnum GetType()const{ return mType; }
	const GS32 GetCount()const{ return mCount; }
	const GMeshType::TStreamUsage::TEnum GetUsage()const{ return mUsage; }
	const GS32 GetVertexDataByteOffset()const{ return mVertexDataByteOffset; }

	//private members;
private:
	GMeshType::TStreamType::TEnum mType;
	GS32 mCount;
	GMeshType::TStreamUsage::TEnum mUsage; //usage is only a hint (though it is used by opengles1 for client state specification
	GS32 mVertexDataByteOffset;

};

#endif 