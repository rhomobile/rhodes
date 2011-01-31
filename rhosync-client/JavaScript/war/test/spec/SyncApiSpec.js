// It is used, because GWT renders javascript after document loaded.
//jQuery(window).load(function(){

describe("Sync cleant API", function() {

	it("have namespaces defined", function() {
		expect(Rhomobile).toBeSet();
		expect(Rhomobile.sync).toBeSet();
		expect(Rhomobile.db).toBeSet();
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
			var inst = new api.DbStorage(dbName);
			expect(inst.dbName).toEqual(dbName);
		});

		it("is able to open database", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy();
			inst.open(function(db){}, errHdlr);
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

		it("should be defined", function() {
			expect(api.Thread).toBeSet();
		});

		it(" shouldn't login with wrong credentials", function() {
			expect(api.Thread.login).toBeSet();
			var result = api.Thread.login(WRONG_LOGIN, WRONG_PASSWD);
			expect(result).not.toBeNull();
			expect(result).toBeFalsy();
		});

		it(" should login OK with proper credentials", function() {
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

//}); //jQuery(window).load
