//
//  GRenderShadow.h
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GRenderShadow_h_
#define _GRenderShadow_h_

#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "GVector3Float.h"
#include "GColour4Float.h"

class GRender;
class GResourceManager;
class GMaterialInstance;
class GMeshManual;
class GScene;
class GCamera;

/**/
class GRenderShadow : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::scoped_ptr<GCamera> TPointerCamera;

	typedef boost::shared_ptr<GRender> TPointerRender;

	//constructor
public:
	GRenderShadow(
		GResourceManager& inout_resourceManager,
		const GVector3Float& in_shadowDirection,
		const GColour4Float& in_shadowColour
		);
	~GRenderShadow();

	//public methods
public:
	void RenderShadows(
		GRender& inout_render,
		GScene& inout_scene,
		GCamera& inout_sceneCamera
		);

	//private members
private:
	GVector3Float mShadowDirection;
	//GColour4Float mShadowColour;
	TWeakMaterialInstance mMaterialShadowStencilFront;
	TWeakMaterialInstance mMaterialShadowStencilBack;
	TWeakMaterialInstance mMaterialShadow;

	TPointerMeshManual mMeshShadow;
	TPointerCamera mOrthoCamera;

};

#endif //_GRenderShadow_h_