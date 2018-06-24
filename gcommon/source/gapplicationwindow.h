//
//  gapplicationwindow.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gapplicationwindow_H_
#define _gapplicationwindow_H_

#include "gcommon.h"
#include "gapplicationwindowtype.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef IPHONE
	class GIPhoneApplicationWindow;
	typedef GIPhoneApplicationWindow GApplicationWindowImplementation;
#elif WIN32
	#include <WINDOWS.H>
	class GWin32ApplicationWindow;
	typedef GWin32ApplicationWindow GApplicationWindowImplementation;
#endif

class GView;
class GRender;
class GInput;

//interface class for application tasks
class GApplicationWindow : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::scoped_ptr<GApplicationWindowImplementation> TPointerApplicationWindowImplementation;

	typedef boost::shared_ptr<GView> TPointerView;
	typedef boost::weak_ptr<GView> TWeakView;

	typedef boost::shared_ptr<GRender> TPointerRender;

	///////////////////////////////////////////////////////
	//constructor
public:
	GApplicationWindow();
	~GApplicationWindow();

	///////////////////////////////////////////////////////
	//public methods
public:
	TPointerView CreateViewRender(
		TPointerRender& out_render,
		const GBOOL in_transparentBackground,
		const GBOOL in_depth,
		const GBOOL in_stencil,
        const GR32 in_scale
		);
	//TPointerView CreateViewCamera();

	void PushViewFront(TPointerView& inout_view);
	void PushViewInfront(TPointerView& inout_view, TPointerView& inout_where);
	void RemoveView(TPointerView& inout_view);

	GVOID PreTick(const GR32 in_timeDelta);
	const GBOOL Tick(const GR32 in_timeDelta);
	GVOID PostTick(const GR32 in_timeDelta);

	void PutText(const std::string& in_text);

	void Show();

	const GInput& GetInput()const;
	//width and height are in device space, but orientation infers what direction is the 'top' of the screen
	void GetScreenDimention(
		GApplicationWindowType::TOrientation::TEnum& out_orientation,
		int& out_width, 
		int& out_height
		)const;
        
	///////////////////////////////////////////////////////
	//private members
private:
	TPointerApplicationWindowImplementation mApplicationWindowImplementation;

};

#endif //#ifndef _gapplicationwindow_H_
