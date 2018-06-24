//
//  GScratchPadLock.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GScratchPadLock_h_
#define _GScratchPadLock_h_

#include <boost/noncopyable.hpp>
#include <vector>

#include "GScratchPad.h"

/**/
template<typename IN_TYPE>
class GScratchPadLock : public boost::noncopyable
{
	//constructor
public:
	GScratchPadLock(const int in_elementCount)
		: mData((IN_TYPE*)GScratchPad::Get().LockMemory(in_elementCount * sizeof(IN_TYPE)))
	{
		return;
	}
	~GScratchPadLock()
	{
		GScratchPad::Get().UnlockMemory();
		return;
	}

	//public methods
public:
	IN_TYPE* const GetMemory(){ return mData; }

private:
	IN_TYPE* const mData;

};

#endif 