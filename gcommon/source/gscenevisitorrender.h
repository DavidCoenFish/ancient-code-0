//
//  GSceneVisitorRender.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorRender_h_
#define _GSceneVisitorRender_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GVector3Float.h"

class GScene;
class GCamera;
class GSceneNode;
class GDrawMesh;
class GMaterialInstance;
class GMatrix16Float;
class GSceneVisitorRenderQueueItem;
class GRender;
class GMatrix16Float;
class GVector3Float;
class GSceneNodeComponentVisual;
class GRenderLight;
class GSceneNodeComponentRope;
class GMesh;

/*
a scene visitor for rendering
*/
class GSceneVisitorRender : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;
	typedef std::vector<TPointerRenderLight> TArrayPointerRenderLight;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(
		GScene& inout_scene,
		GRender& in_renderer,
		GCamera& in_camera		
		);

	static void Run(
		TPointerSceneNode& inout_sceneNode,
		GRender& in_renderer,
		GCamera& in_camera		
		);

	//constructor
private:
	GSceneVisitorRender(
		GRender& in_renderer,
		GCamera& in_camera
		);
	~GSceneVisitorRender();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		);
	void QueryFinish(); //draw pending objects

	//private methods
private:
	void RenderComponentVisual(
		GSceneNode& inout_node,
		GSceneNodeComponentVisual& in_componentVisual
		);

	void RenderObject(
		const GMatrix16Float& in_worldTransform,
		const GMesh* const in_meshLoad,
		const void* const in_meshVertexData,
		const GMaterialInstance* const in_material,
		//const bool in_GetLight,
		GSceneNode& inout_node
		);
	void RenderObjectAlpha(
		const GMatrix16Float& in_worldTransform,
		const GMesh* const in_meshLoad,
		const void* const in_meshVertexData,
		const GMaterialInstance* const in_material,
		const float in_alphaSortValue,
		//const bool in_GetLight,
		GSceneNode& inout_node
		);

	void Render(
		const GMatrix16Float& in_worldTransform,
		//const bool in_GetLight,
		//const TArrayPointerRenderLight* const in_arrayLight,
		const GMesh* const in_meshLoad,
		const void* const in_meshVertexData,
		const GMaterialInstance* const in_material
		);

	//public accessors
public:
	const GVector3Float& GetCameraPosition()const{ return mCameraPosition; }
	const GVector3Float& GetCameraDirection()const{ return mCameraDirection; }

	//private members;
private:
	GRender& mRender;
	GCamera& mCamera;
	GVector3Float mCameraPosition;
	GVector3Float mCameraDirection;

	//GSceneNodeComponentVisual* mRenderQueueAlpha;
	GSceneVisitorRenderQueueItem* mRenderQueueItem;

};

#endif 