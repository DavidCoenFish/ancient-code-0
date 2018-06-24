//
//  GMeshModifierSkinTargetStream.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierSkinTargetStream_h_
#define _GMeshModifierSkinTargetStream_h_

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierSkinTargetStream
{
	//constructor
public:
	GMeshModifierSkinTargetStream(
		const unsigned int in_streamIndex,
		const unsigned int in_flag
		);
	~GMeshModifierSkinTargetStream();

	//public accessors
public:
	const unsigned int GetStreamIndex()const{ return mStreamIndex; }
	const bool GetIsPoint()const; //(x, y, z, 1)
	const bool GetIsVector()const; //(x, y, z, 0)

	//private members
private:
	const unsigned int mStreamIndex;
	const unsigned int mFlag;

};

#endif 