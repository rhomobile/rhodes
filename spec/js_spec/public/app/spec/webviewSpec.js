describe("<WebView specs>", function () {

    /* System property specs */

    it("Browser framework identity string", function () {
        expect(Rho.WebView.getFramework()).isNotEmptyString();
    });

    it("Full screen is false by default", function () {
        expect(Rho.WebView.getFullScreen()).toEqual(false);
    });

    it("Setting full screen mode", function () {
        Rho.WebView.setFullScreen(true);
        expect(Rho.WebView.getFullScreen()).toEqual(true);

        Rho.WebView.setFullScreen(false);
        expect(Rho.WebView.getFullScreen()).toEqual(false);
    });

    it("Reporting native menu items", function () {
        expect(Rho.WebView.getNativeMenu()).isNotEmptyArray();
    });

    //TODO: move to platform dependent test (android)
    it("Zoom is enabled by default", function () {
        expect(Rho.WebView.getEnableZoom()).toEqual(true);
    });

    //TODO: move to platform dependent test (android)
    it("Setting enablement WebView zoom", function () {
        Rho.WebView.setEnableZoom(false);
        expect(Rho.WebView.getEnableZoom()).toEqual(false);

        Rho.WebView.setEnableZoom(true);
        expect(Rho.WebView.getEnableZoom()).toEqual(true);
    });

    //TODO: move to platform dependent test (android)
    it("Showing page loading navigation is true by default", function () {
        expect(Rho.WebView.getEnablePageLoadingIndication()).toEqual(true);
    });

    //TODO: move to platform dependent test (android)
    it("Setting show page loading navigation", function () {
        Rho.WebView.setEnablePageLoadingIndication(false);
        expect(Rho.WebView.getEnablePageLoadingIndication()).toEqual(false);

        Rho.WebView.setEnablePageLoadingIndication(true);
        expect(Rho.WebView.getEnablePageLoadingIndication()).toEqual(true);
    });

    //TODO: move to platform dependent test (android)
    it("Web plugin enabled by default", function () {
        expect(Rho.WebView.getEnableWebPlugins()).toEqual(true);
    });

    //TODO: move to platform dependent test (android)
    it("Setting enablement web plugins", function () {
        Rho.WebView.setEnableWebPlugins(false);
        expect(Rho.WebView.getEnableWebPlugins()).toEqual(false);

        Rho.WebView.setEnableWebPlugins(true);
        expect(Rho.WebView.getEnableWebPlugins()).toEqual(true);
    });

    //TODO: move to platform dependent test (wm)
    it("Navigation timeout is zero milliseconds by default", function () {
        expect(Rho.WebView.getNavigationTimeout()).toEqual(0);
    });

    //TODO: move to platform dependent test (wm)
    it("Setting navigation timeout", function () {
        Rho.WebView.setNavigationTimeout(100);
        expect(Rho.WebView.getNavigationTimeout()).toEqual(100);
    });

    //TODO: move to platform dependent test (wm)
    it("Scroll page technique is FingerScroll by default", function () {
        expect(Rho.WebView.getScrollTechnique()).toEqual("FingerScroll");
    });

    //TODO: move to platform dependent test (wm)
    it("Reporting  default font to use when rendering text in web pages", function () {
        expect(Rho.WebView.getFontFamily()).isNotEmptyString();
    });

    //TODO: move to platform dependent test (wm)
    it("Reporting  user agent", function () {
        expect(Rho.WebView.getUserAgent()).isNotEmptyString();
    });

    //TODO: move to platform dependent test (wm)
    it("Viewport meta tag processing is enabled by default", function () {
        expect(Rho.WebView.getViewportEnabled()).toEqual(true);
    });

    //TODO: move to platform dependent test (wm)
    it("Reporting default viewport width", function () {
        expect(Rho.WebView.getViewportWidth()).isNumberGreaterThenZero();
    });

    //TODO: move to platform dependent test (wm)
    it("Reporting browser cache size  in whole MBs", function () {
        expect(Rho.WebView.getCacheSize()).isNumberGreaterThenZero();
    });

    it("Browser cache is enabled by default", function () {
        expect(Rho.WebView.getEnableCache()).toEqual(true);
    });

    it("Setting enablement browser cache", function () {
        Rho.WebView.setEnableCache(false);
        expect(Rho.WebView.getEnableCache()).toEqual(false);

        Rho.WebView.setEnableCache(true);
        expect(Rho.WebView.getEnableCache()).toEqual(true);
    });

    //TODO: move to platform dependent test (wm)
    it("Setting accept language at HTTP header", function () {
        Rho.WebView.setAcceptLanguage('ru');
        expect(Rho.WebView.getAcceptLanguage()).toEqual('ru');
    });

    //TODO: move to platform dependent test (wm)
    it("Setting zoom page", function () {
        Rho.WebView.setZoomPage(1.5);
        expect(Rho.WebView.getZoomPage()).toEqual(1.5);
    });


    //TODO: move to platform dependent test (wm)
    it("Setting text zoom level", function () {
        Rho.WebView.setTextZoomLevel(1.5);
        expect(Rho.WebView.getTextZoomLevel()).toEqual(1.5);
    });

    it("Reporting active tab index (Zero by default)", function () {
        expect(Rho.WebView.getActiveTab()).toEqual(0);
    });

    //TODO: write specs for module methods
})