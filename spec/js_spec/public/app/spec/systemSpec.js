describe("<System specs>", function () {

    /* System property specs */

    it("Platform name", function () {
        expect(Rho.System.getPlatform()).isNotEmptyString();
    });

    it("Is camera available on the device", function () {
        expect(typeof Rho.System.getHasCamera()).toEqual('boolean');
    });

    it("Screen width in logical pixels", function () {
        expect(Rho.System.getScreenWidth()).isNumberGreaterThenZero();
    });

    it("Screen height in logical pixels", function () {
        expect(Rho.System.getScreenHeight()).isNumberGreaterThenZero();
    });

    it("Screen width in real pixels", function () {
        expect(Rho.System.getRealScreenWidth()).isNumberGreaterThenZero();
    });

    it("Screen height in real pixels", function () {
        expect(Rho.System.getRealScreenHeight()).isNumberGreaterThenZero();
    });

    it("Current screen orientation", function () {
        expect(Rho.System.getScreenOrientation()).isNotEmptyString();
    });

    it("Horizontal PPI (Pixels Per Inch)", function () {
        expect(Rho.System.getPpiX()).isNumberGreaterThenZero();
    });

    it("Vertical PPI (Pixels Per Inch)", function () {
        expect(Rho.System.getPpiY()).isNumberGreaterThenZero();
    });
    if (['WINDOWS', 'WINDOWS_DESKTOP', 'WP8'].indexOf(Rho.System.getPlatform()) == -1) {
        it("Push notifications device ID which may be used to receive push messages", function () {
            expect(Rho.System.getDevicePushId()).isNotEmptyString();
        });
    }
    it("Hardware based ID", function () {
        expect(Rho.System.getPhoneId()).isNotEmptyString();
    });

    it("Name of device", function () {
        expect(Rho.System.getDeviceName()).isNotEmptyString();
    });

    it("Version OS of device", function () {
        expect(Rho.System.getOsVersion()).isNotEmptyString();
    });

    it("Current device locale", function () {
        expect(Rho.System.getLocale()).isNotEmptyString();
    });

    it("Current device country code", function () {
        expect(Rho.System.getCountry()).isNotEmptyString();
    });

    it("Is application is running on emulator", function () {
        expect(Rho.System.getIsEmulator()).isBoolean;
    });

    it("Is calendar support available", function () {
        expect(Rho.System.getHasCalendar()).isBoolean();
    });

    it("Is device supports Motorola device capabilities", function () {
        expect(Rho.System.getIsMotorolaDevice()).isBoolean();
    });

    if (['WINDOWS'].indexOf(Rho.System.getPlatform()) != -1) {
        it("The OEM Information string for the terminal", function () {
            expect(Rho.System.getOemInfo()).isNotEmptyString();
        });

        it("The Unique Unit IDentifier for the terminal", function () {
            expect(Rho.System.getUuid()).isNotEmptyString();
        });
    }

    it("Setting HTTP proxy URI", function () {
        Rho.System.setHttpProxyURI('http://localhost');
        expect(Rho.System.getHttpProxyURI()).toEqual('http://localhost');
    });

    it("Getting keyboard state", function () {
        expect(Rho.System.getKeyboardState()).toEqual('automatic');
    });


    it("Default port of the local (embedded) HTTP server", function () {
        expect(Rho.System.getLocalServerPort()).isNumberGreaterThenZero();
    });

    it("Get free local server port", function () {
        expect(Rho.System.getFreeServerPort()).isNumberGreaterThenZero();
    });

    it("Get screen auto rotate", function () {
        expect(Rho.System.getScreenAutoRotate()).toEqual(true);
    });

    if (['WINDOWS', 'WINDOWS_DESKTOP', 'WP8'].indexOf(Rho.System.getPlatform()) == -1) {
        it("Setting screen auto rotate", function () {
            expect(Rho.System.getScreenAutoRotate()).toEqual(true);

            Rho.System.setScreenAutoRotate(false);
            expect(Rho.System.getScreenAutoRotate()).toEqual(false);
        });
    }
    it("Is device has touch screen", function () {
        expect(Rho.System.getHasTouchscreen()).toEqual(true);
    });

    it("Is the security token check was failed", function () {
        expect(Rho.System.getSecurityTokenNotPassed()).isBoolean();
    });

    it("Browser framework identity string", function () {
        expect(Rho.System.getWebviewFramework()).isNotEmptyString();
    });

    it("Is screen sleeping", function () {
        expect(Rho.System.getScreenSleeping()).toEqual(false);
    });

    if (['WINDOWS', 'WINDOWS_DESKTOP', 'WP8'].indexOf(Rho.System.getPlatform()) == -1) {
        it("Setting screen sleeping", function () {
            Rho.System.setScreenSleeping(true);
            expect(Rho.System.getScreenSleeping()).toEqual(true);

            Rho.System.setScreenSleeping(false);
            expect(Rho.System.getScreenSleeping()).toEqual(false);
        });
    }


    it("Command line parameters", function () {
        expect(typeof Rho.System.getStartParams()).toEqual('string');
    });

    it("Bring application window to the top of screen", function () {
        Rho.System.bringToFront();
    });


    /* ----------          platform dependent specs          ---------- */


    if ('ANDROID' == Rho.System.getPlatform()) {
        it("Primary email of phone owner (works only on Android)", function () {
            expect(Rho.System.deviceOwnerEmail()).isNotEmptyString();
        });

        it("Name(account name) of phone owner (works only on Android)", function () {
            expect(Rho.System.deviceOwnerName()).isNotEmptyString();
        });

    }


    if (['WINDOWS_DESKTOP', 'WINDOWS'].indexOf(Rho.System.getPlatform()) != -1) {

        it("Writing and reading registry value", function () {
            Rho.System.deleteRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})

            Rho.System.setRegistrySetting({hive: 'HKCU', type: 'String', key: 'phone_spec', setting: 'phone_spec_registry_key', value: 'test'});
            expect(Rho.System.getRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})).toEqual('test')
        });

    }

    /* Specs for Windows desktop platfform */

    if ('WINDOWS_DESKTOP' == Rho.System.getPlatform()) {

        it("Setting window frame", function () {
            Rho.System.setWindowFrame(1, 1, 200, 200);
        });

        it("Setting window position", function () {
            Rho.System.setWindowPosition(1, 1);
        });

        it("Locking the window size", function () {
            Rho.System.setLockWindowSize(true);
            expect(Rho.System.getLockWindowSize()).toEqual(true);
        });

        it("Unlocking the window size (only for Windows desktop)", function () {
            Rho.System.setLockWindowSize(false);
            expect(Rho.System.getLockWindowSize()).toEqual(false);
        });
    }

    /* Specs for Apple platform */

    if ('APPLE' == Rho.System.getPlatform()) {
        it("Setting the application icon to have this badge number", function () {
            Rho.System.setApplicationIconBadge(1);
            expect(Rho.System.getApplicationIconBadge()).toEqual(1);
        });
    }


})