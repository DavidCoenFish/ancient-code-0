//
//  GMeshModifierSkinBoneVertexData.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierSkinBoneVertexData_h_
#define _GMeshModifierSkinBoneVertexData_h_

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierSkinBoneVertexData
{
	//constructor
public:
	GMeshModifierSkinBoneVertexData(
		const unsigned int in_vertexTargetIndex,
		const float in_weight
		);
	~GMeshModifierSkinBoneVertexData();

	//public accessors
public:
	const unsigned int GetVertexTargetIndex()const{ return mVertexTargetIndex; }
	const float GetWeight()const{ return mWeight; }

	//private members;
private:
	const unsigned int mVertexTargetIndex;
	const float mWeight;

};

#endif 