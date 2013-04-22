describe("<log module specs>", function () {
    var originalLogSettings = {};

    beforeEach(function () {
        originalLogSettings = {};
        originalLogSettings.logLevel = Rho.Log.level;
        originalLogSettings.destination = Rho.Log.destination;
        originalLogSettings.includeCategories = Rho.Log.includeCategories;
        originalLogSettings.excludeCategories = Rho.Log.excludeCategories;
        originalLogSettings.fileSize = Rho.Log.fileSize;
        originalLogSettings.filePath = Rho.Log.filePath;
        originalLogSettings.memoryPeriod = Rho.Log.memoryPeriod;
        originalLogSettings.netTrace = Rho.Log.netTrace;
        originalLogSettings.skipPost = Rho.Log.skipPost;
        originalLogSettings.excludeFilter = Rho.Log.excludeFilter;
        originalLogSettings.destinationURI = Rho.Log.destinationURI;
    });

    afterEach(function () {
        Rho.Log.logLevel = originalLogSettings.level;
        Rho.Log.destination = originalLogSettings.destination;
        Rho.Log.includeCategories = originalLogSettings.includeCategories;
        Rho.Log.excludeCategories = originalLogSettings.excludeCategories;
        Rho.Log.fileSize = originalLogSettings.fileSize;
        Rho.Log.filePath = originalLogSettings.filePath;
        Rho.Log.memoryPeriod = originalLogSettings.memoryPeriod;
        Rho.Log.netTrace = originalLogSettings.netTrace;
        Rho.Log.skipPost = originalLogSettings.skipPost;
        Rho.Log.excludeFilter = originalLogSettings.excludeFilter;
        Rho.Log.destinationURI = originalLogSettings.destinationURI;
    });

    it("Tests log level property", function () {
        Rho.Log.level = 1;
        expect(Rho.Log.level).toEqual(1);
    });

    it("Tests destination property", function () {
        expect(Rho.Log.destination = ["file"]);
        expect(Rho.Log.destination).toEqual(["file"]);
    });

    it("Test default value of including categories property", function () {
        expect(Rho.Log.includeCategories).toEqual("*");
    });

    it("Test includingCategories property", function () {
        Rho.Log.includeCategories = "abc, def";
        expect(Rho.Log.includeCategories).toEqual("abc, def");
    });

    it("Test excludeCategories property", function () {
        Rho.Log.excludeCategories = "abc, def";
        expect(Rho.Log.excludeCategories).toEqual("abc, def");
    });

    it("Test default value of fileSize property", function () {
        expect(Rho.Log.fileSize).toEqual(0);
    });

    it("Test fileSize property", function () {
        Rho.Log.fileSize = 10000;
        expect(Rho.Log.fileSize).toEqual(10000);
    });

    it("Test default value of filePath property", function () {
        var pathPieces = Rho.Log.filePath.split("/");
        expect(pathPieces[pathPieces.length - 1]).toEqual("rholog.txt");
    });

    it("Test filePath property", function () {
        Rho.Log.filePath = "someFilename.log";
        expect(Rho.Log.filePath).toEqual("someFilename.log");
    });

    it("Test default value of memoryPeriod property", function () {
        expect(Rho.Log.memoryPeriod).toEqual(0);
    });

    it("Test memoryPeriod property", function () {
        Rho.Log.memoryPeriod = 1000;
        expect(Rho.Log.memoryPeriod).toEqual(1000);
    });

    it("Test default value of skipPost property", function () {
        expect(Rho.Log.skipPost).toEqual(false);
    });

    it("Test skipPost property", function () {
        Rho.Log.skipPost = true;
        expect(Rho.Log.skipPost).toEqual(true);
    });

    it("Test default value of excludeFilter property", function () {
        expect(Rho.Log.excludeFilter).toEqual("");
    });

    it("Test excludeFilter property", function () {
        Rho.Log.excludeFilter = "abc";
        expect(Rho.Log.excludeFilter).toEqual("abc");

        Rho.Log.excludeFilter = "def";
        expect(Rho.Log.excludeFilter).toEqual("abc,def");
    });

    it("Test default value of destinationURI property", function () {
        expect(Rho.Log.destinationURI).toEqual("");
    });

    it("Test destinationURI property", function () {
        Rho.Log.destinationURI = "http://localhost";
        expect(Rho.Log.destinationURI).toEqual("http://localhost");
    });

    //TODO: test log file contains the string
    it("Test trace method", function () {
        Rho.Log.trace("test trace message", "test");
    });

    //TODO: test log file contains the string
    it("Test info method", function () {
        Rho.Log.info("test info message", "test");
    });

    //TODO: test log file contains the string
    it("Test warning method", function () {
        Rho.Log.warning("test warning message", "test");
    });

    //TODO: test log file contains the string
    it("Test error method", function () {
        Rho.Log.error("test error message", "test");
    });

    //TODO: test log file contains the string
    //it("Test fatalError method", function () {
    //    Rho.Log.fatalError("test fatal error message", "test");
    //});

    //TODO: add test on receiving file
    //TODO: add test on callback
    xit("Test sendLogFile method", function () {
        Rho.Log.sendLogFile();
    });

    //TODO: how test it?
    xit("Test showLog method", function () {
        Rho.Log.showLog();
    });

    //TODO: test that log file is empty
    xit("Test cleanLogFile method", function () {
        Rho.Log.cleanLogFile();
    });

    it("Test readLogFile method", function () {
        expect(Rho.Log.readLogFile(1000).length).toEqual(1000);
    });


    if (isAndroidOrApplePlatform() || isWindowsPhone8Platform()) {
        it("Test default value of netTrace property", function () {
            expect(Rho.Log.netTrace).toEqual(false);
        });

        it("Test netTrace property", function () {
            Rho.Log.netTrace = true;
            expect(Rho.Log.netTrace).toEqual(true);
        });

    }
});
