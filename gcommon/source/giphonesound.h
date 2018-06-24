//
//  GIPhoneSound.h
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GIPhoneSound_h_
#define _GIPhoneSound_h_

#include <string>

class GIPhoneSound
{
	//disabled
private:
	~GIPhoneSound();

	//static public methods
public:
	static const unsigned int LoadSound(const std::string& in_resourceName);
	
};

#endif 