//
//  gwin32applicationwindow.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gwin32applicationwindow_H_
#define _gwin32applicationwindow_H_

#include "gcommon.h"
#include "gvector2float.h"
#include "gapplicationwindowtype.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <WINDOWS.H>

class GView;
class GRender;
class GInput;
class GWin32WindowInput;

/**/
class GWin32ApplicationWindow : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GRender> TPointerRender;

	typedef boost::shared_ptr<GView> TPointerView;
	typedef boost::weak_ptr<GView> TWeakView;
	typedef std::vector<TWeakView> TArrayWeakView;
	typedef boost::scoped_ptr<GWin32WindowInput> TPointerWin32WindowInput;

	///////////////////////////////////////////////////////
	//constructor
public:
	GWin32ApplicationWindow();
	~GWin32ApplicationWindow();
	
	///////////////////////////////////////////////////////
	//public methods
public:
	 GVOID CreateViewRender(
		TPointerView& out_view,
		TPointerRender& out_render,
		const GBOOL in_transparentBackground,
		const GBOOL in_depth,
		const GBOOL in_stencil,
		const GR32 in_scale
		);
	//TPointerView CreateViewCamera();

	GVOID PushViewFront(TPointerView& inout_view);
	GVOID PushViewInfront(TPointerView& inout_view, TPointerView& inout_where);
	GVOID RemoveView(TPointerView& inout_view);

	GVOID PreTick(const GR32 in_timeDelta);
	const GBOOL Tick(const GR32 in_timeDelta);
	GVOID PostTick(const GR32 in_timeDelta);

	GVOID PutText(const std::string& in_text);

	GVOID Show();

	const GInput& GetInput()const;

	void GetScreenDimention(
		GApplicationWindowType::TOrientation::TEnum& out_orientation,
		int& out_width, 
		int& out_height
		)const;

	///////////////////////////////////////////////////////
	//public static methods
public:
	static unsigned int __stdcall WindowProc(
		HWND in_hWnd,
		GU32 in_uMessage,
		GU32 in_wParam,
		GU32 in_lParam
		);

	///////////////////////////////////////////////////////
	//private methods
private:
	GVOID Activate(const GU32 in_wParam);
	GVOID Resize(const GU32 in_sizeX, const GU32 in_sizeY);
	GVOID Paint();

	///////////////////////////////////////////////////////
	//private members
private:
	HWND mWindow;
	GU32 mWindowWidth;
	GU32 mWindowHeight;
	TPointerWin32WindowInput mInput;

	TArrayWeakView mArrayView;

	GBOOL mContinue;
	GBOOL mFocus;



};

#endif //#ifndef _gwin32applicationwindow_H_
