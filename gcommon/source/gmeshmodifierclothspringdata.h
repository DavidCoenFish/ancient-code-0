//
//  GMeshModifierClothSpringData.h
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifierClothSpringData_h_
#define _GMeshModifierClothSpringData_h_

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifierClothSpringData
{
	//constructor
public:
	GMeshModifierClothSpringData(
		const int in_vertexIndexOne,
		const int in_vertexIndexTwo,
		const float in_length
		);
	~GMeshModifierClothSpringData();

	//public accessors
public:
	const int GetSpringVertexIndexOne()const{ return mVertexIndexOne; }
	const int GetSpringVertexIndexTwo()const{ return mVertexIndexTwo; }
	const float GetLength()const{ return mLength; }

	//private members
private:
	const int mVertexIndexOne;
	const int mVertexIndexTwo;
	const float mLength;

};

#endif 