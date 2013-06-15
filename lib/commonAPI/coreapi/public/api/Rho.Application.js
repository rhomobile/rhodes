(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = 'Rho.Application';
    var apiReq = rhoUtil.apiReqFor(moduleNS);
    var App = {};

    // At this moment we unable to extend target namespace with property proxies defined in a local namespace.
    // So defining (overwriting) them right in the target namespace.
    rhoUtil.createPropsProxy( Rho.Application, [
        { propName: 'defaultNativeMenu', propAccess: 'r', customGet: function(){
            return {
                'Home': 'home',
                'Refresh': 'refresh',
                'Sync': 'sync',
                'Settings': 'options',
                'Log': 'log',
                'separator': '',
                'Exit': 'close'
            };
        } }
    ], apiReq);

    // Unnecessary because nothing has been defined for App object.
    // rhoUtil.namespace(moduleNS, App, true);

})(Rho.jQuery, Rho, Rho.util);
