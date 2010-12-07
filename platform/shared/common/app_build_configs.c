
#include <string.h>

#include "app_build_configs.h"


#include "app_build_configs.inc"


const char* get_app_build_config_item(const char* key) {
  int i;
  for (i = 0; i < APP_BUILD_CONFIG_COUNT; i++) {
    if (strcmp(key, keys[i]) == 0) {
      return values[i];
    }
  }
  return 0;
}