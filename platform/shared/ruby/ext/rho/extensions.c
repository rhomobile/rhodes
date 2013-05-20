// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
#pragma comment(lib, "CoreAPI.lib")
#pragma comment(lib, "GenPropBag.lib")
#pragma comment(lib, "Rhoconnect-client.lib")
#pragma comment(lib, "Barcode.lib")
#pragma comment(lib, "CoreAPI.lib")
#pragma comment(lib, "Instrumentation.lib")
#pragma comment(lib, "Indicators.lib")
#pragma comment(lib, "Cardreader.lib")
#pragma comment(lib, "Signature.lib")
#pragma comment(lib, "RhoElementsExt.lib")
#pragma comment(lib, "rhowebkit.lib")
extern void Init_CoreAPI_Extension(void);
extern void Init_GenPropBag_extension(void);
extern void Init_Rhoconnectclient_extension(void);
extern void Init_Barcode_extension(void);
extern void Init_CoreAPI_Extension(void);
extern void Init_Instrumentation_extension(void);
extern void Init_RhoElementsExt_extension(void);
extern void Init_Indicators_extension(void);
extern void Init_Cardreader_extension(void);
extern void Init_Signature_extension(void);
void Init_Extensions(void) {
    Init_CoreAPI_Extension();
    Init_GenPropBag_extension();
    Init_Rhoconnectclient_extension();
    Init_Barcode_extension();
    Init_CoreAPI_Extension();
    Init_Instrumentation_extension();
    Init_RhoElementsExt_extension();
    Init_Indicators_extension();
    Init_Cardreader_extension();
    Init_Signature_extension();
}
