//
//  giphoneviewutility.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneViewUtility_h_
#define _GIPhoneViewUtility_h_

#include <UIKit/UIKit.h>

class GIPhoneViewUtility
{
	//////////////////////////////////////////////////////////
	//disabled
private:
	~GIPhoneViewUtility();

	//////////////////////////////////////////////////////////
	//static public methods
public:
	static UIImage* const NewUIImageFromBlitOpenGL(UIImage* const in_underlay);
	
	static UIImage* const NewUIImageFromBlit(UIImage* const in_underlay, UIImage* const in_overlay);

};

#endif //_GIPhoneViewUtility_h_