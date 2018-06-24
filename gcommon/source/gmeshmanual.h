//
//  gmesh.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GMeshManual_h_
#define _GMeshManual_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "gcommon.h"
#include "gmeshtype.h"

class GMesh;
class GMeshVertexData;
class GMeshStreamInfo;

/**/
class GMeshManual : public boost::noncopyable 
{
	////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::scoped_ptr<GMesh> TPointerMesh;
	typedef boost::shared_ptr<GMeshVertexData> TPointerMeshVertexData;
	typedef std::vector<GU8> TArrayByte;
	typedef std::vector<GU16> TArrayShort;
	typedef std::vector<GMeshStreamInfo> TArrayStreamInfo;

	////////////////////////////////////////////////////
	//constructor
public:
	//without index, vertex data count == vertex count
	GMeshManual(
		const GMeshType::TPrimitiveType::TEnum in_topologyType,
		const GS32 in_vertexCount,
		const TArrayStreamInfo& in_arrayStreamInfo
		);
	GMeshManual(
		const GMeshType::TPrimitiveType::TEnum in_topologyType,
		const TArrayByte& in_arrayVertexIndex, //vertex count = in_arrayVertexIndex.size
		const GS32 in_vertexDataCount, //assert in_vertexDataCount < 256
		const TArrayStreamInfo& in_arrayStreamInfo
		);
	GMeshManual(
		const GMeshType::TPrimitiveType::TEnum in_topologyType,
		const TArrayShort& in_arrayVertexIndex,
		const GS32 in_vertexDataCount, //assert in_vertexDataCount < 65536
		const TArrayStreamInfo& in_arrayStreamInfo
		);
	~GMeshManual();

	////////////////////////////////////////////////////
	//public accessors
public:
	const GMesh& GetMesh()const;

	const GR32 GetFloat(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)const;
	GR32& GetFloatRef(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex);

	const GU8 GetByte(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex)const;
	GU8& GetByteRef(const GS32 in_vertexDataIndex, const GS32 in_streamIndex, const GS32 in_subIndex);

	//wanted to allow change of topology data (index) but don't want byte or short exposed
	void SetArrayIndex(const GS32 in_index, const GS32 in_value);
	void SetElementCount(const GS32 in_elementCount);
	const GS32 GetElementCount()const;

	const GS32 GetMaxElementCount()const;
	const GS32 GetMaxVertexCount()const;

	////////////////////////////////////////////////////
	//private members
private:
	TPointerMesh mMesh;
	TPointerMeshVertexData mVertexData;
	TArrayByte mArrayIndexByte;
	TArrayShort mArrayIndexShort;
	TArrayStreamInfo mArrayStreamInfo;

};

#endif 