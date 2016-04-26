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
