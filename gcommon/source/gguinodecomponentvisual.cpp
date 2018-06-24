//
//  GGuiNodeComponentVisual.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentVisual.h"
#include "GGuiType.h"
#include "GMeshManual.h"
#include "GMesh.h"

struct TGuiNodeComponentVisualFlag
{
	enum TEnum
	{
		TNone		= 0x0000
		, TVisible	= 0x0001
	};
};

//static public methods
/*static*/ const unsigned int GGuiNodeComponentVisual::GetComponentFlag()
{
	return GGuiType::TComponentFlag::TVisual;
}

//constructor
GGuiNodeComponentVisual::GGuiNodeComponentVisual(
	TPointerMeshManual& in_meshManual,
	TPointerMaterialInstance& in_materialInstance,
	const bool in_visible
	)
	: GGuiNodeComponentBase(GetComponentFlag())
	, mFlag(0)
	, mMeshManual(in_meshManual)
	, mMaterialInstance(in_materialInstance)
{
	SetVisible(in_visible);
	return;
}

/*virtual*/ GGuiNodeComponentVisual::~GGuiNodeComponentVisual()
{
	return;
}

//public accessors
const bool GGuiNodeComponentVisual::GetVisible()const
{
	return (0 != (mFlag & TGuiNodeComponentVisualFlag::TVisible));
}

void GGuiNodeComponentVisual::SetVisible(const bool in_visible)
{
	if (in_visible)
	{
		mFlag |= TGuiNodeComponentVisualFlag::TVisible;
	}
	else
	{
		mFlag &= ~TGuiNodeComponentVisualFlag::TVisible;
	}
	return;
}

// else provide public render method? render
const GMesh* const GGuiNodeComponentVisual::GetMesh()const
{
	TPointerMeshManual meshManual = mMeshManual.lock();
	if (meshManual)
	{
		return &meshManual->GetMesh();
	}
	return NULL;
}

//const void* const GGuiNodeComponentVisual::GetMeshVertexData()const
//{
//	TPointerRenderMeshManual meshManual = mRenderMeshManual.lock();
//	if (meshManual)
//	{
//		return meshManual->GetMesh().GetVertexData();
//	}
//	return NULL;
//}

GMaterialInstance* const GGuiNodeComponentVisual::GetMaterial()
{
	return mMaterialInstance.lock().get();
}

void GGuiNodeComponentVisual::SetMaterial(TPointerMaterialInstance& inout_materialInstance)
{
	mMaterialInstance = inout_materialInstance;
	return;
}
