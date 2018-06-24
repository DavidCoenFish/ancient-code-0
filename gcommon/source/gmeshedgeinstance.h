//
//  GMeshEdgeInstance.h
//
//  Created by David Coen on 2011 04 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshEdgeInstance_h_
#define _GMeshEdgeInstance_h_

class GMatrix16Float;

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GVector3Float.h"

class GMesh;
class GMeshManual;
class GVector3Float;

/**/
class GMeshEdgeInstance : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshManual> TPointerRenderMeshManual;

	//constructor
public:
	GMeshEdgeInstance(const GMesh& in_meshLoad);
	~GMeshEdgeInstance();

	//public methods
public:
	void Update(
		const GMesh& in_meshLoad,
		const void* const in_vertexData,
		const GVector3Float& in_direction,
		const float in_distance
		);

	//public accessors
public:
	const GMesh& GetShadowMesh()const;
	const void* const GetShadowVertexData()const;

	//private members;
private:
	const int mStreamPosByteOffset;
	const int mStreamPosCount;

	TPointerRenderMeshManual mShadowMesh;

	//cache update param, if not dynamic, we may not have to regenerate shadow volume
	GVector3Float mDirection;
	float mDistance;

};

#endif 