//
//  GSceneVisitorRenderShadow.h
//
//  Created by David Coen on 2011 04 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneVisitorRenderShadow_h_
#define _GSceneVisitorRenderShadow_h_

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
class GRender;
class GMatrix16Float;
class GVector3Float;
class GSceneNodeComponentVisual;
class GRenderLight;
class GSceneNodeComponentRope;
class GMesh;
class GMeshManual;

/*
a scene visitor for rendering
*/
class GSceneVisitorRenderShadow : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GDrawMesh> TPointerRenderDrawMesh;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//public static methods
public:
	static void Run(
		GScene& inout_scene,
		GRender& in_renderer,
		const GVector3Float& in_shadowDirection,
		const float in_shadowDistance,
		const GMaterialInstance& in_materialShadowFront,
		const GMaterialInstance& in_materialShadowBack,
		const GMaterialInstance& in_materialShadow,
		const GMeshManual& in_shadowMesh,
		GCamera& inout_worldCamera,
		GCamera& inout_shadowCamera
		);

	static void Run(
		TPointerSceneNode& inout_sceneNode,
		GRender& in_renderer,
		const GVector3Float& in_shadowDirection,
		const float in_shadowDistance,
		const GMaterialInstance& in_materialShadowFront,
		const GMaterialInstance& in_materialShadowBack,
		const GMaterialInstance& in_materialShadow,
		const GMeshManual& in_shadowMesh,
		GCamera& inout_worldCamera,
		GCamera& inout_shadowCamera
		);

	//constructor
private:
	GSceneVisitorRenderShadow(
		GRender& in_renderer,
		const GVector3Float& in_shadowDirection,
		const float in_shadowDistance,
		const GMaterialInstance& in_materialShadowFront,
		const GMaterialInstance& in_materialShadowBack,
		const GMaterialInstance& in_materialShadow,
		const GMeshManual& in_shadowMesh,
		GCamera& inout_worldCamera,
		GCamera& inout_shadowCamera
		);
	~GSceneVisitorRenderShadow();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerSceneNode& inout_node
		);
	void QueryFinish(); //draw pending objects

	//private members;
private:
	GRender& mRenderer;
	const GVector3Float& mShadowDirection;
	const float mShadowDistance;
	const GMaterialInstance& mMaterialShadowFront;
	const GMaterialInstance& mMaterialShadowBack;
	const GMaterialInstance& mMaterialShadow;
	const GMeshManual& mShadowMesh;
	GCamera& mShadowCamera;

};

#endif 