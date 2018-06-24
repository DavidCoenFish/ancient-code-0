//
//  GMeshEdgeNormalDynamic.h
//
//  Created by David Coen on 2011 03 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdgeNormalDynamic_h_
#define _GMeshEdgeNormalDynamic_h_

#include <boost/noncopyable.hpp>

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshEdgeNormalDynamic : public boost::noncopyable 
{
	//typedef
private:

	//constructor
public:
	GMeshEdgeNormalDynamic(
		const int in_vertexIndex0,
		const int in_vertexIndex1,
		const int in_vertexIndex2
		);
	~GMeshEdgeNormalDynamic();

	//public accessors
public:
	const int GetVertexIndex0()const{ return mVertexIndex0; }
	const int GetVertexIndex1()const{ return mVertexIndex1; }
	const int GetVertexIndex2()const{ return mVertexIndex2; }

	//private members;
private:
	const int mVertexIndex0;
	const int mVertexIndex1;
	const int mVertexIndex2;

};

#endif 