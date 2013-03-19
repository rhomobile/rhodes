describe("<system specs>", function(){

    /* system property specs */

	it("Platform name", function(){
        expect( Rho.System.platform()).isNotEmptyString();
	});

	it("Is camera available on the device", function(){
        expect(typeof Rho.System.hasCamera()).toEqual('boolean');
	});

	it("Screen width in logical pixels", function(){
        expect(Rho.System.screenWidth()).isNumberGreaterThenZero();
	});

	it("Screen height in logical pixels", function(){
        expect(Rho.System.screenHeight()).isNumberGreaterThenZero();
	});

	it("Screen width in real pixels", function(){
        expect(Rho.System.realScreenWidth()).isNumberGreaterThenZero();
	});

	it("Screen height in real pixels", function(){
        expect(Rho.System.realScreenHeight()).isNumberGreaterThenZero();
	});

	it("Current screen orientation", function(){
        expect( Rho.System.screenOrientation()).isNotEmptyString();
	});

	it("Horizontal PPI (Pixels Per Inch)", function(){
        expect(Rho.System.ppiX()).isNumberGreaterThenZero();
	});

	it("Vertical PPI (Pixels Per Inch)", function(){
        expect(Rho.System.ppiY()).isNumberGreaterThenZero();
	});

	it("Device phone number", function(){
        expect( Rho.System.phoneNumber()).isNotEmptyString();
	});

	it("Primary email of phone owner (works only on Android)", function(){
        expect( Rho.System.deviceOwnerEmail()).isNotEmptyString();

	});

	it("Name(account name) of phone owner (works only on Android)", function(){
        expect( Rho.System.deviceOwnerName()).isNotEmptyString();
	});

	it("Push notifications device ID which may be used to receive push messages", function(){
        expect( Rho.System.devicePushId()).isNotEmptyString();
	});

	it("Hardware based ID", function(){
        expect( Rho.System.phoneId()).isNotEmptyString();
	});

	it("Name of device", function(){
        expect( Rho.System.deviceName()).isNotEmptyString();
	});

	it("Version OS of device", function(){
        expect( Rho.System.osVersion()).isNotEmptyString();
	});

	it("Current device locale", function(){
        expect( Rho.System.locale()).isNotEmptyString();        
	});

	it("Current device country code", function(){
        expect( Rho.System.country()).isNotEmptyString();
	});

	it("Is application is running on emulator", function(){
		expect(Rho.System.isEmulator()).isBoolean;
	});

	it("Is calendar support available", function(){
		expect(Rho.System.hasCalendar()).isBoolean();
	});

	it("Is device supports Motorola device capabilities", function(){
		expect(Rho.System.isMotorolaDevice()).isBoolean();
	});

	it("The OEM Information string for the terminal", function(){
        expect( Rho.System.oemInfo()).isNotEmptyString();        
	});

	it("The Unique Unit IDentifier for the terminal", function(){
        expect( Rho.System.uuid()).isNotEmptyString();
	});

	it("Setting the application icon to have this badge number (iOS only)", function(){
        Rho.System.applicationIconBadge(1);
		expect(Rho.System.applicationIconBadge()).toEqual(1);
	});

	it("Setting HTTP proxy URI", function(){
        Rho.System.httpProxyURI('http://localhost');
        expect(Rho.System.httpProxyURI()).toEqual('http://localhost');
	});

    it("Locking the window size (only for Windows desktop)", function(){
        Rho.System.lockWindowSize(true);
        expect(Rho.System.lockWindowSize()).toEqual(true);
    });

    it("Unlocking the window size (only for Windows desktop)", function(){
        Rho.System.lockWindowSize(false);
        expect(Rho.System.lockWindowSize()).toEqual(false);
    });

	it("Displaying software keyboard", function(){
		Rho.System.showKeyboard(true);
		expect(Rho.System.showKeyboard()).toEqual(true);
	});

    it("Hiding software keyboard", function(){
        Rho.System.showKeyboard(false);
        expect(Rho.System.showKeyboard()).toEqual(false);
    });

    it("Default port of the local (embedded) HTTP server", function(){
        expect(Rho.System.localServerPort()).toEqual(8080);
    });

	it("Setting port of the local (embedded) HTTP server", function(){
		Rho.System.localServerPort(7777);
		expect(Rho.System.localServerPort()).toEqual(7777);
	});

	it("Get free local server port", function(){
        expect(Rho.System.freeServerPort()).isNumberGreaterThenZero();
	});

    it("Setting screen auto rotate", function(){
        Rho.System.screenAutoRotate(true)
        expect(Rho.System.screenAutoRotate()).toEqual(true);

        Rho.System.screenAutoRotate(false)
        expect(Rho.System.screenAutoRotate()).toEqual(false);
    });

	it("Is device has touch screen", function(){
		expect(Rho.System.hasTouchscreen()).isBoolean();
	});

	it("Is the security token check was failed", function(){
		expect( Rho.System.securityTokenNotPassed()).isBoolean();
	});

	it("Browser framework identity string", function(){
		expect(Rho.System.webviewFramework()).isNotEmptyString();
	});

	it("Setting screen sleeping", function(){
		Rho.System.screenSleeping(true);
		expect(Rho.System.screenSleeping()).toEqual(true);

        Rho.System.screenSleeping(false);
        expect(Rho.System.screenSleeping()).toEqual(false);
	});

/* system method specs */

    it("Command line parameters", function(){
        expect(Rho.System.getStartParams()).isNotEmptyString();
    });

    it("Writing and reading registry value", function(){
        Rho.System.setRegistrySetting('HKEY_CURRENT_USER', 'REG_SZ', 'phone_spec' ,'phone_spec_registry_key', 'test');
        expect(Rho.System.getRegistrySetting('HKEY_CURRENT_USER', 'REG_SZ', 'phone_spec' ,'phone_spec_registry_key')).toEqual('test')
    });

    it("Setting window frame (only Windows desktop)", function(){
        Rho.System.setWindowFrame(1, 1, 200, 200);
    });

    it("Setting window position (only Windows desktop)", function(){
        Rho.System.setWindowPosition(1, 1);
    });

    it("Bring application window to the top of screen", function(){
        Rho.System.bringToFront();
    });


})