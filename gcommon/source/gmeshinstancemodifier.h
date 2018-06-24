//
//  GMeshInstanceModifier.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifier_h_
#define _GMeshInstanceModifier_h_

#include <boost/noncopyable.hpp>

class GMeshVertexData;
class GMesh;
class GMatrix16Float;
class GSceneNode;

/**/
class GMeshInstanceModifier : public boost::noncopyable
{
	//constructor
public:
	GMeshInstanceModifier();
	virtual ~GMeshInstanceModifier();

	//public methods
public:
	void TickApplyModifier(
		GMeshVertexData& inout_vertexData,
		int& inout_flagWrittenStreams,
		const float in_timeDelta,
		const GMesh& in_meshLoad,
		GSceneNode& in_sceneNode //rather than pass matrix, invert matrix and teleport flag?
		);

	//interface
private:
	virtual void OnTickApplyModifier(
		GMeshVertexData& inout_vertexData,
		int& inout_flagWrittenStreams,
		const float in_timeDelta,
		const GMesh& in_meshLoad,
		GSceneNode& in_sceneNode
		);

};

#endif 