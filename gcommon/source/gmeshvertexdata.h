//
//  gmeshvertexdata.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GMeshVertexData_h_
#define _GMeshVertexData_h_

#include <boost/noncopyable.hpp>
#include "gcommon.h"
#include "gbuffer.h"

/*
	instances of a mesh need there own copy of the mesh memory block
	also may need read/ write access to a memory block
*/
class GMeshVertexData : public boost::noncopyable 
{
	//constructor
public:
	GMeshVertexData(const GS32 in_vertexCount, const GS32 in_vertexStride);
	GMeshVertexData(const GS32 in_size, const GVOID* const in_data);
	~GMeshVertexData();

	//static public methods
public:
	/*
		GMeshVertexData owns memory block
		static as we may want to get data out of an 'unowned' memory block, ie, that loaded from file with GMesh
	*/
	template <typename IN_TYPE> static const IN_TYPE GetItem(
		const GVOID* const in_data,
		const GS32 in_index,
		const GS32 in_vertexByteStride,
		const GS32 in_streamByteOffset,
		const GS32 in_subIndex
		)
	{
		return *((IN_TYPE*)(((GU8*)in_data) + (in_streamByteOffset + (in_index * in_vertexByteStride) + (in_subIndex * sizeof(IN_TYPE)))));
	}

	template <typename IN_TYPE> static IN_TYPE& GetItemRef(
		GVOID* const in_data,
		const GS32 in_index,
		const GS32 in_vertexByteStride,
		const GS32 in_streamByteOffset,
		const GS32 in_subIndex
		)
	{
		return *((IN_TYPE*)(((unsigned char*)in_data) + (in_streamByteOffset + (in_index * in_vertexByteStride) + (in_subIndex * sizeof(IN_TYPE)))));
	}

	//public methods
public:
	const GVOID* const GetData()const;
	GVOID* const GetData();

	//private members;
private:
	GBuffer mBuffer;

};

#endif 