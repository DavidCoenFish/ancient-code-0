//
//  GQuaternion4Float.cpp
//
//  Created by David Coen on 23/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GQuaternion4Float.h"

//static public members
/*static*/ const GQuaternion4Float GQuaternion4Float::sIdentity(1.0F, 0.0F, 0.0F, 0.0F);

//constructors
GQuaternion4Float::GQuaternion4Float(const float in_dataW, const float in_dataX, const float in_dataY, const float in_dataZ)
{
	m_w = in_dataW;
	m_x = in_dataX;
	m_y = in_dataY;
	m_z = in_dataZ;
	return;
}

GQuaternion4Float::GQuaternion4Float(const GQuaternion4Float& in_src)
{
	(*this) = in_src;
	return;
}

GQuaternion4Float::~GQuaternion4Float()
{
	return;
}

//operators
const GQuaternion4Float& GQuaternion4Float::operator=(const GQuaternion4Float& in_rhs)
{
	m_w = in_rhs.m_w;
	m_x = in_rhs.m_x;
	m_y = in_rhs.m_y;
	m_z = in_rhs.m_z;
	return (*this);
}
