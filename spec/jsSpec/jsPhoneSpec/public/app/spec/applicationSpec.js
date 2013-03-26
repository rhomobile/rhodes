describe("<application specs>", function(){

	it("Path to apps/app folder inside application bundle.", function(){
		expect(Rho.Application.appBundleFolder()).isNotEmptyString();
	});

	it("Path to apps folder inside application bundle.", function(){
        expect(Rho.Application.appsBundleFolder()).isNotEmptyString();
	});

	it("Path to folder where application can write files and create subfolders.", function(){
		expect(Rho.Application.userFolder()).isNotEmptyString();
	});

	it("Path to configuration file.", function(){
		expect(Rho.Application.configPath()).isNotEmptyString();
	});

	it("Path to file with model list.", function(){
		expect(Rho.Application.modelsManifestPath()).isNotEmptyString(0);
	});

	it("Path to folder where database blob files are stored.", function(){
		expect(Rho.Application.databaseBlobFolder()).isNotEmptyString(0);
	});

	it("Path to application public folder.", function(){
		expect(Rho.Application.publicFolder()).isNotEmptyString();
	});

	it("Startup page for your application.", function(){
        Rho.Application.setStartURI('index.html');
		expect(Rho.Application.startURI()).toEqual('index.html');
	});

	it("Settings page URI.", function(){
        Rho.Application.setSettingsPageURI('settings.html');
		expect(Rho.Application.settingsPageURI()).toEqual('settings.html');
	});

	it("Splash screen image display options. This a string with several parameters divided by ';': delay=5;center;hcenter;vcenter;vzoom;hzoom;zoom", function(){
		expect(Rho.Application.splash()).isNotEmptyString();
	});

	it("Version from build time configuration file.", function(){
		expect(Rho.Application.version()).isNotEmptyString();
	});

	it("Define Window caption text.If missed - caption from page used.", function(){
		expect(Rho.Application.title()).isNotEmptyString();
	});

	it("Application name.", function(){
		expect(Rho.Application.name()).isNotEmptyString();
	});

	it("Current application locale.Like 'en', 'ru' etc.", function(){
		expect(Rho.Application.locale()).isNotEmptyString();
	});

	it("Current application country code.", function(){
		expect(Rho.Application.country()).isNotEmptyString();
	});

	it("Bad link URI to navigate in browser.", function(){
        Rho.Application.setBadLinkURI('badLink.html');
		expect(Rho.Application.badLinkURI()).toEqual('badLink.html');
	});

})