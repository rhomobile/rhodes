function isAnyWindowsPlatform() {

    return ["WINDOWS", "WINDOWS_DESKTOP", "WP8"].indexOf(Rho.System.getPlatform()) != -1;

}

function isNotAnyWindowsPlatform() {

    return !isAnyWindowsPlatform();
}

function isWindowsMobileOrAndroidPlatform() {

    return ["WINDOWS", "ANDROID"].indexOf(Rho.System.getPlatform()) != -1;

}

function isAndroidPlatform(){

    return ["ANDROID"] == (Rho.System.getPlatform());

}

function isApplePlatform(){

    return ["APPLE"] == (Rho.System.getPlatform());

}

function isWindowsDesktopPlatform(){

    return "WINDOWS_DESKTOP" == Rho.System.getPlatform();

}

