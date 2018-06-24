//
//  ZombieCameraMixer.h
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieCameraMixer_h_
#define _ZombieCameraMixer_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <GComponentLerp.h>
#include <GVector3Float.h>

class GCamera;
class GComponentCameraInput;
class GInput;
class GVector3Float;
class GVector2Float;
class ZombieCameraCinematic;
class ZombieCameraDebug;
class ZombieCameraGame;
class ZombieCameraDeath;

class ZombieCameraMixer : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GCamera> TPointerCamera;
	typedef boost::shared_ptr<GComponentLerp> TPointerComponentLerp;
	typedef boost::shared_ptr<ZombieCameraCinematic> TPointerZombieCameraCinematic;
	typedef boost::shared_ptr<ZombieCameraDebug> TPointerZombieCameraDebug;
	typedef boost::shared_ptr<ZombieCameraGame> TPointerZombieCameraGame;
	typedef boost::shared_ptr<ZombieCameraDeath> TPointerZombieCameraDeath;

	//static public methods
public:
	static const int GetCameraTypeCinematicNoRoll(); //CinematicMainMenu?
	static const int GetCameraTypeCinematic();
	static const int GetCameraTypeGame();
	static const int GetCameraTypeDeath();
	static const int GetCameraTypeDebug();
	static const int GetCameraTypeIntro();
	static const float GetPerspectiveRatio();

	//constructor
public:
	ZombieCameraMixer();
	~ZombieCameraMixer();

	//public methods
public:
	void ResizeWindow(const int in_width, const int in_height);
	void Tick(const float in_timeDelta);
	void Input(const GInput& in_input);

	void SetPosition(
		const int in_cameraType,
		const GVector3Float& in_position, 
		const float in_lerpDuration = 0.0F,
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);

	void SetRotation(
		const int in_cameraType,
		const GVector3Float& in_at, 
		const GVector3Float& in_up, 
		const float in_lerpDuration = 0.0F,
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);

	void SetCameraType(
		const int in_type,
		const float in_lerpDuration = 0.0F,
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);

	//the death camera ignores transitions till this is hit
	void UnlockDeathCamera(){ mDeathLockCameraChange = false; }

	//public accessors
public:
	GCamera& GetCamera();
	const int GetCameraTypeTarget()const{ return mCameraTypeTarget; }

	//private accessors
private:
	void GetCameraData(
		GVector3Float& out_position,
		GVector3Float& out_at,
		GVector3Float& out_up,
		const int in_type
		);

	void GetData(
		GVector3Float& out_position, 
		GVector3Float& out_at, 
		GVector3Float& out_up
		);

	//private members
private:
	TPointerCamera mCamera;
	GVector3Float mJoystickDirection; //accelerometor average

	TPointerComponentLerp mComponentLerp;
	int mCameraTypeTarget;
	int mCameraTypeOld;

	TPointerZombieCameraCinematic mZombieCameraCinematic;
	TPointerZombieCameraGame mZombieCameraGame;
	TPointerZombieCameraDebug mZombieCameraDebug;
	TPointerZombieCameraDeath mZombieCameraDeath;

	//this may be overkill, since death is ment to stop sequences, but want to be sure that we don't get yoinked out of death camera
	bool mDeathLockCameraChange;

};

#endif //_ZombieCameraMixer_h_
