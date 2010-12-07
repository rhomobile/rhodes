// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
// Generated 2010-12-07 21:11:23 +0300
package com.rho;

public class AppBuildConfig {
static final String keys[] = { ""
};

static final String values[] = { ""
};

static final int APP_BUILD_CONFIG_COUNT = 1;

public static String getItem(String key){
  for (int i = 1; i < APP_BUILD_CONFIG_COUNT; i++) {
    if ( key.compareTo( keys[i]) == 0) {
      return values[i];
    }
  }
  return null;
}
}
