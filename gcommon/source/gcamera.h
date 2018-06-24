//
//  gcamera.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GCamera_h_
#define _GCamera_h_

#include "gcommon.h"
#include "gmatrix16float.h"
#include "gvector3float.h"
#include "gapplicationwindowtype.h"

class GVector2Float;

class GCamera //copyable for grender
{
	////////////////////////////////////////////////////////////////////////
	//typedef
public:
	struct TType
	{
		enum TEnum
		{
			TOrthorgraphic = 0,
			TPerspective,
			TCount
		};
	};
	
	////////////////////////////////////////////////////////////////////////
	//constructor
public:
	GCamera(const GCamera& in_src);
	GCamera(
		const GApplicationWindowType::TOrientation::TEnum in_orientation = GApplicationWindowType::TOrientation::TTwelveOclock,
		const TType::TEnum in_type = TType::TOrthorgraphic,
		const GR32 in_near = 1.0F,
		const GR32 in_far = -1.0F,
		const GR32 in_top = 1.0F, //on near plane
		const GR32 in_bottom = -1.0F, //on near plane
		const GR32 in_left = -1.0F, //on near plane
		const GR32 in_right = 1.0F, //on near plane
		const GMatrix16Float& in_transform = GMatrix16Float::sIdentity
		);
	~GCamera();

	GCamera& operator=(const GCamera& in_rhs);

	////////////////////////////////////////////////////////////////////////
	//public methods
public:
	const GVector3Float ReverseProjectPoint(
		const GVector2Float& in_screenPoint,
		const GVector3Float& in_worldPointOnScreenPlane //define the plane that the screen point is reverse projected onto
		)const;
	void ReverseProjectPoint(
		GVector3Float& out_worldSpacePointNearPlane,
		GVector3Float& out_worldSpacePointFarPlane,
		const GVector2Float& in_screenPoint
		)const;

	////////////////////////////////////////////////////////////////////////
	//public accessors
public:
	const GApplicationWindowType::TOrientation::TEnum GetOrientation(){ return mOrientation; }
	void SetOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation);

	const GR32 GetNear()const{ return mNear; }
	void SetNear(const GR32 in_value);

	const GR32 GetFar()const{ return mFar; }
	void SetFar(const GR32 in_far);

	const GR32 GetTop()const{ return mTop; }
	void SetTop(const GR32 in_top);

	const GR32 GetBottom()const{ return mBottom; }
	void SetBottom(const GR32 in_bottom);

	const GR32 GetLeft()const{ return mLeft; }
	void SetLeft(const GR32 in_left);

	const GR32 GetRight()const{ return mRight; }
	void SetRight(const GR32 in_right);
	
	const GMatrix16Float& GetTransformInvert()const{ return mTransformInvert; }
	void SetTransformInvert(
		const GMatrix16Float& in_transformInvert
		);
	void SetTransform(
		const GVector3Float& in_at, 
		const GVector3Float& in_up,
		const GVector3Float& in_pos
		);
	//GMatrix16Float& GetTransform(); //sets dirty flag
	
	const GMatrix16Float& GetProjectionMatrix(); //not const, may need to calculate
	const GMatrix16Float& GetProjectionMatrixInvert(); //not const, may need to calculate
	
	const GVector3Float& GetTransformAt()const{ return mTransformAt; }
	const GVector3Float& GetTransformUp()const{ return mTransformUp; }
	const GVector3Float& GetTransformPos()const{ return mTransformPos; }

	const GR32 GetAspect()const{ return (mRight - mLeft)  / (mTop - mBottom); }

	////////////////////////////////////////////////////////////////////////
	//private members
private:
	GApplicationWindowType::TOrientation::TEnum mOrientation;
	TType::TEnum mType;
	GR32 mNear;
	GR32 mFar;
	GR32 mTop; //on near plane
	GR32 mBottom; //on near plane
	GR32 mLeft; //on near plane
	GR32 mRight; //on near plane
	GVector3Float mTransformAt;
	GVector3Float mTransformUp;
	GVector3Float mTransformPos;
	GMatrix16Float mTransformInvert; //where the camera eye is

	GMatrix16Float mFustrum;
	GMatrix16Float mProjectionMatrix;  //fustrum x invert.transform
	GMatrix16Float mProjectionMatrixInvert; 
	GBOOL mDirty; //re. mProjectionMatrix
	GBOOL mDirtyInvert; //re. mProjectionMatrixInvert
	
};

#endif 