describe('<database SQLite3 module specs>', function () {
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
        expect(db.execute('SELECT * FROM t;')).toEqual([{x: '10', y: 'ten', z: 'TEN'}]);
        db.commitTransaction();
        expect(db.execute('SELECT * FROM t;')).toEqual([{x: '10', y: 'ten', z: 'TEN'}]);
    });

    // SQLite3.prototype.startTransaction = function(/* optional function */ oResult)
    // SQLite3.prototype.rollbackTransaction = function(/* optional function */ oResult)
    it('starts and rollbacks transaction', function() {
        db.execute('DROP TABLE IF EXISTS t; CREATE TABLE t(x INTEGER, y TEXT, z VARCHAR(10));', true);

        db.startTransaction();
        db.execute('INSERT INTO t (x, y, z) VALUES (?, ?, ?);', false, [10, 'ten', 'TEN']);
        expect(db.execute('SELECT * FROM t;')).toEqual([{x: '10', y: 'ten', z: 'TEN'}]);
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
        expect(db.execute('SELECT * FROM t;')).toEqual([{x: '10', y: 'ten', z: 'TEN'}]);
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
