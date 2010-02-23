#include "stdafx.h"

extern "C" void alert_show_popup(const char* message) {
    CAtlStringW strMsgW( message );
    ::MessageBox(NULL,strMsgW,L"", MB_OK);
}

extern "C" void alert_vibrate(void*) {
}

extern "C" void alert_play_file(char* file_name, ...) {
}
