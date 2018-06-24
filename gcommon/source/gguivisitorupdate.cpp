//
//  GGuiVisitorUpdate.cpp
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiVisitorUpdate.h"

#include "GGui.h"
#include "GGuiManager.h"
#include "GRender.h"
#include "GGuiNodeComponentPanel.h"
#include "GGuiNodeComponentText.h"
#include "GMesh.h"

//public static methods
/*static*/ void GGuiVisitorUpdate::Run(
	GGui& inout_Gui
	)
{
	inout_Gui.Update();
	GGuiVisitorUpdate guiVisitor(inout_Gui);
	inout_Gui.QueryAll(guiVisitor, GGuiNodeComponentPanel::GetComponentFlag() | GGuiNodeComponentText::GetComponentFlag());
	return;
}

//constructor
GGuiVisitorUpdate::GGuiVisitorUpdate(
	GGui& inout_gui
	)
	: mGui(inout_gui)
{
	return;
}

GGuiVisitorUpdate::~GGuiVisitorUpdate()
{
	return;
}

//public methods
void GGuiVisitorUpdate::QuerySubmitNode(
	TPointerGuiNode& inout_node
	)
{
	GGuiNode& node = *inout_node;
	const int count = node.GetComponentCount();
	for (int index = 0; index < count; ++index)
	{
		GGuiNodeComponentPanel* const pPanel = node.GetComponent<GGuiNodeComponentPanel>(index);
		if (pPanel)
		{
			pPanel->Update(mGui, node);
		}

		GGuiNodeComponentText* const pText = node.GetComponent<GGuiNodeComponentText>(index);
		if (pText)
		{
			pText->Update(mGui, node);
		}
	}
	return;
}
