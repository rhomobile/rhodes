// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
int rho_ruby_is_started();
#pragma comment(lib, "CoreAPI.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "mediacapture.lib")
#pragma comment(lib, "Barcode.lib")
extern void Init_CoreAPI_Extension(void);
extern void Init_zlib(void);
extern void Init_Mediacapture_extension(void);
extern void Init_Barcode_extension(void);
void Init_Extensions(void) {
    Init_CoreAPI_Extension();
    if (rho_ruby_is_started()) Init_zlib();
    Init_Mediacapture_extension();
    Init_Barcode_extension();
}
