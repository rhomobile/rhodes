(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = 'Rho.Application';
    var apiReq = rhoUtil.apiReqFor(moduleNS);
    var App = {};

    // At this moment we unable to extend target namespace with property proxies defined in a local namespace.
    // So defining (overwriting) them right in the target namespace.
    rhoUtil.createPropsProxy( Rho.Application, [
        { propName: 'defaultNativeMenu', propAccess: 'r', customGet: function(){
            return [
                {'label':'Home', 'action': 'home'},
                {'label':'Refresh', 'action': 'refresh'},
                {'label':'Sync', 'action': 'sync'},
                {'label':'Settings', 'action': 'options'},
                {'label':'Log', 'action': 'log'},
                {'label':'separator', 'action': ''},
                {'label':'Exit', 'action': 'close'}
            ];
        } }
    ], apiReq);

    // Unnecessary because nothing has been defined for App object.
    // rhoUtil.namespace(moduleNS, App, true);

})(Rho.jQuery, Rho, Rho.util);
