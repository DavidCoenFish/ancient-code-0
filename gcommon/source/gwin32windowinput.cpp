//file: GWin32WindowInput.cpp

#include "GWin32WindowInput.h"

#include "GInput.h"
#include "GAssert.h"
#include "GMatrix9Float.h"
#include "GInputButton.h"
#include "GInputJoystick.h"
#include "GInputTouch.h"
#include <windowsx.h>

#define GLSHACK

//constructors
GWin32WindowInput::GWin32WindowInput()
: mInput()
, mMouseButtonDownLeft(false)
, mMouseButtonDownMiddle(false)
, mMouseButtonDownRight(false)
, mMouseButtonDownLeftEdge(false)
, mMouseButtonDownMiddleEdge(false)
, mMouseButtonDownRightEdge(false)
, mMouseOldRatio()
, mCaptureCount(0)
, mAccelerometerAt(-GVector3Float::sUnitY)
, mAccelerometerUp(-GVector3Float::sUnitZ)
, mKeyboardButtonDownLeft(false)
, mKeyboardButtonDownRight(false)
, mKeyboardButtonDownUp(false)
, mKeyboardButtonDownDown(false)
, mKeyboardButtonDownPageUp(false)
, mKeyboardButtonDownPageDown(false)
, mEscapeButtonDown(false)
{
	mInput.reset(new GInput);
	if (mInput)
	{
		mInput->SetJoystickCount(1);
		mInput->SetButtonCount(1);
	}
	return;
}

GWin32WindowInput::~GWin32WindowInput()
{
	return;
}

// public methods
void GWin32WindowInput::Tick(const float in_timeDelta)
{
	if (!mInput)
	{
		return;
	}

	//hack a button in, shake equivalent
	if (1 <= mInput->GetButtonCount())
	{
		mInput->GetButton(0).SetState(mEscapeButtonDown);
	}

	mInput->Tick(in_timeDelta);

#ifdef GLSHACK
	if (mKeyboardButtonDownLeft != mKeyboardButtonDownRight)
	{
		if (mKeyboardButtonDownLeft)
		{
			mAccelerometerAt.m_x = -1.0F;
		}
		else
		{
			mAccelerometerAt.m_x = 1.0F;
		}
	}
	else
	{
		mAccelerometerAt.m_x = 0.0F;
	}
	if (mKeyboardButtonDownDown != mKeyboardButtonDownUp)
	{
		if (mKeyboardButtonDownDown)
		{
			mAccelerometerAt.m_y = -1.0F;
		}
		else
		{
			mAccelerometerAt.m_y = 1.0F;
		}
	}
	else
	{
		mAccelerometerAt.m_y = 0.0F;
	}

	mInput->GetJoystick(0).GetDirection() = mAccelerometerAt;
#else
	//keypad controlls accelerometer
	bool dirty = false;
	if (mKeyboardButtonDownLeft != mKeyboardButtonDownRight)
	{
		dirty = true;
		float amount = 1.0F * in_timeDelta;
		if (mKeyboardButtonDownRight)
		{
			amount = -amount;
		}
		const GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(mAccelerometerUp, amount);
		mAccelerometerAt = mAccelerometerAt * rotationMatrix;
	}
	if (mKeyboardButtonDownUp != mKeyboardButtonDownDown)
	{
		dirty = true;
		float amount = 1.0F * in_timeDelta;
		if (mKeyboardButtonDownDown)
		{
			amount = -amount;
		}

		const GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(CrossProduct(mAccelerometerAt, mAccelerometerUp), amount);
		mAccelerometerAt = mAccelerometerAt * rotationMatrix;
		mAccelerometerUp = mAccelerometerUp * rotationMatrix;
	}
	if (mKeyboardButtonDownPageUp != mKeyboardButtonDownPageDown)
	{
		dirty = true;
		float amount = 10.0F * in_timeDelta;
		if (mKeyboardButtonDownPageDown)
		{
			amount = -amount;
		}

		const GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(CrossProduct(mAccelerometerAt, mAccelerometerUp), amount);
		mAccelerometerAt = mAccelerometerAt * rotationMatrix;
		mAccelerometerUp = mAccelerometerUp * rotationMatrix;
	}

	if (dirty)
	{
		Orthogonalise(mAccelerometerAt, mAccelerometerUp);
	}

	mInput->GetJoystick(0).GetDirection() = mAccelerometerAt;
#endif

	DummyMouseMove();

	return;
}

void GWin32WindowInput::ProcessMessages(
	HWND in_hWnd,
	const unsigned int in_uMessage,
	const unsigned int in_wParam,
	const unsigned int in_lParam			
	)
{
	switch( in_uMessage )
	{
	default:
		{
			break;
		}
	case WM_MOUSEMOVE:
		{
			MouseMove(in_hWnd, in_lParam);
			break;
		}
	case WM_LBUTTONDOWN:
		{
			mMouseButtonDownLeft = true;
			mMouseButtonDownLeftEdge = true;
			break;
		}
	case WM_LBUTTONUP:
		{
			mMouseButtonDownLeft = false;
			mMouseButtonDownLeftEdge = true;
			break;
		}
	case WM_MBUTTONDOWN:
		{
			mMouseButtonDownMiddle = true;
			mMouseButtonDownMiddleEdge = true;
			break;
		}
	case WM_MBUTTONUP:
		{
			mMouseButtonDownMiddle = false;
			mMouseButtonDownMiddleEdge = true;
			break;
		}
	case WM_RBUTTONDOWN:
		{
			mMouseButtonDownRight = true;
			mMouseButtonDownRightEdge = true;
			break;
		}
	case WM_RBUTTONUP:
		{
			mMouseButtonDownRight = false;
			mMouseButtonDownRightEdge = true;
			break;
		}
    case WM_KEYDOWN: 
		{
			KeyDown(in_wParam, in_lParam);
			break;
		}
	case WM_KEYUP:
		{
			KeyUp(in_wParam, in_lParam);
			break;
		}
	}

	return;
}

// private methods
void GWin32WindowInput::MouseMove(HWND in_hWnd, const unsigned int in_lParam)
{
	if (!mInput)
	{
		return;
	}

	RECT rect;
	memset(&rect, 0, sizeof(RECT));
	if(0 == GetClientRect(in_hWnd, &rect))
	{
		return;
	}

	const int width = rect.right - rect.left;
	const int height = rect.bottom - rect.top;

	const int xPos = GET_X_LPARAM(in_lParam); 
	const int yPos = GET_Y_LPARAM(in_lParam); 

	GVector2Float newRatio;
	if (0 != width)
	{
		newRatio.m_x = (((float)(xPos)) / ((float)(width)));
	}

	if( 0 != height )
	{
		newRatio.m_y = (((float)(yPos)) / ((float)(height)));
	}
	if (mMouseButtonDownLeft)
	{
		mInput->AddTouch(1, newRatio, newRatio - mMouseOldRatio, mMouseButtonDownLeftEdge, false, false);
		mMouseButtonDownLeftEdge = false;
	}
	else if (mMouseButtonDownRight)
	{
		mInput->AddTouch(2, newRatio, newRatio - mMouseOldRatio, mMouseButtonDownRightEdge, false, false);
		mInput->AddTouch(3, GVector2Float::sUnit - newRatio, -(newRatio - mMouseOldRatio), mMouseButtonDownRightEdge, false, false);
		mMouseButtonDownRightEdge = false;
	}
	else if (mMouseButtonDownMiddle)
	{
		//pinch with drag in same direction
		mInput->AddTouch(4, newRatio, newRatio - mMouseOldRatio, mMouseButtonDownMiddleEdge, false, false);
		mInput->AddTouch(5, newRatio - GVector2Float(0.5F, 0.5F), newRatio - mMouseOldRatio, mMouseButtonDownMiddleEdge, false, false);
		mMouseButtonDownMiddleEdge = false;
	}

	mMouseOldRatio = newRatio;
}

void GWin32WindowInput::KeyDown(const unsigned int in_wParam, const unsigned int in_lParam)
{
	switch (in_wParam)
	{
	default:
		break;
	case VK_LEFT:
		mKeyboardButtonDownLeft = true;
		break;
	case VK_RIGHT:
		mKeyboardButtonDownRight = true;
		break;
	case VK_UP:
		mKeyboardButtonDownUp = true;
		break;
	case VK_DOWN:
		mKeyboardButtonDownDown = true;
		break;
	case VK_ESCAPE:
		mEscapeButtonDown = true;
		break;
	case VK_PRIOR:
		mKeyboardButtonDownPageUp = true;
		break;
	case VK_NEXT:
		mKeyboardButtonDownPageDown = true;
		break;
	}

	return;
}
void GWin32WindowInput::KeyUp(const unsigned int in_wParam, const unsigned int in_lParam)
{
	switch (in_wParam)
	{
	default:
		break;
	case VK_LEFT:
		mKeyboardButtonDownLeft = false;
		break;
	case VK_RIGHT:
		mKeyboardButtonDownRight = false;
		break;
	case VK_UP:
		mKeyboardButtonDownUp = false;
		break;
	case VK_DOWN:
		mKeyboardButtonDownDown = false;
		break;
	case VK_ESCAPE:
		mEscapeButtonDown = false;
		break;
	case VK_PRIOR:
		mKeyboardButtonDownPageUp = false;
		break;
	case VK_NEXT:
		mKeyboardButtonDownPageDown = false;
		break;
	}

	return;
}

static const bool LocalTestTouchId(const GInput& in_input, const int in_id)
{
	for (int index = 0; index < in_input.GetTouchCount(); ++index)
	{
		if (in_id == in_input.GetTouch(index).GetTouchId())
		{
			return true;
		}
	}
	return false;
}

void GWin32WindowInput::DummyMouseMove()
{
	if (mMouseButtonDownLeft)// && !LocalTestTouchId(*mInput, 1))
	{
		mInput->AddTouch(1, mMouseOldRatio, GVector2Float::sZero, mMouseButtonDownLeftEdge, false, false);
		mMouseButtonDownLeftEdge = false;
	}
	else if (mMouseButtonDownRight)// && !LocalTestTouchId(*mInput, 2))
	{
		mInput->AddTouch(2, mMouseOldRatio, GVector2Float::sZero, mMouseButtonDownRightEdge, false, false);
		mInput->AddTouch(3, GVector2Float::sUnit - mMouseOldRatio, GVector2Float::sZero, mMouseButtonDownRightEdge, false, false);
		mMouseButtonDownRightEdge = false;
	}
	else if (mMouseButtonDownMiddle)// && !LocalTestTouchId(*mInput, 4))
	{
		//pinch with drag in same direction
		mInput->AddTouch(4, mMouseOldRatio, GVector2Float::sZero, mMouseButtonDownMiddleEdge, false, false);
		mInput->AddTouch(5, mMouseOldRatio - GVector2Float(0.5F, 0.5F), GVector2Float::sZero, mMouseButtonDownMiddleEdge, false, false);
		mMouseButtonDownMiddleEdge = false;
	}

	if (mMouseButtonDownLeftEdge)
	{
		mInput->AddTouch(1, mMouseOldRatio, GVector2Float::sZero, false, true, false);
		mMouseButtonDownLeftEdge = false;
	}
	if (mMouseButtonDownRightEdge)
	{
		mInput->AddTouch(2, mMouseOldRatio, GVector2Float::sZero, false, true, false);
		mInput->AddTouch(3, GVector2Float::sUnit - mMouseOldRatio, GVector2Float::sZero, false, true, false);
		mMouseButtonDownRightEdge = false;
	}
	if (mMouseButtonDownMiddleEdge)
	{
		mInput->AddTouch(4, mMouseOldRatio, GVector2Float::sZero, false, true, false);
		mInput->AddTouch(5, mMouseOldRatio - GVector2Float(0.5F, 0.5F), GVector2Float::sZero, false, true, false);
		mMouseButtonDownMiddleEdge = false;
	}

	return;
}

// public accessors
const GInput& GWin32WindowInput::GetInput()const
{
	GASSERT(mInput, "invalid state");
	return (*mInput);
}
