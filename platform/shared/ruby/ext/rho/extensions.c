// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
int rho_ruby_is_started();
extern void Init_CoreAPI_Extension(void);
extern void Init_Crosswalk_extension(void);
extern void Init_Barcode_extension(void);
void Init_Extensions(void) {
    Init_CoreAPI_Extension();
    Init_Crosswalk_extension();
    Init_Barcode_extension();
}
