// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!

#include <string.h>

//#include "app_build_configs.h"

static const char* keys[] = { ""
,"moto-plugins"
,"shared-runtime"
,"webkit_outprocess"
,"webengine"
};

static const char* values[] = { ""
,"ALL"
,"1"
,"0"
,"IE"
};

#define APP_BUILD_CONFIG_COUNT 5

const char* get_app_build_config_item(const char* key) {
  int i;
  for (i = 1; i < APP_BUILD_CONFIG_COUNT; i++) {
    if (strcmp(key, keys[i]) == 0) {
      return values[i];
    }
  }
  return 0;
}

