//file: GWin32WindowInput.h
#ifndef _GWin32WindowInput_H_
#define _GWin32WindowInput_H_

#include <WINDOWS.H>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "GVector2Float.h"
#include "GVector3Float.h"
class GInput;


/**/
class GWin32WindowInput : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GInput> TPointerInput;

	//constructors
public:
	GWin32WindowInput();
	~GWin32WindowInput();

	// public methods
public:
	void Tick(const float in_timeDelta);
	void Clear();
	void ProcessMessages(
		HWND in_hWnd,
		const unsigned int in_uMessage,
		const unsigned int in_wParam,
		const unsigned int in_lParam			
		);

	// public accessors
public:
	const GInput& GetInput()const;

	// private methods
private:
	void MouseMove(HWND in_hWnd, const unsigned int in_lParam);
	void KeyDown(const unsigned int in_wParam, const unsigned int in_lParam);
	void KeyUp(const unsigned int in_wParam, const unsigned int in_lParam);

	void DummyMouseMove();

private:
	TPointerInput mInput;

	bool mMouseButtonDownLeft;
	bool mMouseButtonDownMiddle;
	bool mMouseButtonDownRight;
	bool mMouseButtonDownLeftEdge;
	bool mMouseButtonDownMiddleEdge;
	bool mMouseButtonDownRightEdge;

	bool mKeyboardButtonDownLeft;
	bool mKeyboardButtonDownRight;
	bool mKeyboardButtonDownUp;
	bool mKeyboardButtonDownDown;
	bool mKeyboardButtonDownPageUp;
	bool mKeyboardButtonDownPageDown;

	bool mEscapeButtonDown;

	GVector2Float mMouseOldRatio;
	int mCaptureCount;

	GVector3Float mAccelerometerAt;
	GVector3Float mAccelerometerUp;
};

/**/
#endif // _GWin32WindowInput_H_