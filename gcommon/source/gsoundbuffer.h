//
//  GSoundBuffer.h
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundBuffer_h_
#define _GSoundBuffer_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

class GFileSystem;

class GSoundBuffer : public boost::noncopyable
{
	//typedef
private:

	//constructor
public:
	GSoundBuffer(
		GFileSystem& inout_fileSystem,
		const std::string& in_resourceName
		);
	~GSoundBuffer();

	//public accessors
public:
	const unsigned int GetBufferId()const{ return mBufferId; }

	//private members;
private:
	unsigned int mBufferId;

};

#endif 