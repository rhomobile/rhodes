describe("<log module specs>", function () {

    //TODO: add initialing log system before executing each test?

    it("Tests log level property", function () {
        Rho.Log.level = 1;
        expect(Rho.Log.level).toEqual(1);
    });

    it("Tests destination property", function () {
        expect(Rho.Log.destination = ["file"]);
        expect(Rho.Log.destination).toEqual(["file"]);
    });

    xit("Test default value of including categories property", function () {
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

    xit("Test default value of fileSize property", function () {
        expect(Rho.Log.fileSize()).toEqual(0);
    });

    it("Test fileSize property", function () {
        Rho.Log.fileSize = 1000;
        expect(Rho.Log.fileSize).toEqual(1000);
    });

    it("Test default value of filePath property", function () {
        expect(Rho.Log.filePath).toEqual("rholog.txt");
    });

    xit("Test filePath property", function () {
        Rho.Log.filePath = "someFilename.log";
        expect(Rho.Log.filePath).toEqual("someFilename.log");
    });

    xit("Test default value of memoryPeriod property", function () {
        expect(Rho.Log.memoryPeriod).toEqual(0);
    });

    xit("Test memoryPeriod property", function () {
        Rho.Log.memoryPeriod = 1000;
        expect(Rho.Log.memoryPeriod).toEqual(1000);
    });

    xit("Test default value of netTrace property", function () {
        expect(Rho.Log.netTrace).toEqual(false);
    });

    //TODO: move to platform dependent tests
    xit("Test netTrace property", function () {
        Rho.Log.netTrace = true;
        expect(Rho.Log.netTrace).toEqual(true);
    });

   xit("Test default value of skipPost property", function() {
       expect(Rho.Log.skipPost).toEqual(false);
   });

    xit("Test skipPost property", function() {
        Rho.Log.skipPost = true;
        expect(Rho.Log.skipPost()).toEqual(true);
    });

    xit("Test default value of excludeFilter property", function(){
        expect(Rho.Log.excludeFilter).toEqual("");
    });

    xit("Test excludeFilter property", function(){
        Rho.Log.excludeFilter = "abc";
        expect(Rho.Log.excludeFilter).toEqual("abc");

        Rho.Log.excludeFilter = "def";
        expect(Rho.Log.excludeFilter).toEqual("abc,def");
    });

    xit("Test default value of destinationURI property", function (){
        expect(Rho.Log.destinationURI).toEqual("");
    });

    xit("Test destinationURI property", function (){
        Rho.Log.destinationURI = "http://localhost";
        expect(Rho.Log.destinationURI).toEqual("http://localhost");
    });

    //TODO: test log file contains the string
    xit("Test trace method", function(){
        Rho.Log.trace("test trace message", "test");
    });

    //TODO: test log file contains the string
    xit("Test info method", function(){
        Rho.Log.info("test info message", "test");
    });

    //TODO: test log file contains the string
    xit("Test warning method", function(){
        Rho.Log.warning("test warning message", "test");
    });

    //TODO: test log file contains the string
    xit("Test error method", function(){
        Rho.Log.error("test error message", "test");
    });

    //TODO: test log file contains the string
    xit("Test fatalError method", function(){
        Rho.Log.fatalError("test fatal error message", "test");
    });

    //TODO: add test on receiving file
    //TODO: add test on callback
    xit("Test sendLogFile method", function(){
        Rho.Log.sendLogFile();
    });

    //TODO: how test it?
    xit("Test showLog method", function(){
        Rho.Log.showLog();
    });

    //TODO: test that log file is empty
    xit("Test cleanLogFile method", function() {
        Rho.Log.cleanLogFile();
    });

    it("Test readLogFile method", function() {
        expect(Rho.Log.readLogFile(1000).length).toEqual(1000);
    });
});
