// jsr_class.js
//
// JSONscriptRequest -- a simple class for making HTTP requests
// using dynamically generated script tags and JSON
//
// Author: Jason Levitt
// Date: December 7th, 2005
//
// A SECURITY WARNING FROM DOUGLAS CROCKFORD:
// "The dynamic <script> tag hack suffers from a problem. It allows a page 
// to access data from any server in the web, which is really useful. 
// Unfortunately, the data is returned in the form of a script. That script 
// can deliver the data, but it runs with the same authority as scripts on 
// the base page, so it is able to steal cookies or misuse the authorization 
// of the user with the server. A rogue script can do destructive things to 
// the relationship between the user and the base server."
//
// So, be extremely cautious in your use of this script.
//
//
// Sample Usage:
//
// <script type="text/javascript" src="jsr_class.js"></script>
// 
// function callbackfunc(jsonData) {
//      alert('Latitude = ' + jsonData.ResultSet.Result[0].Latitude + 
//            '  Longitude = ' + jsonData.ResultSet.Result[0].Longitude);
//      aObj.removeScriptTag();
// }
//
// request = 'http://api.local.yahoo.com/MapsService/V1/geocode?appid=YahooDemo&
//            output=json&callback=callbackfunc&location=78704';
// aObj = new JSONscriptRequest(request);
// aObj.buildScriptTag();
// aObj.addScriptTag();
//
//


// Constructor -- pass a REST request URL to the constructor
//
function JSONscriptRequest(fullUrl) {
    // REST request path
    this.fullUrl = fullUrl; 
    // Keep IE from caching requests
    this.noCacheIE = '&noCacheIE=' + (new Date()).getTime();
    // Get the DOM location to put the script tag
    this.headLoc = document.getElementsByTagName("head").item(0);
    // Generate a unique script tag id
    this.scriptId = 'JscriptId' + JSONscriptRequest.scriptCounter++;
}

// Static script ID counter
JSONscriptRequest.scriptCounter = 1;

// buildScriptTag method
//
JSONscriptRequest.prototype.buildScriptTag = function () {

    // Create the script tag
    this.scriptObj = document.createElement("script");
    
    // Add script object attributes
    this.scriptObj.setAttribute("type", "text/javascript");
    this.scriptObj.setAttribute("charset", "utf-8");
    this.scriptObj.setAttribute("src", this.fullUrl + this.noCacheIE);
    this.scriptObj.setAttribute("id", this.scriptId);
}
 
// removeScriptTag method
// 
JSONscriptRequest.prototype.removeScriptTag = function () {
    // Destroy the script tag
    this.headLoc.removeChild(this.scriptObj);  
}

// addScriptTag method
//
JSONscriptRequest.prototype.addScriptTag = function () {
    // Create the script tag
    this.headLoc.appendChild(this.scriptObj);
}

