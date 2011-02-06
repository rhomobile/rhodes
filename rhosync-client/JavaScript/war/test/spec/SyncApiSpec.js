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

		var fsmName = 'sample Machine';
		var fsmInst = null;

		beforeEach(function(){
			fsmInst = new api.Machine(fsmName);
		});

		it("is defined", function() {
			expect(api.Machine).toBeSet();
		});

		it("is able to construct an instance", function() {
			expect(api.define).toBeSet();
			var fsmInst2 = api.define('another '+fsmName);
			expect(fsmInst2.name).not.toEqual(fsmName);
			expect(fsmInst.name).toEqual(fsmName);
		});

		it("is able to obtain definition", function() {
			var def1 = jasmine.createSpy('Machine1 definition');
			var def2 = jasmine.createSpy('Machine2 definition');
			// states can be defined after creation
			fsmInst.define(def1);
			// or while creating the instance
			var fsmInst2 = api.define('another '+fsmName, def2);
			expect(def1).toHaveBeenCalledWith(fsmInst);
			expect(def2).toHaveBeenCalledWith(fsmInst2);
		});

		it("prohibits to define the same status twice", function() {
			var stateName1 = 'sample input';
			var stateName2 = stateName1;

			fsmInst.state(stateName1);

			var exceptionHappens = false;
			try {
				fsmInst.state(stateName2);
			} catch(ex) {
				jasmine.log('Exception thrown: '+ex);
				exceptionHappens = true;
			}
			expect(exceptionHappens).toEqual(true);
		});

		it("is able to have states", function() {
			var stName1 = 'sample state';
			var stName2 = 'another '+stName1;

			expect(fsmInst.state).toBeSet();
			fsmInst.state(stName1);
			fsmInst.state(stName2);

			expect(fsmInst.states).toBeSet();
			expect(fsmInst.states[stName1].name).toEqual(stName1);
			expect(fsmInst.states[stName2].name).toEqual(stName2);
		});

		it("is able to have current state", function() {
			var stName1 = 'sample state';
			var stName2 = 'another '+stName1;

			// if no states has been defined yet,
			// then first one becomes current state
			var st1 = fsmInst.state(stName1);
			var st2 = fsmInst.state(stName2);
			expect(fsmInst.currentState).toBeSet();
			expect(fsmInst.currentState).toEqual(st1);
		});

		it("is able to have initial state", function() {
			var stName1 = 'sample state';
			var stName2 = 'another '+stName1;

			// if no states has been defined yet,
			// then first one becomes initial state
			var st1 = fsmInst.state(stName1);
			var st2 = fsmInst.state(stName2);
			expect(fsmInst.initialState).toBeSet();
			expect(fsmInst.initialState).toEqual(st1);

			// and then we can change it to any defined
			expect(fsmInst.withInitialState).toBeSet();
			fsmInst.withInitialState(stName2);
			// it changes
			expect(fsmInst.initialState).toEqual(st2);
			// but current state doesn't
			expect(fsmInst.currentState).toEqual(st1);
		});

		it("is able to be reset", function() {
			var stName1 = 'sample state';
			var stName2 = 'another '+stName1;

			var st1 = fsmInst.state(stName1);
			var st2 = fsmInst.state(stName2);

			// first defined state becomes current one
			expect(fsmInst.currentState).toEqual(st1);

			// define initial state explicitly
			fsmInst.withInitialState(stName2);

			// reset to initial state
			expect(fsmInst.reset).toBeSet();
			fsmInst.reset();
			expect(fsmInst.currentState).toEqual(st2);

			// reset to any state
			fsmInst.resetTo(stName1);
			// it changes
			expect(fsmInst.currentState).toEqual(st1);
			// but initial state still the same
			expect(fsmInst.initialState).toEqual(st2);
		});

		it("is able to receive an input", function() {
			expect(fsmInst.inputWith).toBeSet();
			var inputSpy = spyOn(fsmInst, 'inputWith');
			fsmInst.inputWith('button pressed', 5);
			expect(inputSpy).toHaveBeenCalledWith('button pressed', 5);
		});

		it("can change state on appropriate input applied", function() {

			var lampSwitchFsm = api.define('Lamp switch FSM', function(fsm){
				fsm.state('Light is OFF')
						.on('turn switch on').transitsTo('Light is ON');
				fsm.state('Light is ON')
						.on('turn switch off').transitsTo('Light is OFF');
			});

			expect(lampSwitchFsm).toBeSet();
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');

			lampSwitchFsm.inputWith('turn switch off');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');

			lampSwitchFsm.inputWith('turn switch on');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is ON');

			lampSwitchFsm.inputWith('turn switch on');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is ON');

			lampSwitchFsm.inputWith('turn switch off');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');
		});

		it("can handle states with immediate transition", function() {

			var lampSwitchFsm = api.define('Lamp switch FSM', function(fsm){
				expect(fsm.state('some fake intact state w/o transitions').immediateTransitsTo).toBeSet();

				fsm.state('Light is OFF')
						.on('turn switch on').transitsTo('ON click sound produced');
				fsm.state('ON click sound produced')
						.immediateTransitsTo('Light is ON');
				fsm.state('Light is ON')
						.on('turn switch off').transitsTo('OFF click sound produced');
				fsm.state('OFF click sound produced')
						.immediateTransitsTo('Light is OFF');
			}).withInitialState('Light is OFF').reset();

			expect(lampSwitchFsm).toBeSet();
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');

			lampSwitchFsm.inputWith('turn switch off');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');

			lampSwitchFsm.inputWith('turn switch on');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is ON');

			lampSwitchFsm.inputWith('turn switch on');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is ON');

			lampSwitchFsm.inputWith('turn switch off');
			expect(lampSwitchFsm.currentState.name).toEqual('Light is OFF');
		});

		it("can be rendered as a graph in a DOT notation", function() {

			var lampSwitchFsm = api.define('Lamp switch FSM', function(fsm){
				expect(fsm.state('some fake intact state w/o transitions').immediateTransitsTo).toBeSet();

				fsm.state('Light is OFF')
						.on('turn switch on').transitsTo('ON click sound produced');
				fsm.state('ON click sound produced')
						.immediateTransitsTo('Light is ON');
				fsm.state('Light is ON')
						.on('turn switch off').transitsTo('OFF click sound produced');
				fsm.state('OFF click sound produced')
						.immediateTransitsTo('Light is OFF');
			}).withInitialState('Light is OFF').reset();

			var dot = lampSwitchFsm.renderAsDot();
			expect(dot).toBeSet();
			expect(dot.length).toBeSet();
			expect(dot.length).toBeGreaterThan("graph".length);
			jasmine.log(dot);
		});

	});

	describe("Rhomobile.fsm.State", function() {
		var api = Rhomobile.fsm;

		var fsmName = 'sample Machine';
		var fsmInst = null;

		var stateName = 'sample State';
		var stateInst = null;

		beforeEach(function(){
			fsmInst = api.define(fsmName);
			stateInst = fsmInst.state(stateName);
		});

		it("is defined", function() {
			expect(api.State).toBeSet();
		});

		it("is able to construct an instance", function() {
			var stateInst2 = fsmInst.state('another '+stateName);
			expect(stateInst2.name).not.toEqual(stateName);
			expect(stateInst.name).toEqual(stateName);
		});

		it("is able to receive an input from Machine", function() {
			expect(stateInst.inputWith).toBeSet();
			var inputSpy = spyOn(stateInst, 'inputWith');
			fsmInst.inputWith('button pressed', 5);
			expect(inputSpy).toHaveBeenCalledWith('button pressed', 5);
		});

		it("is able to have inputs defined", function() {
			var inputName1 = 'sample input';
			var inputName2 = 'another '+inputName1;

			expect(stateInst.on).toBeSet();
			stateInst.on(inputName1);
			stateInst.on(inputName2);

			expect(stateInst.inputs).toBeSet();
			expect(stateInst.inputs[inputName1].name).toEqual(inputName1);
			expect(stateInst.inputs[inputName2].name).toEqual(inputName2);
		});

		it("prohibits to define the same input twice", function() {
			var inputName1 = 'sample input';
			var inputName2 = inputName1;

			stateInst.on(inputName1);

			var exceptionHappens = false;
			try {
				stateInst.on(inputName2);
			} catch(ex) {
				jasmine.log('Exception thrown: '+ex);
				exceptionHappens = true;
			}
			expect(exceptionHappens).toEqual(true);
		});

		it("can fire actions properly", function() {

			var unknownInput = 'unknown input';
			var knownInput = 'well known input w/o transition follows';
			var turnOnInput = 'turn switch on';
			var turnOffInput = 'turn switch off';

			var stateOn = 'Light is ON';
			var stateOff = 'Light is OFF';

			var spyOnEntry = jasmine.createSpy('spy on entry action');
			var spyOnExit = jasmine.createSpy('spy on exit action');
			var spyOnInput = jasmine.createSpy('spy on input action');
			var spyOnTransit = jasmine.createSpy('spy on transition action');

			var lampSwitchFsm = api.define('Lamp switch FSM', function(fsm){

				fsm.state('Light is OFF')
						.on(turnOnInput).transitsTo(stateOn)
						.withEntryAction('entry action1', spyOnEntry)
						.withExitAction('exit action1', spyOnExit)
						.withActionOnInput(knownInput, 'input action1', spyOnInput)
						.withActionOnTransitTo(stateOff, 'transit action1', spyOnTransit)
						.withActionOnTransitTo(stateOn, 'transit action1', spyOnTransit)
						;

				fsm.state('Light is ON')
						.on(turnOffInput).transitsTo(stateOff)
						.withEntryAction('entry action2', spyOnEntry)
						.withExitAction('exit action2', spyOnExit)
						.withActionOnInput(knownInput, 'input action3', spyOnInput)
						.withActionOnTransitTo(stateOn, 'transit action2', spyOnTransit)
						.withActionOnTransitTo(stateOff, 'transit action2', spyOnTransit)
						;
			});

			var cntEntry = 0;
			var cntExit = 0;
			var cntInput = 0;
			var cntTrans = 0;

			var reportCounts = function(logMsg) {
				jasmine.log(logMsg);
				jasmine.log('-- entry'); 		expect(spyOnEntry.callCount).toEqual(cntEntry);
				jasmine.log('-- exit');  		expect(spyOnExit.callCount).toEqual(cntExit);
				jasmine.log('-- input'); 		expect(spyOnInput.callCount).toEqual(cntInput);
				jasmine.log('-- transition');	expect(spyOnTransit.callCount).toEqual(cntTrans);
			};

			reportCounts(stateOff+': initial state');

			lampSwitchFsm.inputWith(unknownInput);
			// nothing should happen
			reportCounts(stateOff+': '+unknownInput);

			lampSwitchFsm.inputWith(knownInput, 123);
			cntInput++;
			// still at the same state, but action happens
			reportCounts(stateOff+': '+knownInput);
			expect(spyOnInput).toHaveBeenCalledWith(123);

			lampSwitchFsm.inputWith(turnOffInput);
			// nothing should happen
			reportCounts(stateOff+': '+turnOffInput);

			lampSwitchFsm.inputWith(turnOnInput);
			// three more actions should be performed:
			// exit current state, transition to the new one, and entry to the new state
			cntEntry++;
			cntTrans++;
			cntExit++;
			reportCounts(stateOff+': '+turnOnInput);

			lampSwitchFsm.inputWith(unknownInput);
			// no changes should happen
			reportCounts(stateOn+': '+unknownInput);

			lampSwitchFsm.inputWith(knownInput, 123);
			// still at the same state, but action happens
			cntInput++;
			reportCounts(stateOn+': '+knownInput);
			expect(spyOnInput).toHaveBeenCalledWith(123);

			lampSwitchFsm.inputWith(turnOnInput);
			// nothing should happen
			reportCounts(stateOn+': '+turnOnInput);

			lampSwitchFsm.inputWith(turnOffInput);
			// three more actions should be performed:
			// exit current state, transition to the new one, and entry to the new state
			cntEntry++;
			cntTrans++;
			cntExit++;
			reportCounts(stateOn+': '+turnOffInput);
		});
	});

	describe("Rhomobile.fsm.Input", function() {
		var api = Rhomobile.fsm;

		var fsmName = 'sample Machine';
		var fsmInst = null;

		var stateName = 'sample State';
		var stateInst = null;

		var inputName = 'sample Input';
		var inputInst = null;

		beforeEach(function(){
			fsmInst = api.define(fsmName);
			stateInst = fsmInst.state(stateName);
			inputInst = stateInst.on(inputName);
		});

		it("is defined", function() {
			expect(api.Input).toBeSet();
		});

		it("is able to construct an instance", function() {
			var inputInst2 = stateInst.on('another '+inputName);
			expect(inputInst2.name).not.toEqual(inputName);
			expect(inputInst.name).toEqual(inputName);
		});

		it("is able to have transition defined", function() {
			var transName1 = 'sample sample transition';

			expect(inputInst.transitsTo).toBeSet();

			var stateCheck = inputInst.transitsTo(transName1);
			// state should be returned
			expect(stateCheck).toEqual(stateInst);

			expect(inputInst.transition).toBeSet();
			expect(inputInst.transition).toEqual(transName1);
		});

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
