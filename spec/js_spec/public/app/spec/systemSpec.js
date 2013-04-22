describe("<system module specs>", function () {

    /* System property specs */

    it("Test platform property", function () {
        expect(Rho.System.platform).isNotEmptyString();
    });


    it("Test hasCamera property", function () {
        expect(typeof Rho.System.hasCamera).toEqual('boolean');
    });

    it("Test screenWidth property", function () {
        expect(Rho.System.screenWidth).isNumberGreaterThenZero();
    });

    it("Test screenHeight property", function () {
        expect(Rho.System.screenHeight).isNumberGreaterThenZero();
    });

    it("Test realScreenWidth property", function () {
        expect(Rho.System.realScreenWidth).isNumberGreaterThenZero();
    });

    it("Test realScreenHeight property", function () {
        expect(Rho.System.realScreenHeight).isNumberGreaterThenZero();
    });

    it("Test screen orientation property", function () {
        expect(Rho.System.screenOrientation).isNotEmptyString();
    });

    it("Test ppiX property", function () {
        expect(Rho.System.ppiX).isNumberGreaterThenZero();
    });

    it("Test ppiY property", function () {
        expect(Rho.System.ppiY).isNumberGreaterThenZero();
    });

    it("Test deviceName property", function () {
        expect(Rho.System.deviceName).isNotEmptyString();
    });

    it("Test OsVersion property", function () {
        expect(Rho.System.osVersion).isNotEmptyString();
    });

    it("Test locale property", function () {
        expect(Rho.System.locale).isNotEmptyString();
    });

    it("Test country property", function () {
        expect(Rho.System.country).isNotEmptyString();
    });

    it("Test isSimulator property", function () {
        expect(Rho.System.isEmulator).isBoolean;
    });

    it("Test hasCalendar property", function () {
        expect(Rho.System.hasCalendar).isBoolean();
    });

    it("Test isMotorolaDevice property", function () {
        expect(Rho.System.isMotorolaDevice).isBoolean();
    });

    it("Test default value of localServerPort property", function () {
        expect(Rho.System.localServerPort).isNumberGreaterThenZero();
    });

    it("Test freeServerPort property", function () {
        expect(Rho.System.freeServerPort).isNumberGreaterThenZero();
    });

    it("Test screenAutoRotate property", function () {
        expect(Rho.System.screenAutoRotate).toEqual(true);
    });

    it("Test hasTouchscreen property", function () {
        expect(Rho.System.hasTouchscreen).toEqual(true);
    });

    it("Test securityTokenNotPased property", function () {
        expect(Rho.System.securityTokenNotPassed).isBoolean();
    });

    it("Test webviewFramework property", function () {
        expect(Rho.System.webviewFramework).isNotEmptyString();
    });

    it("Test startParams method", function () {
        expect(typeof Rho.System.getStartParams()).toEqual('string');
    });

    it("Test bringToFront method", function () {
        Rho.System.bringToFront();
    });

    it("Test callback", function () {

        var flag, value;

        var callbackFunction = function () {
            value = 10;
            flag = true
        };

        runs(function () {
            value = 0;
            flag = false;
            Rho.System.getStartParams(callbackFunction);
        });

        waitsFor(
            function () {
                return flag;
            },
            "Timeout",
            500
        );

        runs(function () {
            expect(value).toEqual(10)
        })
    });


    /* ----------          platform dependent specs          ---------- */


    if (isAnyButApplePlatform()) {
        it("Test phoneId property", function () {
            expect(Rho.System.phoneId).isNotEmptyString();
        });

        it("Test httpProxyURI property", function () {
            Rho.System.setHttpProxyURI = 'http://localhost';
            expect(Rho.System.httpProxyURI).toEqual('http://localhost');
        });

        it("Test keyboardState property", function () {
            expect(Rho.System.keyboardState).toEqual('automatic');
        });

    }

    if (isAndroidPlatform()) {
        it("Test deviceOwnerEmail property", function () {
            expect(Rho.System.deviceOwnerEmail).isNotEmptyString();
        });

        it("Test deviceOwnerName property", function () {
            expect(Rho.System.deviceOwnerName).isNotEmptyString();
        });
    }

    if (isApplePlatform()) {
        it("Test applicationIconBadge property", function () {
            Rho.System.applicationIconBadge = 1;
            expect(Rho.System.applicationIconBadge).toEqual(1);
        });
    }

    if (isAnyButWindowsFamilyPlatform()) {
        it("Test devicePushId property", function () {
            expect(Rho.System.devicePushId).isNotEmptyString();
        });

        it("Test screenAutoRotate property", function () {
            Rho.System.screenSleeping = true;
            expect(Rho.System.screenSleeping).toEqual(true);

            Rho.System.screenSleeping = false;
            expect(Rho.System.screenSleeping).toEqual(false);
        });
    }

    if (isWindowsMobileOrAndroidPlatform()) {
        it("Test oemInfo property", function () {
            expect(Rho.System.oemInfo).isNotEmptyString();
        });

        it("Test uuid property", function () {
            expect(Rho.System.uuid).isNotEmptyString();
        });
    }

    if (isWindowsMobileOrWindowsDesktopPlatform()) {
        it("Test deleteRegistrySetting property", function () {
            Rho.System.deleteRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})

            Rho.System.setRegistrySetting({hive: 'HKCU', type: 'String', key: 'phone_spec', setting: 'phone_spec_registry_key', value: 'test'});
            expect(Rho.System.getRegistrySetting({hive: 'HKCU', key: 'phone_spec', setting: 'phone_spec_registry_key'})).toEqual('test')
        });
    }

    if (isWindowsDesktopPlatform()) {
        it("Test default value of lockWindowSize property", function () {
            expect(Rho.System.lockWindowSize).toEqual(false);
        });

        it("Test lockWindowsSize property", function () {
            Rho.System.lockWindowSize = false;
            expect(Rho.System.lockWindowSize).toEqual(false);
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
;