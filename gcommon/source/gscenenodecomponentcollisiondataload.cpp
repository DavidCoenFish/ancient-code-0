//
//  GSceneNodeComponentCollisionDataLoad.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataLoad.h"

#include "GAssert.h"
#include "GSceneNodeComponentCollisionDataBox.h"
#include "GSceneNodeComponentCollisionDataBoxLoad.h"
#include "GSceneNodeComponentCollisionDataCapsual.h"
#include "GSceneNodeComponentCollisionDataCapsualLoad.h"
#include "GSceneNodeComponentCollisionDataSphere.h"
#include "GSceneNodeComponentCollisionDataSphereLoad.h"
#include "GSceneNodeComponentCollisionType.h"
#include "GSceneNodeLoad.h"

//constructor
GSceneNodeComponentCollisionDataLoad::GSceneNodeComponentCollisionDataLoad(
	const int in_type,
	const void* const in_data
	)
	: mType(in_type)
	, mData(in_data)
{
	return;
}

GSceneNodeComponentCollisionDataLoad::~GSceneNodeComponentCollisionDataLoad()
{
	return;
}

//public methods
GSceneNodeComponentCollisionDataLoad::TPointerSceneNodeComponentCollisionDataBase GSceneNodeComponentCollisionDataLoad::Factory()const
{
	TPointerSceneNodeComponentCollisionDataBase pointer;
	switch (mType)
	{
	default:
		break;
	case GSceneNodeComponentCollisionType::TType::TSphere:
		{
			const GSceneNodeComponentCollisionDataSphereLoad& sphereLoad = *(GSceneNodeComponentCollisionDataSphereLoad*)mData;
			pointer.reset(new GSceneNodeComponentCollisionDataSphere(
				sphereLoad.GetRadius(),
				*(sphereLoad.GetOrigin())
				));
		}
		break;
	case GSceneNodeComponentCollisionType::TType::TCapsual:
		{
			const GSceneNodeComponentCollisionDataCapsualLoad& capsualLoad = *(GSceneNodeComponentCollisionDataCapsualLoad*)mData;
			pointer.reset(new GSceneNodeComponentCollisionDataCapsual(
				*(capsualLoad.GetOrigin()),
				*(capsualLoad.GetDirection()),
				capsualLoad.GetRadius(),
				capsualLoad.GetLength()
				));
		}
		break;
	case GSceneNodeComponentCollisionType::TType::TBox:
		{
			const GSceneNodeComponentCollisionDataBoxLoad& boxLoad = *(GSceneNodeComponentCollisionDataBoxLoad*)mData;
			pointer.reset(new GSceneNodeComponentCollisionDataBox(
				*(boxLoad.GetLow()),
				*(boxLoad.GetHigh())
				));
		}
		break;
	}

	return pointer;
}
