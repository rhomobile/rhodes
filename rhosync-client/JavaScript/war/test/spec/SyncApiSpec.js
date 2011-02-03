// Rhodes js sync client API test spec

describe("Sync client API", function() {

	it("have namespaces defined", function() {
		expect(Rhomobile).toBeSet();
		expect(Rhomobile.event).toBeSet();
		expect(Rhomobile.internal).toBeSet();
		expect(Rhomobile.sync).toBeSet();
        expect(Rhomobile.db).toBeSet();
        expect(Rhomobile.fsm).toBeSet();
	});

    it("loadModules is defined", function() {
        expect(Rhomobile.loadModules).toBeSet();
    });

	describe("Rhomobile.fsm.Machine", function() {
		var api = Rhomobile.fsm;
		var instName = 'sample Machine';
		var instance = null;

		beforeEach(function(){
			instance = new api.Machine(instName);
		});

		it("is defined", function() {
			expect(api.Machine).toBeSet();
		});

		it("is able to construct an instance", function() {
			expect(api.define).toBeSet();
			var inst2 = api.define('another '+instName);
			expect(inst2.name).not.toEqual(instName);
			expect(instance.name).toEqual(instName);
		});

		it("is able to obtain definition", function() {
			var def1 = jasmine.createSpy('Machine1 definition');
			var def2 = jasmine.createSpy('Machine2 definition');
			// states can be defined after creation
			instance.define(def1);
			// or while creating the instance
			var inst2 = api.define('another '+instName, def2);
			expect(def1).toHaveBeenCalledWith(instance);
			expect(def2).toHaveBeenCalledWith(inst2);
		});

		it("is able to have states", function() {
			var stName1 = 'sample state';
			var stName2 = 'another'+stName1;
			var stName3 = 'one more'+stName1;

			expect(instance.state).toBeSet();
			instance.state(stName1);
			instance.state(stName2);
			instance.state(stName3);
			expect(instance.states).toBeSet();
			expect(instance.states[stName1].name).toEqual(stName1);
			expect(instance.states[stName2].name).toEqual(stName2);
			expect(instance.states[stName3].name).toEqual(stName3);
		});

		it("is able to receive an input", function() {
			expect(instance.input).toBeSet();
			spyOn(instance, 'input');
			instance.input('button pressed', 5);
			expect(instance).toHaveBeenCalledWith('button pressed', 5);
		});
	});

	describe("Rhomobile.fsm.State", function() {
		var api = Rhomobile.fsm;

		var instName = 'sample State';
		var instance = null;

		var fsmName = 'sample Machine';
		var fsmInst = null;

		beforeEach(function(){
			fsmInst = api.define(fsmName);
			instance = fsmInst.state(instName);
		});

		it("is defined", function() {
			expect(api.State).toBeSet();
		});

		it("is able to construct an instance", function() {
			var inst2 = fsmInst.state('another '+instName);
			expect(inst2.name).not.toEqual(instName);
			expect(instance.name).toEqual(instName);
		});

/*
		it("is able to obtain definition", function() {
			var name = "sample FSM";
			var def1 = jasmine.createSpy('FSM1 definition');
			var def2 = jasmine.createSpy('FSM2 definition');
			var inst1 = (new api.Machine(name)).define(def1);
			var inst2 = api.define('another '+name, def2);
			expect(def1).toHaveBeenCalledWith(inst1);
			expect(def2).toHaveBeenCalledWith(inst2);
		});
*/

	});

	describe("Rhomobile.data.Model", function() {
		var api = Rhomobile.data;

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
		var api = Rhomobile;

		var models = [
		              new Rhomobile.data.Model("Customer", {
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

		              new Rhomobile.data.Model("Product", {
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

		it("is defined", function() {
			expect(api.Config).toBeSet();
		});

		it("is able to add a source", function() {
			expect(api.Config.addSource).toBeSet();
		});
	});

	describe("Rhomobile.db.DbStorage", function() {
		var	api = Rhomobile.db;

		var dbName = "sampleSyncDb";

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
			var errHdlr = jasmine.createSpy('for error');
			expect(inst.open).toBeSet();
			inst.open(function(db){}, errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

		it("is able to perform a query", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy('for error');
			expect(inst.open).toBeSet();
			inst.open("SELECT name FROM sqlite_master WHERE type='table'", errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
		});

		it("is able to initialized", function() {
			var inst = new api.DbStorage(dbName);
			var errHdlr = jasmine.createSpy('for error');
			expect(inst.initSchema).toBeSet();
			inst.initSchema(errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
			var names = inst.getAllTableNames(true, errHdlr);
			expect(errHdlr).not.toHaveBeenCalled();
//			expect(names).toBeSet();
//			expect(names.length).toEqual(4+1);
//			expect(names).toContain('sources');
		});

	});

	describe("Rhomobile.sync.SyncThread", function() {
		var api = Rhomobile.sync;

		var OK_LOGIN = "lars";
		var OK_PASSWD = "larspass";
		var WRONG_LOGIN = "not-lars";
		var WRONG_PASSWD = "not-larspass";

		it("is defined", function() {
			expect(api.Thread).toBeSet();
		});

/*
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
*/

	});

});
