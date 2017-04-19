
var path = require('path');
var util = require('util');

function JXmobile(x) {
  if (!(this instanceof JXmobile)) return new JXmobile(x);

  this.name = x;
}

function callJXcoreNative(name, args) {
  var params = Array.prototype.slice.call(args, 0);
  var fnc = [name];
  var arr = fnc.concat(params);
  arr.push(cb);
  process.natives.callJXcoreNative.apply(null, arr);
}

JXmobile.prototype.call = function () {
  callJXcoreNative(this.name, arguments);
  return this;
};

global.Mobile = JXmobile;


// see jxcore.java - jxcore.m
process.setPaths();

jxcore.tasks.register(process.setPaths);

global.Mobile.onError = function(message, stack) {
    //TODO
};

// define feunction for inform RHomobile about http server started
global.Mobile.httpServerStarted = function() {
    process.natives.httpServerStarted();
}

// load Rhomobile API
var fs = require('fs');
var jsp = path.join(process.cwd(), '../rhoapi/rhoapi.js' );
var jsf = fs.readFileSync(jsp, 'utf8');
eval(jsf);

Rho.Log.info("Platform = " + process.platform, 'Node.js JS');
Rho.Log.info("Process ARCH = " + process.arch, 'Node.js JS');


global.Mobile.loadMainFile = function (filePath) {
  try {
    require(path.join(process.cwd(), filePath));
  } catch (e) {
    Error.captureStackTrace(e);
    Rho.Log.error('ERROR with loading app.js!', 'Node.js JS');
    Rho.Log.error(e.toString(), 'Node.js JS');
  }
};

process.on('uncaughtException', function (e) {
  Error.captureStackTrace(e);
  Rho.Log.error('ERROR:Exception !', 'Node.js JS');
  Rho.Log.error(e.message.toString(), 'Node.js JS');
  Rho.Log.error(JSON.stringify(e.stack).toString(), 'Node.js JS');
  Mobile.onError(e.message, JSON.stringify(e.stack));
});


// load Rhodes application code
jsp = path.join(process.cwd(), '../rhoapp.js' );
jsf = fs.readFileSync(jsp, 'utf8');
eval(jsf);
