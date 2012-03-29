// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
#pragma comment(lib, "digest.lib")
#pragma comment(lib, "digest-md5.lib")
#pragma comment(lib, "digest-sha1.lib")
#pragma comment(lib, "RhoElementsExt.lib")
#pragma comment(lib, "rhowebkit.lib")
extern void Init_digest(void);
extern void Init_md5(void);
extern void Init_sha1(void);
extern void Init_RhoElementsExt_extension(void);
void Init_Extensions(void) {
    Init_digest();
    Init_md5();
    Init_sha1();
    Init_RhoElementsExt_extension();
}
