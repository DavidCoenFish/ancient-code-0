//
//  GMeshInstanceModifierSkin.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifierSkin_h_
#define _GMeshInstanceModifierSkin_h_

class GMatrix16Float;

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GMeshInstanceModifier.h"

//class GMeshInstanceModifierSkinBone;
class GMesh;
class GMeshModifierSkin;
class GSkeletonInstance;

/**/
class GMeshInstanceModifierSkin : public GMeshInstanceModifier
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshInstanceModifierSkin> TPointerRenderMeshInstanceModifierSkin;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

	typedef std::vector<int> TArrayInt;

	//static public methods
public:
	static TPointerRenderMeshInstanceModifierSkin Factory(
		const GMesh& in_meshLoad,
		const int in_modifierIndex
		);

	//constructor
public:
	GMeshInstanceModifierSkin(
		const int in_arrayBoneCount,
		const int in_modifierIndex
		);
	~GMeshInstanceModifierSkin();

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
	TArrayInt mArraySkeletonBoneIndex;

};

#endif 