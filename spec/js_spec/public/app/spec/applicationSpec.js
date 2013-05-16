var appEventsTest = {};

appEventsTest.ajax = function(url)
{
	var ajax = new XMLHttpRequest();
	//ajax.onreadystatechange = function () {} //Async call
	ajax.open("GET", url, false); //Sync Call
	ajax.send();
	return ajax.responseText;
};

appEventsTest.eventCallback = function(event,eventData)
{
	appEventsTest.lastEvent = event;
    appEventsTest.lastEventData = eventData;
	appEventsTest.eventList.push(event);
	appEventsTest.callbackFired = true;
};

appEventsTest.setCallback = function ()
{
	appEventsTest.ajax('/app/AppEventsTest/set_callback');
};

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

    //TODO: implement spec for setActivationNotify with callback on all platforms

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

describe('<application module events specs>', function() {
    if (isApplePlatform()) {
        beforeEach(function() {
            Rho.AppEvents.simulateEvent(Rho.AppEvents.APP_EVENT_ACTIVATED);
            Rho.AppEvents.simulateEvent(Rho.AppEvents.APP_EVENT_UICREATED);
            appEventsTest.callbackFired = false;
            appEventsTest.lastEvent = "";
            appEventsTest.lastEventData = {};
            appEventsTest.eventList = [];
        });
        
        it('Should fire callback', function(){
            runs(function ()
            {
                appEventsTest.setCallback();
                // state is "activated" should be changed
                Rho.AppEvents.simulateEvent(Rho.AppEvents.APP_EVENT_DEACTIVATED);
            });
            
            waitsFor(function ()
            {
                return appEventsTest.callbackFired;
            }, "the callback didnt fire", 750);
            
            runs(function ()
            {
                expect(appEventsTest.lastEvent).toEqual(Rho.AppEvents.APP_EVENT_DEACTIVATED);
            });
        });

        it('Should fire callback not once', function(){
            events = [
                Rho.AppEvents.APP_EVENT_DEACTIVATED,
                Rho.AppEvents.APP_EVENT_UIDESTROYED,
                Rho.AppEvents.APP_EVENT_UICREATED,
                Rho.AppEvents.APP_EVENT_ACTIVATED
            ];

            runs(function ()
            {
                appEventsTest.setCallback();
                for(var i = 0; i < events.length; i++)
                {
                    Rho.AppEvents.simulateEvent(events[i]);
                }
            });
            
            waitsFor(function ()
            {
                return appEventsTest.eventList.length == events.length;
            }, "the callback didnt fire", 750);
            
            runs(function ()
            {
                expect(appEventsTest.eventList).toEqual(events);
            });
        });

        it('Should not duplicate callbacks', function(){
            events = [
                Rho.AppEvents.APP_EVENT_DEACTIVATED,
                Rho.AppEvents.APP_EVENT_UIDESTROYED,
                Rho.AppEvents.APP_EVENT_ACTIVATED,
                Rho.AppEvents.APP_EVENT_UICREATED
            ];

            runs(function ()
            {
                appEventsTest.setCallback();
                for(var i = 0; i < events.length; i++)
                {
                    Rho.AppEvents.simulateEvent(events[i]);
                    Rho.AppEvents.simulateEvent(events[i]);
                }
            });
            
            waitsFor(function ()
            {
                return appEventsTest.eventList.length == events.length;
            }, "the callback didnt fire", 750);
            
            runs(function ()
            {
                expect(appEventsTest.eventList).toEqual(events);
            }); 
        });

        it('Should handle conflicts', function(){
            template = {
              "intProp" : ['128', '42'],
              "stringProp" : ["ohr","abc"]
            };

            runs(function ()
            {
                appEventsTest.setCallback();
                Rho.AppEvents.addConflictInt("intProp",64,128);
                Rho.AppEvents.addConflictString("stringProp","rho","ohr");
                Rho.AppEvents.simulateConflicts();
            });
            
            waitsFor(function ()
            {
                return appEventsTest.callbackFired;
            }, "the callback didnt fire", 750);
            
            runs(function ()
            {
                expect(appEventsTest.lastEventData).toEqual(template);
            });
        });
    }
});
