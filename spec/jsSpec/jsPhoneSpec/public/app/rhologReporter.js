(function () {
    if (!jasmine) {
        throw new Exception("jasmine library does not exist in global namespace!");
    }

    /**
     * Reporter that outputs spec results to the rho log file.
     *
     * Usage:
     *
     * jasmine.getEnv().addReporter(new jasmine.RhologReporter());
     * jasmine.getEnv().execute();
     */
    var RhologReporter = function () {
        this.started = false;
        this.finished = false;
    };

    RhologReporter.prototype = {
        reportRunnerResults: function (runner) {
            var dur = (new Date()).getTime() - this.startTime;
            var failed = this.executedSpecs - this.passedSpecs;
            var specStr = this.executedSpecs + (this.executedSpecs === 1 ? " spec, " : " specs, ");
            var failStr = failed + (failed === 1 ? " failure in " : " failures in ");
            this.log("I'm finished : " + specStr + failStr + (dur / 1000) + "s.");
            this.finished = true;
        },


        reportRunnerStarting: function (runner) {
            this.started = true;
            this.startTime = (new Date()).getTime();
            this.executedSpecs = 0;
            this.passedSpecs = 0;
            this.log("I'm started");

        },

        reportSpecResults: function (spec) {
            this.executedSpecs++;
            var resultText = "Failed.";
            if (spec.results().passed()) {
                this.passedSpecs++;
                resultText = "Passed.";
            }
            this.log(spec.suite.description + ' : ' + spec.description + ' : ' + resultText);
        },

        log: function (str) {
            Rho.Log.info(str, 'Jasmine specRunner')
        }
    };

    jasmine.RhologReporter = RhologReporter;
})();