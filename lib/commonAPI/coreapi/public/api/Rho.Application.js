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

    function processApplicationEvent(event) {
        applicationEvent = event.applicationEvent;
        eventData = event.eventData;
        
        // UICreated
        if (applicationEvent == Rho.Application.APP_EVENT_UICREATED) {
            console.log("AppEvent: "+ applicationEvent);
            start_url = Rho.Application.startURI;
            if (!start_url){
                start_url = "";
            }
            if ((Rho.System.platform != Rho.System.PLATFORM_WP8) && (Rho.System.platform != Rho.System.PLATFORM_UWP)) {
              invalid_security_token_start_path = Rho.Application.invalidSecurityTokenStartPath;

              if (Rho.Application.securityTokenNotPassed) {
                if (invalid_security_token_start_path && invalid_security_token_start_path.length() > 0) {
                  start_url = invalid_security_token_start_path;
                } else {
                  // exit from application - old way
                  Rho.Log.fatalError('processApplicationEvent: security_token is not passed - application will closed','EROOR');
                  Rho.System.exit();
                }
              }
            }
            console.log("on_ui_created.navigate to start url: "+ start_url);
            Rho.WebView.navigate(start_url);
        }
        // UIDestroyed
        else if (applicationEvent==Rho.Application.APP_EVENT_UIDESTROYED) {
            console.log("AppEvent: "+ applicationEvent);
        }
        else if (applicationEvent==Rho.Application.APP_EVENT_CONFIGCONFLICT) {
            console.log("AppEvent: "+ applicationEvent);
        }
        //
        else if (applicationEvent==Rho.Application.APP_EVENT_DBMIGRATESOURCE) {
            console.log("AppEvent: "+ applicationEvent);
            console.log( "default on_migrate_source - do nothing; old_version :"+eventData.old_version+"; new_src : "+eventData.new_src);
            //if new_src['schema']
            //    db = ::Rho::RHO.get_src_db(new_src['name'])
            //    db.delete_table(new_src['name'])
            //    return false  #create new table
            //end
            return true
        }
        //
        else if (applicationEvent==Rho.Application.APP_EVENT_SYNCUSERCHANGED) {
            Rho.ORM.databaseFullReset(false, false)
            //::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET client_id=?", "")
        }
    }

    var App = {
        processApplicationEvent : function(event) {
            processApplicationEvent(event);
        }
    };

    rhoUtil.namespace(moduleNS, App, true);

    // function setAppEventsHandler()
    // {
    //     Rho.Application.setApplicationNotify(processApplicationEvent);
    // }

    // $(document).ready( setAppEventsHandler );

})(Rho.jQuery, Rho, Rho.util);
