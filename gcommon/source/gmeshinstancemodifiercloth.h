//
//  GMeshInstanceModifierCloth.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifierCloth_h_
#define _GMeshInstanceModifierCloth_h_

class GMatrix16Float;

#include <boost/shared_ptr.hpp>
#include <vector>
#include "GMeshInstanceModifier.h"
#include "GMeshInstanceModifierClothVertexData.h"

class GMesh;
class GMeshInstanceModifierClothVertexData;

/**/
class GMeshInstanceModifierCloth : public GMeshInstanceModifier
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshInstanceModifierCloth> TPointerRenderMeshInstanceModifierCloth;
	typedef std::vector<GMeshInstanceModifierClothVertexData> TArrayVertexData;

	//static public methods
public:
	static TPointerRenderMeshInstanceModifierCloth Factory(
		const GMesh& in_meshLoad,
		const int in_modifierIndex
		);

	//constructor
public:
	GMeshInstanceModifierCloth(
		const int in_modifierIndex
		);
	~GMeshInstanceModifierCloth();

	//implement GMeshInstanceModifier
private:
	virtual void OnTickApplyModifier(
		GMeshVertexData& inout_vertexData,
		int& inout_flagWrittenStreams,
		const float in_timeDelta,
		const GMesh& in_meshLoad,
		GSceneNode& in_sceneNode
		);

	//private members;
private:
	const int mModifierIndex;
	TArrayVertexData mArrayVertexData;

};

#endif 