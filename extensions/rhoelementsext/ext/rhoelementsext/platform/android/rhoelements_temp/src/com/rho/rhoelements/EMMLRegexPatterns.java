package com.rho.rhoelements;

public class EMMLRegexPatterns
{
	protected static final String[][] regExPatternsHE =
	{
	  	// Key Capture HTTP-Equiv
	    {"^OnKey0x(..)", "OnKey-keyvalue0x$1"},
	    {"^OnKeyDispatch0x(..)", "OnKey-Dispatch0x$1"},
	    {"^OnAllKeys$", "OnKey-AllKeys"},
	    {"^OnAllKeysDispatch$", "OnKey-AllKeysDispatch"},
	    {"^acceleratekey$", "KeyCapture-AccelerateKey"},
	    {"^HomeKey$", "KeyCapture-HomeKeyValue"},
	    {"^OnTrigger$", "KeyCapture-TriggerEvent"},
		
		// Scanner (RSM) HTTP-Equiv    
	    {"^RSMGetNavigate$", "RSM-RSMGetEvent"},
	    {"^RSMGetParam$", "RSM"},
	    {"^RSMSetParam$", "RSM"},
	    
		// Scanner (Decoders) HTTP-Equiv
	    {"^all_decoders$", "scanner-all_decoders"},
	    {"^auspostal$", "scanner-auspostal"},
	    {"^Canpostal$", "scanner-canpostal"},
	    {"^Codabar$", "scanner-codabar"},
	    {"^Codabar-CLSiEditing$", "scanner-codabarclsiediting"},
	    {"^Codabar-maxlength$", "scanner-codabarmaxlength"},
	    {"^Codabar-minlength$", "scanner-codabarminlength"},
	    {"^Codabar-NotisEditing$", "scanner-notisediting"},
	    {"^Codabar-Redundancy$", "scanner-codabarredundancy"},
	    {"^code11$", "scanner-code11"},
	    {"^Code11-CheckDigitCount$", "scanner-code11checkdigitcount"},
	    {"^Code11-Redundancy$", "scanner-code11redundancy"},
	    {"^code11-reportcheckdiget$", "scanner-code11reportcheckdigit"},
	    {"^code128$", "scanner-code128"},
	    {"^code128-ean128$", "scanner-code128ean128"},
	    {"^code128-isbt128$", "scanner-code128isbt128"},
	    {"^Code128-maxlength$", "scanner-Code128maxlength"},
	    {"^Code128-minlength$", "scanner-Code128minlength"},
	    {"^code128-other128$", "scanner-code128other128"},
	    {"^Code128-Redundancy$", "scanner-Code128Redundancy"},
	    {"^Code39$", "scanner-Code39"},
	    {"^Code39-code32prefix$", "scanner-Code39code32prefix"},
	    {"^Code39-concatenation$", "scanner-Code39concatenation"},
	    {"^Code39-ConvertToCode32$", "scanner-Code39ConvertToCode32"},
	    {"^Code39-fullascii$", "scanner-Code39fullascii"},
	    {"^Code39-maxlength$", "scanner-Code39maxlength"},
	    {"^Code39-minlength$", "scanner-Code39minlength"},
	    {"^Code39-Redundancy$", "scanner-Code39Redundancy"},
	    {"^Code39-ReportCheckDigit$", "scanner-Code39ReportCheckDigit"},
	    {"^Code39-VerifyCheckDigit$", "scanner-Code39VerifyCheckDigit"},
	    {"^Code93$", "scanner-Code93"},
	    {"^Code93-maxlength$", "scanner-Code93maxlength"},
	    {"^Code93-minlength$", "scanner-Code93minlength"},
	    {"^Code93-Redundancy$", "scanner-Code93Redundancy"},
	    {"^Composit_ab$", "scanner-Composit_ab"},
	    {"^Composit_c$", "scanner-Composit_c"},
	    {"^d2of5$", "scanner-d2of5"},
	    {"^d2of5-maxlength$", "scanner-d2of5maxlength"},
	    {"^d2of5-minlength$", "scanner-d2of5minlength"},
	    {"^d2of5-Redundancy$", "scanner-d2of5Redundancy"},
	    {"^DataMatrix$", "scanner-DataMatrix"},
	    {"^DutchPostal$", "scanner-DutchPostal"},
	    {"^Ean13$", "scanner-Ean13"},
	    {"^ean8$", "scanner-ean8"},
	    {"^ean8-ConvertToEAN13$", "scanner-ean8ConvertToEAN13"},
	    {"^i2of5$", "scanner-i2of5"},
	    {"^i2of5-ConvertToEAN13$", "scanner-i2of5ConvertToEAN13"},
	    {"^i2of5-maxlength$", "scanner-i2of5maxlength"},
	    {"^i2of5-minlength$", "scanner-i2of5minlength"},
	    {"^i2of5-Redundancy$", "scanner-i2of5Redundancy"},
	    {"^12of5-ReportCheckDigit$", "scanner-12of5ReportCheckDigit"},
	    {"^i2of5-VerifyCheckDigit$", "scanner-i2of5VerifyCheckDigit"},
	    {"^JapPostal$", "scanner-JapPostal"},
	    {"^MacroMicroPDF$", "scanner-MacroMicroPDF"},
	    {"^MacroPDF$", "scanner-MacroPDF"},
	    {"^MaxiCode$", "scanner-MaxiCode"},
	    {"^MicroPDF$", "scanner-MicroPDF"},
	    {"^Msi$", "scanner-Msi"},
	    {"^Msi-checkdigits$", "scanner-Msicheckdigits"},
	    {"^Msi-CheckDigitScheme$", "scanner-MsiCheckDigitScheme"},
	    {"^Msi-maxlength$", "scanner-Msimaxlength"},
	    {"^Msi-minlength$", "scanner-Msiminlength"},
	    {"^Msi-Redundancy$", "scanner-MsiRedundancy"},
	    {"^Msi-ReportCheckDigit$", "scanner-MsiReportCheckDigit"},
	    {"^Qrcode$", "scanner-Qrcode"},
	    {"^rss14$", "scanner-rss14"},
	    {"^Rssexp$", "scanner-Rssexp"},
	    {"^Rsslim$", "scanner-Rsslim"},
	    {"^Signature$", "scanner-Signature"},
	    {"^tlc39$", "scanner-tlc39"},
	    {"^Trioptic39$", "scanner-Trioptic39"},
	    {"^trioptic39-Redundancy$", "scanner-trioptic39Redundancy"},
	    {"^UKPostal$", "scanner-UKPostal"},
	    {"^upc_ean-bookland$", "scanner-upc_eanbookland"},
	    {"^upc_ean-coupon$", "scanner-upc_eancoupon"},
	    {"^upc_ean-lineardecode$", "scanner-upc_eanlineardecode"},
	    {"^upc_ean-randomweightcheckdigit$", "scanner-upc_eanrandomweightcheckdigit"},
	    {"^upc_ean-securitylevel$", "scanner-upc_eansecuritylevel"},
	    {"^Upc_ean-supplemental2$", "scanner-Upc_eansupplemental2"},
	    {"^Upc_ean-supplemental5$", "scanner-Upc_eansupplemental5"},
	    {"^upc_ean-supplementalmode$", "scanner-upc_eansupplementalmode"},
	    {"^Upca$", "scanner-Upca"},
	    {"^upca-preamble$", "scanner-upcapreamble"},
	    {"^upca-ReportCheckDigit$", "scanner-upcaReportCheckDigit"},
	    {"^upce0$", "scanner-upce0"},
	    {"^upce0-ConvertToupca$", "scanner-upce0ConvertToupca"},
	    {"^upce0-preamble$", "scanner-upce0preamble"},
	    {"^upce0-ReportCheckDigit$", "scanner-upce0ReportCheckDigit"},
	    {"^upce1$", "scanner-upce1"},
	    {"^upce1-ConvertToupca$", "scanner-upce1ConvertToupca"},
	    {"^upce1-preamble$", "scanner-upce1preamble"},
	    {"^upce1-ReportCheckDigit$", "scanner-upce1ReportCheckDigit"},
	    {"^Usplanet$", "scanner-Usplanet"},
	    {"^Uspostnet$", "scanner-Uspostnet"},
	    {"^Webcode$", "scanner-Webcode"},
	    {"^scannernavigate$", "scanner-decodeevent"},
	    {"^enumscannernavigate$", "scanner-enumscannerevent"},
	    
		// Imager HTTP-Equiv
	    {"^imagernavigate$", "imager-imagerevent"},
	    
		// Indicators HTTP-Equiv
	    {"^batterynavigate$", "battery-batteryevent"},
	    {"^signalnavigate$", "signal-signalevent"},

		// Signature Capture HTTP-Equiv
	    {"^signaturenavigate$", "signaturecapture-SignatureSaveEvent"},
	    
		// Telemetry HTTP-Equiv
	    {"^TelemetryNavigate$", "Telemetry-TelemetryDataEvent"},
		
		// IO HTTP-Equiv    
	    {"^IONavigate$", "IO-PortEvent"},
	    {"^IOAnalog$", "IO"},
	    {"^IOSystem$", "IO"},
	    {"^IODigital$", "IO"},
	    
		// Device & Application HTTP-Equiv
	    {"^TimerNavigate$", "Timer-Start"},
	    {"^TimerInterval$", "Timer-Interval"},
	    {"^MoveSIP$", "SIP"},
	    {"^SIPControl$", "SIP"},
	    {"^CursorPos$", "Hourglass"},
	    {"^SNTP$", "SNTP-ServerIP"},
	    {"^Volume$", "Volume-SetVolume"},
	    {"^Reboot$", "Reboot-BootType"},
	    {"^PowerOn$", "PowerOn-PowerOnEventLegacy"},
	    
		// Card Reader HTTP-Equiv
	    {"^msrnavigate$", "cardreader-readevent"},
	    {"^dcrnavigate$", "cardreader-readevent"},
	    {"^dcr$", "cardreader"},
	    
		// Push HTTP-Equiv
	    {"^PushNavigate$", "Push-Detected"},
	    {"^Unattended$", "Push-Unattended"},
	    
		// File Management HTTP-Equiv
	    {"^FileTransferNavigate$", "FileTransfer-TransferEvent"},

		// Controls (Buttons) HTTP-Equiv
	    {"^TextButton$", "ZoomTextButton"}
//		//  Position
//		{"^OnKey0x(..)", "OnKey-keyvalue0x$1" }, // Converts Onkeys to 1.1 compatible tag
//		{"^OnKeyDispatch0x(..)", "OnKey-Dispatch0x$1" }, // Converts Onkeys to 1.1 compatible tag
//		{"^OnAllKeys$", "OnKey-AllKeys" }, // Converts Onkeys to 1.1 compatible tag
//		{"^OnAllKeysDispatch$", "OnKey-AllKeysDispatch" }, // Converts Onkeys to 1.1 compatible tag
//
//		{"^RSMGetNavigate$", "RSM-RSMGetEvent" }, // Converts RSM to 1.1 compatible tag
//		{"^RSMGetParam$", "RSM" }, // Converts RSM to 1.1 compatible tag
//		{"^RSMSetParam$", "RSM" }, // Converts RSM to 1.1 compatible tag
//
//		{"^all_decoders$", "scanner-alldecoders" }, // Converts decoder to 1.1 compatible tag
//		{"^auspostal$", "scanner-auspostal" }, // Converts decoder to 1.1 compatible tag
//		{"^Canpostal$", "scanner-canpostal" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar$", "scanner-codabar" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar-CLSiEditing$", "scanner-codabarclsiediting" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar-maxlength$", "scanner-codabarmaxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar-minlength$", "scanner-codabarminlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar-NotisEditing$", "scanner-notisediting" }, // Converts decoder to 1.1 compatible tag
//		{"^Codabar-Redundancy$", "scanner-codabarredundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^code11$", "scanner-code11" }, // Converts decoder to 1.1 compatible tag
//		{"^Code11-CheckDigitCount$", "scanner-code11checkdigitcount" }, // Converts decoder to 1.1 compatible tag
//		{"^Code11-Redundancy$", "scanner-code11redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^code11-reportcheckdiget$", "scanner-code11reportcheckdigit" }, // Converts decoder to 1.1 compatible tag
//		{"^code128$", "scanner-code128" }, // Converts decoder to 1.1 compatible tag
//		{"^code128-ean128$", "scanner-code128ean128" }, // Converts decoder to 1.1 compatible tag
//		{"^code128-isbt128$", "scanner-code128isbt128" }, // Converts decoder to 1.1 compatible tag
//		{"^Code128-maxlength$", "scanner-Code128maxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Code128-minlength$", "scanner-Code128minlength" }, // Converts decoder to 1.1 compatible tag
//		{"^code128-other128$", "scanner-code128other128" }, // Converts decoder to 1.1 compatible tag
//		{"^Code128-Redundancy$", "scanner-Code128Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39$", "scanner-Code39" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-code32prefix$", "scanner-Code39code32prefix" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-concatenation$", "scanner-Code39concatenation" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-ConvertToCode32$", "scanner-Code39ConvertToCode32" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-fullascii$", "scanner-Code39fullascii" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-maxlength$", "scanner-Code39maxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-minlength$", "scanner-Code39minlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-Redundancy$", "scanner-Code39Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-ReportCheckDigit$", "scanner-Code39ReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^Code39-VerifyCheckDigit$", "scanner-Code39VerifyCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^Code93$", "scanner-Code93" }, // Converts decoder to 1.1 compatible tag
//		{"^Code93-maxlength$", "scanner-Code93maxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Code93-minlength$", "scanner-Code93minlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Code93-Redundancy$", "scanner-Code93Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^Composit_ab$", "scanner-Composit_ab" }, // Converts decoder to 1.1 compatible tag
//		{"^Composit_c$", "scanner-Composit_c" }, // Converts decoder to 1.1 compatible tag
//		{"^d2of5$", "scanner-d2of5" }, // Converts decoder to 1.1 compatible tag
//		{"^d2of5-maxlength$", "scanner-d2of5maxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^d2of5-minlength$", "scanner-d2of5minlength" }, // Converts decoder to 1.1 compatible tag
//		{"^d2of5-Redundancy$", "scanner-d2of5Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^DataMatrix$", "scanner-DataMatrix" }, // Converts decoder to 1.1 compatible tag
//		{"^DutchPostal$", "scanner-DutchPostal" }, // Converts decoder to 1.1 compatible tag
//		{"^Ean13$", "scanner-Ean13" }, // Converts decoder to 1.1 compatible tag
//		{"^ean8$", "scanner-ean8" }, // Converts decoder to 1.1 compatible tag
//		{"^ean8-ConvertToEAN13$", "scanner-ean8ConvertToEAN13" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5$", "scanner-i2of5" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5-ConvertToEAN13$", "scanner-i2of5ConvertToEAN13" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5-maxlength$", "scanner-i2of5maxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5-minlength$", "scanner-i2of5minlength" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5-Redundancy$", "scanner-i2of5Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^12of5-ReportCheckDigit$", "scanner-12of5ReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^i2of5-VerifyCheckDigit$", "scanner-i2of5VerifyCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^JapPostal$", "scanner-JapPostal" }, // Converts decoder to 1.1 compatible tag
//		{"^MacroMicroPDF$", "scanner-MacroMicroPDF" }, // Converts decoder to 1.1 compatible tag
//		{"^MacroPDF$", "scanner-MacroPDF" }, // Converts decoder to 1.1 compatible tag
//		{"^MaxiCode$", "scanner-MaxiCode" }, // Converts decoder to 1.1 compatible tag
//		{"^MicroPDF$", "scanner-MicroPDF" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi$", "scanner-Msi" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-checkdigits$", "scanner-Msicheckdigits" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-CheckDigitScheme$", "scanner-MsiCheckDigitScheme" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-maxlength$", "scanner-Msimaxlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-minlength$", "scanner-Msiminlength" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-Redundancy$", "scanner-MsiRedundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^Msi-ReportCheckDigit$", "scanner-MsiReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^Qrcode$", "scanner-Qrcode" }, // Converts decoder to 1.1 compatible tag
//		{"^rss14$", "scanner-rss14" }, // Converts decoder to 1.1 compatible tag
//		{"^Rssexp$", "scanner-Rssexp" }, // Converts decoder to 1.1 compatible tag
//		{"^Rsslim$", "scanner-Rsslim" }, // Converts decoder to 1.1 compatible tag
//		{"^Signature$", "scanner-Signature" }, // Converts decoder to 1.1 compatible tag
//		{"^tlc39$", "scanner-tlc39" }, // Converts decoder to 1.1 compatible tag
//		{"^Trioptic39$", "scanner-Trioptic39" }, // Converts decoder to 1.1 compatible tag
//		{"^trioptic39-Redundancy$", "scanner-trioptic39Redundancy" }, // Converts decoder to 1.1 compatible tag
//		{"^UKPostal$", "scanner-UKPostal" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-bookland$", "scanner-upc_eanbookland" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-coupon$", "scanner-upc_eancoupon" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-lineardecode$", "scanner-upc_eanlineardecode" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-randomweightcheckdigit$", "scanner-upc_eanrandomweightcheckdigit" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-securitylevel$", "scanner-upc_eansecuritylevel" }, // Converts decoder to 1.1 compatible tag
//		{"^Upc_ean-supplemental2$", "scanner-Upc_eansupplemental2" }, // Converts decoder to 1.1 compatible tag
//		{"^Upc_ean-supplemental5$", "scanner-Upc_eansupplemental5" }, // Converts decoder to 1.1 compatible tag
//		{"^upc_ean-supplementalmode$", "scanner-upc_eansupplementalmode" }, // Converts decoder to 1.1 compatible tag
//		{"^Upca$", "scanner-Upca" }, // Converts decoder to 1.1 compatible tag
//		{"^upca-preamble$", "scanner-upcapreamble" }, // Converts decoder to 1.1 compatible tag
//		{"^upca-ReportCheckDigit$", "scanner-upcaReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^upce0$", "scanner-upce0" }, // Converts decoder to 1.1 compatible tag
//		{"^upce0-ConvertToupca$", "scanner-upce0ConvertToupca" }, // Converts decoder to 1.1 compatible tag
//		{"^upce0-preamble$", "scanner-upce0preamble" }, // Converts decoder to 1.1 compatible tag
//		{"^upce0-ReportCheckDigit$", "scanner-upce0ReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^upce1$", "scanner-upce1" }, // Converts decoder to 1.1 compatible tag
//		{"^upce1-ConvertToupca$", "scanner-upce1ConvertToupca" }, // Converts decoder to 1.1 compatible tag
//		{"^upce1-preamble$", "scanner-upce1preamble" }, // Converts decoder to 1.1 compatible tag
//		{"^upce1-ReportCheckDigit$", "scanner-upce1ReportCheckDigit" }, // Converts decoder to 1.1 compatible tag
//		{"^Usplanet$", "scanner-Usplanet" }, // Converts decoder to 1.1 compatible tag
//		{"^Uspostnet$", "scanner-Uspostnet" }, // Converts decoder to 1.1 compatible tag
//		{"^Webcode$", "scanner-Webcode" }, // Converts decoder to 1.1 compatible tag
//		{"^scannernavigate$", "scanner-decodeevent" }, // Converts decoder to 1.1 compatible tag
//		{"^enumscannernavigate$", "scanner-enumscannerevent" }, // Converts decoder to 1.1 compatible tag
//		{"^imagernavigate$", "imager-imagerevent" }, // Converts imager to 1.1 compatible tag
//		{"^batterynavigate$", "battery-batteryevent" }, // Converts battery to 1.1 compatible tag
//		{"^signalnavigate$", "signal-signalevent" }, // Converts signal to 1.1 compatible tag
//		{"^signaturenavigate$", "signaturecapture-transferevent" }, // Converts signature to 1.1 compatible tag
//		{"^TelemetryNavigate$", "Telemetry-TelemetryDataEvent" }, // Converts Telemetry Callback 
//
//		{"^IONavigate$", "IO-PortEvent" }, // 
//		{"^IOAnalog$", "IO" }, // 
//		{"^IOSystem$", "IO" }, // 
//		{"^IODigital$", "IO" }, // 
//
//		{"^TimerNavigate$", "Timer-Start" }, // Starts or stops a timer depending on the meta tag contents.  Module will respond appropriately to either 'killtimer' or a URI in the contents.
//		{"^TimerInterval$", "Timer-Interval" }, // Converts Timer Interval parameter to EMML 1.1
//		{"^MoveSIP$", "SIP" }, // All SIP Related modules now handled by new module, 'SIP'.  Parameters for MoveSIP are x and y, handled by common conversions.
//		{"^SIPControl$", "SIP" }, // All SIP Related modules now handled by new module, 'SIP'.  Contents strings remain unchanged for EMML 1.1
//		{"^CursorPos$", "Hourglass" }, // Module renamed in EMML1.1.  All contents strings are handled by common conversions
//		{"^SNTP$", "SNTP-ServerIP" }, // Added ServerIP Parameter as sole parameter of module in EMML1.1.  EMML1.0 had a single 'method'
//		{"^Volume$", "Volume-SetVolume" }, // Added SetVolume Parameter as sole parameter of module in EMML1.1.  EMML1.0 had a single 'method'
//		{"^Reboot$", "Reboot-BootType" }, // Added BootType Parameter as sole parameter of module in EMML1.1.  EMML1.0 had a single 'method'
//		{"^PowerOn$", "PowerOn-PowerOnEvent" }, // EMML 1.0 tag PowerOn has a single parameter (PowerOnEvent) which is an event in EMML1.1.
//
//		{"^msrnavigate$", "cardreader-readevent" }, // 
//		{"^dcrnavigate$", "cardreader-readevent" }, // 
//		{"^dcr$", "cardreader" }, // 
//
//		{"^PushNavigate$", "Push-PushDetected" }, // 
//		{"^Unattended$", "Push-Unattended" }, // 
//
//		{"^FileTransferNavigate$", "FileTransfer-TransferEvent" }, // 
	};

	protected static final String[][] regExPatternsCT =
	{
		// Common Contents
	    {"^$", "url('')"},
	    {"^Javascript:(.*)", "url('Javascript:$1');"},
	    {"^(x)=(-?[0-9]+)", "left:$2"},
	    {"^(y)=(-?[0-9]+)", "top:$2"},
	    {"^(w)=(-?[0-9]+)", "width:$2"},
	    {"^(h)=(-?[0-9]+)", "height:$2"},
	    {"^show$", "visibility:visible"},
	    {"^hide$", "visibility:hidden"},
		// Colour
	    {"(.*):([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)", "$1:#$2$3$4;"},
	    {"(.*)=([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)", "$1:#$2$3$4;"},
		{"^RGB:(.*)", "color:$1"},
		
		// File Management Contents	
	    {"^source=(http://.*)", "source:url('$1')"},	
	    {"^dest=(http://.*)", "destination:url('$1')"},	
	    {"^source=(ftp://.*)", "source:url('$1')"},	
	    {"^dest=(ftp://.*)", "destination:url('$1')"},	
	    {"^http://(.*)", "url('http://$1');"},
	    {"^https://(.*)", "url('https://$1');"},
	    {"^file://(.*)", "url('file://$1');"},
	    {"^ftp://(.*)", "url('ftp://$1');"},
	    {"^http:$", "protocol:http"},
	    {"^ftp:$", "protocol:ftp"},
	    {"^http$", "protocol:http"},
	    {"^http:([0-9]+)", "protocol:http; port:$1"},
	    {"^ftp$", "protocol:ftp"},
	    {"^ftp:([0-9]+)", "protocol:ftp; port:$1"},
	    {"^file$", "protocol:file"},
	    {"^source=(.*)", "source:$1"},	
	    {"^dest=(.*)", "destination:$1"},
	    {"^user=(.*)", "username:$1"},
	    {"^pass=(.*)", "password:$1"},
	    {"^username=(.*)", "username:$1"},
	    {"^password=(.*)", "password:$1"},
	    {"^send$", "transfer;SetFileDestination:FALSE"},
	    {"^receive$", "transfer;SetFileDestination:TRUE"},
		{"^createfolders=(.*)", "createfolders:$1"},
		{"^overwrite=(.*)", "overwrite:$1"},
		  
		  
		// Signature Capture Contents	
	    {"^noborder$", "border:hidden"},
	    {"^border$", "border:visible"},
	    {"^penwidth=(.*)", "penwidth:$1"},
	    {"^pencolor=(.*)", "pencolor:$1"},
	    {"^bgcolor=(.*)", "bgcolor:$1"},
	    {"^url=(.*)", "destination:$1"},
	    {"^user=(.*)", "username:$1"},
	    {"^pass=(.*)", "password:$1"},
	    {"^name=(.*)", "name:$1"},
	    {"^aim=(on|off)", "aim:$1"},
	    {"^lamp=(on|off)", "lamp:$1"},
	    
		// Scanner (Decoders) Contents
	    {"^autoenter$", "autoenter:enabled"},
	    {"^autotab$", "autotab:enabled"},
	    {"^RASTER_MODE_NONE$", "rastermode:none"},
	    {"^RASTER_MODE_OPEN_ALWAYS$", "rastermode:open_always"},
	    {"^RASTER_MODE_SMART$", "rastermode:smart"},
	    {"^RASTER_MODE_CYCLONE$", "rastermode:cyclone"},
	    {"^AIM_TYPE_TRIGGER$", "aimtype:trigger"},
	    {"^AIM_TYPE_TIMED_HOLD$", "aimtype:timed_hold"},
	    {"^AIM_TYPE_TIMED_RELEASE$", "aimtype:timed_release"},
	    {"^AIM_TYPE_CONTINUOUS_TRIGGER$", "aimtype:continuous_trigger"},
	    {"^AIM_TYPE_PRESENTATION$", "aimtype:presentation"},
	    {"^I2OF5_NO_CHECK_DIGIT$", "none"},
	    {"^I2OF5_USS_CHECK_DIGIT$", "uss"},
	    {"^I2OF5_OPCC_CHECK_DIGIT$", "opcc"},
	    {"^msi_chkdgt_mod_11_10$", "mod_11_10"},
	    {"^msi_chkdgt_mod_10_10$", "mod_10_10"},
	    {"^security_none$", "none"},
	    {"^security_ambiguous$", "ambiguous"},
	    {"^security_all$", "all"},
	    {"^supplementals_none$", "none"},
	    {"^supplementals_always$", "always"},
	    {"^supplementals_auto$", "auto"},
	    {"^PREAMBLE_NONE$", "none"},
	    {"^PREAMBLE_SYSTEM_CHAR$", "system_char"},
	    {"^PREAMBLE_COUNTRY_AND_SYSTEM_CHARS$", "country_and_system_chars"},
	    
		// Indicators Contents
	    {"^Left_GrowFromTop$", "IconPosition:Left;GraphPosition:Top"},
	    {"^Right_GrowFromTop$", "IconPosition:Right;GraphPosition:Top"},
	    {"^Bottom_GrowFromRight$", "IconPosition:Bottom;GraphPosition:Right"},
	    {"^Top_GrowFromRight$", "IconPosition:Top;GraphPosition:Right"},
	    {"^Left_GrowFromBottom$", "IconPosition:Left;GraphPosition:Bottom"},
	    {"^Right_GrowFromBottom$", "IconPosition:Right;GraphPosition:Bottom"},
	    {"^Bottom_GrowFromLeft$", "IconPosition:bottom;GraphPosition:left"},
	    {"^Top_GrowFromLeft$", "IconPosition:top;GraphPosition:left"},
	    
		// Card Reader Contents
	    {"^pintimeout=([0-9]+)", "pintimeout:$1"},
	    {"^pinentry=(on|off)", "pinentry:$1"},
	    {"^pandata=(.*)", "pandata:$1"},

		// Key Capture Contents
	    {"^remapto:(.*)", "remapto=$1"},

		// Device & Application Contents
	    {"^Interval=([0-9][0-9]):([0-9][0-9]):([0-9][0-9])", "Interval:$1-$2-$3"},
	    {"^Repeat=(true|false)", "Repeat:$1"},
	    // Alarm Time Parameter
		{"^Time=([0-9][0-9][0-9][0-9])-([0-9][0-9])-([0-9][0-9])t([0-9][0-9]):([0-9][0-9]):([0-9][0-9])([+-][0-9][0-9]):([0-9][0-9])", "Time:$1-$2-$3t$4-$5-$6$7-$8"},
		
		// Push Contents
	    {"^Port=([0-9]+)", "Port:$1"},
	    {"^Passkey=(.+)", "Passkey:$1"},
	    {"^Passkey=$", "Passkey:url('')"},

		// Telemetry Contents
		// Enable Parameters
		{"^Enable(ABSControlStatus)$", "Enable:$1"},
		{"^Enable(ThrottlePosition)$", "Enable:$1"},
		{"^Enable(ServiceBrakeSwitch)$", "Enable:$1"},
		{"^Enable(IgnitionStatus)$", "Enable:$1"},
		{"^Enable(ParkingBrakeSwitch)$", "Enable:$1"},
		{"^Enable(RoadSpeed)$", "Enable:$1"},
		{"^Enable(CruiseControlSwitch)$", "Enable:$1"},
		{"^Enable(BrakesSwitch)$", "Enable:$1"},
		{"^Enable(ClutchStatus)$", "Enable:$1"},
		{"^Enable(CruiseControlSetSpeed)$", "Enable:$1"},
		{"^Enable(AcceleratorPedalPos)$", "Enable:$1"},
		{"^Enable(EngineLoad)$", "Enable:$1"},
		{"^Enable(EngineOilPressure)$", "Enable:$1"},
		{"^Enable(BoostPressure)$", "Enable:$1"},
		{"^Enable(EngineCoolantTemperature)$", "Enable:$1"},
		{"^Enable(EngineRetarderStatus)$", "Enable:$1"},
		{"^Enable(ATC_ASR_Status)$", "Enable:$1"},
		{"^Enable(TransmissionRangeSelected)$", "Enable:$1"},
		{"^Enable(TransmissionRangeAttained)$", "Enable:$1"},
		{"^Enable(BatteryVoltage)$", "Enable:$1"},
		{"^Enable(EngineOilTemperature)$", "Enable:$1"},
		{"^Enable(TransmissionOilTemperature)$", "Enable:$1"},
		{"^Enable(FuelRate)$", "Enable:$1"},
		{"^Enable(InstantaneousFuelEconomy)$", "Enable:$1"},
		{"^Enable(AverageFuelEconomy)$", "Enable:$1"},
		{"^Enable(EngineRPM)$", "Enable:$1"},
		{"^Enable(TransmitterDiagnosticCode)$", "Enable:$1"},
		{"^Enable(TransmitterDiagnosticTable)$", "Enable:$1"},
		{"^Enable(VehicleIDNumber)$", "Enable:$1"},
		{"^Enable(VehicleGlobalPosition)$", "Enable:$1"},
		{"^Enable(TotalDistance)$", "Enable:$1"},
		// Disable Parameters
		{"^Disable(ABSControlStatus)$", "Disable:$1"},
		{"^Disable(ThrottlePosition)$", "Disable:$1"},
		{"^Disable(ServiceBrakeSwitch)$", "Disable:$1"},
		{"^Disable(IgnitionStatus)$", "Disable:$1"},
		{"^Disable(ParkingBrakeSwitch)$", "Disable:$1"},
		{"^Disable(RoadSpeed)$", "Disable:$1"},
		{"^Disable(CruiseControlSwitch)$", "Disable:$1"},
		{"^Disable(BrakesSwitch)$", "Disable:$1"},
		{"^Disable(ClutchStatus)$", "Disable:$1"},
		{"^Disable(CruiseControlSetSpeed)$", "Disable:$1"},
		{"^Disable(AcceleratorPedalPos)$", "Disable:$1"},
		{"^Disable(EngineLoad)$", "Disable:$1"},
		{"^Disable(EngineOilPressure)$", "Disable:$1"},
		{"^Disable(BoostPressure)$", "Disable:$1"},
		{"^Disable(EngineCoolantTemperature)$", "Disable:$1"},
		{"^Disable(EngineRetarderStatus)$", "Disable:$1"},
		{"^Disable(ATC_ASR_Status)$", "Disable:$1"},
		{"^Disable(TransmissionRangeSelected)$", "Disable:$1"},
		{"^Disable(TransmissionRangeAttained)$", "Disable:$1"},
		{"^Disable(BatteryVoltage)$", "Disable:$1"},
		{"^Disable(EngineOilTemperature)$", "Disable:$1"},
		{"^Disable(TransmissionOilTemperature)$", "Disable:$1"},
		{"^Disable(FuelRate)$", "Disable:$1"},
		{"^Disable(InstantaneousFuelEconomy)$", "Disable:$1"},
		{"^Disable(AverageFuelEconomy)$", "Disable:$1"},
		{"^Disable(EngineRPM)$", "Disable:$1"},
		{"^Disable(TransmitterDiagnosticCode)$", "Disable:$1"},
		{"^Disable(TransmitterDiagnosticTable)$", "Disable:$1"},
		{"^Disable(VehicleIDNumber)$", "Disable:$1"},
		{"^Disable(VehicleGlobalPosition)$", "Disable:$1"},
		{"^Disable(TotalDistance)$", "Disable:$1"}
//		//  Position
//		{"^noborder$", "border:hidden" }, // 
//		{"^border$", "border:visible" }, // 
//		{"^penwidth=(.*)", "penwidth:$1" }, // 
//		{"^pencolor=(.*)", "pencolor:$1" }, // 
//		{"^bgcolor=(.*)", "bgcolor:$1" }, // 
//		{"^url=(.*)", "destination:url('$1');" }, // 
//		{"^user=(.*)", "username:$1" }, // 
//		{"^pass=(.*)", "password:$1" }, // 
//		{"^name=(.*)", "name:$1" }, // 
//
//		{"^Javascript:(.*)", "url('Javascript:$1');" }, // Converts url string to 1.1 compatible url
//		{"^http://(.*)", "url('http://$1');" }, // Converts url string to 1.1 compatible url
//		{"^https://(.*)", "url('https://$1');" }, // Converts url string to 1.1 compatible url
//		{"^file://(.*)", "url('file://$1');" }, // Converts url string to 1.1 compatible url
//		{"^ftp://(.*)", "url('ftp://$1');" }, // Converts url string to 1.1 compatible url
//		{"(x)=(-?[0-9]+)", "left:$2" }, // Converts X params to left Param
//		{"(y)=(-?[0-9]+)", "top:$2" }, // Converts Y params to top Param
//		{"(w)=(-?[0-9]+)", "width:$2" }, // Converts W params to width Param
//		{"(h)=(-?[0-9]+)", "height:$2" }, // Converts H params to height Param
//		{"^show$", "visibility:visible" }, // Converts to visibility
//		{"^hide$", "visibility:hidden" }, // Converts to visibility
//		{"(.*):([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)", "$1:#$2$3$4;" }, // Converts color references
//		{"(.*)=([a-f0-9]+),([a-f0-9]+),([a-f0-9]+)", "$1:#$2$3$4;" }, // Converts color references
//
//		{"^autoenter$", "autoenter:enabled" }, // converting autoenter to 1.1
//		{"^autotab$", "autotab:enabled" }, // converting autotab to 1.1
//		{"^RASTER_MODE_NONE$", "rastermode:none" }, // converting scanner parameters
//		{"^RASTER_MODE_OPEN_ALWAYS$", "rastermode:open_always" }, // converting scanner parameters
//		{"^RASTER_MODE_SMART$", "rastermode:smart" }, // converting scanner parameters
//		{"^RASTER_MODE_CYCLONE$", "rastermode:cyclone" }, // converting scanner parameters
//		{"^AIM_TYPE_TRIGGER$", "aimtype:trigger" }, // converting scanner parameters
//		{"^AIM_TYPE_TIMED_HOLD$", "aimtype:timed_hold" }, // converting scanner parameters
//		{"^AIM_TYPE_TIMED_RELEASE$", "aimtype:timed_release" }, // converting scanner parameters
//		{"^AIM_TYPE_CONTINUOUS_TRIGGER$", "aimtype:continuous_trigger" }, // converting scanner parameters
//		{"^AIM_TYPE_PRESENTATION$", "aimtype:presentation" }, // converting scanner parameters
//		{"^I2OF5_NO_CHECK_DIGIT$", "none" }, // converting scanner parameters
//		{"^I2OF5_USS_CHECK_DIGIT$", "uss" }, // converting scanner parameters
//		{"^I2OF5_OPCC_CHECK_DIGIT$", "opcc" }, // converting scanner parameters
//		{"^msi_chkdgt_mod_11_10$", "mod_11_10" }, // converting scanner parameters
//		{"^msi_chkdgt_mod_10_10$", "mod_10_10" }, // converting scanner parameters
//		{"^security_none$", "none" }, // converting scanner parameters
//		{"^security_ambiguous$", "ambiguous" }, // converting scanner parameters
//		{"^security_all$", "all" }, // converting scanner parameters
//		{"^supplementals_none$", "none" }, // converting scanner parameters
//		{"^supplementals_always$", "always" }, // converting scanner parameters
//		{"^supplementals_auto$", "auto" }, // converting scanner parameters
//		{"^PREAMBLE_NONE$", "none" }, // converting scanner parameters
//		{"^PREAMBLE_SYSTEM_CHAR$", "system_char" }, // converting scanner parameters
//		{"^PREAMBLE_COUNTRY_AND_SYSTEM_CHARS$", "country_and_system_chars" }, // converting scanner parameters
//
//		{"^http:$", "protocol:http" }, // converting imager parameters
//		{"^ftp:$", "protocol:ftp" }, // converting imager parameters
//
//		{"^Left_GrowFromTop$", "IconPosition:Left;GraphPosition:Top" }, // converting battery parameters
//		{"^Right_GrowFromTop$", "IconPosition:Right;GraphPosition:Top" }, // converting battery parameters
//		{"^Bottom_GrowFromRight$", "IconPosition:Bottom;GraphPosition:Right" }, // converting battery parameters
//		{"^Top_GrowFromRight$", "IconPosition:Top;GraphPosition:Right" }, // converting battery parameters
//		{"^Left_GrowFromBottom$", "IconPosition:Left;GraphPosition:Bottom" }, // converting battery parameters
//		{"^Right_GrowFromBottom$", "IconPosition:Right;GraphPosition:Bottom" }, // converting battery parameters
//		{"^Bottom_GrowFromLeft$", "IconPosition:bottom;GraphPosition:left" }, // converting battery parameters
//		{"^Top_GrowFromLeft$", "IconPosition:top;GraphPosition:left" }, // converting battery parameters
//		{"^RGB:(.*)", "color:$1" }, // converting battery parameters
//
//		{"^http$", "protocol:http" }, // Convert HTTP Protocol
//		{"^http:([0-9]+)", "protocol:http; port:$1" }, // Convert HTTP Protocol and Port
//		{"^ftp$", "protocol:ftp" }, // Convert FTP Protocol
//		{"^ftp:([0-9]+)", "protocol:ftp; port:$1" }, // Convert FTP Protocol and Port
//		{"^file$", "protocol:file" }, // Convert File Protocol
//		{"^dest=(.*)", "destination:$1" }, // No need to shorten names
//		{"^user=(.*)", "username:$1" }, // No need to shorten names
//		{"^pass=(.*)", "password:$1" }, // No need to shorten names
//		{"^send=(.*)", "transfer:$1" }, // Amalgamated send and receive
//		{"^receive$", "transfer" }, // Amalgamated send and receive
//
//		{"^pandata=(.*)", "pandata:$1" }, //
	};
}