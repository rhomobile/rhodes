//
//  BundleUpdate.h
//  Development
//
//  Created by Dmitry Soldatenkov on 18.08.14.
//
//

#include "common/RhodesApp.h"


extern "C" void callback_system_update_bundle(void *arg, rho::String const &strQuery);
extern "C" void callback_system_update_bundle_callback(void *arg, rho::String const &strQuery);
extern "C" void callback_system_get_info_callback(void *arg, rho::String const &strQuery);


extern "C" void Init_UpdateBundle();


extern "C" void Bundle_update_on_triple_tap();
extern "C" void Bundle_update_on_quadro_tap();