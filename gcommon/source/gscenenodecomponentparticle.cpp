//
//  GSceneNodeComponentParticle.cpp
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentParticle.h"

#include "GAssert.h"
#include "GScene.h"
#include "GSceneType.h"
#include "GSceneNode.h"
#include "GSceneNodeComponentParticleLoad.h"
#include "GSceneNodeComponentVisual.h"
//#include "GParticleSystemInstance.h"
#include "GStringUtility.h"
#include "GMaterialManager.h"

typedef boost::shared_ptr<GRender> TPointerRenderComponent;
typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;
typedef boost::weak_ptr<GMeshManual> TWeakRenderMeshManual;
//typedef boost::shared_ptr<GParticleManager> TPointerParticleManager;

//static public methods
/*static*/ GSceneNodeComponentParticle::TPointerSceneNodeComponentBase GSceneNodeComponentParticle::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_componentData)
	{
		return pointer;
	}
	const GSceneNodeComponentParticleLoad& componentLoad = *((GSceneNodeComponentParticleLoad*)in_componentData);

	pointer = GSceneNodeComponentParticle::FactoryManual(
		inout_scene,
		inout_owner,
		GStringUtility::SafeString(componentLoad.GetMaterialName()),
		componentLoad.GetRadius(),
		componentLoad.GetMaxParticleCount(),
		componentLoad.GetMaxLineCount(),
		componentLoad.GetMaxEmittorCount(),
		componentLoad.GetDefaultEmittor(),
		componentLoad.GetDefaultEmittorDuration(),
		GStringUtility::SafeString(componentLoad.GetData())
		);

	return pointer;
}

/*static*/ GSceneNodeComponentParticle::TPointerSceneNodeComponentBase GSceneNodeComponentParticle::FactoryManual(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	const std::string& in_materialName,
	const float in_radius,
	const int in_maxParticleCount,
	const int in_maxLineCount,
	const int in_maxEmittorCount,
	const int in_defaultEmittor,
	const float in_defaultEmittorDuration,
	const std::string& in_data
	)
{
	//TPointerParticleManager particleManager = inout_scene.GetParticleManager();
	//TPointerParticleSystemInstance particleSystemInstance;
	//particleSystemInstance.reset(new GParticleSystemInstance(
	//	particleManager,
	//	in_maxParticleCount,
	//	in_maxLineCount,
	//	in_maxEmittorCount
	//	));
	//
	//TPointerRenderComponent pointerRenderComponent = inout_scene.GetRender();
	//TWeakMaterialInstance material = inout_scene.GetRenderMaterialManager()->GetMaterial(in_materialName, pointerRenderComponent);

	//TWeakRenderMeshManual renderMesh = particleSystemInstance->GetMeshManual();
	//TPointerSceneNodeComponentVisual componentVisual;
	//componentVisual.reset(new GSceneNodeComponentVisual(
	//	"particleSystemVisual",
	//	renderMesh,
	//	material,
	//	in_radius,
	//	0 //visible flag not exposed?
	//	));
	//componentVisual->SetVisible(true);
	//componentVisual->SetJustUsePosition(true);
	//TPointerSceneNodeComponentBase componentVisualBase(componentVisual);
	//inout_owner->AddComponent(componentVisualBase);

	TPointerSceneNodeComponentBase pointer;
	//pointer.reset(new GSceneNodeComponentParticle(
	//	in_defaultEmittor,
	//	in_defaultEmittorDuration,
	//	in_data,
	//	componentVisual,
	//	particleSystemInstance
	//	));

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentParticle::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TParticle;
}

//constructor
GSceneNodeComponentParticle::GSceneNodeComponentParticle(
	const int in_defaultEmittor,
	const float in_defaultEmittorDuration,
	const std::string& in_data,
	TPointerSceneNodeComponentVisual& inout_componentVisual,
	TPointerParticleSystemInstance& inout_particleSystemInstance //WARN: transfer ownership
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mDefaultEmittor(in_defaultEmittor)
	, mDefaultEmittorDuration(in_defaultEmittorDuration)
	, mData(in_data)
	, mComponentVisual(inout_componentVisual)
	//, mParticleSystemInstance(inout_particleSystemInstance)
{
	if (-1 != mDefaultEmittor)
	{
		RequestEmittor(mDefaultEmittor, mDefaultEmittorDuration);
	}
	return;
}

/*virtual*/ GSceneNodeComponentParticle::~GSceneNodeComponentParticle()
{
	return;
}

//public methods
void GSceneNodeComponentParticle::Tick(TPointerSceneNode& in_parent, const float in_timeDelta)
{
	//if (!mParticleSystemInstance || !in_parent)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//GSceneNode& sceneNode = *in_parent;

	//particleSystemInstance.Tick(
	//	in_timeDelta,
	//	sceneNode.GetWorldTransform()
	//	);
	return;
}

void GSceneNodeComponentParticle::VisitorAllCamera(GSceneNode& in_parentSceneNode, GCamera& in_camera)
{
	//if (!mParticleSystemInstance)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//TPointerSceneNodeComponentVisual pointerVisual = mComponentVisual.lock();
	//if (!pointerVisual)
	//{
	//	return;
	//}

	//GSceneNodeComponentVisual& visual = *pointerVisual;
	//if (visual.GetVisible() &&
	//	!visual.GetFreeze() &&
	//	!visual.GetOffscreen())
	//{
	//	particleSystemInstance.UpdateMesh(
	//		in_parentSceneNode.GetWorldTransform().GetPosition(),
	//		in_camera
	//		);
	//}

	return;
}

void GSceneNodeComponentParticle::SetVisible(const bool in_visible)
{
	TPointerSceneNodeComponentVisual visual = mComponentVisual.lock();
	if (visual)
	{
		visual->SetVisible(in_visible);
	}
	return;
}

const int GSceneNodeComponentParticle::GetActiveEmittorCount()const
{
	//if (mParticleSystemInstance)
	//{
	//	return mParticleSystemInstance->GetActiveEmittorCount();
	//}
	return 0;
}


void GSceneNodeComponentParticle::RequestParticle(
	const int in_particleIndex, 
	const GVector3Float& in_position,
	const GVector3Float& in_dataOne,
	const GVector3Float& in_dataTwo,
	const float in_lifespan,
	const float in_radiusStart,
	const float in_radiusEnd,
	const float in_alphaStart,
	const float in_alphaEnd
	)
{
	//if (!mParticleSystemInstance)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//particleSystemInstance.RequestParticle(
	//	in_particleIndex,
	//	in_position,
	//	in_dataOne,
	//	in_dataTwo,
	//	in_lifespan,
	//	in_radiusStart,
	//	in_radiusEnd,
	//	in_alphaStart,
	//	in_alphaEnd
	//	);

	return;
}

void GSceneNodeComponentParticle::RequestEmittor(
	const int in_emittorIndex, 
	const float in_duration //emittor needs 'HasDuration' flag to pay attention to duration
	)
{
	//if (!mParticleSystemInstance)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//particleSystemInstance.RequestEmittor(
	//	in_emittorIndex,
	//	in_duration
	//	);

	return;
}

void GSceneNodeComponentParticle::StartDefaultEmittor()
{
	RequestEmittor(mDefaultEmittor, mDefaultEmittorDuration);
	return;
}

void GSceneNodeComponentParticle::ClearAllEmittor()
{
	//if (!mParticleSystemInstance)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//particleSystemInstance.ClearAllEmittor();

	return;
}

void GSceneNodeComponentParticle::ClearAllParticles()
{
	//if (!mParticleSystemInstance)
	//{
	//	return;
	//}
	//GParticleSystemInstance& particleSystemInstance = *mParticleSystemInstance;
	//particleSystemInstance.ClearAllParicle();

	return;
}
