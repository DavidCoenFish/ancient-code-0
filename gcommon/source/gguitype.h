//
//  GGuiType.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiType_h_
#define _GGuiType_h_

class GGuiType
{
	//typedef
public:
	//WARNING: enum reflected in pipeline scene packer
	struct TComponentFlag
	{
		enum TFlag
		{
			TNone				= 0x0000,
			TAnimation			= 0x0001,
			TButton				= 0x0002,
			TPanel				= 0x0004,
			TText				= 0x0008,
			TVisual				= 0x0010,
			TPad				= 0xFFFFFFFF
		};
	};
	//disabled
private:
	~GGuiType();

	//static public methods
public:

};

#endif 