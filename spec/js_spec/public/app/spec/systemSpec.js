describe("<system module specs>", function () {

    /* System property specs */

    it("Test platform property", function () {
        expect(Rho.System.getPlatform()).isNotEmptyString();
    });

    it("Test hasCamera property", function () {
        expect(typeof Rho.System.getHasCamera()).toEqual('boolean');
    });

    it("Test screenWidth property", function () {
        expect(Rho.System.getScreenWidth()).isNumberGreaterThenZero();
    });

    it("Test screenHeight property", function () {
        expect(Rho.System.getScreenHeight()).isNumberGreaterThenZero();
    });

    it("Test realScreenWidth property", function () {
        expect(Rho.System.getRealScreenWidth()).isNumberGreaterThenZero();
    });

    it("Test realScreenHeight property", function () {
        expect(Rho.System.getRealScreenHeight()).isNumberGreaterThenZero();
    });

    it("Test screen orientation property", function () {
        expect(Rho.System.getScreenOrientation()).isNotEmptyString();
    });

    it("Test ppiX property", function () {
        expect(Rho.System.getPpiX()).isNumberGreaterThenZero();
    });

    it("Test ppiY property", function () {
        expect(Rho.System.getPpiY()).isNumberGreaterThenZero();
    });

    it("Test phoneId property", function () {
        expect(Rho.System.getPhoneId()).isNotEmptyString();
    });

    it("Test deviceName property", function () {
        expect(Rho.System.getDeviceName()).isNotEmptyString();
    });

    it("Test OsVersion property", function () {
        expect(Rho.System.getOsVersion()).isNotEmptyString();
    });

    it("Test locale property", function () {
        expect(Rho.System.getLocale()).isNotEmptyString();
    });

    it("Test country property", function () {
        expect(Rho.System.getCountry()).isNotEmptyString();
    });

    it("Test isSimulator property", function () {
        expect(Rho.System.getIsEmulator()).isBoolean;
    });

    it("Test hasCalendar property", function () {
        expect(Rho.System.getHasCalendar()).isBoolean();
    });

    it("Test isMotorolaDevice property", function () {
        expect(Rho.System.getIsMotorolaDevice()).isBoolean();
    });

    it("Test httpProxyURI property", function () {
        Rho.System.setHttpProxyURI('http://localhost');
        expect(Rho.System.getHttpProxyURI()).toEqual('http://localhost');
    });

    it("Test keyboardState property", function () {
        expect(Rho.System.getKeyboardState()).toEqual('automatic');
    });

    it("Test default value of localServerPort property", function () {
        expect(Rho.System.getLocalServerPort()).isNumberGreaterThenZero();
    });

    it("Test freeServerPort property", function () {
        expect(Rho.System.getFreeServerPort()).isNumberGreaterThenZero();
    });

    it("Test screenAutoRotate property", function () {
        expect(Rho.System.getScreenAutoRotate()).toEqual(true);
    });

    it("Test hasTouchscreen property", function () {
        expect(Rho.System.getHasTouchscreen()).toEqual(true);
    });

    it("Test securityTokenNotPased property", function () {
        expect(Rho.System.getSecurityTokenNotPassed()).isBoolean();
    });

    it("Test webviewFramework property", function () {
        expect(Rho.System.getWebviewFramework()).isNotEmptyString();
    });

    it("Test screenSpleeping property", function () {
        expect(Rho.System.getScreenSleeping()).toEqual(false);
    });

    it("Test startParams method", function () {
        expect(typeof Rho.System.getStartParams()).toEqual('string');
    });

    it("Test bringToFront method", function () {
        Rho.System.bringToFront();
    });


    /* ----------          platform dependent specs          ---------- */


    if (isAndroidPlatform()) {

        it("Test deviceOwnerEmail property", function () {
            expect(Rho.System.deviceOwnerEmail()).isNotEmptyString();
        });

        it("Test deviceOwnerName property", function () {
            expect(Rho.System.deviceOwnerName()).isNotEmptyString();
        });

    }

    if (isApplePlatform()) {
        it("Test applicationIconBadge property", function () {
            Rho.System.setApplicationIconBadge(1);
            expect(Rho.System.getApplicationIconBadge()).toEqual(1);
        });

    }

    if (isNotAnyWindowsPlatform()) {

        it("Test devicePushId property", function () {
            expect(Rho.System.getDevicePushId()).isNotEmptyString();
        });

        it("Test screenAutoRotate property", function () {
            expect(Rho.System.getScreenAutoRotate()).toEqual(true);

            Rho.System.setScreenAutoRotate(false);
            expect(Rho.System.getScreenAutoRotate()).toEqual(false);
        });

    }

    if ('WINDOWS' != Rho.System.getPlatform()) {
        it("Test oemInfo property", function () {
            expect(Rho.System.getOemInfo()).isNotEmptyString();
        });

        it("Test uuid property", function () {
            expect(Rho.System.getUuid()).isNotEmptyString();
        });
    }

    if (['WINDOWS_DESKTOP', 'WINDOWS'].indexOf(Rho.System.getPlatform()) != -1) {

        it("Test deleteRegistrySetting property", function () {
            Rho.System.deleteRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})

            Rho.System.setRegistrySetting({hive: 'HKCU', type: 'String', key: 'phone_spec', setting: 'phone_spec_registry_key', value: 'test'});
            expect(Rho.System.getRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})).toEqual('test')
        });

        it("Test screenSleeping property", function () {
            Rho.System.setScreenSleeping(true);
            expect(Rho.System.getScreenSleeping()).toEqual(true);

            Rho.System.setScreenSleeping(false);
            expect(Rho.System.getScreenSleeping()).toEqual(false);
        });

    }

    if (isWindowsDesktopPlatform()) {

        it("Test default value of lockWindowSize property", function () {
            expect(Rho.System.getLockWindowSize()).toEqual(false);
        });

        it("Test lockWindowsSize property", function () {
            Rho.System.setLockWindowSize(false);
            expect(Rho.System.getLockWindowSize()).toEqual(false);
        });

        it("Test windowFrame method", function () {
            Rho.System.setWindowFrame(1, 1, 200, 200);
        });

        it("Test windowPosition method", function () {
            Rho.System.setWindowPosition(1, 1);
        });

        it("Test windowSize method", function () {
            Rho.System.setWindowSize(100, 100);
        });

    }

})