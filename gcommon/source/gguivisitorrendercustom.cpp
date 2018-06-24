//
//  GGuiVisitorRenderCustom.cpp
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiVisitorRenderCustom.h"

#include "GGui.h"
#include "GGuiManager.h"
#include "GRender.h"
#include "GGuiNodeComponentVisual.h"
#include "GMesh.h"
#include "GCamera.h"
#include "GMatrix16Float.h"

//public static methods
/*static*/ void GGuiVisitorRenderCustom::Run(
	GGui& inout_Gui,
	GRender& inout_renderer, 
	const GMatrix16Float& in_matrix
	)
{
	GGuiVisitorRenderCustom guiVisitorRender(inout_renderer);
	GCamera camera(inout_Gui.GetScreenOrientation());
	inout_renderer.SetCamera(camera);
	inout_renderer.SetObjectTransform(in_matrix);
	inout_Gui.QueryAll(guiVisitorRender, GGuiNodeComponentVisual::GetComponentFlag());
	//inout_renderer.DebugObjectDraw();
	return;
}

//constructor
GGuiVisitorRenderCustom::GGuiVisitorRenderCustom(
	GRender& inout_renderer
	)
	: mRender(inout_renderer)
{
	return;
}

GGuiVisitorRenderCustom::~GGuiVisitorRenderCustom()
{
	return;
}

//public methods
void GGuiVisitorRenderCustom::QuerySubmitNode(
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
