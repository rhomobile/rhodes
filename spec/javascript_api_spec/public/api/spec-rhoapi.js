describe("Rho common API", function () {

    var defaultId = 'DEFAULT_OBJECT_ID';

    beforeEach(function(){
        jasmine.Matchers.prototype.toBeNonEmpty = jasmine.Matchers.prototype.toBeTruthy;
    });

    it("is available under Rho namespace", function () {
        expect(Rho).toBeNonEmpty();
    });

    describe("utility module", function () {

        it("is available", function () {
            expect(Rho.util).toBeNonEmpty();
        });

        describe("namespace() function", function () {

            it("is available", function () {
                expect(typeof Rho.util.namespace).toEqual('function');
            });

            it("able to extend namespaces", function () {
                Rho.util.namespace("Rho.Test", {test:123});
                Rho.util.namespace("Rho.Test.A", {a:1});
                Rho.util.namespace("Rho.Test.A", {aa:11});
                Rho.util.namespace("Rho.Test.A.AA", {aaa:111});
                Rho.util.namespace("Rho.Test.B", {b:2});
                Rho.util.namespace("Rho.Test.C", {c:3});

                Rho.util.namespace("Rho.Test.A", {a:'should not override if already defined'});

                expect(Rho.Test.test).toEqual(123);
                expect(Rho.Test.A.a).toEqual(1);
                expect(Rho.Test.A.aa).toEqual(11);
                expect(Rho.Test.A.AA.aaa).toEqual(111);
                expect(Rho.Test.B.b).toEqual(2);
                expect(Rho.Test.C.c).toEqual(3);
            });

        });

        describe("nextId() function", function () {

            it("is available", function () {
                expect(typeof Rho.util.nextId).toEqual('function');
            });

            it("generates next ID value each time", function () {
                var ids = [];
                for (var i = 0; i < 3; i++) {
                    ids[i] = Rho.util.nextId();
                    expect(ids[i]).toBeNonEmpty();
                    for (var j = 0; j < ids.length; j++) {
                        if (i == j) continue;
                        expect(ids[i]).not.toEqual(ids[j]);
                    }
                }
            });

        });

        describe("apiReqFor() function", function () {

            it("is available", function () {
                expect(typeof Rho.util.apiReqFor).toEqual('function');
            });

            it("provides module request function", function () {
                expect(typeof Rho.util.apiReqFor('SOME_MODULE')).toEqual('function');
            });

            it("can perform synchronous request", function () {
                var apiReq = Rho.util.apiReqFor('Rho:System');
                var result = apiReq({
                    instanceId: '0',
                    args: [],
                    method: 'deviceName'
                });
                expect(result).toBeNonEmpty();
                expect(typeof result).toEqual('string');
                expect(result.length).toBeGreaterThan(0);
                jasmine.log('platform is: ' +result);
            });

            // TODO: fix asynchronous requests
            if(false) {

            it("can perform asynchronous request with callback", function () {
                var apiReq = Rho.util.apiReqFor('Rho:System');

                var spy = jasmine.createSpy('callback');

                apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3, spy],
                    method: 'testMethod',
                    valueCallbackIndex: 4
                });

                waitsFor(function () {
                    return spy.wasCalled;
                }, 'Callback should be called', 1000);

                runs(function () {
                    expect(spy).toHaveBeenCalledWith('abc123');
                });
            });

            it("can perform asynchronous request with deferred", function () {
                var apiReq = Rho.util.apiReqFor('Rho:System');

                var dfr = apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3, function(){}],
                    method: 'testMethod',
                    valueCallbackIndex: 4
                });

                expect(dfr).toBeNonEmpty();
                expect(typeof dfr.done).toEqual('function');

                var spy = jasmine.createSpy('callback');

                dfr.done(spy);

                waitsFor(function () {
                    return spy.wasCalled;
                }, 'Callback should be called', 1000);

                runs(function () {
                    expect(spy).toHaveBeenCalledWith('abc123');
                });
            });

            }

        });

    });

    describe("System module", function () {

        it("is available", function () {
            expect(Rho.System).toBeNonEmpty();
        });

        // TODO: there's no default instance of System!
        if (false) {

        it("it have default() method", function () {
            expect(typeof Rho.System['default']).toEqual('function');
        });

        it("can provide default instance via default() method", function () {
            var defInst = Rho.System['default']();
            expect(defInst.constructor.name).toEqual('System');
        });

        it("can enumerate instances", function () {
            var objs = Rho.System.enumerate();

            expect(objs).toBeNonEmpty();
            expect(objs instanceof Array).toBeNonEmpty();
            expect(objs.length).toBeGreaterThan(0);

            for(var i=0; i<objs.length; i++) {
                expect(objs[i].constructor.name).toEqual('System');
                expect(typeof objs[i].getId()).toEqual('string');
                for (var j = 0; j < objs.length; j++) {
                    // they all should have different ids
                    if (i == j) continue;
                    expect(objs[i].getId()).not.toEqual(objs[j].getId());
                }
            }
        });

        describe("as default instance", function () {

            var defInstance;

            beforeEach(function () {
                defInstance = Rho.System['default']();
            });

            it("can get properties", function () {
                var props = defInstance.getProperties();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                defInstance.setProperties({ resolution: '800x600' }, function(props) {
                   expect(props).toBeNonEmpty();
                   expect(typeof props).toEqual('object');

                   expect(typeof props.resolution).toEqual('string');
                   expect(props.resolution).toEqual('800x600');
                });
            });

        });

        describe("as non-default instance", function () {

            var instance = null;

            beforeEach(function(){
                instance = Rho.System.enumerate()[0];
            });

            it("can get properties", function () {
                var props = instance.getProperties();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                instance.setProperties({ resolution: '800x600' }, function(props) {
                   expect(props).toBeNonEmpty();
                   expect(typeof props).toEqual('object');

                   expect(typeof props.resolution).toEqual('string');
                   expect(props.resolution).toEqual('800x600');
                });
            });

        });

        }

    });

    describe('Rho.Database.SQLite3 module', function () {
        it('passes test by Dmitry P.', function() {
            var queryInit = 'DROP TABLE IF EXISTS todo ; CREATE TABLE todo (id INT, task VARCHAR(128))';

            var db = new Rho.Database.SQLite3("path", "local");
            db.execute(queryInit, true, []);
            db.close();

            dbInit();
        });
    });

    describe('Rho.Database.SQLite3 module', function () {
        var db = null;

        beforeEach(function() {
            db = new Rho.Database.SQLite3('path', 'local');
        });

        afterEach(function() {
            db.close();
            db = null;
        });

        // function SQLite3()
        // SQLite3.prototype.close = function(/* optional function */ oResult)
        it('creates SQLite3 object and closes it', function() {
            // look at beforeEach (creates database object) and afterEach (closes it)
        });

        // SQLite3.prototype.startTransaction = function(/* optional function */ oResult)
        // SQLite3.prototype.commitTransaction = function(/* optional function */ oResult)
        it('starts and commits transaction', function() {
            db.startTransaction();
            db.commitTransaction();
        });

        // SQLite3.prototype.startTransaction = function(/* optional function */ oResult)
        // SQLite3.prototype.rollbackTransaction = function(/* optional function */ oResult)
        it('starts and rollbacks transaction', function() {
            db.startTransaction();
            db.rollbackTransaction();
        });

        // SQLite3.prototype.lockDb = function(/* optional function */ oResult)
        // SQLite3.prototype.unlockDb = function(/* optional function */ oResult)
        it('locks and unlocks database', function() {
            db.lockDb();
            db.unlockDb();
        });

        // SQLite3.prototype.import = function(/* const rho::String& */ zipName, /* optional function */ oResult)
        it('imports zip', function() {
            db.import('');
        });

        // SQLite3.prototype.export = function(/* optional function */ oResult)
        it('exports zip', function() {
            db.export();
        });

        // SQLite3.prototype.destroyTables = function(/* const rho::Vector<rho::String>& */ include, /* const rho::Vector<rho::String>& */ exclude, /* optional function */ oResult)
        it('destroys tables', function() {
            db.destroyTables('', '');
        });

        // SQLite3.prototype.isTableExist = function(/* const rho::String& */ tableName, /* optional function */ oResult)
        it('checks for table existence', function() {
            expect(db.isTableExist('')).toBe(false);
        });

        // SQLite3.prototype.isUiWaitForDb = function(/* optional function */ oResult)
        it('checks something strange', function() {
            expect(db.isUiWaitForDb('')).toBe(false);
        });

        // SQLite3.prototype.execute = function(/* const rho::String& */ sqlStmt, /* bool */ isBatch, /* const rho::Vector<rho::String>& */ args, /* optional function */ oResult)
        it('executes SQL statements', function() {
            db.execute('select * from table;');
        });

        // SQLite3.prototype.import = function(/* const rho::String& */ zipName, /* optional function */ oResult)
        it('imports zip', function() {
            db.import('');
        });

        // SQLite3.prototype.export = function(/* optional function */ oResult)
        it('exports zip', function() {
            db.export();
        });

        // SQLite3.prototype.destroyTables = function(/* const rho::Vector<rho::String>& */ include, /* const rho::Vector<rho::String>& */ exclude, /* optional function */ oResult)
        it('destroys tables', function() {
            db.destroyTables('', '');
        });

        // SQLite3.prototype.isTableExist = function(/* const rho::String& */ tableName, /* optional function */ oResult)
        it('checks for table existence', function() {
            expect(db.isTableExist('')).toBe(false);
        });

        // SQLite3.prototype.isUiWaitForDb = function(/* optional function */ oResult)
        it('checks something strange', function() {
            expect(db.isUiWaitForDb('')).toBe(false);
        });

        // SQLite3.prototype.execute = function(/* const rho::String& */ sqlStmt, /* bool */ isBatch, /* const rho::Vector<rho::String>& */ args, /* optional function */ oResult)
        it('executes SQL statements', function() {
            db.execute('DROP TABLE IF EXISTS todo', true, []);
        });

    });

    describe('Rho.Database module', function() {
        var db = null;

        beforeEach(function() {
            db = new Rho.Database_('path', 'local');
        });

        afterEach(function() {
            db.close();
            db = null;
        });


        // function Database()
        // Database.prototype.close = function()
        it('creates Database object and closes it', function() {
        });

        // Database.prototype.startTransaction = function()
        // Database.prototype.commitTransaction = function()
        it('starts and commits transaction', function() {
            db.startTransaction();
            db.commitTransaction();
        });

        // Database.prototype.startTransaction = function()
        // Database.prototype.rollbackTransaction = function()
        it('starts and rollbacks transaction', function() {
            db.startTransaction();
            db.rollbackTransaction();
        });

        // Database.prototype.lockDb = function()
        // Database.prototype.unlockDb = function()
        it('locks and unlocks database', function() {
            db.lockDb();
            db.unlockDb();
        });

        // Database.prototype.isUiWaitForDb = function()
        it('calls isUiWaitForDb', function() {
            db.isUiWaitForDb();
        });

        // Database.prototype.executeSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args)
        it('executes SQL statements', function() {
            db.executeSql('select * from table;');
        });

        // Database.prototype.executeBatchSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args)
        it('executes SQL statements as batch', function() {
            db.executeBatchSql ('select * from table;');
        });

        // Database.prototype.import = function(/* const rho::String& */ zipName)
        it('imports zip', function() {
            db.import('');
        });

        // Database.prototype.export = function()
        it('exports zip', function() {
            db.export();
        });

        // Database.prototype.destroyTable = function(/* const rho::String& */ tableName)
        it('destroys table', function() {
            db.destroyTable('');
        });

        // Database.prototype.destroyTables = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap)
        it('destroys multiple tables', function() {
            db.destroyTable({'include': ''});
        });

        // Database.prototype.isTableExist = function(/* const rho::String& */ tableName)
        it('check for table existence', function() {
            expect(db.isTableExist('')).toEqual(true);
        });

        // Database.prototype.setDoNotBackupAttribute = function(/* bool */ setFlag)
        it('sets "don\'t backup attribute"', function() {
            db.setDoNotBackupAttribute(true);
        });

    });

});
