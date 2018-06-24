//
//  GMeshModifierMorphTargetStream.h
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierMorphTargetStream_h_
#define _GMeshModifierMorphTargetStream_h_

class GMeshModifierMorphTarget;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierMorphTargetStream
{
	//constructor
public:
	GMeshModifierMorphTargetStream(
		const void* const in_streamData,
		const int in_arrayDuplicateTableOffsetCount,
		const int* const in_arrayDuplicateTableOffset
		);
	~GMeshModifierMorphTargetStream();

	//public accessors
public:
	const void* const GetStreamData()const{ return mStreamData; }
	const int GetArrayDuplicateTableOffsetCount()const{ return mArrayDuplicateTableOffsetCount; }
	const int* const GetArrayDuplicateTableOffset()const{ return mArrayDuplicateTableOffset; }

	//private members;
private:
	const void* const mStreamData;
	const int mArrayDuplicateTableOffsetCount;
	const int* const mArrayDuplicateTableOffset;

};

#endif 