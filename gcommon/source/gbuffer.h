//
//  gbuffer.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GBuffer_h_
#define _GBuffer_h_

#include <vector>
#include "gcommon.h"

class GBuffer
{
	//////////////////////////////////////////////////
	//typedef
private:
	typedef std::vector<GU8> TArrayByte;

	//////////////////////////////////////////////////
	//constructor
public:
	GBuffer(const GS32 in_byteCount);
	GBuffer(const GBuffer& in_src);
	GBuffer();
	~GBuffer();

	//////////////////////////////////////////////////
	//operators
public:
	const GBuffer& operator=(const GBuffer& in_rhs);

	//////////////////////////////////////////////////
	//public methods
public:
	//assert if not enough space
	void SetData(
		const GS32 in_byteOffset, 
		const GS32 in_byteLength,
		const GU8* const in_data
		);

	void AppendData(
		const GS32 in_byteLength,
		const GU8* const in_data
		);

	void Clear();

	//////////////////////////////////////////////////
	//public accessors
public:
	void SetCount(const GS32 in_byteCount);
	const GS32 GetCount()const;

	const GU8* const GetData()const{ return &mArrayData[0]; }
	GU8* const GetData(){ return &mArrayData[0]; }

	//////////////////////////////////////////////////
	//private members;
private:
	TArrayByte mArrayData;

};

#endif 