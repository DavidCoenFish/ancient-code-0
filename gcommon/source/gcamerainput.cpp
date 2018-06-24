//
//  gcamerainput.cpp
//  GCommon
//
//  Created by David Coen on 2011 09 28
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#include "GCameraInput.h"

#include "GCamera.h"
#include "GInput.h"
#include "GInputJoystick.h"
#include "GMatrix9Float.h"
#include "GVector2Float.h"
#include "GInputTouch.h"
#include "GMath.h"

static const GR32 s_inputFactorPan = 500.0F;
static const GR32 s_inputFactorRot = 100.0F;
static const GR32 s_inputFactorZoom = 500.0F;

static const float LocalGetAngle(
	const GVector2Float& in_pointOne,
	const GVector2Float& in_pointTwo
	)
{
	const float rize = in_pointOne.m_y - in_pointTwo.m_y;
	const float run = in_pointOne.m_x - in_pointTwo.m_x;
	const float value = GMath::ATan(rize, run);
	return value;
}

//constructor
GCameraInput::GCameraInput(
	const GR32 in_defaultCameraPerspectiveNear,
	const GR32 in_defaultCameraPerspectiveFar,
	const GR32 in_defaultCameraBounds,
	const GVector3Float& in_deviceAt,
	const GVector3Float& in_deviceUp,
	const GVector3Float& in_cameraPositionOrigin,
	const GR32 in_cameraDollyDistance
	)
	: m_defaultCameraBounds(in_defaultCameraBounds)
	, m_deviceAt(in_deviceAt)
	, m_deviceUp(in_deviceUp)
	, m_cameraPositionOrigin(in_cameraPositionOrigin)
	, m_cameraDollyDistance(in_cameraDollyDistance)
	, m_inputPanX(0.0F)
	, m_inputPanY(0.0F)
	, m_inputRotX(0.0F)
	, m_inputRotY(0.0F)
	, m_inputZoom(0.0F)
	, m_camera()
{
	m_camera.reset(new GCamera(
		GApplicationWindowType::TOrientation::TTwelveOclock, 
		GCamera::TType::TPerspective, 
		in_defaultCameraPerspectiveNear, 
		in_defaultCameraPerspectiveFar
		));

	if (m_camera)
	{
		GCamera& camera = *m_camera;
		camera.SetLeft(-m_defaultCameraBounds);
		camera.SetRight(m_defaultCameraBounds);
		camera.SetTop(m_defaultCameraBounds);
		camera.SetBottom(-m_defaultCameraBounds);
	}

	UpdateCamera();

	return;
}
GCameraInput::~GCameraInput()
{
	return;
}

// public methods
void GCameraInput::Input(const GInput& in_input)
{
	m_inputPanX = 0.0F;
	m_inputPanY = 0.0F;
	m_inputRotX = 0.0F;
	m_inputRotY = 0.0F;
	m_inputZoom = 0.0F;

	const int touchCount = in_input.GetTouchCount();
	if (touchCount == 1)
	{
		const GInputTouch& touch = in_input.GetTouch(0);
        m_inputRotX += touch.GetTouchDelta().m_x;
        m_inputRotY += touch.GetTouchDelta().m_y;
	}
	else if (1 < touchCount)
	{
		for (int index = 0; index < touchCount; ++index)
		{
			const GInputTouch& touch = in_input.GetTouch(index);
            m_inputPanX = touch.GetTouchDelta().m_x;
            m_inputPanY = touch.GetTouchDelta().m_y;
		}

		const GInputTouch& touchBase = in_input.GetTouch(0);
		for (int index = 1; index < touchCount; ++index)
		{
			const GInputTouch& touchSubject = in_input.GetTouch(index);
			const float oldLength = Length((touchBase.GetTouchLocation() - touchBase.GetTouchDelta()) -
				(touchSubject.GetTouchLocation() - touchSubject.GetTouchDelta()));
			const float newLength = Length(touchBase.GetTouchLocation() - touchSubject.GetTouchLocation());
			m_inputZoom += (newLength - oldLength);
		}
	}
}

void GCameraInput::Tick(const GR32 in_timeDelta)
{
	GVector3Float deviceCross = CrossProduct(m_deviceUp, m_deviceAt);

	bool orthogonalise = false;
	if (0.0F != m_inputRotX)
	{
		orthogonalise = true;
		//rotate device at around device up
		GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(
			m_deviceUp,
			m_inputRotX * in_timeDelta * s_inputFactorRot
			);

		m_deviceAt = m_deviceAt * rotationMatrix;
	}

	if (0.0F != m_inputRotY)
	{
		orthogonalise = true;
		//rotate device up around device cross
		GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(
			deviceCross,
			-(m_inputRotY * in_timeDelta * s_inputFactorRot)
			);

		m_deviceUp = m_deviceUp * rotationMatrix;
		m_deviceAt = m_deviceAt * rotationMatrix;
	}

	if (orthogonalise)
	{
		Orthogonalise(m_deviceAt, m_deviceUp);
	}

	m_cameraPositionOrigin += (deviceCross * (m_inputPanX * in_timeDelta * s_inputFactorPan));
	m_cameraPositionOrigin += (m_deviceUp * (m_inputPanY * in_timeDelta * s_inputFactorPan));
	m_cameraDollyDistance += (in_timeDelta * m_inputZoom * s_inputFactorZoom);

	UpdateCamera();
}

void GCameraInput::SetScreen(
	const GS32 in_screenWidth, 
	const GS32 in_screenHeight, 
	const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
	)
{
	GCamera& camera = *m_camera;

	const GR32 aspect = ((GR32)in_screenWidth) / ((GR32)in_screenHeight);
	if (1.0F < aspect)
	{
		camera.SetLeft(-aspect * m_defaultCameraBounds);
		camera.SetRight(aspect * m_defaultCameraBounds);
		camera.SetTop(m_defaultCameraBounds);
		camera.SetBottom(-m_defaultCameraBounds);
	}
	else
	{
		camera.SetLeft(-m_defaultCameraBounds);
		camera.SetRight(m_defaultCameraBounds);
		camera.SetTop(m_defaultCameraBounds / aspect);
		camera.SetBottom((-m_defaultCameraBounds) / aspect);
	}

	camera.SetOrientation(in_screenOrientation);

	return;
}


GCamera& GCameraInput::GetCamera()
{
	return (*m_camera);
}

void GCameraInput::Reset(
	const GVector3Float& in_deviceAt,
	const GVector3Float& in_deviceUp,
	const GVector3Float& in_cameraPositionOrigin,
	const float in_cameraDollyDistance
	)
{
	m_deviceAt = in_deviceAt;
	m_deviceUp = in_deviceUp;
	m_cameraPositionOrigin = in_cameraPositionOrigin;
	m_cameraDollyDistance = in_cameraDollyDistance;

	UpdateCamera();

	return;
}


// private methods
void GCameraInput::UpdateCamera()
{
	GMatrix16Float deviceMatrix = GMatrix16FloatConstructAtUp(
		-GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		m_deviceAt,
		m_deviceUp
		);

	//const GVector3Float pos = mDeviceAt * (5.0F);
	const GVector3Float cameraPosition =  GMatrix16FloatMultiplyNoTranslate(
		-(m_cameraPositionOrigin + (m_deviceAt * m_cameraDollyDistance)), 
		deviceMatrix
		);
	deviceMatrix.TransposeSelf(); //:= inverse if orthographic and no translation
	deviceMatrix.SetPosition(cameraPosition);

	m_camera->SetTransformInvert(deviceMatrix);

	return;
}
