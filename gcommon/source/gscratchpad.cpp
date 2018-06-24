//
//  GScratchPad.cpp
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GScratchPad.h"
#include "GASSERT.h"

/*static*/ GScratchPad* GScratchPad::sSelf = NULL;

//static public methods
/*static*/ GScratchPad& GScratchPad::Get()
{
	GASSERT(NULL != sSelf, "invalid state");
	return (*sSelf);
}

//constructor
GScratchPad::GScratchPad(const int in_byteSizeHint)
: mArrayMemory()
, mLock(false)
{
	GASSERT(NULL == sSelf, "invalid state");
	sSelf = this;

	if ((int)mArrayMemory.size() < in_byteSizeHint)
	{
		mArrayMemory.resize(in_byteSizeHint);
	}

	return;
}

GScratchPad::~GScratchPad()
{
	GASSERT(NULL != sSelf, "invalid state");
	sSelf = NULL;
	return;
}

//private methods
void* const GScratchPad::LockMemory(const int in_byteSize)
{
	GASSERT(mLock == false, "access to locked scratch pad");

	if ((int)mArrayMemory.size() < in_byteSize)
	{
		mArrayMemory.resize(in_byteSize);
	}

	mLock = true;
	return &mArrayMemory[0];
}

void GScratchPad::UnlockMemory()
{
	//allow multiple unlock, makes exit easier
	mLock = false;
	return;
}

