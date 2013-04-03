 describe("<application specs>", function(){

	it("Path to apps/app folder inside application bundle", function(){
		expect(Rho.Application.getAppBundleFolder()).isNotEmptyString();
	});

	it("Path to apps folder inside application bundle", function(){
        expect(Rho.Application.getAppsBundleFolder()).isNotEmptyString();
	});

	it("Path to folder where application can write files and create subfolders", function(){
		expect(Rho.Application.getUserFolder()).isNotEmptyString();
	});

	xit("Path to configuration file", function(){
		expect(Rho.Application.getConfigPath()).isNotEmptyString();
	});

	it("Path to file with model list", function(){
		expect(Rho.Application.getModelsManifestPath()).isNotEmptyString(0);
	});

	it("Path to folder where database blob files are stored", function(){
		expect(Rho.Application.getDatabaseBlobFolder()).isNotEmptyString(0);
	});

	it("Path to application public folder", function(){
		expect(Rho.Application.getPublicFolder()).isNotEmptyString();
	});

	it("Startup page for your application", function(){
        Rho.Application.setStartURI('index.html');
		expect(Rho.Application.getStartURI()).toEqual('index.html');
	});

	it("Settings page URI", function(){
        Rho.Application.setSettingsPageURI('settings.html');
		expect(Rho.Application.getSettingsPageURI()).toEqual('settings.html');
	});

	it("Splash screen image display options", function(){
		expect(Rho.Application.getSplash()).isNotEmptyString();
	});

	it("Version from build time configuration file", function(){
		expect(Rho.Application.getVersion()).isNotEmptyString();
	});

    //TODO: Move to platform dependent tests
	it("Define Window caption text.If missed - caption from page used", function(){
        expect(Rho.Application.getTitle()).isNotEmptyString();

        Rho.Application.setTitle('Title');
		expect(Rho.Application.getTitle()).toEqual('Title');
	});

	it("Application name", function(){
		expect(Rho.Application.getName()).isNotEmptyString();
	});
	
    //TODO: Implement Localization for Javascript app
	//it("Current application locale.Like 'en', 'ru' etc", function(){
	//	expect(Rho.Application.getLocale()).isNotEmptyString();
	//});

    //TODO: Implement Localization for Javascript app
	//it("Current application country code", function(){
	//	expect(Rho.Application.getCountry()).isNotEmptyString();
	//});

     //TODO: Move to platform dependent tests
	it("Bad link URI to navigate in browser", function(){
        Rho.Application.setBadLinkURI('badLink.html');
		expect(Rho.Application.getBadLinkURI()).toEqual('badLink.html');
	});

})