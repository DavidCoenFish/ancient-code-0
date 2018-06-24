//
//  GSkeletonBone.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSkeletonBone_h_
#define _GSkeletonBone_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GMatrix16Float.h"

class GSkeletonLoad;

/**/
class GSkeletonBone : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GSkeletonBone> TPointerSkeletonBone;
	typedef std::vector<TPointerSkeletonBone> TArrayPointerSkeletonBone;

	//constructor
public:
	GSkeletonBone(const GMatrix16Float& in_parentRelativeTransform);
	~GSkeletonBone();

	//public methods
public:
	const bool SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform);
	const bool MarkObjectSpaceTransformDirty();

	//public accessors
public:
	const GMatrix16Float& GetParentRelativeTransform()const{ return mParentRelativeTransform; }
	const GMatrix16Float& GetObjectSpaceTransform(
		const int in_boneIndex,
		TArrayPointerSkeletonBone& inout_arrayInstanceSkinBone,
		const GSkeletonLoad& in_skeletonLoad
		);

	//private members;
private:
	GMatrix16Float mParentRelativeTransform;
	GMatrix16Float mObjectSpaceTransform;
	bool mObjectSpaceTransformDirty;

};

#endif 