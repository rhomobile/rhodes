describe('<database module spes>', function() {
    var db = null;

    beforeEach(function() {
        db = new Rho.Database(Rho.Application.databaseFilePath('local'), 'local');
    });

    afterEach(function() {
        db.close();
        db = null;
    });


    // Database.prototype.startTransaction = function()
    // Database.progtotype.commitTransaction = function()
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
    xit('sets "don\'t backup attribute"', function() {
        db.setDoNotBackupAttribute(true);
    });
});
