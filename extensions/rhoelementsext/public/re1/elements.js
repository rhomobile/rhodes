if(/Android/.test(navigator.userAgent)) {

window.addressBar = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('addressBar', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('addressBar', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('addressBar', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('addressBar', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('addressBar', val.toString());
    }
}) ();
    
window.alarm = new (function() {
    
    Object.defineProperty(this, 'interval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('alarm', 'interval:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'repeat', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('alarm', 'repeat:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'time', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('alarm', 'time:' + val.toString());
        }
    });
	
    this['clear'] = function() {
        metaReceiver.setMeta('alarm', 'clear');
    }
	
    this['set'] = function() {
        metaReceiver.setMeta('alarm', 'set');
    }
	
    Object.defineProperty(this, 'alarmTriggered', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('alarm', 'alarmTriggered:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('alarm', val.toString());
    }
}) ();
    
window.application = new (function() {
    
    this['quit'] = function() {
        metaReceiver.setMeta('application', 'quit');
    }
	
    this['minimize'] = function() {
        metaReceiver.setMeta('application', 'minimize');
    }
	
    this['restore'] = function() {
        metaReceiver.setMeta('application', 'restore');
    }
	
    this['getVersion'] = function() {
        metaReceiver.setMeta('application', 'getVersion');
    }
	
    Object.defineProperty(this, 'applicationEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('application', 'applicationEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'versionEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('application', 'versionEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('application', val.toString());
    }
}) ();
    
window.appSwitcher = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('appSwitcher', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('appSwitcher', val.toString());
    }
}) ();
    
window.audioCapture = new (function() {
    
    Object.defineProperty(this, 'duration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'duration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'destination', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'destination:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'username', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'username:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'password', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'password:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'name', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'name:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codec', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'codec:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('audioCapture', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('audioCapture', 'stop');
    }
	
    this['cancel'] = function() {
        metaReceiver.setMeta('audioCapture', 'cancel');
    }
	
    Object.defineProperty(this, 'audioSaveEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('audioCapture', 'audioSaveEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('audioCapture', val.toString());
    }
}) ();
    
window.backButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('backButton', val.toString());
    }
}) ();
    
window.backlight = new (function() {
    
    Object.defineProperty(this, 'intensity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backlight', 'intensity:' + val.toString());
        }
    });
	
    this['on'] = function() {
        metaReceiver.setMeta('backlight', 'on');
    }
	
    this['off'] = function() {
        metaReceiver.setMeta('backlight', 'off');
    }
	
    this['getBacklightSettings'] = function() {
        metaReceiver.setMeta('backlight', 'getBacklightSettings');
    }
	
    Object.defineProperty(this, 'backlightSettingsEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('backlight', 'backlightSettingsEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('backlight', val.toString());
    }
}) ();
    
window.battery = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'layout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'layout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'color', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'color:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'sampleInterval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'sampleInterval:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setSystemNotifications', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'setSystemNotifications:' + val.toString());
        }
    });
	
    this['getSmartBatteryStatus'] = function() {
        metaReceiver.setMeta('battery', 'getSmartBatteryStatus');
    }
	
    this['getBatteryStatus'] = function() {
        metaReceiver.setMeta('battery', 'getBatteryStatus');
    }
	
    Object.defineProperty(this, 'batteryEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'batteryEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'smartBatteryEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('battery', 'smartBatteryEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('battery', val.toString());
    }
}) ();
    
window.bottomCommandArea = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('bottomCommandArea', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('bottomCommandArea', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'color', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('bottomCommandArea', 'color:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'image', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('bottomCommandArea', 'image:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('bottomCommandArea', val.toString());
    }
}) ();
    
window.cardReader = new (function() {
    
    Object.defineProperty(this, 'pinTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'pinTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pinEntry', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'pinEntry:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'panData', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'panData:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoTab', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'autoTab:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoEnter', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'autoEnter:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'moduleName', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'moduleName:' + val.toString());
        }
    });
	
    this['open'] = function() {
        metaReceiver.setMeta('cardReader', 'open');
    }
	
    this['close'] = function() {
        metaReceiver.setMeta('cardReader', 'close');
    }
	
    Object.defineProperty(this, 'readEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cardReader', 'readEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('cardReader', val.toString());
    }
}) ();
    
window.comm = new (function() {
    
    Object.defineProperty(this, 'baudRate', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'baudRate:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dataBits', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'dataBits:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stopBits', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'stopBits:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'parity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'parity:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'handshake', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'handshake:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'port', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'port:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'chars', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'chars:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'endChar', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'endChar:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'time', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'time:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'writeBytes', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'writeBytes:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'writeString', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'writeString:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'writeFile', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'writeFile:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoEnter', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'autoEnter:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoTab', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'autoTab:' + val.toString());
        }
    });
	
    this['open'] = function() {
        metaReceiver.setMeta('comm', 'open');
    }
	
    this['close'] = function() {
        metaReceiver.setMeta('comm', 'close');
    }
	
    Object.defineProperty(this, 'commEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('comm', 'commEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('comm', val.toString());
    }
}) ();
    
window.cradle = new (function() {
    
    this['getCradleId'] = function() {
        metaReceiver.setMeta('cradle', 'getCradleId');
    }
	
    Object.defineProperty(this, 'cradleEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('cradle', 'cradleEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('cradle', val.toString());
    }
}) ();
    
window.device = new (function() {
    
    this['suspend'] = function() {
        metaReceiver.setMeta('device', 'suspend');
    }
	
    this['powerOff'] = function() {
        metaReceiver.setMeta('device', 'powerOff');
    }
	
    this['wake'] = function() {
        metaReceiver.setMeta('device', 'wake');
    }
	
    this['calibrate'] = function() {
        metaReceiver.setMeta('device', 'calibrate');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('device', val.toString());
    }
}) ();
    
window.eink = new (function() {
    
    Object.defineProperty(this, 'screenUpdateBpp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('eink', 'screenUpdateBpp:' + val.toString());
        }
    });
	
    this['forceFullScreenUpdate'] = function() {
        metaReceiver.setMeta('eink', 'forceFullScreenUpdate');
    }
	
    this['pauseFullScreenUpdate'] = function() {
        metaReceiver.setMeta('eink', 'pauseFullScreenUpdate');
    }
	
    this['resumeFullScreenUpdate'] = function() {
        metaReceiver.setMeta('eink', 'resumeFullScreenUpdate');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('eink', val.toString());
    }
}) ();
    
window.emmlProfile = new (function() {
    
    Object.defineProperty(this, 'name', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('emmlProfile', 'name:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'importProfile', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('emmlProfile', 'importProfile:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'clear', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('emmlProfile', 'clear:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'apply', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('emmlProfile', 'apply:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('emmlProfile', val.toString());
    }
}) ();
    
window.fileTransfer = new (function() {
    
    Object.defineProperty(this, 'destination', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'destination:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'source', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'source:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'username', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'username:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'password', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'password:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'createFolders', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'createFolders:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'overWrite', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'overWrite:' + val.toString());
        }
    });
	
    this['transfer'] = function() {
        metaReceiver.setMeta('fileTransfer', 'transfer');
    }
	
    Object.defineProperty(this, 'transferEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('fileTransfer', 'transferEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('fileTransfer', val.toString());
    }
}) ();
    
window.forwardButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('forwardButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('forwardButton', val.toString());
    }
}) ();
    
window.gesture = new (function() {
    
    Object.defineProperty(this, 'type', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'type:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'id', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'id:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preset', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'preset:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'diagnostics', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'diagnostics:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'startX', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'startX:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'startY', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'startY:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'endX', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'endX:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'endY', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'endY:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'skew', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'skew:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'deviation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'deviation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'regionWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'regionWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'centerX', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'centerX:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'centerY', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'centerY:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'radius', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'radius:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'start', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'start:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'end', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'end:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tolerance', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'tolerance:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'sensitivity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'sensitivity:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'delay', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'delay:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'interval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'interval:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'TargetX', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'TargetX:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'TargetY', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'TargetY:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'TargetZ', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'TargetZ:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'TiltTolerance', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'TiltTolerance:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'Hysteresis', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'Hysteresis:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'Threshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'Threshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'Quiet', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'Quiet:' + val.toString());
        }
    });
	
    this['create'] = function() {
        metaReceiver.setMeta('gesture', 'create');
    }
	
    this['delete'] = function() {
        metaReceiver.setMeta('gesture', 'delete');
    }
	
    Object.defineProperty(this, 'detected', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('gesture', 'detected:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('gesture', val.toString());
    }
}) ();
    
window.goButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('goButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('goButton', val.toString());
    }
}) ();
    
window.homeButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('homeButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('homeButton', val.toString());
    }
}) ();
    
window.hourglass = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('hourglass', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('hourglass', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('hourglass', 'top:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('hourglass', val.toString());
    }
}) ();
    
window.imager = new (function() {
    
    Object.defineProperty(this, 'enabled', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'enabled:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'desiredWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'desiredWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'desiredHeight', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'desiredHeight:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lamp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'lamp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'aim', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'aim:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'username', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'username:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'password', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'password:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'sound', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'sound:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'destination', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'destination:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'deviceId', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'deviceId:' + val.toString());
        }
    });
	
    this['enable'] = function() {
        metaReceiver.setMeta('imager', 'enable');
    }
	
    this['disable'] = function() {
        metaReceiver.setMeta('imager', 'disable');
    }
	
    this['capture'] = function() {
        metaReceiver.setMeta('imager', 'capture');
    }
	
    this['enumerate'] = function() {
        metaReceiver.setMeta('imager', 'enumerate');
    }
	
    Object.defineProperty(this, 'imagerEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'imagerEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imagerCaptureEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'imagerCaptureEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imagerEnumEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('imager', 'imagerEnumEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('imager', val.toString());
    }
}) ();
    
window.ioAnalog = new (function() {
    
    Object.defineProperty(this, 'generalVoltage1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage1MaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage1MaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage1MinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage1MinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage1Debounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage1Debounce:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage2MaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage2MaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage2MinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage2MinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'generalVoltage2Debounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'generalVoltage2Debounce:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryVoltage', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryVoltage:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryVoltageMaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryVoltageMaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryVoltageMinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryVoltageMinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryVoltageDebounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryVoltageDebounce:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryCurrent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryCurrent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryCurrentMaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryCurrentMaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryCurrentMinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryCurrentMinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'BatteryCurrentDebounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'BatteryCurrentDebounce:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'AmbientTemperature', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'AmbientTemperature:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'AmbientTemperatureMaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'AmbientTemperatureMaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'AmbientTemperatureMinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'AmbientTemperatureMinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'AmbientTemperatureDebounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'AmbientTemperatureDebounce:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'HeaterTemperature', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'HeaterTemperature:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'HeaterTemperatureMaximumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'HeaterTemperatureMaximumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'HeaterTemperatureMinimumThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'HeaterTemperatureMinimumThreshold:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'HeaterTemperatureDebounce', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'HeaterTemperatureDebounce:' + val.toString());
        }
    });
	
    this['invokeGeneralVoltage1'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeGeneralVoltage1');
    }
	
    this['invokeGeneralVoltage2'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeGeneralVoltage2');
    }
	
    this['invokeBatteryVoltage'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeBatteryVoltage');
    }
	
    this['invokeBatteryCurrent'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeBatteryCurrent');
    }
	
    this['invokeAmbientTemperature'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeAmbientTemperature');
    }
	
    this['invokeHeaterTemperature'] = function() {
        metaReceiver.setMeta('ioAnalog', 'invokeHeaterTemperature');
    }
	
    Object.defineProperty(this, 'portEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioAnalog', 'portEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('ioAnalog', val.toString());
    }
}) ();
    
window.ioDigital = new (function() {
    
    Object.defineProperty(this, 'input1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input3', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input3:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState3', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState3:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState3', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState3:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState3', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState3:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input4', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input4:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState4', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState4:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState4', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState4:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState4', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState4:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input6', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input6:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState6', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState6:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState6', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState6:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState6', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState6:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input7', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input7:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState7', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState7:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState7', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState7:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState7', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState7:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'input8', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'input8:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputState8', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputState8:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputWarmBootState8', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputWarmBootState8:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'outputColdBootState8', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'outputColdBootState8:' + val.toString());
        }
    });
	
    this['InvokeDigital1'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital1');
    }
	
    this['InvokeDigital2'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital2');
    }
	
    this['InvokeDigital3'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital3');
    }
	
    this['InvokeDigital4'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital4');
    }
	
    this['InvokeDigital5'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital5');
    }
	
    this['InvokeDigital6'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital6');
    }
	
    this['InvokeDigital7'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital7');
    }
	
    this['InvokeDigital8'] = function() {
        metaReceiver.setMeta('ioDigital', 'InvokeDigital8');
    }
	
    Object.defineProperty(this, 'PortEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioDigital', 'PortEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('ioDigital', val.toString());
    }
}) ();
    
window.ioSystem = new (function() {
    
    Object.defineProperty(this, 'ignition', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioSystem', 'ignition:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'powerSource', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioSystem', 'powerSource:' + val.toString());
        }
    });
	
    this['invokeIgnition'] = function() {
        metaReceiver.setMeta('ioSystem', 'invokeIgnition');
    }
	
    this['invokePowerSource'] = function() {
        metaReceiver.setMeta('ioSystem', 'invokePowerSource');
    }
	
    Object.defineProperty(this, 'portEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('ioSystem', 'portEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('ioSystem', val.toString());
    }
}) ();
    
window.keyCapture = new (function() {
    
    Object.defineProperty(this, 'dispatch', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'dispatch:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'keyValue', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'keyValue:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'remap', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'remap:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'accelerateKey', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'accelerateKey:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'homeKeyValue', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'homeKeyValue:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'keyEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'keyEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'triggerEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyCapture', 'triggerEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('keyCapture', val.toString());
    }
}) ();
    
window.keylight = new (function() {
    
    Object.defineProperty(this, 'intensity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keylight', 'intensity:' + val.toString());
        }
    });
	
    this['on'] = function() {
        metaReceiver.setMeta('keylight', 'on');
    }
	
    this['off'] = function() {
        metaReceiver.setMeta('keylight', 'off');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('keylight', val.toString());
    }
}) ();
    
window.keyState = new (function() {
    
    Object.defineProperty(this, 'right', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'right:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('keyState', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('keyState', val.toString());
    }
}) ();
    
window.lightsensor = new (function() {
    
    Object.defineProperty(this, 'status', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('lightsensor', 'status:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'minimumInterval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('lightsensor', 'minimumInterval:' + val.toString());
        }
    });
	
    this['getSensorData'] = function() {
        metaReceiver.setMeta('lightsensor', 'getSensorData');
    }
	
    Object.defineProperty(this, 'LightSensorEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('lightsensor', 'LightSensorEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('lightsensor', val.toString());
    }
}) ();
    
window.mediaPlayer = new (function() {
    
    Object.defineProperty(this, 'filename', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('mediaPlayer', 'filename:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'type', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('mediaPlayer', 'type:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('mediaPlayer', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('mediaPlayer', 'stop');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('mediaPlayer', val.toString());
    }
}) ();
    
window.memory = new (function() {
    
    Object.defineProperty(this, 'lowMemThreshold', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('memory', 'lowMemThreshold:' + val.toString());
        }
    });
	
    this['getMemoryStats'] = function() {
        metaReceiver.setMeta('memory', 'getMemoryStats');
    }
	
    Object.defineProperty(this, 'memoryEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('memory', 'memoryEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('memory', val.toString());
    }
}) ();
    
window.minimizeButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('minimizeButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('minimizeButton', val.toString());
    }
}) ();
    
window.network = new (function() {
    
    Object.defineProperty(this, 'host', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'host:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'port', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'port:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkPollInterval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'networkPollInterval:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'connectionTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'connectionTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'connectWan', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'connectWan:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('network', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('network', 'stop');
    }
	
    this['disconnectWan'] = function() {
        metaReceiver.setMeta('network', 'disconnectWan');
    }
	
    Object.defineProperty(this, 'networkEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'networkEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'wanStatusEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('network', 'wanStatusEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('network', val.toString());
    }
}) ();
    
window.notification = new (function() {
    
    Object.defineProperty(this, 'setLEDOnDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setLEDOnDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setLEDOffDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setLEDOffDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setLEDNumberOfCycles', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setLEDNumberOfCycles:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setBeeperFrequency', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setBeeperFrequency:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setBeeperVolume', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setBeeperVolume:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setBeeperDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setBeeperDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setVibrateDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'setVibrateDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stateOn', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'stateOn:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stateOff', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'stateOff:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stateCycle', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'stateCycle:' + val.toString());
        }
    });
	
    this['enumerate'] = function() {
        metaReceiver.setMeta('notification', 'enumerate');
    }
	
    Object.defineProperty(this, 'enumNotificationsEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('notification', 'enumNotificationsEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('notification', val.toString());
    }
}) ();
    
window.zoomInPageButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageup', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'imageup:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imagedown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'imagedown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomInPageButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('zoomInPageButton', val.toString());
    }
}) ();
    
window.zoomOutPageButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageup', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'imageup:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imagedown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'imagedown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomOutPageButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('zoomOutPageButton', val.toString());
    }
}) ();
    
window.powerOn = new (function() {
    
    Object.defineProperty(this, 'powerOnEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('powerOn', 'powerOnEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('powerOn', val.toString());
    }
}) ();
    
window.push = new (function() {
    
    Object.defineProperty(this, 'port', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'port:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'passKey', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'passKey:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'response', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'response:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'path', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'path:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'unattended', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'unattended:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('push', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('push', 'stop');
    }
	
    Object.defineProperty(this, 'detected', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('push', 'detected:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('push', val.toString());
    }
}) ();
    
window.quitButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('quitButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('quitButton', val.toString());
    }
}) ();
    
window.settingsButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('settingsButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('settingsButton', val.toString());
    }
}) ();

window.RawSensors = new (function() {
    
    Object.defineProperty(this, 'accelerometer', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'accelerometer:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'deviceOrientation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'deviceOrientation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tiltangle', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'tiltangle:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'motion', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'motion:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ecompass', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'ecompass:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'magnetometer', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'magnetometer:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'gyroscope', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'gyroscope:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ambientlight', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'ambientlight:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'proximity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'proximity:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'proximitylongrange', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'proximitylongrange:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pressure', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'pressure:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'temperature', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'temperature:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'humidity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'humidity:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'gravity', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'gravity:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'linearAcceleration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'linearAcceleration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rotation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'rotation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'orientation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'orientation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'all', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'all:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'minimumInterval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'minimumInterval:' + val.toString());
        }
    });
	
    this['getSensorData'] = function() {
        metaReceiver.setMeta('RawSensors', 'getSensorData');
    }
	
    Object.defineProperty(this, 'sensorEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('RawSensors', 'sensorEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('RawSensors', val.toString());
    }
}) ();
    
window.reboot = new (function() {
    
    Object.defineProperty(this, 'bootType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reboot', 'bootType:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('reboot', val.toString());
    }
}) ();
    
window.registry = new (function() {
    
    Object.defineProperty(this, 'hive', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'hive:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'key', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'key:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setting', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'setting:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'type', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'type:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'persistent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'persistent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'value', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('registry', 'value:' + val.toString());
        }
    });
	
    this['delete'] = function() {
        metaReceiver.setMeta('registry', 'delete');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('registry', val.toString());
    }
}) ();
    
window.reloadButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('reloadButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('reloadButton', val.toString());
    }
}) ();
    
window.rfid = new (function() {
    
    Object.defineProperty(this, 'antennaSelected', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'antennaSelected:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'beepOnRead', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'beepOnRead:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagAccessStatus', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagAccessStatus:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagAntennaID', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagAntennaID:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagCRC', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagCRC:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagXPC', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagXPC:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagPC', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagPC:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagRSSI', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagRSSI:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagSeenCount', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagSeenCount:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTagUTCTimeStamp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enableTagUTCTimeStamp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'invMemBank', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'invMemBank:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lockField', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'lockField:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lockPrivilege', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'lockPrivilege:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matchPattern', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'matchPattern:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'newTagID', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'newTagID:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'newAccessPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'newAccessPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'newKillPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'newKillPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'performStateAwareSingulation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'performStateAwareSingulation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterStateAwareAction', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterStateAwareAction:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterTarget', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterTarget:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterStateUnawareAction', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterStateUnawareAction:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterBitCount', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterBitCount:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterBitOffset', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterBitOffset:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterHexPattern', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterHexPattern:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterID', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterID:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'preFilterMemBank', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'preFilterMemBank:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'startPeriod', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'startPeriod:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'reportTrigger', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'reportTrigger:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'reportUniqueTags', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'reportUniqueTags:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'RFMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'RFMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'singulationSession', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'singulationSession:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'singulationTagPopulation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'singulationTagPopulation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'singulationInventoryState', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'singulationInventoryState:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'singulationSLFlag', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'singulationSLFlag:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'startTriggerType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'startTriggerType:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stopDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'stopDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stopObservationCount', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'stopObservationCount:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'stopTriggerType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'stopTriggerType:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagByteOffset', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagByteOffset:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagID', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagID:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagKillPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagKillPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagMemBank', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagMemBank:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternAByteOffset', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternAByteOffset:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternAHexMask', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternAHexMask:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternAHexPattern', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternAHexPattern:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternAMemBank', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternAMemBank:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternBByteOffset', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternBByteOffset:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternBHexMask', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternBHexMask:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternBHexPattern', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternBHexPattern:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagPatternBMemBank', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagPatternBMemBank:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagReadSize', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagReadSize:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagWriteData', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagWriteData:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'transmitPower', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'transmitPower:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'useAccessFilter', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'useAccessFilter:' + val.toString());
        }
    });
	
    this['enumerate'] = function() {
        metaReceiver.setMeta('rfid', 'enumerate');
    }
	
    this['connect'] = function() {
        metaReceiver.setMeta('rfid', 'connect');
    }
	
    this['disconnect'] = function() {
        metaReceiver.setMeta('rfid', 'disconnect');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('rfid', 'stop');
    }
	
    this['performInventory'] = function() {
        metaReceiver.setMeta('rfid', 'performInventory');
    }
	
    this['locateTag'] = function() {
        metaReceiver.setMeta('rfid', 'locateTag');
    }
	
    this['killTag'] = function() {
        metaReceiver.setMeta('rfid', 'killTag');
    }
	
    this['getRadioPowerState'] = function() {
        metaReceiver.setMeta('rfid', 'getRadioPowerState');
    }
	
    this['addPreFilter'] = function() {
        metaReceiver.setMeta('rfid', 'addPreFilter');
    }
	
    this['deletePreFilter'] = function() {
        metaReceiver.setMeta('rfid', 'deletePreFilter');
    }
	
    this['clearPreFilters'] = function() {
        metaReceiver.setMeta('rfid', 'clearPreFilters');
    }
	
    this['radioPowerStateOn'] = function() {
        metaReceiver.setMeta('rfid', 'radioPowerStateOn');
    }
	
    this['radioPowerStateOff'] = function() {
        metaReceiver.setMeta('rfid', 'radioPowerStateOff');
    }
	
    this['tagRead'] = function() {
        metaReceiver.setMeta('rfid', 'tagRead');
    }
	
    this['tagWrite'] = function() {
        metaReceiver.setMeta('rfid', 'tagWrite');
    }
	
    this['setTagID'] = function() {
        metaReceiver.setMeta('rfid', 'setTagID');
    }
	
    this['setTagAccessPassword'] = function() {
        metaReceiver.setMeta('rfid', 'setTagAccessPassword');
    }
	
    this['setTagKillPassword'] = function() {
        metaReceiver.setMeta('rfid', 'setTagKillPassword');
    }
	
    this['setSingulation'] = function() {
        metaReceiver.setMeta('rfid', 'setSingulation');
    }
	
    this['getSingulation'] = function() {
        metaReceiver.setMeta('rfid', 'getSingulation');
    }
	
    this['getRFParams'] = function() {
        metaReceiver.setMeta('rfid', 'getRFParams');
    }
	
    this['tagLock'] = function() {
        metaReceiver.setMeta('rfid', 'tagLock');
    }
	
    this['getRFModeInfo'] = function() {
        metaReceiver.setMeta('rfid', 'getRFModeInfo');
    }
	
    this['setRFParams'] = function() {
        metaReceiver.setMeta('rfid', 'setRFParams');
    }
	
    this['getLastAccessResult'] = function() {
        metaReceiver.setMeta('rfid', 'getLastAccessResult');
    }
	
    Object.defineProperty(this, 'enumRFIDEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'enumRFIDEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rfParamsEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'rfParamsEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rfModeInfoEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'rfModeInfoEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tagEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'tagEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'radioPowerStateEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'radioPowerStateEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'statusEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'statusEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'singulationEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'singulationEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'operationCompleteEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'operationCompleteEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lastAccessResultEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rfid', 'lastAccessResultEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('rfid', val.toString());
    }
}) ();
    
window.rsm = new (function() {
    
    Object.defineProperty(this, 'bluetoothAuthentication', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothAuthentication:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothEncryption', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothEncryption:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothPinCode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothPinCode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothPinCodeType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothPinCodeType:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothReconnectAttempts', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothReconnectAttempts:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothBeepOnReconnectAttempt', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothBeepOnReconnectAttempt:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothHidAutoReconnect', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothHidAutoReconnect:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothFriendlyName', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothFriendlyName:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothInquiryMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothInquiryMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothAutoReconnect', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'bluetoothAutoReconnect:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'forceSavePairingBarcode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'forceSavePairingBarcode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lowBatteryIndication', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'lowBatteryIndication:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'lowBatteryIndicationCycle', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'lowBatteryIndicationCycle:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'scanLineWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'scanLineWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'goodScansDelay', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'goodScansDelay:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'decodeFeedback', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'decodeFeedback:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ignoreCode128Usps', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'ignoreCode128Usps:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'scanTriggerWakeup', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'scanTriggerWakeup:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'mems', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'mems:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'proximityEnable', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'proximityEnable:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'proximityContinuous', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'proximityContinuous:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'proximityDistance', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'proximityDistance:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pagingEnable', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'pagingEnable:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pagingBeepSequence', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'pagingBeepSequence:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pagingActivate', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'pagingActivate:' + val.toString());
        }
    });
	
    this['markForRetrievalModelNumber'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalModelNumber');
    }
	
    this['markForRetrievalSerialNumber'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalSerialNumber');
    }
	
    this['markForRetrievalDateOfManufacture'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalDateOfManufacture');
    }
	
    this['markForRetrievalDateOfService'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalDateOfService');
    }
	
    this['markForRetrievalBluetoothAddress'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothAddress');
    }
	
    this['markForRetrievalFirmwareVersion'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalFirmwareVersion');
    }
	
    this['markForRetrievalDeviceClass'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalDeviceClass');
    }
	
    this['markForRetrievalBatteryStatus'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBatteryStatus');
    }
	
    this['markForRetrievalBatteryCapacity'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBatteryCapacity');
    }
	
    this['markForRetrievalBatteryId'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBatteryId');
    }
	
    this['markForRetrievalBluetoothAuthentication'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothAuthentication');
    }
	
    this['markForRetrievalBluetoothEncryption'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothEncryption');
    }
	
    this['markForRetrievalBluetoothPinCode'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothPinCode');
    }
	
    this['markForRetrievalBluetoothPinCodeType'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothPinCodeType');
    }
	
    this['markForRetrievalBluetoothReconnectAttempts'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothReconnectAttempts');
    }
	
    this['markForRetrievalBluetoothBeepOnReconnectAttempt'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothBeepOnReconnectAttempt');
    }
	
    this['markForRetrievalBluetoothHidAutoReconnect'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothHidAutoReconnect');
    }
	
    this['markForRetrievalBluetoothFriendlyName'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothFriendlyName');
    }
	
    this['markForRetrievalBluetoothInquiryMode'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothInquiryMode');
    }
	
    this['markForRetrievalBluetoothAutoReconnect'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalBluetoothAutoReconnect');
    }
	
    this['markForRetrievalForceSavePairingBarcode'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalForceSavePairingBarcode');
    }
	
    this['markForRetrievalLowBatteryIndication'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalLowBatteryIndication');
    }
	
    this['markForRetrievalLowBatteryIndicationCycle'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalLowBatteryIndicationCycle');
    }
	
    this['markForRetrievalScanLineWidth'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalScanLineWidth');
    }
	
    this['markForRetrievalGoodScansDelay'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalGoodScansDelay');
    }
	
    this['markForRetrievalDecodeFeedback'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalDecodeFeedback');
    }
	
    this['markForRetrievalIgnoreCode128Usps'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalIgnoreCode128Usps');
    }
	
    this['markForRetrievalScanTriggerWakeup'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalScanTriggerWakeup');
    }
	
    this['markForRetrievalMems'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalMems');
    }
	
    this['markForRetrievalProximityEnable'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalProximityEnable');
    }
	
    this['markForRetrievalProximityContinuous'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalProximityContinuous');
    }
	
    this['markForRetrievalProximityDistance'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalProximityDistance');
    }
	
    this['markForRetrievalPagingEnable'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalPagingEnable');
    }
	
    this['markForRetrievalPagingBeepSequence'] = function() {
        metaReceiver.setMeta('rsm', 'markForRetrievalPagingBeepSequence');
    }
	
    this['bluetoothDisconnect'] = function() {
        metaReceiver.setMeta('rsm', 'bluetoothDisconnect');
    }
	
    this['bluetoothUnpair'] = function() {
        metaReceiver.setMeta('rsm', 'bluetoothUnpair');
    }
	
    Object.defineProperty(this, 'rsmGetEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('rsm', 'rsmGetEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('rsm', val.toString());
    }
}) ();
    
window.screenOrientation = new (function() {
    
    Object.defineProperty(this, 'autoRotate', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('screenOrientation', 'autoRotate:' + val.toString());
        }
    });
	
    this['normal'] = function() {
        metaReceiver.setMeta('screenOrientation', 'normal');
    }
	
    this['rightHanded'] = function() {
        metaReceiver.setMeta('screenOrientation', 'rightHanded');
    }
	
    this['leftHanded'] = function() {
        metaReceiver.setMeta('screenOrientation', 'leftHanded');
    }
	
    this['upsideDown'] = function() {
        metaReceiver.setMeta('screenOrientation', 'upsideDown');
    }
	
    Object.defineProperty(this, 'screenOrientationEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('screenOrientation', 'screenOrientationEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('screenOrientation', val.toString());
    }
}) ();
    
window.signal = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'layout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'layout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'color', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'color:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signalEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signal', 'signalEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('signal', val.toString());
    }
}) ();
    
window.signatureCapture = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'border', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'border:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'penWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'penWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'penColor', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'penColor:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bgColor', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'bgColor:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'destination', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'destination:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'username', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'username:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'password', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'password:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'name', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'name:' + val.toString());
        }
    });
	
    this['clear'] = function() {
        metaReceiver.setMeta('signatureCapture', 'clear');
    }
	
    this['capture'] = function() {
        metaReceiver.setMeta('signatureCapture', 'capture');
    }
	
    Object.defineProperty(this, 'signatureSaveEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'signatureSaveEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signatureCaptureEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'signatureCaptureEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'vectorEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('signatureCapture', 'vectorEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('signatureCapture', val.toString());
    }
}) ();
    
window.sip = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sip', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sip', 'top:' + val.toString());
        }
    });
	
    this['manual'] = function() {
        metaReceiver.setMeta('sip', 'manual');
    }
	
    this['automatic'] = function() {
        metaReceiver.setMeta('sip', 'automatic');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('sip', val.toString());
    }
}) ();
    
window.sipButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sipButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('sipButton', val.toString());
    }
}) ();
    
window.sntp = new (function() {
    
    Object.defineProperty(this, 'serverIp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('sntp', 'serverIp:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('sntp', val.toString());
    }
}) ();
    
window.stopButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('stopButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('stopButton', val.toString());
    }
}) ();
    
window.stylus = new (function() {
    
    this['enable'] = function() {
        metaReceiver.setMeta('stylus', 'enable');
    }
	
    this['disable'] = function() {
        metaReceiver.setMeta('stylus', 'disable');
    }
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('stylus', val.toString());
    }
}) ();
    
window.systemTime = new (function() {
    
    Object.defineProperty(this, 'setLocal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('systemTime', 'setLocal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'setUtc', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('systemTime', 'setUtc:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'sntpServerIp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('systemTime', 'sntpServerIp:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('systemTime', val.toString());
    }
}) ();
    
window.telemetry = new (function() {
    
    Object.defineProperty(this, 'queryFrequency', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('telemetry', 'queryFrequency:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enabled', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('telemetry', 'enabled:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'disabled', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('telemetry', 'disabled:' + val.toString());
        }
    });
	
    this['invoke'] = function() {
        metaReceiver.setMeta('telemetry', 'invoke');
    }
	
    Object.defineProperty(this, 'telemetryDataEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('telemetry', 'telemetryDataEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'telemetryArrayEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('telemetry', 'telemetryArrayEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('telemetry', val.toString());
    }
}) ();
    
window.timer = new (function() {
    
    Object.defineProperty(this, 'interval', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('timer', 'interval:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('timer', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('timer', 'stop');
    }
	
    Object.defineProperty(this, 'timeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('timer', 'timeout:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('timer', val.toString());
    }
}) ();
    
window.topCommandArea = new (function() {
    
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('topCommandArea', 'visibility:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('topCommandArea', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'color', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('topCommandArea', 'color:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'image', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('topCommandArea', 'image:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('topCommandArea', val.toString());
    }
}) ();
    
window.videoCapture = new (function() {
    
    Object.defineProperty(this, 'duration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'duration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'destination', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'destination:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'username', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'username:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'password', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'password:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'name', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'name:' + val.toString());
        }
    });
	
    this['start'] = function() {
        metaReceiver.setMeta('videoCapture', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('videoCapture', 'stop');
    }
	
    this['cancel'] = function() {
        metaReceiver.setMeta('videoCapture', 'cancel');
    }
	
    Object.defineProperty(this, 'videoSaveEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('videoCapture', 'videoSaveEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('videoCapture', val.toString());
    }
}) ();
    
window.volume = new (function() {
    
    Object.defineProperty(this, 'setVolume', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('volume', 'setVolume:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('volume', val.toString());
    }
}) ();
    
window.wake = new (function() {
    
    Object.defineProperty(this, 'wakeLock', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wake', 'wakeLock:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'wifiLock', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wake', 'wifiLock:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('wake', val.toString());
    }
}) ();
    
window.wlan = new (function() {
    
    Object.defineProperty(this, 'parameter', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'parameter:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileGuid', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileGuid:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkUsername', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'networkUsername:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'networkPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkDomain', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'networkDomain:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'selectLoginProfile', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'selectLoginProfile:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileEssid', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileEssid:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileName', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileName:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profilePassphrase', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profilePassphrase:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileEncryption', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileEncryption:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileSecurityMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileSecurityMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'deleteAfterImport', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'deleteAfterImport:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'importConfigFromLocation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'importConfigFromLocation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'exportType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'exportType:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'exportConfigToLocation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'exportConfigToLocation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'configFilePassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'configFilePassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileGuids', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileGuids:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'countryCode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'countryCode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'optionsPassword', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'optionsPassword:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'regulatory11dMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'regulatory11dMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'regulatoryCountryCode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'regulatoryCountryCode:' + val.toString());
        }
    });
	
    this['enableAdapter'] = function() {
        metaReceiver.setMeta('wlan', 'enableAdapter');
    }
	
    this['disableAdapter'] = function() {
        metaReceiver.setMeta('wlan', 'disableAdapter');
    }
	
    this['getAdapterPowerState'] = function() {
        metaReceiver.setMeta('wlan', 'getAdapterPowerState');
    }
	
    this['resetToDefault'] = function() {
        metaReceiver.setMeta('wlan', 'resetToDefault');
    }
	
    this['deleteProfile'] = function() {
        metaReceiver.setMeta('wlan', 'deleteProfile');
    }
	
    this['networkLogin'] = function() {
        metaReceiver.setMeta('wlan', 'networkLogin');
    }
	
    this['networkLogout'] = function() {
        metaReceiver.setMeta('wlan', 'networkLogout');
    }
	
    this['cancelNetworkLogin'] = function() {
        metaReceiver.setMeta('wlan', 'cancelNetworkLogin');
    }
	
    this['addProfile'] = function() {
        metaReceiver.setMeta('wlan', 'addProfile');
    }
	
    this['connectProfile'] = function() {
        metaReceiver.setMeta('wlan', 'connectProfile');
    }
	
    this['importConfig'] = function() {
        metaReceiver.setMeta('wlan', 'importConfig');
    }
	
    this['getAllProfiles'] = function() {
        metaReceiver.setMeta('wlan', 'getAllProfiles');
    }
	
    this['getCurrentProfile'] = function() {
        metaReceiver.setMeta('wlan', 'getCurrentProfile');
    }
	
    this['getConnectionState'] = function() {
        metaReceiver.setMeta('wlan', 'getConnectionState');
    }
	
    this['exportConfig'] = function() {
        metaReceiver.setMeta('wlan', 'exportConfig');
    }
	
    this['enableDot11d'] = function() {
        metaReceiver.setMeta('wlan', 'enableDot11d');
    }
	
    this['disableDot11d'] = function() {
        metaReceiver.setMeta('wlan', 'disableDot11d');
    }
	
    this['getDot11dMode'] = function() {
        metaReceiver.setMeta('wlan', 'getDot11dMode');
    }
	
    this['getCountryCode'] = function() {
        metaReceiver.setMeta('wlan', 'getCountryCode');
    }
	
    this['getCountryList'] = function() {
        metaReceiver.setMeta('wlan', 'getCountryList');
    }
	
    this['setCountryCode'] = function() {
        metaReceiver.setMeta('wlan', 'setCountryCode');
    }
	
    this['disableProfile'] = function() {
        metaReceiver.setMeta('wlan', 'disableProfile');
    }
	
    this['enableProfile'] = function() {
        metaReceiver.setMeta('wlan', 'enableProfile');
    }
	
    this['validateOptionsPassword'] = function() {
        metaReceiver.setMeta('wlan', 'validateOptionsPassword');
    }
	
    this['getOptionsPasswordState'] = function() {
        metaReceiver.setMeta('wlan', 'getOptionsPasswordState');
    }
	
    this['getRegulatorySettings'] = function() {
        metaReceiver.setMeta('wlan', 'getRegulatorySettings');
    }
	
    Object.defineProperty(this, 'adapterPowerStateEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'adapterPowerStateEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'loginResponseEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'loginResponseEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'logoutResponseEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'logoutResponseEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'cancelLoginResponseEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'cancelLoginResponseEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'loginCandidateProfilesEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'loginCandidateProfilesEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkLoginEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'networkLoginEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'networkLoginCloseEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'networkLoginCloseEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'importConfigEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'importConfigEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'addedProfileEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'addedProfileEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'allProfilesEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'allProfilesEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'currentProfileEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'currentProfileEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'connectionStateEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'connectionStateEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'exportConfigEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'exportConfigEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dot11dModeEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'dot11dModeEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'countryCodeEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'countryCodeEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'countryListEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'countryListEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'profileStateEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'profileStateEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'validateOptionsPasswordEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'validateOptionsPasswordEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'optionsPasswordStateEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'optionsPasswordStateEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'regulatorySettingsEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('wlan', 'regulatorySettingsEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('wlan', val.toString());
    }
}) ();
    
window.zoom = new (function() {
    
    Object.defineProperty(this, 'text', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoom', 'text:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'page', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoom', 'page:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('zoom', val.toString());
    }
}) ();
    
window.zoomTextButton = new (function() {
    
    Object.defineProperty(this, 'left', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'left:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'top', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'top:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'height', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'height:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'width', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'width:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageUp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'imageUp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'imageDown', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'imageDown:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'visibility', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('zoomTextButton', 'visibility:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('zoomTextButton', val.toString());
    }
}) ();
    
window.scanner = new (function() {
    
    Object.defineProperty(this, 'allDecoders', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'allDecoders:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ausPostal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ausPostal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'aztec', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'aztec:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'canPostal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'canPostal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'chinese2of5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'chinese2of5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabar', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabar:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabarClsiEditing', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabarClsiEditing:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabarMaxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabarMaxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabarMinLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabarMinLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabarNotisEditing', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabarNotisEditing:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'codabarRedundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'codabarRedundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11checkDigitCount', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11checkDigitCount:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code11reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code11reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128checkIsBtTable', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128checkIsBtTable:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128Ean128', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128Ean128:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128isBt128', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128isBt128:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128isBt128ConcatMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128isBt128ConcatMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128other128', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128other128:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code128securityLevel', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code128securityLevel:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39code32prefix', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39code32prefix:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39concatenation', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39concatenation:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39convertToCode32', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39convertToCode32:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39FullAscii', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39FullAscii:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39securityLevel', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39securityLevel:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code39verifyCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code39verifyCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code93', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code93:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code93maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code93maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code93minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code93minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'code93redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'code93redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'compositeC', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'compositeC:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'compositeAb', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'compositeAb:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'compositeAbUccLinkMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'compositeAbUccLinkMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'compositeAbUseUpcPreambleCheckDigitRules', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'compositeAbUseUpcPreambleCheckDigitRules:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'd2of5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'd2of5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'd2of5maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'd2of5maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'd2of5minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'd2of5minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'd2of5redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'd2of5redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dataMatrix', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'dataMatrix:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dutchPostal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'dutchPostal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ean13', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ean13:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ean8', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ean8:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ean8convertToEan13', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ean8convertToEan13:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'gs1dataBar', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'gs1dataBar:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'gs1dataBarExpanded', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'gs1dataBarExpanded:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'gs1dataBarLimited', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'gs1dataBarLimited:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5convertToEan13', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5convertToEan13:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2of5reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2of5reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'i2Of5verifyCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'i2Of5verifyCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'japPostal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'japPostal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'korean3of5maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'korean3of5maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'korean3of5minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'korean3of5minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'korean3of5redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'korean3of5redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'korean3of5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'korean3of5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroMicroPdf', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroMicroPdf:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroMicroPdfBufferLabels', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroMicroPdfBufferLabels:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroMicroPdfConvertToMicroPdf', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroMicroPdfConvertToMicroPdf:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroMicroPdfExclusive', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroMicroPdfExclusive:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroMicroPdfReportAppendInfo', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroMicroPdfReportAppendInfo:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroPdf', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroPdf:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroPdfBufferLabels', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroPdfBufferLabels:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroPdfConvertToPdf417', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroPdfConvertToPdf417:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'macroPdfExclusive', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'macroPdfExclusive:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matrix2of5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'matrix2of5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matrix2of5maxLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'matrix2of5maxLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matrix2of5minLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'matrix2of5minLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matrix2of5reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'matrix2of5reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'matrix2of5verifyCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'matrix2of5verifyCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'maxiCode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'maxiCode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'microPdf', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'microPdf:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'microQr', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'microQr:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msi', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msi:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiCheckDigits', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiCheckDigits:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiCheckDigitScheme', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiCheckDigitScheme:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiMaxlength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiMaxlength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiMinLength', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiMinLength:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiRedundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiRedundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'msiReportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'msiReportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'pdf417', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'pdf417:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'qrCode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'qrCode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rss14', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'rss14:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rssExp', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'rssExp:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rssLim', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'rssLim:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enabled', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'enabled:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoEnter', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'autoEnter:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'autoTab', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'autoTab:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'linearSecurityLevel', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'linearSecurityLevel:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'scanTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'scanTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rasterMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'rasterMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'rasterHeight', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'rasterHeight:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'aimType', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'aimType:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'timedAimDuration', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'timedAimDuration:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'sameSymbolTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'sameSymbolTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'differentSymbolTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'differentSymbolTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'aimMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'aimMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'picklistMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'picklistMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderX', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderX:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderY', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderY:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderHeight', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderHeight:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderFeedback', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderFeedback:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'viewfinderFeedbackTime', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'viewfinderFeedbackTime:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'focusMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'focusMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'illuminationMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'illuminationMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dpmMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'dpmMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'inverse1dMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'inverse1dMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'poorQuality1dMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'poorQuality1dMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'beamWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'beamWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dbpMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'dbpMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'klasseEins', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'klasseEins:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'adaptiveScanning', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'adaptiveScanning:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bidirectionalRedundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'bidirectionalRedundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'barcodeDataFormat', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'barcodeDataFormat:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'dataBufferSize', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'dataBufferSize:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'connectionIdleTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'connectionIdleTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'disconnectBtOnDisable', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'disconnectBtOnDisable:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'displayBtAddressBarcodeOnEnable', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'displayBtAddressBarcodeOnEnable:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enableTimeout', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'enableTimeout:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signature', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'signature:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signatureImageHeight', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'signatureImageHeight:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signatureImageQuality', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'signatureImageQuality:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signatureImageSize', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'signatureImageSize:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'signatureImageWidth', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'signatureImageWidth:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'tlc39', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'tlc39:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'trioptic39', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'trioptic39:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'trioptic39redundancy', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'trioptic39redundancy:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ukPostal', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ukPostal:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'ukPostalreportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'ukPostalreportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upca', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upca:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcaPreamble', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcaPreamble:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcaReportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcaReportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce0', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce0:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce0convertToUpca', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce0convertToUpca:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce0preamble', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce0preamble:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce0reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce0reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce1', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce1:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce1convertToUpca', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce1convertToUpca:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce1preamble', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce1preamble:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upce1reportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upce1reportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanBookland', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanBookland:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanBooklandFormat', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanBooklandFormat:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanConvertGs1dataBarToUpcEan', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanConvertGs1dataBarToUpcEan:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanConvertRssToUpcEan', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanConvertRssToUpcEan:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanCoupon', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanCoupon:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanLinearDecode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanLinearDecode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanRandomWeightCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanRandomWeightCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanRetryCount', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanRetryCount:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanSecurityLevel', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanSecurityLevel:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanSupplemental2', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanSupplemental2:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanSupplemental5', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanSupplemental5:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'upcEanSupplementalMode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'upcEanSupplementalMode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'us4state', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'us4state:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'us4stateFics', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'us4stateFics:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'usPlanet', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'usPlanet:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'usPlanetreportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'usPlanetreportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'usPostNet', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'usPostNet:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'usPostNetreportCheckDigit', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'usPostNetreportCheckDigit:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'webcode', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'webcode:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'webcodeDecodeGtSubtype', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'webcodeDecodeGtSubtype:' + val.toString());
        }
    });
	
    this['enumerate'] = function() {
        metaReceiver.setMeta('scanner', 'enumerate');
    }
	
    this['enable'] = function() {
        metaReceiver.setMeta('scanner', 'enable');
    }
	
    this['disable'] = function() {
        metaReceiver.setMeta('scanner', 'disable');
    }
	
    this['start'] = function() {
        metaReceiver.setMeta('scanner', 'start');
    }
	
    this['stop'] = function() {
        metaReceiver.setMeta('scanner', 'stop');
    }
	
    Object.defineProperty(this, 'decodeEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'decodeEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'bluetoothStatusEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'bluetoothStatusEvent:' + val.toString());
        }
    });
	
    Object.defineProperty(this, 'enumScannerEvent', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('scanner', 'enumScannerEvent:' + val.toString());
        }
    });
	
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('scanner', val.toString());
    }
}) ();

window.generic = new (function() {
    Object.defineProperty(this, 'UUID', {
        enumerable: true,
        get: function () { return genericImpl.getuuid(); },
        set: function(val) {}
    });
    Object.defineProperty(this, 'OEMInfo', {
        enumerable: true,
        get: function () { return genericImpl.getOEMInfo(); },
        set: function(val) {}
    });
    
    this['InvokeMETAFunction'] = function(equiv, content) {
        metaReceiver.setMeta(equiv, content);
    };
    
    this['LaunchProcessNonBlocking'] = function(appName, args) {
        return genericImpl.LaunchProcessNonBlocking(appName, args);
    };
    
    this['GetProcessExitCode'] = function(handle) {
        return genericImpl.GetProcessExitCode(handle);
    };
    
    this['CloseProcess'] = function(handle) {
        return genericImpl.CloseProcess(handle);
    };
	
    this['PlayWave'] = function(filename, flags) {
		return genericImpl.PlayWave(filename, flags);
	};
	
	this['SPBLog'] = function(logMessage, severity) {
		return genericImpl.SPBLog(logMessage, severity);
	};
	
	this['Log'] = function(logMessage, severity) {
		return genericImpl.Log(logMessage, severity);
	};
})();

window.apd = new (function() {
    this['PSExternal'] = function(iCommand, strParameter) {
        return apdImpl.PSExternal(iCommand, strParameter);
    };
    
    this['PSExternalEx'] = function(iCommand, strParameter) {
        return apdImpl.PSExternalEx(iCommand, strParameter);
    };
    
    this['PSGetLastMessage'] = function() {
        return apdImpl.PSGetLastMessage();
    };
})();

window.nosip = new (function() {
	this['ShowSIP'] = function(isShow) {
		setTimeout(function() {nosipImpl.ShowSIP(isShow)}, 0);
	};
})();

window.ActiveXObject = function (string) {
    var lowerCaseString = string.toLowerCase();
    switch (lowerCaseString) {
        case "pocketbrowser.generic":
        case "symbolbrowser.generic":
            return generic;
        case "pocketbrowser.airbeamsmart":
        case "symbolbrowser.airbeamsmart":
            return airbeam;
        case "pocketbrowser.narrowband":
        case "symbolbrowser.narrowband":
            return apd;
        case "nosip.nsedit":
            return nosip;
        case "ceodax.odax":
            return odax;
    }
    throw undefined;
};

}
