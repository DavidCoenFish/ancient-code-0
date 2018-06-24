//  guibuttoncallback.h

#ifndef _zombieguimainbuttoncallback_h_
#define _zombieguimainbuttoncallback_h_

#include <GGuiNodeComponentButtonCallback.h>

class GVector2Float;
class ZombieGuiMain;

class ZombieGuiMainButtonCallback : public GGuiNodeComponentButtonCallback
{
	//constructor
public:
	ZombieGuiMainButtonCallback(ZombieGuiMain& inout_parent);
	virtual ~ZombieGuiMainButtonCallback();

	//implement GGuiNodeComponentButtonCallback
private:
	virtual void OnClickDown(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId		
		);

	virtual void OnClickDrag(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId	
		);

	//in_activate is true if click location has never left the button (ie, click release inside button)
	virtual void OnClickRelease(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId,
		const bool in_activate
		);

	//private members
private:
	ZombieGuiMain& m_parent;

};

#endif //_guibuttoncallback_h_
