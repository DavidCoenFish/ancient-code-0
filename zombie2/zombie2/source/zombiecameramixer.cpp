//
//  ZombieCameraMixer.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieCameraMixer.h"
#include "ZombieCameraCinematic.h"
#include "ZombieCameraDebug.h"
#include "ZombieCameraDeath.h"
#include "ZombieCameraGame.h"
#include "applicationtaskgame.h"

#include <GCamera.h>
#include <GInput.h>
#include <GInputJoystick.h>

#include <GVector3Float.h>
#include <GStringUtility.h>

//#define PRINT_CAMERA_VAR
//#define FORCE_DEBUG_CAMERA

static const float sZombieCameraPerspectiveNear = 0.1F;
static const float sZombieCameraPerspectiveAspect = 0.062F; //0.062F; //45deg FOV for 320x480 //sZombieCameraPerspectiveNear * 0.75F;
static const float sZombieCameraPerspectiveFar = 1000.0F;
static const float sSmooth = 0.9F;

struct TCameraType
{
	enum TEnum
	{
		TCinematic = 0,
		TGame,
		TDebug,
		TCinematicNoRoll,
		TDeath,
		TIntro,

		TCount
	};
};

//static public methods
/*static*/ const int ZombieCameraMixer::GetCameraTypeCinematicNoRoll()
{
	return TCameraType::TCinematicNoRoll;
}

/*static*/ const int ZombieCameraMixer::GetCameraTypeCinematic()
{
	return TCameraType::TCinematic;
}

/*static*/ const int ZombieCameraMixer::GetCameraTypeGame()
{
	return TCameraType::TGame;
}

/*static*/ const int ZombieCameraMixer::GetCameraTypeDebug()
{
	return TCameraType::TDebug;
}

/*static*/ const int ZombieCameraMixer::GetCameraTypeDeath()
{
	return TCameraType::TDeath;
}

/*static*/ const int ZombieCameraMixer::GetCameraTypeIntro()
{
	return TCameraType::TIntro;
}

/*static*/ const float ZombieCameraMixer::GetPerspectiveRatio()
{
	return sZombieCameraPerspectiveAspect / sZombieCameraPerspectiveNear;
}

//constructor
ZombieCameraMixer::ZombieCameraMixer()
: mCamera()
, mJoystickDirection(-GVector3Float::sUnitY)
, mComponentLerp()
, mCameraTypeTarget(TCameraType::TCount)
, mCameraTypeOld(TCameraType::TCount)
, mZombieCameraCinematic()
, mZombieCameraGame()
, mZombieCameraDebug()
, mDeathLockCameraChange(false)
{
	mCamera.reset(new GCamera(
		GApplicationWindowType::TOrientation::TTwelveOclock,
		GCamera::TType::TPerspective,
		sZombieCameraPerspectiveNear,
		sZombieCameraPerspectiveFar,
		sZombieCameraPerspectiveAspect,
		-sZombieCameraPerspectiveAspect,
		-sZombieCameraPerspectiveAspect,
		sZombieCameraPerspectiveAspect
		));

	mComponentLerp.reset(new GComponentLerp);
	mZombieCameraCinematic.reset(new ZombieCameraCinematic);
	mZombieCameraGame.reset(new ZombieCameraGame);
	mZombieCameraDebug.reset(new ZombieCameraDebug);
	mZombieCameraDeath.reset(new ZombieCameraDeath);

	return;
}

ZombieCameraMixer::~ZombieCameraMixer()
{
	return;
}

//public methods
void ZombieCameraMixer::ResizeWindow(const int in_width, const int in_height)
{
	GCamera& camera = GetCamera();

	const float aspect = ((float)in_width) / ((float)in_height);
	camera.SetTop(sZombieCameraPerspectiveAspect);
	camera.SetBottom(-sZombieCameraPerspectiveAspect);
	camera.SetLeft(-sZombieCameraPerspectiveAspect * aspect);
	camera.SetRight(sZombieCameraPerspectiveAspect * aspect);

	return;
}

void ZombieCameraMixer::Tick(const float in_timeDelta)
{
	mComponentLerp->Tick(in_timeDelta);
	mZombieCameraCinematic->Tick(in_timeDelta);
	mZombieCameraGame->Tick(in_timeDelta);
	mZombieCameraDeath->Tick(in_timeDelta);

	//update camera
	GVector3Float position;
	GVector3Float at;
	GVector3Float up;
	GetData(position, at, up);

#ifdef PRINT_CAMERA_VAR
{
	std::string text("at: ");
	text += GStringUtility::Print(at.m_x);
	text += " ";
	text += GStringUtility::Print(at.m_y);
	text += " ";
	text += GStringUtility::Print(at.m_z);
	text += "\nup: ";
	text += GStringUtility::Print(up.m_x);
	text += " ";
	text += GStringUtility::Print(up.m_y);
	text += " ";
	text += GStringUtility::Print(up.m_z);
	text += "\npos: ";
	text += GStringUtility::Print(position.m_x);
	text += " ";
	text += GStringUtility::Print(position.m_y);
	text += " ";
	text += GStringUtility::Print(position.m_z);

	DEBUG_PUT_TEXT(text);
}
#endif

	mCamera->SetTransform(
		at,
		up,
		position
		);
	
	return;
}

void ZombieCameraMixer::Input(const GInput& in_input)
{
	if (1 <= in_input.GetJoystickCount())
	{
		const GVector3Float& joystick = in_input.GetJoystick(0).GetDirection();
		mJoystickDirection = Normalise((mJoystickDirection * sSmooth) + (joystick * (1.0F - sSmooth)));
	}

#ifdef FORCE_DEBUG_CAMERA
	mZombieCameraDebug->Input(in_input, mJoystickDirection);
#else
	switch (mCameraTypeTarget)
	{
	default:
		break;
	case TCameraType::TCinematic:
	case TCameraType::TCinematicNoRoll:
		mZombieCameraCinematic->Input(in_input, mJoystickDirection);
		break;
	case TCameraType::TGame:
		mZombieCameraGame->Input(in_input, mJoystickDirection);
		break;
	case TCameraType::TDebug:
		mZombieCameraDebug->Input(in_input, mJoystickDirection);
		break;
	case TCameraType::TDeath:
		mZombieCameraDeath->Input(in_input, mJoystickDirection);
		break;
	}
#endif

	return;
}

void ZombieCameraMixer::SetPosition(
	const int in_cameraType,
	const GVector3Float& in_position, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
#ifdef FORCE_DEBUG_CAMERA
	mZombieCameraDebug->SetPosition(in_position);
#else
	switch (in_cameraType)
	{
	default:
		break;
	case TCameraType::TCinematic:
	case TCameraType::TCinematicNoRoll:
		mZombieCameraCinematic->SetPosition(in_position, in_lerpDuration, in_lerpType);
		break;
	case TCameraType::TGame:
		mZombieCameraGame->SetPosition(in_position, in_lerpDuration, in_lerpType);
		break;
	case TCameraType::TDebug:
		mZombieCameraDebug->SetPosition(in_position);
		break;
	}
#endif
	return;
}

void ZombieCameraMixer::SetRotation(
	const int in_cameraType,
	const GVector3Float& in_at, 
	const GVector3Float& in_up, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
#ifdef FORCE_DEBUG_CAMERA
	mZombieCameraDebug->SetRotation(in_at, in_up);
#else
	switch (in_cameraType)
	{
	default:
		break;
	case TCameraType::TCinematic:
	case TCameraType::TCinematicNoRoll:
		mZombieCameraCinematic->SetRotation(in_at, in_up, in_lerpDuration, in_lerpType);
		break;
	case TCameraType::TGame:
		mZombieCameraGame->SetRotation(in_at, in_up, in_lerpDuration, in_lerpType);
		break;
	case TCameraType::TDebug:
		mZombieCameraDebug->SetRotation(in_at, in_up);
		break;
	}
#endif

	return;
}

void ZombieCameraMixer::SetCameraType(
	const int in_type,
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	if (mDeathLockCameraChange)
	{
		return;
	}

	if (TCameraType::TDeath == in_type)
	{
		mDeathLockCameraChange = true;
		//update camera
		GVector3Float position;
		GVector3Float at;
		GVector3Float up;
		GetData(position, at, up);
		mZombieCameraDeath->Activate(position, at, up);
		mComponentLerp->SetLerp(0); 
		mCameraTypeTarget = TCameraType::TDeath;
		return;
	}

	mComponentLerp->SetLerp(in_lerpDuration, in_lerpType); 
	if (in_lerpDuration)
	{
		mCameraTypeOld = mCameraTypeTarget;
	}
	mCameraTypeTarget = in_type;

	return;
}

//public accessors
GCamera& ZombieCameraMixer::GetCamera()
{
	return (*mCamera);
}

//private accessors
void ZombieCameraMixer::GetCameraData(
	GVector3Float& out_position,
	GVector3Float& out_at,
	GVector3Float& out_up,
	const int in_type
	)
{
#ifdef FORCE_DEBUG_CAMERA
	mZombieCameraDebug->GetData(out_position, out_at, out_up);
#else
	switch (in_type)
	{
	default:
		break;
	case TCameraType::TCinematic:
		mZombieCameraCinematic->GetData(out_position, out_at, out_up, true);
		break;
	case TCameraType::TCinematicNoRoll:
		mZombieCameraCinematic->GetData(out_position, out_at, out_up, false);
		break;
	case TCameraType::TGame:
		mZombieCameraGame->GetData(out_position, out_at, out_up);
		break;
	case TCameraType::TDebug:
		mZombieCameraDebug->GetData(out_position, out_at, out_up);
		break;
	case TCameraType::TDeath:
		mZombieCameraDeath->GetData(out_position, out_at, out_up);
		break;
	}
#endif

	return;
}

void ZombieCameraMixer::GetData(
	GVector3Float& out_position, 
	GVector3Float& out_at, 
	GVector3Float& out_up
	)
{
	GetCameraData(
		out_position,
		out_at,
		out_up,
		mCameraTypeTarget
		);

	if (mComponentLerp->GetActive() && (TCameraType::TCount != mCameraTypeOld))
	{
		GVector3Float positionFrom;
		GVector3Float atFrom;
		GVector3Float upFrom;
		GetCameraData(
			positionFrom,
			atFrom,
			upFrom,
			mCameraTypeOld
			);

		const float ratio = mComponentLerp->GetRatio();
		out_position = Lerp(ratio, positionFrom, out_position);
		out_at = Lerp(ratio, atFrom, out_at);
		out_up = Lerp(ratio, upFrom, out_up);
	}

	Orthogonalise(out_at, out_up);

	return;
}