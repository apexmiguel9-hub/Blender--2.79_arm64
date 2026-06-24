#include "GHOST_SystemAndroid.h"
#include <android/native_window.h>
#include <jni.h>

GHOST_SystemAndroid::GHOST_SystemAndroid() : m_window(NULL) {}
GHOST_SystemAndroid::~GHOST_SystemAndroid() {}

bool GHOST_SystemAndroid::processEvents(bool) { return false; }
int GHOST_SystemAndroid::toggleConsole(int) { return 0; }

GHOST_TSuccess GHOST_SystemAndroid::getCursorPosition(GHOST_TInt32 &x, GHOST_TInt32 &y) const
{ x = 0; y = 0; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::setCursorPosition(GHOST_TInt32 x, GHOST_TInt32 y)
{ (void)x; (void)y; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::getCursorGrabState(bool &g, bool &h) const
{ g = false; h = true; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::setCursorGrabState(bool g, bool h)
{ (void)g; (void)h; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::getCursorVisibility(bool &v) const
{ v = true; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::setCursorVisibility(bool v)
{ (void)v; return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::getModifierKeys(GHOST_ModifierKeys &k) const
{ k.clear(); return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::getButtons(GHOST_Buttons &b) const
{ b.clear(); return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::setCursorShape(GHOST_TStandardCursor) { return GHOST_kSuccess; }
GHOST_TSuccess GHOST_SystemAndroid::hasCursorShape(GHOST_TStandardCursor) { return GHOST_kFailure; }
GHOST_TSuccess GHOST_SystemAndroid::setCustomCursorShape(
        GHOST_TUns8 *, GHOST_TUns8 *, int, int, int, int, int, int)
{ return GHOST_kSuccess; }

GHOST_TSuccess GHOST_SystemAndroid::getWindowBounds(
        const GHOST_IWindow *, GHOST_Rect &bounds) const
{ return getClientBounds(NULL, bounds); }

GHOST_TSuccess GHOST_SystemAndroid::getClientBounds(
        const GHOST_IWindow *, GHOST_Rect &bounds) const
{
	if (m_window) { bounds = m_window->getClientBounds(); return GHOST_kSuccess; }
	return GHOST_kFailure;
}

/* JNI C API for Android */
static GHOST_SystemAndroid *g_sys = NULL;

extern "C" int GHOST_Android_init(
        JNIEnv *env, jobject surface, int w, int h)
{
	if (!g_sys) g_sys = new GHOST_SystemAndroid();
	ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
	if (!win) return -1;
	GHOST_WindowAndroid *gh_win = new GHOST_WindowAndroid(win, w, h, false, false, 0);
	if (!gh_win->getValid()) { delete gh_win; return -2; }
	g_sys->m_window = gh_win;
	return 0;
}

extern "C" int GHOST_Android_setSurface(
        JNIEnv *env, jobject surface, int w, int h)
{
	if (!g_sys || !g_sys->m_window) return -1;
	ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
	if (!win) return -1;
	g_sys->m_window->setNativeWindow(win, w, h);
	return 0;
}

extern "C" int GHOST_Android_touchEvent(int action, float x, float y)
{
	if (!g_sys) return -1;
	return 0;
}

extern "C" int GHOST_Android_keyEvent(int down, int keyCode, int metaState)
{
	if (!g_sys) return -1;
	return 0;
}
