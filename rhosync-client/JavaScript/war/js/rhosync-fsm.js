/**
 * Created by Rhomobile.com.
 * User: dipspb@gmail.com
 * Date: 03.02.2011
 * Time: 12:37:39
 */

if (Rhomobile) { Rhomobile.fsm = function(){

	var kls = null;

	// === Machine =========================================

	kls = function(name, definition){
		this.name = name;
		this.states = {};
		this.currentState = null;
		this.initialState = null;
		this.define(definition);
	};

	kls.prototype.define = function(definition) {
		if(definition) {
			this.states = {};
			this.currentState = null;
			this.initialState = null;
			definition(this);
		}
		return this;
	};

	kls.prototype.state = function(stateName) {
		var state = null;
		if(this.states[stateName]) {
			throw('State \'' +stateName +'\' is already defined in Machine \''+ this.name +'\'.');
		} else {
			state = new klass_State(stateName);
			this.states[stateName] = state;
		}
		if (! this.currentState) {
			this.currentState = state;
		}
		if (! this.initialState) {
			this.initialState = state;
		}
		state.fsm = this;
		return state;
	};

	kls.prototype._lookForState = function(stateName) {
		if(! this.states[stateName]) {
			throw('State \'' +stateName +'\' is not defined in Machine \''+ this.name +'\'.');
		}
		return this.states[stateName];
	};

	kls.prototype._notifyStateChanged = function(stateName) {
			// something to post as an event
	};

	kls.prototype._changeStateTo = function(nameOrState) {
		var stateName = ('string' == typeof nameOrState) ? nameOrState : nameOrState.name;

		this.currentState._fireExitActions();
		this.currentState._fireTransitionActions(stateName);
		this.currentState = this._lookForState(stateName);
		this.currentState._fireEntryActions();
		this._notifyStateChanged(stateName);
		return this;
	};

	kls.prototype.withInitialState = function(stateName) {
		this.initialState = this._lookForState(stateName);
		return this;
	};

	kls.prototype.resetTo = function(stateName) {
		this._changeStateTo(this._lookForState(stateName));
		return this;
	};

	kls.prototype.reset = function() {
		if(this.initialState) {
			this._changeStateTo(this.initialState);
		}
		return this;
	};

	kls.prototype.inputWith = function(inputName, data) {
		if(this.currentState) {
			this.currentState.inputWith(inputName, data);
		}
		return this;
	};

	kls.prototype._renderStatesAsDot = function() {
		var dot = "";
		var states = this.states;
		for(var name in states) {
			if(states.hasOwnProperty(name)) {
				dot += states[name]._renderAsDot(states[name] == this.initialState ? 'color=green' : null);
			}
		}
		return dot;
	};

	kls.prototype.renderAsDot = function() {
		var dot = 'digraph fsm {\n'
				+'  label = "' +this.name +'";\n'
				+'  node [style=filled];\n'
				+this._renderStatesAsDot()
				+'}';
		return dot;
	};

	var klass_Machine = kls;

	// === State ===========================================

	kls = function(name) {
		this.name = name;
		this.fsm = null;
		this.immediateTransitionState = null;
		this.inputs = {};
		this.entryActions = [];
		this.exitActions = [];
		this.inputActions = {};
		this.transitionActions = {};
	};

	kls.prototype._reportIntactInput = function(inputName) {
		// some reporting goes here..
	};

	kls.prototype._lookForInput = function(inputName) {
		if(! this.inputs[inputName]) {
			// Input generation may be out of the system scope.
			// So no exception needed, just some reporting maybe.
			this._reportIntactInput(inputName);
		}
		return this.inputs[inputName];
	};


	kls.prototype._fireEntryActions = function() {
		var actions = this.entryActions;
		for(var i in actions) {
			try {
				actions[i]();
			} catch(ex) {/*report action exception*/}
		}
		if(this.immediateTransitionState) {
			this.fsm._changeStateTo(this.immediateTransitionState);
		}
	};

	kls.prototype._fireExitActions = function() {
		var actions = this.exitActions;
		for(var i in actions) {
			try {
				actions[i]();
			} catch(ex) {/*report action exception*/}
		}
	};

	kls.prototype._fireInputActions = function(input, data) {
		var actions = this.inputActions;
		// currently just only action per input
		if (actions[input]) {
			try {
				actions[input](data);
			} catch(ex) {/*report action exception*/}
		}
	};

	kls.prototype._fireTransitionActions = function(transition) {
		var actions = this.transitionActions;
		// currently just only action per transition
		if (actions[transition]) {
			try {
				actions[transition]();
			} catch(ex) {/*report action exception*/}
		}
	};

	kls.prototype.inputWith = function(inputName, data) {
		if(this == this.fsm.currentState) {
			this._fireInputActions(inputName, data);

			var input = this._lookForInput(inputName);
			if (input) {
				//this._fireExitActions();
				//this._fireTransitionActions(input.transition);
				this.fsm._changeStateTo(input.transition);
			}
		}
	};

	kls.prototype.on = function(inputName) {
		var input = null;
		if(this.inputs[inputName]) {
			throw('Input \'' +inputName +'\' is already defined for the State \''
					+this.name +'\' of Machine \'' +this.fsm.name +'\'.');
		} else {
			input = new klass_Input(inputName);
			this.inputs[inputName] = input;
		}
		input.state = this;
		return input;
	};

	kls.prototype.withEntryAction = function(actionName, action) {
		// actionName to be used just to render documentation
		this.entryActions.push(action);
		return this;
	};

	kls.prototype.withExitAction = function(actionName, action) {
		// actionName to be used just to render documentation
		this.exitActions.push(action);
		return this;
	};

	kls.prototype.withActionOnInput = function(inputName, actionName, action) {
		// actionName to be used just to render documentation
		this.inputActions[inputName] = action;
		return this;
	};

	kls.prototype.withActionOnTransitTo = function(stateName, actionName, action) {
		// actionName to be used just to render documentation
		this.transitionActions[stateName] = action;
		return this;
	};

	kls.prototype.immediateTransitsTo = function(stateName) {
		this.immediateTransitionState = stateName;
		return this;
	};


	kls.prototype._renderInputsAsDot = function() {
		var dot = "";
		var inputs = this.inputs;
		if(this.immediateTransitionState) {
			dot += '  "' +this.name +'" -> "' +this.immediateTransitionState +'" [color=blue];\n';
		}
		for(var name in inputs) {
			if(inputs.hasOwnProperty(name)) {
				dot += inputs[name]._renderAsDot(this.name);
			}
		}
		return dot;
	};

	kls.prototype._renderAsDot = function(addStyle) {
		var dot = '\n';
		var attrs = 'label="' +this.name +'"' +(addStyle ? ', ' +addStyle : '');

		var noInputs = true;
		for(var name in this.inputs) {
			if(this.inputs.hasOwnProperty(name)) {
				noInputs = false;
				break;
			}
		}
		attrs += (noInputs && !this.immediateTransitionState) ? ', color=red' : '';
		attrs += this.immediateTransitionState ? ', color=blue, fillcolor=lightblue' : '';

		dot += ('  "' +this.name +'" [' +attrs +'];\n');
		dot += this._renderInputsAsDot();
		dot += '\n';
		return dot;
	};

	var klass_State = kls;

	// === Input ===========================================

	kls = function(name) {
		this.name = name;
		this.transition = null;
		this.state = null;
	};

	kls.prototype.transitsTo = function(stateName) {
		this.transition = stateName;
		return this.state;
	};

	kls.prototype._renderAsDot = function(stateName) {
		return '  "' +stateName +'" -> "' +this.transition +'" [label="' +this.name +'"];\n';
	};
	var klass_Input = kls;

	// === Public members ==================================

	return {
		define: (function(name, definition) {
			return new klass_Machine(name, definition);
		}),
		Machine: klass_Machine,
		State: klass_State,
		Input: klass_Input
	};

}();} // EOF