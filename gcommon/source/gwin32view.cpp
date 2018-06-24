//
//  gwin32view.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gwin32view.h"

///////////////////////////////////////////////////////
//constructor
GWin32View::GWin32View()
: m_width(0)
, m_height(0)
{
	return;
}

/*virtual*/ GWin32View::~GWin32View()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
void GWin32View::Resize(const GU32 in_width, const GU32 in_height)
{
	m_width = in_width;
	m_height = in_height;

	OnResize(in_width, in_height);
	return;
}

void GWin32View::Paint()
{
	OnPaint();
	return;
}

///////////////////////////////////////////////////////
//interface
/*virtual*/ void GWin32View::OnResize(const GU32, const GU32)
{
	return;
}

/*virtual*/ void GWin32View::OnPaint()
{
	return;
}

