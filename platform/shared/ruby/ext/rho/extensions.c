// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
extern void Init_zlib(void);
extern void Init_digest(void);
extern void Init_md5(void);
extern void Init_sha1(void);
extern void Init_sha2(void);
extern void Init_fcntl(void);
extern void Init_openssl(void);
void Init_Extensions(void) {
    Init_zlib();
    Init_digest();
    Init_md5();
    Init_sha1();
    Init_sha2();
    Init_fcntl();
    Init_openssl();
}
