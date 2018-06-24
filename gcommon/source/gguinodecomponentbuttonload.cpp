//
//  GGuiNodeComponentButtonLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentButtonLoad.h"

//constructor
GGuiNodeComponentButtonLoad::GGuiNodeComponentButtonLoad(
	const char* const in_name,
	const char* const in_actionScriptButtonDown,
	const char* const in_actionScriptButtonPress,
	const char* const in_actionScriptButtonCancel,		
	const char* const in_actionScriptButtonEnable,		
	const char* const in_actionScriptButtonDisable		
	)
	: mName(in_name)
	, mActionScriptButtonDown(in_actionScriptButtonDown)
	, mActionScriptButtonPress(in_actionScriptButtonPress)
	, mActionScriptButtonCancel(in_actionScriptButtonCancel)
	, mActionScriptButtonEnable(in_actionScriptButtonEnable)
	, mActionScriptButtonDisable(in_actionScriptButtonDisable)
{
	return;
}

GGuiNodeComponentButtonLoad::~GGuiNodeComponentButtonLoad()
{
	return;
}
