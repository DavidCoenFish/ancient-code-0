//
//  GSceneType.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneType_h_
#define _GSceneType_h_

class GSceneType
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
			TBoneCollision		= 0x0002,
			TClient				= 0x0004,
			TCollision			= 0x0008,
			TFactoryStub		= 0x0010,
			TFade				= 0x0020,
			TLight				= 0x0040,
			TLightCollector		= 0x0080,
			TParticle			= 0x0100,
			TPhysics			= 0x0200,
			TPointAtBone		= 0x0400,
			TNodeAttach			= 0x0800,
			TRope				= 0x1000,
			TSpringBone			= 0x2000,
			TVisual				= 0x4000,
			TPad			= 0xFFFFFFFF
		};
	};
	//disabled
private:
	~GSceneType();

	//static public methods
public:

};

#endif 