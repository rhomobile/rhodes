



function onRhomobileApplicationActivated() {
    Rho.Log.info("Node.js event : APP_EVENT_ACTIVATED", "Node.js JS App");
}

function onRhomobileApplicationActivated() {
    Rho.Log.info("Node.js event : APP_EVENT_ACTIVATED", "Node.js JS App");

    var native_toolbar = [
        {"action": "back", "icon": "/nodejs/server/public/images/bar/back_btn.png"},
        {"action": "separator"},
        {"action": "home", "icon": "/nodejs/server/images/bar/home_btn.png"},
        {"action": "refresh"},
        {"action": "options", "icon": "/nodejs/server/images/bar/gears.png"}
    ];
    Rho.NativeToolbar.create(native_toolbar);
}

function onRhomobileApplicationDeActivated() {
    Rho.Log.info("Node.js event : APP_EVENT_DEACTIVATED", "Node.js JS App");
}

function onRhomobileApplicationUICreated() {
    Rho.Log.info("Node.js event : APP_EVENT_UICREATED", "Node.js JS App");
    // application should restore his state  !
}

function onRhomobileApplicationUIDestroyed() {
    Rho.Log.info("Node.js event : APP_EVENT_UIDESTROYED", "Node.js JS App");
    // application should save his state  !
}

function onRhomobileApplicationScreenOff() {
    Rho.Log.info("Node.js event : APP_EVENT_SCREEN_OFF", "Node.js JS App");
}

function onRhomobileApplicationScreenOn() {
    Rho.Log.info("Node.js event : APP_EVENT_SCREEN_ON", "Node.js JS App");
}






Rho.Application.setApplicationNotify( function(par) {
    var event_type = par['applicationEvent'];
    switch(event_type) {
        case Rho.Application.APP_EVENT_ACTIVATED:
            onRhomobileApplicationActivated();
        break;
        case Rho.Application.APP_EVENT_DEACTIVATED:
            onRhomobileApplicationDeActivated();
        break;
        case Rho.Application.APP_EVENT_UICREATED:
            onRhomobileApplicationUICreated();
        break;
        case Rho.Application.APP_EVENT_UIDESTROYED:
            onRhomobileApplicationUIDestroyed();
        break;
        case Rho.Application.APP_EVENT_SCREEN_OFF:
            onRhomobileApplicationScreenOff();
        break;
        case Rho.Application.APP_EVENT_SCREEN_ON:
            onRhomobileApplicationScreenOn();
        break;
    }
});
