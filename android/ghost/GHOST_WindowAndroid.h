#ifndef __GHOST_WINDOWANDROID_H__
#define __GHOST_WINDOWANDROID_H__

#include "GHOST_Window.h"
#include "GHOST_Rect.h"
#include <android/native_window.h>
#include <EGL/egl.h>

class GHOST_WindowAndroid : public GHOST_Window {
public:
	GHOST_WindowAndroid(ANativeWindow *nativeWindow,
	        GHOST_TInt32 width, GHOST_TInt32 height,
	        bool stereoVisual, bool alphaBackground,
	        GHOST_TUns16 numOfAASamples);
	~GHOST_WindowAndroid();

	GHOST_TSuccess setNativeWindow(ANativeWindow *window, int w, int h);
	GHOST_Rect getClientBounds() const { return m_clientBounds; }
	bool getValid() const;
	void setTitle(const GHOST_TUns8 *title);
	std::string getTitle() const;
	void getWindowBounds(GHOST_Rect &bounds) const;
	void getClientBounds(GHOST_Rect &bounds) const;
	GHOST_TSuccess setClientWidth(GHOST_TUns32 width);
	GHOST_TSuccess setClientHeight(GHOST_TUns32 height);
	GHOST_TSuccess setClientSize(GHOST_TUns32 width, GHOST_TUns32 height);
	void screenToClient(GHOST_TInt32 inX, GHOST_TInt32 inY,
	        GHOST_TInt32 &outX, GHOST_TInt32 &outY) const;
	void clientToScreen(GHOST_TInt32 inX, GHOST_TInt32 inY,
	        GHOST_TInt32 &outX, GHOST_TInt32 &outY) const;
	GHOST_TSuccess swapBuffers();
	GHOST_TSuccess setSwapInterval(int interval);
	GHOST_TSuccess getSwapInterval(int &intervalOut);

private:
	ANativeWindow *m_nativeWindow;
	GHOST_Rect m_clientBounds;
	EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
	bool m_valid;
};

#endif
