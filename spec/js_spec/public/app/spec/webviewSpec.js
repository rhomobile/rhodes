//TODO: restore default webview settings after each test?

describe("<webview module specs>", function () {

    it("Test framework property", function () {
        expect(Rho.WebView.getFramework()).isNotEmptyString();
    });

    it("Test default value of fullScreen property", function () {
        expect(Rho.WebView.getFullScreen()).toEqual(false);
    });

    it("Test fullScreen property", function () {
        Rho.WebView.setFullScreen(true);
        expect(Rho.WebView.getFullScreen()).toEqual(true);
    });

    it("Test default value of activeTab property", function () {
        expect(Rho.WebView.getActiveTab()).toEqual(0);
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
            expect(Rho.WebView.getEnableZoom()).toEqual(true);
        });

        it("Test enablePageLoadingIndication property", function () {
            Rho.WebView.setEnablePageLoadingIndication(false);
            expect(Rho.WebView.getEnablePageLoadingIndication()).toEqual(false);
        });

        it("Test default value of enableWebPlugins property", function () {
            expect(Rho.WebView.getEnableWebPlugins()).toEqual(true);
        });

        it("Test enableWebPlugins property", function () {
            Rho.WebView.setEnableWebPlugins(false);
            expect(Rho.WebView.getEnableWebPlugins()).toEqual(false);
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
            expect(Rho.WebView.getNavigationTimeout()).toEqual(0);
        });

        it("Test navigationTimeout property", function () {
            Rho.WebView.setNavigationTimeout(100);
            expect(Rho.WebView.getNavigationTimeout()).toEqual(100);
        });

        it("Test default value of scrollTechnique property", function () {
            expect(Rho.WebView.getScrollTechnique()).toEqual("FingerScroll");
        });

        it("Test fontFamily property", function () {
            expect(Rho.WebView.getFontFamily()).isNotEmptyString();
        });

        it("Test userAgent property", function () {
            expect(Rho.WebView.getUserAgent()).isNotEmptyString();
        });

        it("Test default value of viewportEnabled property", function () {
            expect(Rho.WebView.getViewportEnabled()).toEqual(true);
        });

        it("Test default value of viewportWidth property", function () {
            expect(Rho.WebView.getViewportWidth()).isNumberGreaterThenZero();
        });

        it("Test cacheSize property", function () {
            expect(Rho.WebView.getCacheSize()).isNumberGreaterThenZero();
        });

        it("Test acceptLanguage property", function () {
            Rho.WebView.setAcceptLanguage('ru');
            expect(Rho.WebView.getAcceptLanguage()).toEqual('ru');
        });

        it("Test zoomPage property", function () {
            Rho.WebView.setZoomPage(1.5);
            expect(Rho.WebView.getZoomPage()).toEqual(1.5);
        });

        it("Test textZoomLevel property", function () {
            Rho.WebView.setTextZoomLevel(1.5);
            expect(Rho.WebView.getTextZoomLevel()).toEqual(1.5);
        });
    }

    if (['ANDROID', 'WINDOWS', 'WINDOWS_DESKTOP'].indexOf(Rho.System.getPlatform()) != -1) {
        it("Test nativeMenu property", function () {
            expect(typeof Rho.WebView.getNativeMenu()).toEqual("object") ;
        });
    }

    if (isAnyButApplePlatform()){
        it("Test default value of enableCache property", function () {
            expect(Rho.WebView.getEnableCache()).toEqual(true);
        });

        it("Test enableCache property", function () {
            Rho.WebView.setEnableCache(false);
            expect(Rho.WebView.getEnableCache()).toEqual(false);
        });
    }

});