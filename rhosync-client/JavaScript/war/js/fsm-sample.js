
Rhomobile.fsm.define("sample FSM", function(/*Rhomobile.fsm.Machine*/fsm){

	fsm.state('Logged out')
			// to implement the Moore type of FSM
			.immediateTransitsTo('Some state to transit immediately, w/o any input received')

			.on('login button click').transitsTo('Login request sent')
			.on('timeout happens').transitsTo('Screensaver ont')

			.withEntryAction(/*comment*/'some tags here: and comment text', function(input)
			{
				// enable login button
			})

			.withExitAction(/*comment*/'login: disable button', function(input)
			{
				// disable login button
			})

			.withActionOnInput(/*ID*/'login button click', /*comment*/'backlight: turn on', function(input)
			{
				// turn backlight on
			})

			.withActionOnTransitTo(/*ID*/'Screensaver on', /*comment*/'backlight: turn off', function(input)
			{
				// turn backlight off
			});


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
