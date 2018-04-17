// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
int rho_ruby_is_started();
#pragma comment(lib, "CoreAPI.lib")
#pragma comment(lib, "zlib.lib")
extern void Init_CoreAPI_Extension(void);
extern void Init_zlib(void);
void Init_Extensions(void) {
    Init_CoreAPI_Extension();
    if (rho_ruby_is_started()) Init_zlib();
}
