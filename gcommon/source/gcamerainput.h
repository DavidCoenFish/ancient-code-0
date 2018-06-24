//
//  gcamerainput.h
//  GCommon
//
//  Created by David Coen on 2011 09 28
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GCameraInput_H_
#define _GCameraInput_H_

#include <boost/noncopyable.hpp>

#include <boost/shared_ptr.hpp>
#include "GVector3Float.h"
#include "GApplicationWindowType.h"


class GCamera;
class GInput;

/*

*/
class GCameraInput : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GCamera> TPointerCamera;

	//constructor
public:
	GCameraInput(
		const GR32 in_defaultCameraPerspectiveNear,
		const GR32 in_defaultCameraPerspectiveFar,
		const GR32 in_defaultCameraBounds,
		const GVector3Float& in_deviceAt,
		const GVector3Float& in_deviceUp,
		const GVector3Float& in_cameraPositionOrigin,
		const GR32 in_cameraDollyDistance
		);
	~GCameraInput();

	// public methods
public:
	void Input(const GInput& in_input);
	void Tick(const GR32 in_timeDelta);
	void SetScreen(
		const GS32 in_screenWidth, 
		const GS32 in_screenHeight, 
		const GApplicationWindowType::TOrientation::TEnum in_screenOrientation
		);
	GCamera& GetCamera();

	void Reset(
		const GVector3Float& in_deviceAt,
		const GVector3Float& in_deviceUp,
		const GVector3Float& in_cameraPositionOrigin,
		const float in_cameraDollyDistance
		);

	// private methods
private:
	void UpdateCamera();

	// private members
private:
	const GR32 m_defaultCameraBounds;

	GVector3Float m_deviceAt;
	GVector3Float m_deviceUp;
	GVector3Float m_cameraPositionOrigin;
	GR32 m_cameraDollyDistance;

	GR32 m_inputPanX;
	GR32 m_inputPanY;
	GR32 m_inputRotX;
	GR32 m_inputRotY;
	GR32 m_inputZoom;

	TPointerCamera m_camera;
};

#endif // _GCameraInput_H_