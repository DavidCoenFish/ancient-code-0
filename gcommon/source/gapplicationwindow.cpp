//
//  gapplicationwindow.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gapplicationwindow.h"

#include "gview.h"
#include "ginput.h"

#ifdef IPHONE
	#include "giphoneapplicationwindow.h"
#elif WIN32
	#include "gwin32applicationwindow.h"
#endif

///////////////////////////////////////////////////////
//constructor
GApplicationWindow::GApplicationWindow()
	: mApplicationWindowImplementation()
{
	mApplicationWindowImplementation.reset(new GApplicationWindowImplementation);
}

GApplicationWindow::~GApplicationWindow()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
GApplicationWindow::TPointerView GApplicationWindow::CreateViewRender(
	TPointerRender& out_render,
	const GBOOL in_transparentBackground,
	const GBOOL in_depth,
	const GBOOL in_stencil,
    const GR32 in_scale
	)
{
	TPointerView pointerView;
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->CreateViewRender(
			pointerView,
			out_render,
			in_transparentBackground,
			in_depth,
			in_stencil,
            in_scale
			);
	}
	return pointerView;
}

//TPointerView CreateViewCamera()

void GApplicationWindow::PushViewFront(TPointerView& inout_view)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->PushViewFront(inout_view);
	}
	return;
}

void GApplicationWindow::PushViewInfront(TPointerView& inout_view, TPointerView& inout_where)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->PushViewInfront(inout_view, inout_where);
	}
	return;
}

void GApplicationWindow::RemoveView(TPointerView& inout_view)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->RemoveView(inout_view);
	}
	return;
}

GVOID GApplicationWindow::PreTick(const GR32 in_timeDelta)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->PreTick(in_timeDelta);
	}
	return;
}

const GBOOL GApplicationWindow::Tick(const GR32 in_timeDelta)
{
	GBOOL result = GFALSE;
	if (GNULL != mApplicationWindowImplementation)
	{
		result = mApplicationWindowImplementation->Tick(in_timeDelta);
	}
	return result;
}

GVOID GApplicationWindow::PostTick(const GR32 in_timeDelta)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->PostTick(in_timeDelta);
	}
	return;
}

void GApplicationWindow::PutText(const std::string& in_text)
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->PutText(in_text);
	}
	return;
}

void GApplicationWindow::Show()
{
	if (GNULL != mApplicationWindowImplementation)
	{
		mApplicationWindowImplementation->Show();
	}
	return;
}

const GInput& GApplicationWindow::GetInput()const
{
	if (GNULL != mApplicationWindowImplementation)
	{
		return mApplicationWindowImplementation->GetInput();
	}
	static GInput sInput;
	return sInput;
}

void GApplicationWindow::GetScreenDimention(
	GApplicationWindowType::TOrientation::TEnum& out_orientation,
	int& out_width, 
	int& out_height
	)const
{
	if (GNULL != mApplicationWindowImplementation)
	{
		return mApplicationWindowImplementation->GetScreenDimention(
			out_orientation,
			out_width, 
			out_height			
			);
	}
	return;
}

