// FSM definition
var lampSwitchFsm = api.define('Lamp switch FSM', function(fsm){

	fsm.state('some fake intact state w/o transitions');

	fsm.state('Light is OFF')
			.on('turn switch on').transitsTo('ON click sound produced')
			.withEntryAction('turn off', function(){
				SomeAPIforExample.turnOff();
			})
			;

	fsm.state('ON click sound produced')
			.immediateTransitsTo('Light is ON')
			.withEntryAction('do click', function(){
				SomeAPIforExample.doClick();
			})
			.withEntryAction('one more action', function(){
				//something else..
			})
			;

	fsm.state('Light is ON')
			.on('turn switch off').transitsTo('OFF click sound produced');

	fsm.state('OFF click sound produced')
			.immediateTransitsTo('Light is OFF')
			.withEntryAction('turn on', function(){
				SomeAPIforExample.turnOn();
			})
			;

}).withInitialState('Light is OFF').reset();


// render fsm to DOT
var dot = lampSwitchFsm.renderAsDot();

// It returns graph definition in a DOT notation, then it could
// be rendered to an image file using Graphviz tool. Look at
// http://www.graphviz.org for Graphviz.
//
// Here is fsm-sample.png file rendered from lampSwitchFsm.
 
