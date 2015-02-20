// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
int rho_ruby_is_started();
#pragma comment(lib, "CoreAPI.lib")
#pragma comment(lib, "WebViewLib.lib")
#pragma comment(lib, "NativeToolbarLib.lib")
#pragma comment(lib, "NativeTabbarLib.lib")
#pragma comment(lib, "AudioCaptureLib.lib")
extern void Init_CoreAPI_Extension(void);
extern void Init_AudioCapture_extension(void);
void Init_Extensions(void) {
    Init_CoreAPI_Extension();
    Init_AudioCapture_extension();
}
