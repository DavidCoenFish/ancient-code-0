//
//  GMeshEdgeTwoPoly.h
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdgeTwoPoly_h_
#define _GMeshEdgeTwoPoly_h_

#include <boost/noncopyable.hpp>

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshEdgeTwoPoly : public boost::noncopyable 
{
	//typedef
private:

	//constructor
public:
	GMeshEdgeTwoPoly(
		const int in_vertexIndexOne,
		const int in_vertexIndexTwo,
		const int in_normalIndexOne,
		const int in_normalIndexTwo
		);
	~GMeshEdgeTwoPoly();

	//public accessors
public:
	const int GetVertexIndexOne()const{ return mVertexIndexOne; }
	const int GetVertexIndexTwo()const{ return mVertexIndexTwo; }
	const int GetNormalIndexOne()const{ return mNormalIndexOne; }
	const int GetNormalIndexTwo()const{ return mNormalIndexTwo; }

	//private members;
private:
	const int mVertexIndexOne;
	const int mVertexIndexTwo;
	const int mNormalIndexOne;
	const int mNormalIndexTwo;

};

#endif 