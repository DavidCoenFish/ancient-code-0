//
//  GMeshEdgeOnePoly.h
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdgeOnePoly_h_
#define _GMeshEdgeOnePoly_h_

#include <boost/noncopyable.hpp>

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshEdgeOnePoly : public boost::noncopyable 
{
	//typedef
private:

	//constructor
public:
	GMeshEdgeOnePoly(
		const int in_vertexIndexOne,
		const int in_vertexIndexTwo,
		const int in_normalIndex
		);
	~GMeshEdgeOnePoly();

	//public accessors
public:
	const int GetVertexIndexOne()const{ return mVertexIndexOne; }
	const int GetVertexIndexTwo()const{ return mVertexIndexTwo; }
	const int GetNormalIndex()const{ return mNormalIndex; }

	//private members;
private:
	const int mVertexIndexOne;
	const int mVertexIndexTwo;
	const int mNormalIndex;

};

#endif 