//
//  GCamera.cpp
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "gcamera.h"

#include "gmatrix16float.h"
#include "gmatrix9float.h"
#include "gvector2float.h"
#include "gvector3float.h"
#include "gassert.h"
#include "gmathutility.h"

GCamera::GCamera(const GCamera& in_src)
	: mOrientation(GApplicationWindowType::TOrientation::TTwelveOclock)
	, mType(TType::TOrthorgraphic)
	, mNear(1.0F)
	, mFar(-1.0F)
	, mTop(1.0F)
	, mBottom(-1.0F)
	, mLeft(-1.0F)
	, mRight(1.0F)
	, mTransformInvert(GMatrix16Float::sIdentity)
	, mTransformAt(GVector3Float::sZero)
	, mTransformUp(GVector3Float::sZero)
	, mTransformPos(GVector3Float::sZero)
	, mFustrum()
	, mProjectionMatrix()
	, mProjectionMatrixInvert()
	, mDirty(true)
	, mDirtyInvert(true)
{
	(*this) = in_src;
	return;
}

GCamera::GCamera(		
	const GApplicationWindowType::TOrientation::TEnum in_orientation,
	const TType::TEnum in_type,
	const GR32 in_near,
	const GR32 in_far,
	const GR32 in_top, //on near plane
	const GR32 in_bottom, //on near plane
	const GR32 in_left, //on near plane
	const GR32 in_right, //on near plane
	const GMatrix16Float& in_transformInvert
	)
	: mOrientation(in_orientation)
	, mType(in_type)
	, mNear(in_near)
	, mFar(in_far)
	, mTop(in_top)
	, mBottom(in_bottom)
	, mLeft(in_left)
	, mRight(in_right)
	, mTransformInvert(in_transformInvert)
	, mTransformAt(GVector3Float::sZero)
	, mTransformUp(GVector3Float::sZero)
	, mTransformPos(GVector3Float::sZero)
	, mFustrum()
	, mProjectionMatrix()
	, mProjectionMatrixInvert()
	, mDirty(true)
	, mDirtyInvert(true)
{
	return;
}

GCamera::~GCamera()
{
	return;
}

GCamera& GCamera::operator=(const GCamera& in_rhs)
{
	if (this != &in_rhs)
	{
		mOrientation = in_rhs.mOrientation;
		mType = in_rhs.mType;
		mNear = in_rhs.mNear;
		mFar = in_rhs.mFar;
		mTop = in_rhs.mTop;
		mBottom = in_rhs.mBottom;
		mLeft = in_rhs.mLeft;
		mRight = in_rhs.mRight;
		mTransformAt = in_rhs.mTransformAt;
		mTransformUp = in_rhs.mTransformUp;
		mTransformPos = in_rhs.mTransformPos;
		mTransformInvert = in_rhs.mTransformInvert;
		mFustrum = in_rhs.mFustrum;
		mProjectionMatrix = in_rhs.mProjectionMatrix;
		mProjectionMatrixInvert = in_rhs.mProjectionMatrixInvert;
		mDirty = in_rhs.mDirty;
		mDirtyInvert = in_rhs.mDirtyInvert;
	}
	return (*this);
}

//public methods
const GVector3Float GCamera::ReverseProjectPoint(
	const GVector2Float& in_screenPoint,
	const GVector3Float& in_worldPointOnScreenPlane //define the plane that the screen point is reverse projected onto
	)const
{
	const GVector2Float clipSpacePoint(
		(in_screenPoint.m_x - 0.5F) * (mRight - mLeft), 
		(0.5F - in_screenPoint.m_y) * (mTop - mBottom)
		);

	const GR32 depth = DotProduct(mTransformAt, in_worldPointOnScreenPlane - mTransformPos);
	const GR32 ratio = depth / mNear;

	const GVector3Float cameraRight = CrossProduct(mTransformAt, mTransformUp);
	const GVector3Float nearPlanePoint = mTransformPos + (mTransformAt * mNear) + (cameraRight * clipSpacePoint.m_x) + (mTransformUp * clipSpacePoint.m_y);

	const GVector3Float result = GMathUtility<GVector3Float>::Interpolate(mTransformPos, nearPlanePoint, ratio);

	return result;
}

void GCamera::ReverseProjectPoint(
	GVector3Float& out_worldSpacePointNearPlane,
	GVector3Float& out_worldSpacePointFarPlane,
	const GVector2Float& in_screenPoint
	)const
{
	const GVector2Float clipSpacePoint(
		(in_screenPoint.m_x - 0.5F) * (mRight - mLeft), 
		(0.5F - in_screenPoint.m_y) * (mTop - mBottom)
		);

	const GVector3Float cameraRight = CrossProduct(mTransformAt, mTransformUp);
	const GVector3Float offset = (mTransformAt * mNear) + (cameraRight * clipSpacePoint.m_x) + (mTransformUp * clipSpacePoint.m_y);
	out_worldSpacePointNearPlane = mTransformPos + offset;
	out_worldSpacePointFarPlane = mTransformPos + (offset * (mFar / mNear));

	return;
}


//public accessors
void GCamera::SetOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation)
{
	if (mOrientation == in_orientation)
	{
		return;
	}
	mOrientation = in_orientation;
	mDirty = true;
	mDirtyInvert = true;
	return;
}


void GCamera::SetNear(const GR32 in_near)
{
	if (mNear == in_near)
	{
		return;
	}

	mNear = in_near;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetFar(const GR32 in_far)
{
	if (mFar == in_far)
	{
		return;
	}
	mFar = in_far;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetTop(const GR32 in_top)
{
	if (mTop == in_top)
	{
		return;
	}
	mTop = in_top;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetBottom(const GR32 in_bottom)
{
	if (mBottom == in_bottom)
	{
		return;
	}
	mBottom = in_bottom;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetLeft(const GR32 in_left)
{
	if (mLeft == in_left)
	{
		return;
	}
	mLeft = in_left;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetRight(const GR32 in_right)
{
	if (mRight == in_right)
	{
		return;
	}
	mRight = in_right;
	mDirty = true;
	mDirtyInvert = true;
	return;
}

//GMatrix16Float& GCamera::GetTransform()
//{
//	mDirty = true;
//	return mTransform;
//}
void GCamera::SetTransform(
	const GVector3Float& in_at, 
	const GVector3Float& in_up,
	const GVector3Float& in_pos
	)
{
	//fun, a camera concept of 'at' is actually to the right of the sceen
	mTransformAt = in_at;
	mTransformUp = in_up;
	mTransformPos = in_pos;
	const GVector3Float cross = CrossProduct(in_at, in_up);
	mTransformInvert = GMatrix16FloatConstructAtUp(
		cross,
		in_up,
		GVector3Float::sUnitX,
		GVector3Float::sUnitY
		);

	//cheap invert
	mTransformInvert.TransposeSelf();
	mTransformInvert.SetPosition(-(in_pos * mTransformInvert));

	mDirty = true;
	mDirtyInvert = true;
	return;
}

void GCamera::SetTransformInvert(
	const GMatrix16Float& in_transformInvert
	)
{
	mTransformInvert = in_transformInvert;
	mDirty = true;
	const GR32* const pData = mTransformInvert.GetData();

	mTransformAt.SetData(-pData[2],-pData[6],-pData[10]);
	mTransformUp.SetData(pData[1],pData[5],pData[9]);

	GMatrix9Float rotation = in_transformInvert.GetRotation();
	rotation.TransposeSelf();
	const GVector3Float pos = in_transformInvert.GetPosition();
	mTransformPos = -(pos * rotation);

	return;
}


const GMatrix16Float& GCamera::GetProjectionMatrix()
{
	if (mDirty)
	{
		mDirty = false;
		switch (mType)
		{
		default:
			{
				GASSERT(0, "unknown case");
				break;
			}
		case TType::TOrthorgraphic:
			{
				mFustrum.SetData(	(2.0F / (mRight - mLeft)),	0.0F,						0.0F,						-((mRight + mLeft) / (mRight - mLeft)),
									0.0F,						(2.0F / (mTop - mBottom)),	0.0F,						-((mTop + mBottom) / (mTop - mBottom)),
									0.0F,						0.0F,						-(2.0F / (mFar - mNear)),	-((mFar + mNear) / (mFar - mNear)),
									0.0F,						0.0F,						0.0F,						1.0F
									);
				break;
			}
		case TType::TPerspective:
			{
				mFustrum.SetData(	((2.0F * mNear) / (mRight - mLeft)),	0.0F,									((mRight + mLeft) / (mRight - mLeft)),		0.0F,
									0.0F,									((2.0F * mNear) / (mTop - mBottom)),	((mTop + mBottom) / (mTop - mBottom)),		0.0F,
									0.0F,									0.0F,									-((mFar + mNear) / (mFar - mNear)),			-((2.0F * mFar * mNear) / (mFar - mNear)),
									0.0F,									0.0F,									-1.0F,										0.0F
									);

				break;
			}
		}

		/*
		Rx(d) = [	1,		0,	  0]
				[0, cos(d), -sin(d)]
				[0, sin(d),  cos(d)]

		Ry(d) = [cos(d), 0,  sin(d)]
				[	0,		1,	  0]
				[-sin(d), 0, cos(d)]

		Rz(d) = [cos(d), -sin(d), 0]
				[sin(d),  cos(d), 0]
				[	0,		0,	  1]
		*/

		switch (mOrientation)
		{
		default:
			break;
		case GApplicationWindowType::TOrientation::TThreeOclock:
			mFustrum = GMatrix16Float(
				0.0F,-1.0F, 0.0F, 0.0F,
				1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F
				) * mFustrum;
			break;
		case GApplicationWindowType::TOrientation::TSixOclock:
			mFustrum = GMatrix16Float(
			   -1.0F, 0.0F, 0.0F, 0.0F,
				0.0F,-1.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F
				) * mFustrum;
			break;
		case GApplicationWindowType::TOrientation::TNineOclock:
			mFustrum = GMatrix16Float(
				0.0F, 1.0F, 0.0F, 0.0F,
			   -1.0F, 0.0F, 0.0F, 0.0F,
				0.0F, 0.0F, 1.0F, 0.0F,
				0.0F, 0.0F, 0.0F, 1.0F
				) * mFustrum;
			break;
		}

		//mFustrum.Transpose();
		//mProjectionMatrix = (mTransform.ReturnInverse()) * mFustrum;
		mProjectionMatrix = mTransformInvert * mFustrum;
	}

	return mProjectionMatrix;
}

const GMatrix16Float& GCamera::GetProjectionMatrixInvert()
{
	if (mDirtyInvert)
	{
		mDirtyInvert = false;
		mProjectionMatrixInvert = GetProjectionMatrix().ReturnInverse();
	}

	return mProjectionMatrixInvert;
}
	