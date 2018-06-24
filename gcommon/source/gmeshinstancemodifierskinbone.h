//
//  GMeshInstanceModifierSkinBone.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstanceModifierSkinBone_h_
#define _GMeshInstanceModifierSkinBone_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GMatrix16Float.h"

class GMeshModifierSkin;

/**/
class GMeshInstanceModifierSkinBone : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshInstanceModifierSkinBone> TPointerRenderMeshInstanceModifierSkinBone;
	typedef std::vector<TPointerRenderMeshInstanceModifierSkinBone> TArrayPointerRenderMeshInstanceModifierSkinBone;

	//constructor
public:
	GMeshInstanceModifierSkinBone(const GMatrix16Float& in_parentRelativeTransform);
	~GMeshInstanceModifierSkinBone();

	//public methods
public:
	const bool SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform);
	const bool MarkObjectSpaceTransformDirty();

	//public accessors
public:
	const GMatrix16Float& GetParentRelativeTransform()const{ return mParentRelativeTransform; }
	const GMatrix16Float& GetObjectSpaceTransform(
		const int in_boneIndex,
		TArrayPointerRenderMeshInstanceModifierSkinBone& inout_arrayInstanceSkinBone,
		const GMeshModifierSkin& in_modifierSkinLoad
		);

	//private members;
private:
	GMatrix16Float mParentRelativeTransform;
	GMatrix16Float mObjectSpaceTransform;
	bool mObjectSpaceTransformDirty;

};

#endif 