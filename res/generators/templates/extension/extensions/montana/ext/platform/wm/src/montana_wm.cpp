#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>

#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include <RhoNativeViewManager.h>
#include "rubyext/WebView.h"

#include "ruby/ext/rho/rhoruby.h"

#include "<%= name.downcase %>_wm.h"


extern "C" VALUE <%= name.downcase %>_native_process_string(const char* str) {
    
    const char block[] = "<WM>";
    char* buf = NULL;
    buf = (char*)malloc(strlen(str) + strlen(block)*2 + 1);

    strcpy(buf, block);
    strcat(buf, str);
    strcat(buf, block);

    VALUE result = rho_ruby_create_string(buf);

    free(buf);

    return result;
}


extern "C" int <%= name.downcase %>_calc_summ(int x, int y) {
	return (x+y);
}




