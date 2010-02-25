#include "stdafx.h"

#include "MainWindow.h"

extern "C" HWND getMainWnd();


extern "C" void alert_show_popup(char* message) {
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_SHOWPOPUP, 0,(LPARAM)strdup(message));
}

extern "C" void alert_vibrate(void*) {
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_VIBRATE, 0, NULL);
}

extern "C" void alert_play_file(char* file_name, ...) {
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_PLAYFILE, 0,(LPARAM)strdup(file_name));
}
