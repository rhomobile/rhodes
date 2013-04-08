describe("<log module specs>", function () {

    //TODO: add initialing log system before executing each test?

    it("Tests log level property", function () {
        Rho.Log.setLevel(1);
        expect(Rho.Log.getLevel()).toEqual(1);
    });

    it("Tests destination property", function () {
        expect(Rho.Log.setDestination(["file"]));
        expect(Rho.Log.getDestination()).toEqual(["file"]);
    });

    xit("Test default value of including categories property", function () {
        expect(Rho.Log.getIncludeCategories()).toEqual("*");
    });

    it("Test includingCategories property", function () {
        Rho.Log.setIncludeCategories("abc, def");
        expect(Rho.Log.getIncludeCategories()).toEqual("abc, def");
    });

    it("Test excludeCategories property", function () {
        Rho.Log.setExcludeCategories("abc, def");
        expect(Rho.Log.getExcludeCategories()).toEqual("abc, def");
    });

    xit("Test default value of fileSize property", function () {
        expect(Rho.Log.getFileSize()).toEqual(0);
    });

    it("Test fileSize property", function () {
        Rho.Log.setFileSize(1000);
        expect(Rho.Log.getFileSize()).toEqual(1000);
    });

    it("Test default value of filePath property", function () {
        expect(Rho.Log.getFilePath()).toEqual("rholog.txt");
    });

    xit("Test filePath property", function () {
        Rho.Log.setFilePath("someFilename.log");
        expect(Rho.Log.getFilePath()).toEqual("someFilename.log");
    });

    xit("Test default value of memoryPeriod property", function () {
        expect(Rho.Log.getMemoryPeriod()).toEqual(0);
    });

    xit("Test memoryPeriod property", function () {
        Rho.Log.setMemoryPeriod(1000);
        expect(Rho.Log.getMemoryPeriod()).toEqual(1000);
    });

    xit("Test default value of netTrace property", function () {
        expect(Rho.Log.getNetTrace()).toEqual(false);
    });

    //TODO: move to platform dependent tests
    xit("Test netTrace property", function () {
        Rho.Log.setNetTrace(true);
        expect(Rho.Log.getNetTrace()).toEqual(true);
    });

   xit("Test default value of skipPost property", function() {
       expect(Rho.Log.getSkipPost()).toEqual(false);
   });

    xit("Test skipPost property", function() {
        Rho.Log.setSkipPost(true);
        expect(Rho.Log.getSkipPost()).toEqual(true);
    });

    xit("Test default value of excludeFilter property", function(){
        expect(Rho.Log.getExcludeFilter()).toEqual("");
    });

    xit("Test excludeFilter property", function(){
        Rho.Log.setExcludeFilter("abc");
        expect(Rho.Log.getExcludeFilter()).toEqual("abc");

        Rho.Log.setExcludeFilter("def");
        expect(Rho.Log.getExcludeFilter()).toEqual("abc,def");
    });

    xit("Test default value of destinationURI property", function (){
        expect(Rho.Log.getDestinationURI()).toEqual("");
    });

    xit("Test destinationURI property", function (){
        Rho.Log.setDestinationURI("http://localhost");
        expect(Rho.Log.getDestinationURI()).toEqual("http://localhost");
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
