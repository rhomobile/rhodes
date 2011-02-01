// Rhodes js sync client API test spec

describe("Sync client API", function() {

	it("have namespaces defined", function() {
		expect(Rhomobile).toBeSet();
		expect(Rhomobile.sync).toBeSet();
		expect(Rhomobile.db).toBeSet();
	});

	describe("Model", function() {

		beforeEach(function() {
			api = Rhomobile.sync;
		});

		it("is defined", function() {
			expect(api.Model).toBeSet();
		});

		it("is able to construct an instance", function() {
			var name = "sampleModel";
			var props = {a: 1, b:2};
			var inst1 = new api.Model(name, props);
			var inst2 = new api.Model('another '+name, props);
			expect(inst2.name).not.toEqual(name);
			expect(inst1.name).toEqual(name);
		});

	});
	
	describe("DbStorage", function() {
		var dbName = "sampleSyncDb";

		beforeEach(function() {
			api = Rhomobile.db;
		});

		it("is defined", function() {
			expect(api.DbStorage).toBeSet();
		});

		it("is able to construct an instance", function() {
			var inst1 = new api.DbStorage(dbName);
			var inst2 = new api.DbStorage('another '+dbName);
			expect(inst2.dbName).not.toEqual(dbName);
			expect(inst1.dbName).toEqual(dbName);
		});

		it("is able to open database", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy();
			inst.open(function(db){}, errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

		it("is able to perform a query", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy();
			inst.open("SELECT name FROM sqlite_master WHERE type='table'", errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

	});

	describe("SyncThread", function() {

		var OK_LOGIN = "lars";
		var OK_PASSWD = "larspass";
		var WRONG_LOGIN = "not-lars";
		var WRONG_PASSWD = "not-larspass";

		beforeEach(function() {
			api = Rhomobile.sync;
		});

		it("is defined", function() {
			expect(api.Thread).toBeSet();
		});

		it("shouldn't login with wrong credentials", function() {
			expect(api.Thread.login).toBeSet();
			var result = api.Thread.login(WRONG_LOGIN, WRONG_PASSWD);
			expect(result).not.toBeNull();
			expect(result).toBeFalsy();
		});

		it("should login OK with proper credentials", function() {
			expect(api.Thread.login).toBeSet();
			var result = api.Thread.login(OK_LOGIN, OK_PASSWD); 
			expect(result).not.toBeNull();
			expect(result).toBeTruthy();
		});

	});

	/*
	describe("SyncNotify", function() {

		beforeEach(function() {
			api = Rhomobile.sync;
		});

		it("should be defined", function() {
			expect(api.Notify).toBeSet();
		});

		it("should work on login attempt", function() {
			expect(api.Notify.callLoginCallback).toBeSet();

			var callback = jasmine.createSpy();
			
			api.Thread.login(OK_LOGIN, OK_PASSWD); 
			expect(result).not.toBeNull();
			expect(result).toBeTruthy();
		});

	});
	*/

});
