// "http://rhodes-store-server.heroku.com/application";

function waitsForSpies(spy, msg, timeout) {
    timeout = timeout || 1000;
    var spies = $.isArray(spy) ? spy : [spy];
    for(var i in spies) {
        spies[i].refCallCount = spies[i].refCallCount || 0;
        //jasmine.log(spies[i].identity +': ' +spies[i].refCallCount.toString() +' = ' +spies[i].callCount.toString());
    }
    waitsFor(function(){
        for (var i in spies) {
            //jasmine.log(spies[i].identity +': ' +spies[i].refCallCount.toString() +' <? ' +spies[i].callCount.toString());
            if (spies[i].refCallCount < spies[i].callCount) {
                spies[i].refCallCount = spies[i].callCount;
                return true;
            }
        }
        return false;
    }, msg, timeout);
}

beforeEach(function() {

    userlogin = "testUserToFailAuth";
    userpass = "userpass";
    wrongpass = "wrongpass";

    rhosync = RhoSync;
    notified = false;
    notify = function(evt, obj){
        jasmine.log(evt.type + ': ' + $.toJSON(obj));
        notified = true;
    };
});
