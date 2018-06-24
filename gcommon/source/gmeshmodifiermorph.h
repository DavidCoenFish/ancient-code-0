//
//  GMeshModifierMorph.h
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierMorph_h_
#define _GMeshModifierMorph_h_

class GMeshModifierMorphTarget;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierMorph
{
	//constructor
public:
	GMeshModifierMorph(
		const unsigned int in_arrayTargetCount,
		const GMeshModifierMorphTarget* const in_arrayTarget
		);
	~GMeshModifierMorph();

	//public accessors
public:
	const unsigned int GetArrayTargetCount()const{ return mArrayTargetCount; }
	const GMeshModifierMorphTarget* const GetArrayTarget()const{ return mArrayTarget; }

	//private members;
private:
	const unsigned int mArrayTargetCount;
	const GMeshModifierMorphTarget* const mArrayTarget;

};

#endif 