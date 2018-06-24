//
//  giphoneapplicationwindow.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneApplicationWindow_h_
#define _GIPhoneApplicationWindow_h_

#include "gcommon.h"
#include "gapplicationwindowtype.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Foundation/Foundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <vector>

class GRender;
class GView;
class GInput;

/**/
class GIPhoneApplicationWindow : public boost::noncopyable
{
	/////////////////////////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GRender> TPointerRender;
	
	typedef boost::shared_ptr<GView> TPointerView;
	typedef boost::weak_ptr<GView> TWeakView;
	typedef std::vector<TWeakView> TArrayWeakView;

	/////////////////////////////////////////////////////////////////////////
	//constructor
public:
	GIPhoneApplicationWindow();
	~GIPhoneApplicationWindow();
	
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
        
    //return 0.0 to 1.0, (if undefined, returns 1.0)
    //const GR32 GetBatteryLevel()const;  

	/////////////////////////////////////////////////////////////////////////	
	//private members
private:
	id mWindow;
	id mController;
	id mDebugText;
	TArrayWeakView mArrayView;

};

#endif //_GIPhoneApplicationWindow_h_