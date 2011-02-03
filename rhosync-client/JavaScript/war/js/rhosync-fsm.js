/**
 * Created by Rhomobile.com.
 * User: dipspb@gmail.com
 * Date: 03.02.2011
 * Time: 12:37:39
 */

if (Rhomobile) { Rhomobile.fsm = function(){

	// === Machine =========================================

	var klass_Machine = function(name, definition){
		this.name = name;
		this.define(definition);
		this.states = {};
	};

	klass_Machine.prototype.define = function(definition) {
		if(definition) {
			definition(this);
		}
		return this;
	};

	klass_Machine.prototype.state = function(name) {
		var state = null;
		if(this.states[name]) {
			state = this.states[name];
		} else {
			state = new klass_State(name);
			this.states[name] = state;
		}
		return state;
	};

	// === State ===========================================

	var klass_State = function(name) {
		this.name = name;
	};

	// === Public members ==================================

	return {
		define: function(name, definition){return new klass_Machine(name, definition);},
		Machine: klass_Machine,
		State: klass_State
	};

}();}; // EOF