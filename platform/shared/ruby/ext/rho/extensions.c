// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
#pragma comment(lib, "Barcode.lib")
#pragma comment(lib, "Rainbow.lib")
extern void Init_Barcode(void);
extern void Init_Rainbow(void);
void Init_Extensions(void) {
    Init_Barcode();
    Init_Rainbow();
}
