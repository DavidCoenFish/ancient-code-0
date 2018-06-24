//
//  GGuiVisitorRender.cpp
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiVisitorRender.h"

#include "GGui.h"
#include "GGuiManager.h"
#include "GRender.h"
#include "GGuiNodeComponentVisual.h"
#include "GMesh.h"
#include "GCamera.h"

//public static methods
/*static*/ void GGuiVisitorRender::Run(
	GGui& inout_Gui,
	GRender& inout_renderer
	)
{
	GGuiVisitorRender guiVisitorRender(inout_renderer);
	GCamera camera(inout_Gui.GetScreenOrientation());
	inout_renderer.SetCamera(camera);
	inout_renderer.SetObjectTransform(GMatrix16Float::sIdentity);
	inout_Gui.QueryAll(guiVisitorRender, GGuiNodeComponentVisual::GetComponentFlag());
	//inout_renderer.DebugObjectDraw();
	return;
}

//constructor
GGuiVisitorRender::GGuiVisitorRender(
	GRender& inout_renderer
	)
	: mRender(inout_renderer)
{
	return;
}

GGuiVisitorRender::~GGuiVisitorRender()
{
	return;
}

//public methods
void GGuiVisitorRender::QuerySubmitNode(
	TPointerGuiNode& inout_node
	)
{
	GGuiNode& node = *inout_node;
	const int count = node.GetComponentCount();
	for (int index = 0; index < count; ++index)
	{
		GGuiNodeComponentVisual* const pVisual = node.GetComponent<GGuiNodeComponentVisual>(index);
		if (!pVisual || !pVisual->GetVisible())
		{
			continue;
		}

		const GMesh* const meshLoad = pVisual->GetMesh();
		if (!meshLoad)
		{
			continue;
		}

		GMaterialInstance* const material = pVisual->GetMaterial();

		mRender.SetMaterial(*material);
		mRender.DrawMesh(*meshLoad, meshLoad->GetVertexData());

	}
	return;
}
