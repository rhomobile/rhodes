describe("<application module specs>", function () {

    it("Test appBundleFolder property", function () {
        expect(Rho.Application.appBundleFolder).isNotEmptyString();
    });

    it("Test appsBundleFolder proeprty", function () {
        expect(Rho.Application.appsBundleFolder).isNotEmptyString();
    });

    it("Test userFolder property", function () {
        expect(Rho.Application.userFolder).isNotEmptyString();
    });

    xit("Test configPath property", function () {
        expect(Rho.Application.configPath).isNotEmptyString();
    });

    it("Test modelsManifestPath property", function () {
        expect(Rho.Application.modelsManifestPath).isNotEmptyString(0);
    });

    it("Test databaseBlobFolder property", function () {
        expect(Rho.Application.databaseBlobFolder).isNotEmptyString(0);
    });

    it("Test publicFolder property", function () {
        expect(Rho.Application.publicFolder).isNotEmptyString();
    });

    it("Test startURI property", function () {
        Rho.Application.startURI = 'index.html';
        expect(Rho.Application.startURI).toEqual('index.html');
    });

    it("Test settingsPageURI property", function () {
        Rho.Application.settingsPageURI = 'settings.html';
        expect(Rho.Application.settingsPageURI).toEqual('settings.html');
    });

    it("Test splash property", function () {
        expect(Rho.Application.splash).isNotEmptyString();
    });

    it("Test version property", function () {
        expect(Rho.Application.version).isNotEmptyString();
    });

    it("Test name property", function () {
        expect(Rho.Application.getName()).isNotEmptyString();
    });



    //TODO: Implement Localization for Javascript app
    //it("Current application locale.Like 'en', 'ru' etc", function(){
    //	expect(Rho.Application.getLocale()).isNotEmptyString();
    //});

    //TODO: Implement Country for Javascript app
    //it("Current application country code", function(){
    //	expect(Rho.Application.getCountry()).isNotEmptyString();
    //});

    //TODO: implement modelFolderPath method

    //TODO: implement databaseFilePath method

    //TODO: implement expandDatabaseBlobFilePath method

    //TODO: implement quit method

    //TODO: implement modelFolderPath method

    it("Test minimize method", function () {
        expect(Rho.Application.minimize).not.toThrow();
    });

    it("Test restore method", function () {
        expect(Rho.Application.restore).not.toThrow();
    });

    //TODO: implement spec for setActivationNotify with callback

    //TODO: implement spec for getRhoPlatformVersion with callback


    if(isWindowsMobileOrAndroidPlatform()){
        it("Bad link URI to navigate in browser", function () {
            Rho.Application.badLinkURI = 'badLink.html';
            expect(Rho.Application.badLinkURI).toEqual('badLink.html');
        });
    }

    if (['WINDOWS_DESKTOP', 'WINDOWS'].indexOf(Rho.System.platform) != -1) {

        it("Test title property", function () {
            expect(Rho.Application.title()).isNotEmptyString();

            Rho.Application.title = 'Title';
            expect(Rho.Application.title).toEqual('Title');
        });

    }

});