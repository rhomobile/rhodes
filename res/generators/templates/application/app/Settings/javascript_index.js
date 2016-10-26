window.addEventListener("load", function () {

    $("#back").on("click", function () {
        history.back();
    });

    $("#clientId").text(Rho.ORM.getClientId());

    $("#resetDatabaseItem").on("click", function () {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        $('#resetDatabaseModalDialog').modal('show');
    });

    $('#resetDatabaseModalDialog').on('hidden.bs.modal', function (e) {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        Rho.ORM.databaseFullReset();
        Rho.RhoConnectClient.doSync();
        showAlert("Database has been reset.");
    });

    $("#performSyncItem").on("click", function () {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        Rho.RhoConnectClient.doSync();
        showAlert("Sync has been triggered.");
    });

    $("#logoutItem").on("click", function () {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        Rho.RhoConnectClient.logout();
        updateUI();
        showAlert("You have been logged out.");
    });

    updateUI();

});

var updateUI = function() {
    if (Rho.RhoConnectClient == null) {
        return;
    }
    var loggedIn = Rho.RhoConnectClient.isLoggedIn();
    $("#logoutItem").toggleClass("hidden", !loggedIn);
    $("#performSyncItem").toggleClass("hidden", !loggedIn);
    $("#loginItem").toggleClass("hidden", loggedIn);
};


var showAlert = function (aString) {
    $("#alert").text(aString).removeClass("hidden");
    setTimeout(function () {
        $("#alert").empty().addClass("hidden");
    }, 5000)
};