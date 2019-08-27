generator.permissions["#{$app_package_name}.permission.C2D_MESSAGE"] = 'signature'
generator.usesPermissions << "#{$app_package_name}.permission.C2D_MESSAGE"
generator.usesPermissions << 'com.google.android.c2dm.permission.RECEIVE'
generator.usesPermissions << 'android.permission.GET_ACCOUNTS'
generator.usesPermissions << 'android.permission.WAKE_LOCK'
generator.usesPermissions << 'android.permission.INTERNET'
generator.usesPermissions << 'android.permission.RECEIVE_BOOT_COMPLETED'