generator.permissions["#{$app_package_name}.permission.MAPS_RECEIVE"] = 'signature'
generator.usesPermissions << "#{$app_package_name}.permission.MAPS_RECEIVE"
generator.usesPermissions << 'com.google.android.providers.gsf.permission.READ_GSERVICES'
