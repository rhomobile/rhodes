describe("<application module specs>", function () {

    it("Test appBundleFolder property", function () {
        expect(Rho.Application.getAppBundleFolder()).isNotEmptyString();
    });

    it("Test appsBundleFolder proeprty", function () {
        expect(Rho.Application.getAppsBundleFolder()).isNotEmptyString();
    });

    it("Test userFolder property", function () {
        expect(Rho.Application.getUserFolder()).isNotEmptyString();
    });

    xit("Test configPath property", function () {
        expect(Rho.Application.getConfigPath()).isNotEmptyString();
    });

    it("Test modelsManifestPath property", function () {
        expect(Rho.Application.getModelsManifestPath()).isNotEmptyString(0);
    });

    it("Test databaseBlobFolder property", function () {
        expect(Rho.Application.getDatabaseBlobFolder()).isNotEmptyString(0);
    });

    it("Test publicFolder property", function () {
        expect(Rho.Application.getPublicFolder()).isNotEmptyString();
    });

    it("Test startURI property", function () {
        Rho.Application.setStartURI('index.html');
        expect(Rho.Application.getStartURI()).toEqual('index.html');
    });

    it("Test settingsPageURI property", function () {
        Rho.Application.setSettingsPageURI('settings.html');
        expect(Rho.Application.getSettingsPageURI()).toEqual('settings.html');
    });

    it("Test splash property", function () {
        expect(Rho.Application.getSplash()).isNotEmptyString();
    });

    it("Test version property", function () {
        expect(Rho.Application.getVersion()).isNotEmptyString();
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
            Rho.Application.setBadLinkURI('badLink.html');
            expect(Rho.Application.getBadLinkURI()).toEqual('badLink.html');
        });

    }

    if (['WINDOWS_DESKTOP', 'WINDOWS'].indexOf(Rho.System.getPlatform()) != -1) {

        it("Test title property", function () {
            expect(Rho.Application.getTitle()).isNotEmptyString();

            Rho.Application.setTitle('Title');
            expect(Rho.Application.getTitle()).toEqual('Title');
        });

    }

});