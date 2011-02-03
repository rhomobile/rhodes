
Rhomobile.fsm.define("sample FSM", function(fsm){

	fsm.state('Logged out')
			.on('login button click').transitTo('Login request sent')
			.on('timeout happens').transitTo('Screensaver ont')

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
			.on('screen touch').transitTo('Logged out')
			.on('button pressed').transitTo('Logged out')

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

			.withActionOnTransitTo(/*ID*/'Logged out', /*comment*/'commented here', function(input)
			{
			});

}).initialState('Logged off');
