//
//  GSceneNodeComponentVisual.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentVisual_h_
#define _GSceneNodeComponentVisual_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GSceneNodeComponentBase.h"

class GBuffer;
class GScene;
class GSceneVisitorRender;
class GSceneNode;
class GScenePostLoadCallback;
class GSceneNodeComponentVisualLoad;
class GMaterialInstance;
class GMesh;
class GMeshInstance;
class GMeshEdgeInstance;
class GCamera;
class GMeshManual;

/**/
class GSceneNodeComponentVisual : public GSceneNodeComponentBase
{
	//typedef
public:
	//exposed for pipeline
	struct TVisualFlag
	{
		enum TEnum
		{
			TNone				= 0x0000,
			TVisable			= 0x0001,
			TFreeze				= 0x0002, //don't update render mesh even if visible
			TOffscreen			= 0x0004, //visual has failed fustrum test
			TRenderMeshAsset	= 0x0008, //using asset render mesh
			TRenderMeshManual	= 0x0010,  //using manual render mesh
			TUseCameraTransform	= 0x0020, //skybox
			TJustUsePosition	= 0x0040 //just use the specified transform position. default is scene node world transform. skybox, particle

			//dynamic- get from mesh asset, alpha/light get from material
		};
	};

private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;

	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::weak_ptr<GMeshManual> TWeakMeshManual;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GMeshInstance> TPointerMeshInstance;
	typedef boost::shared_ptr<GMeshEdgeInstance> TPointerMeshEdgeInstance;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading. return base rather than type pointer due to generic interface
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		const std::string& in_data,
		const std::string& in_meshName,
		const std::string& in_materialName,
		const float in_radius,
		const int in_flag
		);

	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentVisual(
		const std::string& in_data,
		TPointerBuffer& in_mesh,
		TPointerMaterialInstance& in_materialInstance,
		const float in_radius,
		const int in_flag
		);
	GSceneNodeComponentVisual(
		const std::string& in_data,
		TPointerMeshManual& in_meshManual,
		TPointerMaterialInstance& in_materialInstance,
		const float in_radius,
		const int in_flag
		);
	virtual ~GSceneNodeComponentVisual();

	//public methods
public:
	//updates dynamic mesh (IF VISIBLE?)
	void Tick(TPointerSceneNode& in_parent, const float in_timeDelta);

	void UpdateOffscreen(GSceneNode& in_parent, GCamera& in_camera);

	//public accessors
public:
	const std::string& GetData()const{ return mData; }

	const bool GetVisible()const;
	void SetVisible(const bool in_visible);

	const bool GetFreeze()const;
	void SetFreeze(const bool in_freeze);

	const bool GetOffscreen()const;
	void SetOffscreen(const bool in_offscreen);

	const bool GetHasAlpha()const;
	const bool GetHasLight()const;
	const bool GetMeshHasModifier()const;
	const bool GetMeshHasShadow()const;

	const bool GetUseCameraTransform()const;
	void SetUseCameraTransform(const bool in_useCameraTransform);

	const bool GetJustUsePosition()const;
	void SetJustUsePosition(const bool in_justUsePosition);

	// else provide public render method? render
	const GMesh* const GetMesh()const; //can return null
	const void* const GetMeshVertexData()const; //can return null

	TPointerMaterialInstance GetMaterial();
	void SetMaterial(TPointerMaterialInstance& inout_material);
	const float GetRadius()const{ return mRadius; }

	TPointerMeshInstance& GetMeshInstance();
	TPointerMeshEdgeInstance& GetMeshEdgeInstance();

	//private members;
private:
	const std::string mData;
	//want a mesh or a manual mesh
	TWeakBuffer mMesh;
	TWeakMeshManual mMeshManual;
	TWeakMaterialInstance mMaterialInstance;
	float mRadius;
	int mFlag;

	//create on demand, for dynamic mesh
	TPointerMeshInstance mMeshInstance;
	//create on demand, for mesh with shadows
	TPointerMeshEdgeInstance mMeshEdgeInstance;

};

#endif 