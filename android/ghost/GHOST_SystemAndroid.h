#ifndef __GHOST_SYSTEMANDROID_H__
#define __GHOST_SYSTEMANDROID_H__

#include "GHOST_System.h"
#include "GHOST_WindowAndroid.h"

class GHOST_SystemAndroid : public GHOST_System {
public:
	GHOST_SystemAndroid();
	~GHOST_SystemAndroid();

	bool processEvents(bool waitForEvent);
	int toggleConsole(int action);

	GHOST_TSuccess getCursorPosition(GHOST_TInt32 &x, GHOST_TInt32 &y) const;
	GHOST_TSuccess setCursorPosition(GHOST_TInt32 x, GHOST_TInt32 y);
	GHOST_TSuccess getCursorGrabState(bool &grab, bool &hide);
	GHOST_TSuccess setCursorGrabState(bool grab, bool hide);
	GHOST_TSuccess getCursorVisibility(bool &visible);
	GHOST_TSuccess setCursorVisibility(bool visible);
	GHOST_TSuccess getModifierKeys(GHOST_ModifierKeys &keys) const;
	GHOST_TSuccess getButtons(GHOST_Buttons &buttons) const;
	GHOST_TSuccess setCursorShape(GHOST_TStandardCursor cursorShape);
	GHOST_TSuccess hasCursorShape(GHOST_TStandardCursor cursorShape);
	GHOST_TSuccess setCustomCursorShape(
	        GHOST_TUns8 *bitmap, GHOST_TUns8 *mask,
	        int sizex, int sizey, int hotX, int hotY,
	        int fgColor, int bgColor);

	GHOST_TSuccess getWindowBounds(const GHOST_IWindow *window, GHOST_Rect &bounds) const;
	GHOST_TSuccess getClientBounds(const GHOST_IWindow *window, GHOST_Rect &bounds) const;

	GHOST_WindowAndroid *m_window;
};

#endif
