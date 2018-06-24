//
//  GMeshModifierMorphTarget.h
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierMorphTarget_h_
#define _GMeshModifierMorphTarget_h_

class GMeshModifierMorphTargetStream;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierMorphTarget
{
	//constructor
public:
	GMeshModifierMorphTarget(
		const char* const in_targetName,
		const int in_arrayStreamCount,
		const GMeshModifierMorphTargetStream* const in_arrayStream,
		const int* const in_duplicateTable
		);
	~GMeshModifierMorphTarget();

	//public accessors
public:
	const char* const GetTargetName()const{ return mTargetName; }
	const int GetArrayStreamCount()const{ return mArrayStreamCount; }
	const GMeshModifierMorphTargetStream* const GetArrayStream()const{ return mArrayStream; }
	const int* const GetDuplicateTable()const{ return mDuplicateTable; }

	//private members;
private:
	const char* const mTargetName;
	const int mArrayStreamCount;
	const GMeshModifierMorphTargetStream* const mArrayStream;
	const int* const mDuplicateTable; //[ duplicate count, [duplicate0, duplicate1, ....]],

};

#endif 