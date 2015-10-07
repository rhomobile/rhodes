(function () {
	
    // the order is important
    var bridgeMapping = [
        [/RhoSimulator/                    , "rhosimulator" ],
        [/Android/                         , "android"      ],
        [/iPhone|iPod|iPad/                , "ios"          ],
        [/Windows\s+Phone/                 , "wp8"          ],
        [/Windows\s+(?:Mobile|CE)|WM [0-9]/, "wm"           ],
        [/Windows/                         , "win32"        ]
    ];
    
        // autodetect a bridge type
    var detectedPlatformBridge = "";
    
    // Fix for bug in str.toLowerCase() method of String class in QTWebView component executing on Windows.
	for (var i = 0; i < bridgeMapping.length; ++i) 
	{
        if (bridgeMapping[i][0].test(navigator.userAgent)) 
		{
            detectedPlatformBridge = bridgeMapping[i][1];
            break;
        }
    }
	
	if((detectedPlatformBridge=="rhosimulator")||(detectedPlatformBridge=="win32"))
	{
	String.prototype.toLowerCase = function (str) { return __rhoNativeApi.toLowerCase(this); }
	}
	
})();