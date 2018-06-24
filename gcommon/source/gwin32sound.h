//
//  GWin32Sound.h
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GWin32Sound_h_
#define _GWin32Sound_h_

#include <string>

class GFileSystem;

class GWin32Sound
{
	//constructor
private:
	~GWin32Sound();

	//static public methods
public:
	static const unsigned int LoadSound(
		GFileSystem& inout_fileSystem,
		const std::string& in_resourceName
		);

};

#endif 