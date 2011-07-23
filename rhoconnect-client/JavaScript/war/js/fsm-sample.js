
Rhomobile.fsm.define("sample FSM", function(/*Rhomobile.fsm.Machine*/fsm){

	fsm.state('Logged out')/*>>state*/
			// to implement the Moore type of FSM
			.immediateTransitsTo('Some state to transit immediately, w/o any input received')/*>>state*/

			.on('login button click')/*>>input*/.transitsTo('Login request sent')/*>>state*/
			.on('timeout happens')/*>>input*/.transitsTo('Screensaver ont')/*>>state*/

			.withEntryAction(/*comment*/'some tags here: and comment text', function()
			{
				// enable login button
			})/*>>state*/

			.withExitAction(/*comment*/'login: disable button', function()
			{
				// disable login button
			})/*>>state*/

			.withActionOnInput(/*inputName*/'login button click', /*comment*/'backlight: turn on', function(input)
			{
				// turn backlight on
			})/*>>state*/

			.withActionOnTransitTo(/*stateName*/'Screensaver on', /*comment*/'backlight: turn off', function(input)
			{
				// turn backlight off
			})/*>>state*/;


	fsm.state('Screensaver on')
			.on('screen touch').transitsTo('Logged out')
			.on('button pressed').transitsTo('Logged out')

			.withEntryAction(/*comment*/'some tags here: and comment text', function(input)
			{
				// turn screensaver on
			})

			.withExitAction(/*comment*/'screensaver: turn off', function(input)
			{
				// turn screensave off
			})

			.withActionOnInput(/*ID*/'button pressed', /*comment*/'commented here', function(input)
			{
			})

			.withActionOnTransitsTo(/*ID*/'Logged out', /*comment*/'commented here', function(input)
			{
			});

}).withInitialState('Logged off');
