//
//  GMeshInstanceModifierMorph.h
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifierMorph_h_
#define _GMeshInstanceModifierMorph_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GMeshInstanceModifier.h"

class GMesh;
class GSkeletonInstance;

/**/
class GMeshInstanceModifierMorph : public GMeshInstanceModifier
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshInstanceModifierMorph> TPointerRenderMeshInstanceModifierMorph;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

	typedef std::vector<int> TArrayInt;

	//static public methods
public:
	static TPointerRenderMeshInstanceModifierMorph Factory(
		const GMesh& in_meshLoad,
		const int in_modifierIndex
		);

	//constructor
public:
	GMeshInstanceModifierMorph(
		const int in_arrayMorphTargetCount,
		const int in_modifierIndex
		);
	~GMeshInstanceModifierMorph();

	//implement GMeshInstanceModifier
private:
	virtual void OnTickApplyModifier(
		GMeshVertexData& inout_vertexData,
		int& inout_flagWrittenStreams,
		const float in_timeDelta,
		const GMesh& in_meshLoad,
		GSceneNode& in_sceneNode
		);

	//public accessors
public:
	void SetSkeletonInstance(
		TPointerSkeletonInstance& inout_skeletonInstance,
		const GMesh& in_meshLoad		
		);

	//private members;
private:
	const int mModifierIndex;
	TWeakSkeletonInstance mSkeletonInstance;
	TArrayInt mArraySkeletonMorphTargetIndex;

};

#endif 