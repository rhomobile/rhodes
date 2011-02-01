// Rhodes js sync client API test spec

describe("Sync client API", function() {

	it("have namespaces defined", function() {
		expect(Rhomobile).toBeSet();
		expect(Rhomobile.event).toBeSet();
		expect(Rhomobile.internal).toBeSet();
		expect(Rhomobile.sync).toBeSet();
		expect(Rhomobile.db).toBeSet();
	});

	describe("Rhomobile.sync.Model", function() {
		beforeEach(function() {
			api = Rhomobile.sync;
		});

		it("is defined", function() {
			expect(api.Model).toBeSet();
		});

		it("is able to construct an instance", function() {
			var name = "sampleModel";
			var inst1 = new api.Model(name);
			var inst2 = new api.Model('another '+name);
			expect(inst2.name).not.toEqual(name);
			expect(inst1.name).toEqual(name);
		});

	});

	describe("Rhomobile.Config", function() {
		var models = [
		              new Rhomobile.sync.Model("Customer", {
		            	  // options
		            	  'enable': ['sync'],
		            	  'set': {'sync_priority': 1, 'schema_version': '1.0'}
		              }, {
		            	  // properties
		            	  'address':	'string',
		            	  'created_at':	'string',
		            	  'city':		'string',
		            	  'email':		'string',
		            	  'first':		'string',
		            	  'last':		'string',
		            	  'updated_at':	'string',
		            	  'lat':		'string',
		            	  'long':		'string',
		            	  'phone':		'string',
		            	  'state':		'string',
		            	  'zip':		'string',

		            	  'SurveySectionID':	'string',    
		            	  'SurveyID':			'string',    
		            	  'CallID':				'string',    
		            	  'SurveyResultID':		'string'    
		              }),

		              new Rhomobile.sync.Model("Product", {
			            	  // options
			            	  'enable': ['sync', 'pass_through'],
			            	  'set': {'sync_priority': 2, 'schema_version': '1.0'},
			            	  'belongs_to': [{'quantity': 'Customer_s'}, {'sku': 'Customer_s'}]
		            	  }, {
		            		  // properties
		            		  'brand':		'string',
		            		  'created_at':	'string',
		            		  'name':		'string',
		            		  'price':		'string',
		            		  'quantity':	'string',
		            		  'sku':		'string',
		            		  'updated_at':	'string'
		              })
		              ];
		
		beforeEach(function() {
			api = Rhomobile;
		});

		it("is defined", function() {
			expect(api.Config).toBeSet();
		});

		it("is able to add a source", function() {
			expect(api.Config.addSource).toBeSet();
		});
	});

	describe("Rhomobile.db.DbStorage", function() {
		var dbName = "one_more3_sampleSyncDb";

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
			expect(inst.open).toBeSet();
			inst.open(function(db){}, errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

		it("is able to perform a query", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy();
			expect(inst.open).toBeSet();
			inst.open("SELECT name FROM sqlite_master WHERE type='table'", errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

		it("is able to initialized", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy();
			expect(inst.initSchema).toBeSet();
			inst.initSchema(errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
			//var names = inst.getAllTableNames(errHdlr);
			//expect(errHdlr).not.toHaveBeenCalled();
			//expect(names).toBeSet();
			//expect(names.length).toEqual(4+1);
			//expect(names).toContain('sources');
		});

	});

	describe("Rhomobile.sync.SyncThread", function() {

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
