//
//  applicationtask.h
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _applicationtasktestparticle_H_
#define _applicationtasktestparticle_H_

#include <gcommon.h>
#include <gapplicationtask.h>
#include <GApplicationWindowType.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GApplicationWindow;
class GRender;
class GResourceManager;
class GFileSystem;
class GView;
class GScratchPad;
class GGuiManager;
class GMissionMaster;
class GInput;
class GMaterialInstance;
class GMeshManual;
class GCamera;
class GCameraInput;
class GScene;
class StateBase;
class ZombieMissionClient;
class GSceneNodeComponentParticle;
class GParticleManager;
class GSceneNode;

//interface class for application tasks
class ApplicationTaskTestParticle : public GApplicationTask
{
	///////////////////////////////////////////////////////
	//constructor
public:
	ApplicationTaskTestParticle();
	virtual ~ApplicationTaskTestParticle();

	//////////////////////////////////////////////////////
	//implement GApplicationTask
private:
	virtual const GBOOL OnTick(const GR32 in_timeDelta);
	virtual GVOID OnSetActive(const GBOOL in_active);

	///////////////////////////////////////////////////////
	//private methods
private:
	GVOID Input(const GInput& in_input);
	GVOID Tick(const GR32 in_timeDelta);
	GVOID Draw(GRender& inout_render);
	GVOID SetScreen(
		const GS32 in_screenWidth, 
		const GS32 in_screenHeight, 
		const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
		);

	///////////////////////////////////////////////////////
	//private members
private:
	boost::scoped_ptr<GApplicationWindow> mApplicationWindow;
	boost::scoped_ptr<GScratchPad> mScratchPad;

	boost::shared_ptr<GView> mViewRender;
	boost::shared_ptr<GRender> mRender;
	boost::shared_ptr<GFileSystem> mFileSystem;
	boost::shared_ptr<GResourceManager> mResourceManager;
	boost::shared_ptr<GParticleManager> mParticleManager;

	boost::weak_ptr<GMaterialInstance> m_material;
	boost::shared_ptr<GMeshManual> m_mesh;
	boost::shared_ptr<GCameraInput> m_cameraInput;

	boost::shared_ptr< GScene > m_scene;
	boost::shared_ptr< GSceneNode > m_sceneNodeParticle;
	boost::shared_ptr< GSceneNodeComponentParticle > m_sceneNodeComponentParticle;
};

#endif //#ifndef _applicationtask_H_
