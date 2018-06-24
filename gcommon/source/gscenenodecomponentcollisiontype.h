//
//  GSceneNodeComponentCollisionType.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionType_h_
#define _GSceneNodeComponentCollisionType_h_

struct GSceneNodeComponentCollisionType
{
	//typedef
public:
	struct TType
	{
		enum TEnum
		{
			TSphere = 0,
			TCapsual,
			TBox,

			TCount
		};
	};

	//constructor
private:
	~GSceneNodeComponentCollisionType();

};

#endif 