//
//  GScratchPad.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GScratchPad_h_
#define _GScratchPad_h_

#include <boost/noncopyable.hpp>
#include <vector>

/*
singelton generic block of memory for short term use. 
singelton access for lock and unlock, but class is expected to be created normally in code
*/
class GScratchPad : public boost::noncopyable
{
	//typedef
private:
	typedef std::vector<unsigned char> TArrayByte;

	//public static methods
public:
	static GScratchPad& Get();

	//constructor
public:
	GScratchPad(const int in_byteSizeHint = 0);
	~GScratchPad();

	//public methods
public:
	void* const LockMemory(const int in_byteSize);
	void UnlockMemory();

	//private members
private:
	static GScratchPad* sSelf;

	TArrayByte mArrayMemory;
	bool mLock;

};

#endif 