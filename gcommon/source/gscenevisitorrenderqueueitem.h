//
//  GSceneVisitorRenderQueueItem.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorRenderQueueItem_h_
#define _GSceneVisitorRenderQueueItem_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GMatrix16Float.h"

class GMesh;
class GMaterialInstance;
class GRenderLight;

/**/
class GSceneVisitorRenderQueueItem : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	//typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;
	//typedef std::vector<TPointerRenderLight> TArrayPointerRenderLight;

	//constructor
public:
	GSceneVisitorRenderQueueItem(
		const float in_sortValue,
		const GMatrix16Float& inout_worldTransform,
		const GMesh* const in_meshLoad,
		const void* const in_meshVertexData,
		const GMaterialInstance* const inout_material//,
		//const bool in_GetLight,
		//const TArrayPointerRenderLight* const in_arrayLight
		);
	~GSceneVisitorRenderQueueItem();

	//public accessors
public:
	GSceneVisitorRenderQueueItem* GetQueueItemNext()const{ return mQueueItemNext; }
	void SetQueueItemNext(GSceneVisitorRenderQueueItem* in_queueItemNext){ mQueueItemNext = in_queueItemNext; return; }

	const float GetSortValue()const{ return mSortValue; }
	const GMatrix16Float& GetWorldTransform(){ return mWorldTransform; }
	const GMesh* const GetMesh(){ return mMesh; }
	const void* const GetMeshVertexData(){ return mMeshVertexData; }
	const GMaterialInstance* const GetMaterial()const{ return mMaterial; }

	//const bool GetLight(){ return mGetLight; }
	//const TArrayPointerRenderLight* const GetArrayLight()const{ return mArrayLight; }

	//private members;
private:
	GSceneVisitorRenderQueueItem* mQueueItemNext;
	const float mSortValue;
	const GMatrix16Float mWorldTransform; //was tempted to keep reference, but could be a stack variable
	const GMesh* const mMesh;
	const void* const mMeshVertexData;
	//TPointerMaterialInstance mMaterial;
	const GMaterialInstance* const mMaterial;

	//const bool mGetLight;
	//const TArrayPointerRenderLight* const mArrayLight;

};

#endif 