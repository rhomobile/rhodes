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

    describe('Rho.Application module', function() {
        it('returns path to database file', function() {
            expect(Rho.Application.databaseFilePath('local')).toMatch(/.*syncdblocal\.sqlite$/);
        });
    });

    describe('Rho.Database.SQLite3 module', function () {
        var db = null;

        beforeEach(function() {
            db = new Rho.Database.SQLite3(Rho.Application.databaseFilePath('local'), 'local');
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
            db.execute('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));', true);

            db.startTransaction();
            db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [10, 'ten', 'TEN']);
            expect(db.execute('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
            db.commitTransaction();
            expect(db.execute('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
        });

        // SQLite3.prototype.startTransaction = function(/* optional function */ oResult)
        // SQLite3.prototype.rollbackTransaction = function(/* optional function */ oResult)
        it('starts and rollbacks transaction', function() {
            db.execute('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));', true);

            db.startTransaction();
            db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [10, 'ten', 'TEN']);
            expect(db.execute('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
            db.rollbackTransaction();
            expect(db.execute('SELECT * FROM t;')).toEqual([]);
        });

        // SQLite3.prototype.lockDb = function(/* optional function */ oResult)
        // SQLite3.prototype.unlockDb = function(/* optional function */ oResult)
        it('locks and unlocks database', function() {
            db.lockDb();
            db.execute('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));', true);
            db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [10, 'ten', 'TEN']);
            db.unlockDb();
            expect(db.execute('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
        });

        // SQLite3.prototype.destroyTables = function(/* const rho::Vector<rho::String>& */ include, /* const rho::Vector<rho::String>& */ exclude, /* optional function */ oResult)
        describe('destroys tables', function() {
            var makeTest = function(name, include, exclude, remaining) {
                var contains = function(array, item) {
                    for (var i = 0; i < array.length; ++i) {
                        if (array[i] === item) {
                            return true;
                        }
                    }
                    return false;
                };

                it(name, function() {
                    for (var i = 1; i < 5; ++i) {
                        db.execute('CREATE TABLE IF NOT EXISTS t' + i + '(x INTEGER);');
                    }

                    db.destroyTables(include, exclude);

                    var actual = [];
                    for (var i = 1; i < 5; ++i) {
                        if (db.isTableExist('t' + i)) {
                            actual.push('t' + i);
                        }
                    }
                    expect(actual).toEqual(remaining);
                });
            };

            // names of tables to delete            included      excluded      remaining
            makeTest('all'                       , [          ], [          ], [                ]);
            makeTest('all but excluded'          , [          ], ['t2', 't1'], ['t1', 't2'      ]);
            makeTest('included only'             , ['t2', 't1'], [          ], ['t3', 't4'      ]);
            makeTest('included only but excluded', ['t2', 't1'], ['t2', 't3'], ['t2', 't3', 't4']);
        });

        // SQLite3.prototype.isTableExist = function(/* const rho::String& */ tableName, /* optional function */ oResult)
        it('checks for table existence', function() {
            db.execute('DROP TABLE IF EXISTS t;');
            expect(db.isTableExist('t')).toBe(false);

            db.execute('CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');
            expect(db.isTableExist('t')).toBe(true);

            db.execute('DROP TABLE IF EXISTS t;');
            expect(db.isTableExist('t')).toBe(false);
        });

        // SQLite3.prototype.isUiWaitForDb = function(/* optional function */ oResult)
        it('checks something strange', function() {
            expect(db.isUiWaitForDb()).toBe(false);
        });

        // SQLite3.prototype.execute = function(/* const rho::String& */ sqlStmt, /* bool */ isBatch, /* const rho::Vector<rho::String>& */ args, /* optional function */ oResult)
        it('executes SQL statements', function() {
            db.execute('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));', true);
            db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [10, 'ten', 'TEN']);
            db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [11, 'eleven', 'ELEVEN']);
            expect(db.execute('SELECT * FROM t ORDER BY x DESC;')).toEqual([{x: 11, y: 'eleven', z: 'ELEVEN'}, {x: 10, y: 'ten', z: 'TEN'}]);
        });
    });

    describe('Rho.Database module', function() {
        var db = null;

        beforeEach(function() {
            db = new Rho.Database(Rho.Application.databaseFilePath('local'), 'local');
        });

        afterEach(function() {
            db.close();
            db = null;
        });


        // function Database()
        // Database.prototype.close = function()
        it('creates Database object and closes it', function() {
            // look at beforeEach and afterEach calls above
        });

        // Database.prototype.startTransaction = function()
        // Database.prototype.commitTransaction = function()
        it('starts and commits transaction', function() {
            db.executeBatchSql('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');

            db.startTransaction();
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [10, 'ten', 'TEN']);
            expect(db.executeSql('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
            db.commitTransaction();
            expect(db.executeSql('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
        });

        // Database.prototype.startTransaction = function()
        // Database.prototype.rollbackTransaction = function()
        it('starts and rollbacks transaction', function() {
            db.executeBatchSql('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');

            db.startTransaction();
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [10, 'ten', 'TEN']);
            expect(db.executeSql('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
            db.rollbackTransaction();
            expect(db.executeSql('SELECT * FROM t;')).toEqual([]);
        });

        // Database.prototype.lockDb = function()
        // Database.prototype.unlockDb = function()
        it('locks and unlocks database', function() {
            db.lockDb();
            db.executeBatchSql('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [10, 'ten', 'TEN']);
            db.unlockDb();
            expect(db.executeSql('SELECT * FROM t;')).toEqual([{x: 10, y: 'ten', z: 'TEN'}]);
        });

        // Database.prototype.isUiWaitForDb = function()
        it('calls isUiWaitForDb', function() {
            expect(db.isUiWaitForDb()).toBe(false);
        });

        // Database.prototype.executeSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args)
        it('executes SQL statements', function() {
            db.executeSql('DROP TABLE IF EXISTS t;');
            db.executeSql('CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [10, 'ten', 'TEN']);
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [11, 'eleven', 'ELEVEN']);
            expect(db.executeSql('SELECT * FROM t ORDER BY x DESC;')).toEqual([{x: 11, y: 'eleven', z: 'ELEVEN'}, {x: 10, y: 'ten', z: 'TEN'}]);
        });

        // Database.prototype.executeBatchSql = function(/* const rho::String& */ sqlStmt, /* const rho::Vector<rho::String>& */ args)
        it('executes SQL statements as batch', function() {
            db.executeBatchSql('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [10, 'ten', 'TEN']);
            db.executeSql('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', [11, 'eleven', 'ELEVEN']);
            expect(db.executeSql('SELECT * FROM t ORDER BY x DESC;')).toEqual([{x: 11, y: 'eleven', z: 'ELEVEN'}, {x: 10, y: 'ten', z: 'TEN'}]);
        });

        // Database.prototype.destroyTable = function(/* const rho::String& */ tableName)
        it('destroys table', function() {
            db.executeBatchSql('CREATE TABLE IF NOT EXISTS t1(x INTEGER); CREATE TABLE IF NOT EXISTS t2(x INTEGER);');

            db.destroyTable('t1');
            db.destroyTable('t3');

            expect(db.isTableExist('t1')).toBe(false);
            expect(db.isTableExist('t2')).toBe(true);
            expect(db.isTableExist('t3')).toBe(false);
        });

        // Database.prototype.destroyTables = function(/* const rho::Hashtable<rho::String, rho::String>& */ propertyMap)
        describe('destroys multiple tables', function() {
            var makeTest = function(name, include, exclude, remaining) {
                var contains = function(array, item) {
                    for (var i = 0; i < array.length; ++i) {
                        if (array[i] === item) {
                            return true;
                        }
                    }
                    return false;
                };

                it(name, function() {
                    for (var i = 1; i < 5; ++i) {
                        db.executeSql('CREATE TABLE IF NOT EXISTS t' + i + '(x INTEGER);');
                    }

                    db.destroyTables({'include': include, 'exclude': exclude});

                    var actual = [];
                    for (var i = 1; i < 5; ++i) {
                        if (db.isTableExist('t' + i)) {
                            actual.push('t' + i);
                        }
                    }
                    expect(actual).toEqual(remaining);
                });
            };

            // names of tables to delete            included      excluded      remaining
            makeTest('all'                       , [          ], [          ], [                ]);
            makeTest('all but excluded'          , [          ], ['t2', 't1'], ['t1', 't2'      ]);
            makeTest('included only'             , ['t2', 't1'], [          ], ['t3', 't4'      ]);
            makeTest('included only but excluded', ['t2', 't1'], ['t2', 't3'], ['t2', 't3', 't4']);
        });

        // Database.prototype.isTableExist = function(/* const rho::String& */ tableName)
        it('checks for table existence', function() {
            db.executeSql('DROP TABLE IF EXISTS t;');
            expect(db.isTableExist('t')).toBe(false);

            db.executeSql('CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));');
            expect(db.isTableExist('t')).toBe(true);

            db.executeSql('DROP TABLE IF EXISTS t;');
            expect(db.isTableExist('t')).toBe(false);
        });

        // Database.prototype.setDoNotBackupAttribute = function(/* bool */ setFlag)
        it('sets "don\'t backup attribute"', function() {
            db.setDoNotBackupAttribute(true);
        });
    });
});
