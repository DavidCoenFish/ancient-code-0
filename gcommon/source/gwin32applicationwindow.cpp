//
//  gwin32applicationwindow.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "gwin32applicationwindow.h"
#include "gwin32windowutility.h"
#include "gwin32windowparam.h"
#include "gview.h"
#include "gwin32applicationdelegate.h"
#include "gwin32windowopengl.h"
#include "GWin32WindowInput.h"
#include "grender.h"
#include "ginput.h"
#include <windowsx.h>

typedef boost::shared_ptr<GWin32WindowOpenGL> TPointerWin32WindowOpenGL;
typedef boost::shared_ptr<GWin32View> TPointerViewImplementation;

#define APPLICATION_ORIENTATION GApplicationWindowType::TOrientation::TTwelveOclock;
//#define APPLICATION_ORIENTATION GApplicationWindowType::TOrientation::TThreeOclock;
//#define APPLICATION_ORIENTATION GApplicationWindowType::TOrientation::TSixOclock;
//#define APPLICATION_ORIENTATION GApplicationWindowType::TOrientation::TNineOclock;

extern HINSTANCE GetHInstance(); //alt is from TPointerApplicationWindowCallback? or pass it down

///////////////////////////////////////////////////////
//constructor
GWin32ApplicationWindow::GWin32ApplicationWindow()
	: mWindow(GNULL)
	, mWindowWidth(0)
	, mWindowHeight(0)
	, mArrayView()
	, mContinue(GTRUE)
	, mFocus(GFALSE)
{
	GWin32WindowParam windowParam;
	mWindowWidth = windowParam.WindowDimentionXGet();
	mWindowHeight = windowParam.WindowDimentionYGet();

	mInput.reset(new GWin32WindowInput);

	mWindow = GWin32WindowUtility::Create(
		GetHInstance(),
		windowParam,
		this,
		(WNDPROC)WindowProc
		);

	return;
}

GWin32ApplicationWindow::~GWin32ApplicationWindow()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
GVOID GWin32ApplicationWindow::CreateViewRender(
	TPointerView& out_view,
	TPointerRender& out_render,
	const GBOOL in_transparentBackground,
	const GBOOL in_depth,
	const GBOOL in_stencil,
	const GR32 in_scale
	)
{
	TPointerWin32WindowOpenGL pointerOpenGL;
	pointerOpenGL.reset(new GWin32WindowOpenGL(mWindow, in_depth, in_stencil));
	out_render.reset(new GRender(pointerOpenGL.get()));
	TPointerViewImplementation downcastViewImplementation;
	downcastViewImplementation = pointerOpenGL;
	out_view.reset(new GView(downcastViewImplementation));
	return;
}

GVOID GWin32ApplicationWindow::PushViewFront(TPointerView& inout_view)
{
	if (GNULL == inout_view)
	{
		return;
	}
	mArrayView.push_back(inout_view);
	return;
}

GVOID GWin32ApplicationWindow::PushViewInfront(TPointerView& inout_view, TPointerView& inout_where)
{
	if (GNULL == inout_view)
	{
		return;
	}
	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		if (inout_where == (*iterator).lock())
		{
			mArrayView.insert(iterator, inout_view);
			return;
		}
	}
	return;
}

GVOID GWin32ApplicationWindow::RemoveView(TPointerView& inout_view)
{
	if (GNULL == inout_view)
	{
		return;
	}
	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		if (inout_view == (*iterator).lock())
		{
			mArrayView.erase(iterator);
			return;
		}
	}
	return;
}

GVOID GWin32ApplicationWindow::PreTick(const GR32 in_timeDelta)
{
	if (mInput)
	{
		mInput->Tick(in_timeDelta);
	}

	return;
}

const GBOOL GWin32ApplicationWindow::Tick(const GR32 in_timeDelta)
{
    MSG message;
	SecureZeroMemory( &message, sizeof( message ) );

	BOOL result;
	while ((result = PeekMessage(&message, mWindow, 0, 0, PM_REMOVE)) != 0)
	{
		if (result == -1)
		{
		   // handle the error and possibly exit
			mContinue = false; 
		}

		TranslateMessage(&message); 
		DispatchMessage(&message); 

		if (WM_PAINT == message.message)
		{
			break;
		}
	};

	if (mContinue)
	{
		Paint();
	}

	return mContinue;
}

GVOID GWin32ApplicationWindow::PostTick(const GR32 in_timeDelta)
{
	return;
}


GVOID GWin32ApplicationWindow::PutText(const std::string& in_text)
{
	return;
}

GVOID GWin32ApplicationWindow::Show()
{
	ShowWindow(mWindow, SW_SHOW);
	UpdateWindow(mWindow);
	return;		
}

const GInput& GWin32ApplicationWindow::GetInput()const
{
	return mInput->GetInput();
}

void GWin32ApplicationWindow::GetScreenDimention(
	GApplicationWindowType::TOrientation::TEnum& out_orientation,
	int& out_width, 
	int& out_height
	)const
{
	out_orientation = APPLICATION_ORIENTATION;
	out_width = mWindowWidth;
	out_height = mWindowHeight;
	return;
}

///////////////////////////////////////////////////////
//public static methods
/*static*/ unsigned int __stdcall GWin32ApplicationWindow::WindowProc(
	HWND in_hWnd,
	GU32 in_uMessage,
	GU32 in_wParam,
	GU32 in_lParam
	)
{
    switch(in_uMessage) 
    { 
	default:
		{
			GWin32ApplicationWindow* const pWindow = (GWin32ApplicationWindow* const)GWin32WindowUtility::UserDataGet(in_hWnd);
			if (pWindow)
			{
				pWindow->mInput->ProcessMessages(in_hWnd, in_uMessage, in_wParam, in_lParam);
			}
			break;
		}
	case WM_ACTIVATE:
		{	
			GWin32ApplicationWindow* const pWindow = (GWin32ApplicationWindow* const)GWin32WindowUtility::UserDataGet(in_hWnd);
			if (pWindow)
			{
				pWindow->Activate(in_wParam);
			}
			break;
		}
	case WM_CLOSE:
		{
			GWin32ApplicationWindow* const pWindow = (GWin32ApplicationWindow* const)GWin32WindowUtility::UserDataGet(in_hWnd);
			if (pWindow)
			{
				pWindow->mContinue = GFALSE;
			}
			return 0; 
		}
	case WM_CREATE: 
		{
			GWin32WindowUtility::UserDataSet(in_hWnd, in_lParam);
			GWin32ApplicationWindow* const pWindow = (GWin32ApplicationWindow* const)GWin32WindowUtility::UserDataGet(in_hWnd);
			if (pWindow)
			{
				pWindow->mWindow = in_hWnd;
			}
			return 0; 
		}
	case WM_PAINT: 
		{
			return 0; 
		}
	case WM_ERASEBKGND:
		{
			return 1;
		}
	case WM_SIZE: 
		{
			GWin32ApplicationWindow* const pWindow = (GWin32ApplicationWindow* const)GWin32WindowUtility::UserDataGet(in_hWnd);
			if (pWindow)
			{
				pWindow->Resize(LOWORD(in_lParam), HIWORD(in_lParam));
			}
			return 0; 
		}
    } 

	return (DefWindowProc(in_hWnd, in_uMessage, in_wParam, in_lParam));
}

///////////////////////////////////////////////////////
//private methods
GVOID GWin32ApplicationWindow::Activate(const GU32 in_wParam)
{
	switch( LOWORD( in_wParam ) )
	{
	default:
		{
			return;
		}
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		{
			mFocus = GTRUE;
			//emulating IPhone behaviour
			GWin32ApplicationDelegate::GetInstance()->ApplicationDidBecomeActive();
			break;
		}
	case WA_INACTIVE:
		{
			mFocus = GFALSE;
			//emulating IPhone behaviour
			GWin32ApplicationDelegate::GetInstance()->ApplicationWillResignActive();
			break;
		}
	}

	return;
}

GVOID GWin32ApplicationWindow::Resize(const GU32 in_sizeX, const GU32 in_sizeY)
{
	mWindowWidth = in_sizeX;
	mWindowHeight = in_sizeY;

	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		TPointerView pointerView = (*iterator).lock();
		if (GNULL != pointerView)
		{
			pointerView->GetPointerViewImplementation()->Resize(in_sizeX, in_sizeY);
		}
	}

	return;	
}

GVOID GWin32ApplicationWindow::Paint()
{
	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		TPointerView pointerView = (*iterator).lock();
		if (GNULL != pointerView)
		{
			pointerView->GetPointerViewImplementation()->Paint();
		}
	}
	return;	
}