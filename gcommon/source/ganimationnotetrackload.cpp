//
//  GAnimationNoteTrackLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationNoteTrackLoad.h"

//constructor
GAnimationNoteTrackLoad::GAnimationNoteTrackLoad(
	const float in_time,
	const char* const in_note
	)
	: mTime(in_time)
	, mNote(in_note)
{
	return;
}

GAnimationNoteTrackLoad::~GAnimationNoteTrackLoad()
{
	return;
}

