/*
    In case you need to use pre-4.0 JS API together with new Common JS API you can
    involve compatibility layer this way:

        <!-- required for pre-4.0 JS API -->
        <script type="text/javascript" src="/public/jquery/jquery-1.6.4.min.js"></script>
        <script type="text/javascript" src="/public/jquery/jquery.json-2.3.min.js"></script>

        <!-- pre-4.0 JS API -->
        <script type="text/javascript" src="/public/js/rho_javascript_api.js"></script>
        <!-- compatibility layer -->
        <script type="text/javascript" src="/public/js/rho_common_api_noconflict.js"></script>

        <!-- new Common JS API -->
        <script type="text/javascript" charset="utf-8" src="/public/api/rhoapi-modules.js"></script>

    NOTE: load order is important!
*/

var RhoOld = (function () {

    var rhoFiltered = {};
    var enabledModules = [
        'MapView',
        'AudioCapture',
        'Camera',
        'SignatureCapture',
        'GeoLocation',
        'pre4_callback_handler'
    ];

    if ('undefined' != typeof window.Rho) {
        if ('object' == typeof window.Rho) {
            for (var i=0; i<enabledModules.length; i++) {
                if (window.Rho.hasOwnProperty(enabledModules[i]) && 'undefined' != typeof window.Rho[enabledModules[i]]) {
                    rhoFiltered[enabledModules[i]] = window.Rho[enabledModules[i]];
                }
            }
        }
        //delete window.Rho;
        window.Rho = undefined;
    }

    return rhoFiltered;
})();
