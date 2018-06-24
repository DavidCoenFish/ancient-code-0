//
//  GMeshInstanceModifier.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifier.h"

//constructor
GMeshInstanceModifier::GMeshInstanceModifier()
{
	return;
}

/*virtual*/ GMeshInstanceModifier::~GMeshInstanceModifier()
{
	return;
}

//public methods
void GMeshInstanceModifier::TickApplyModifier(
	GMeshVertexData& inout_vertexData,
	int& inout_flagWrittenStreams,
	const float in_timeDelta,
	const GMesh& in_meshLoad,
	GSceneNode& in_sceneNode //rather than pass matrix, invert matrix and teleport flag?
	)
{
	OnTickApplyModifier(
		inout_vertexData,
		inout_flagWrittenStreams,
		in_timeDelta,
		in_meshLoad,
		in_sceneNode
		);
	return;
}

//interface
/*virtual*/ void GMeshInstanceModifier::OnTickApplyModifier(
	GMeshVertexData& inout_vertexData,
	int& inout_flagWrittenStreams,
	const float in_timeDelta,
	const GMesh& in_meshLoad,
	GSceneNode& in_sceneNode
	)
{
	return;
}