//TODO: restore default webview settings after each test?

describe("<webview module specs>", function () {

    it("Test framework property", function () {
        expect(Rho.WebView.framework).isNotEmptyString();
    });

    it("Test default value of fullScreen property", function () {
        expect(Rho.WebView.fullScreen).toEqual(false);
    });

    it("Test fullScreen property", function () {
        Rho.WebView.fullScreen = true;
        expect(Rho.WebView.fullScreen).toEqual(true);
    });

    it("Test default value of activeTab property", function () {
        expect(Rho.WebView.activeTab).toEqual(0);
    });

    xit("Test refresh method", function () {
        expect(Rho.WebView.refresh(-1));
    });

    xit("Test navigate method", function () {
        expect(Rho.WebView.navigate("http://localhost", -1));
    });

    xit("Test navigateBack method", function () {
        expect(Rho.WebView.navigateBack(-1));
    });

    it("Test currentLocation method", function () {
        expect(Rho.WebView.currentLocation(-1)).isNotEmptyString();
    });

    //TODO: can we use result executed js in spec
    xit("Test executeJavascript method", function () {
        expect(Rho.WebView.executeJavascript("function(){ var a = 10;}"));
    });

    //TODO: not implemented for motorola solutions webkit
    it("Test setCookie method", function () {
        expect(Rho.WebView.setCookie("http://localhost", "specCookie=123"));
    });


    /* ----------          platform dependent specs          ---------- */


    if (isAndroidPlatform()) {
        it("Test default value of enableZoom property", function () {
            expect(Rho.WebView.enableZoom).toEqual(true);
        });

        it("Test enablePageLoadingIndication property", function () {
            Rho.WebView.enablePageLoadingIndication = false;
            expect(Rho.WebView.enablePageLoadingIndication).toEqual(false);
        });

        it("Test default value of enableWebPlugins property", function () {
            expect(Rho.WebView.enableWebPlugins).toEqual(true);
        });

        it("Test enableWebPlugins property", function () {
            Rho.WebView.enableWebPlugins = false;
            expect(Rho.WebView.enableWebPlugins).toEqual(false);
        });

        //TODO: add check on existing saved file
        it("Test save method", function () {
            expect(Rho.WebView.save("jpeg", "someFilename", -1));
        });
    }

    if (isAndroidOrApplePlatform()) {
        it("Test currentURL method", function () {
            expect(Rho.WebView.currentURL(-1)).isNotEmptyString();
        });
    }

    if (isWindowsMobilePlatform()) {
        it("Test default value of navigationTimeout property", function () {
            expect(Rho.WebView.navigationTimeout).toEqual(0);
        });

        it("Test navigationTimeout property", function () {
            Rho.WebView.navigationTimeout = 100;
            expect(Rho.WebView.navigationTimeout).toEqual(100);
        });

        it("Test default value of scrollTechnique property", function () {
            expect(Rho.WebView.scrollTechnique).toEqual("FingerScroll");
        });

        it("Test fontFamily property", function () {
            expect(Rho.WebView.fontFamily).isNotEmptyString();
        });

        it("Test userAgent property", function () {
            expect(Rho.WebView.userAgent).isNotEmptyString();
        });

        it("Test default value of viewportEnabled property", function () {
            expect(Rho.WebView.viewportEnabled).toEqual(true);
        });

        it("Test default value of viewportWidth property", function () {
            expect(Rho.WebView.viewportWidth).isNumberGreaterThenZero();
        });

        it("Test cacheSize property", function () {
            expect(Rho.WebView.cacheSize).isNumberGreaterThenZero();
        });

        it("Test acceptLanguage property", function () {
            Rho.WebView.acceptLanguage = 'ru';
            expect(Rho.WebView.acceptLanguage).toEqual('ru');
        });

        it("Test zoomPage property", function () {
            Rho.WebView.zoomPage = 1.5;
            expect(Rho.WebView.zoomPage).toEqual(1.5);
        });

        it("Test textZoomLevel property", function () {
            Rho.WebView.textZoomLevel = 1.5;
            expect(Rho.WebView.textZoomLevel).toEqual(1.5);
        });
    }

    if (['ANDROID', 'WINDOWS', 'WINDOWS_DESKTOP'].indexOf(Rho.System.platform) != -1) {
        it("Test nativeMenu property", function () {
            expect(typeof Rho.WebView.nativeMenu).toEqual("object") ;
        });
    }

    if (isAnyButApplePlatform()){
        it("Test default value of enableCache property", function () {
            expect(Rho.WebView.enableCache).toEqual(true);
        });

        it("Test enableCache property", function () {
            Rho.WebView.enableCache = false;
            expect(Rho.WebView.enableCache).toEqual(false);
        });
    }

});