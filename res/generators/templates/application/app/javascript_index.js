var IndexPage = function () {

    this.updateRhoClientControlButtons = function () {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        var loggedIn = Rho.RhoConnectClient.isLoggedIn();
        $("#logoutBtn").toggleClass("hidden", !loggedIn);
        $("#syncBtn").toggleClass("hidden", !loggedIn);
        $("#loginBtn").toggleClass("hidden", loggedIn);

    };

    var that = this;
    return {
        init: function () {

            Rho.NativeToolbar.create([
                {action: "back"},
                {action: "separator"},
                {action: "home"},
                {action: "options"}
            ], {});

            that.updateRhoClientControlButtons();

            $("#logoutBtn").on("click", function () {
                if (Rho.RhoConnectClient == null) {
                    return;
                }
                Rho.RhoConnectClient.logout();
                that.updateRhoClientControlButtons();
            });

            $("#syncBtn").on("click", function () {
                if (Rho.RhoConnectClient == null) {
                    return;
                }
                Rho.RhoConnectClient.doSync(true);
                that.updateRhoClientControlButtons();
            });
        }
    }
};

var SettingsPage = function () {

    this.showAlert = function (aString) {
        var timeout = 5000;
        $("#alert").text(aString).removeClass("hidden");
        setTimeout(function () {
            $("#alert").empty().addClass("hidden");
        }, timeout);
    };

    this.updateUI = function () {
        if (Rho.RhoConnectClient == null) {
            return;
        }
        var loggedIn = Rho.RhoConnectClient.isLoggedIn();
        $("#logoutItem").toggleClass("hidden", !loggedIn);
        $("#performSyncItem").toggleClass("hidden", !loggedIn);
        $("#loginItem").toggleClass("hidden", loggedIn);
    };

    var that = this;

    return {
        init: function () {

            that.updateUI();

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

            $('#resetDatabaseModalDialog').on('hidden.bs.modal', function () {
                if (Rho.RhoConnectClient == null) {
                    return;
                }
                Rho.ORM.databaseFullReset();
                Rho.RhoConnectClient.doSync();
                that.showAlert("Database has been reset.");
            });

            $("#performSyncItem").on("click", function () {
                if (Rho.RhoConnectClient == null) {
                    return;
                }
                Rho.RhoConnectClient.doSync();
                that.showAlert("Sync has been triggered.");
            });

            $("#logoutItem").on("click", function () {
                if (Rho.RhoConnectClient == null) {
                    return;
                }
                Rho.RhoConnectClient.logout();
                that.updateUI();
                that.showAlert("You have been logged out.");
            });
        }
    }
};

var LoginPage = function () {
    this.showAlert = function (aString) {
        var timeout = 5000;
        $("#alert").text(aString).removeClass("hidden");
        setTimeout(function () {
            $("#alert").empty().addClass("hidden");
        }, timeout);
    };

    var that = this;

    return {
        init: function () {
            $("#back").on("click", function () {
                history.back();
            });

            $("#loginBtn").on("click", function () {
                var username = $("#username").val();
                var password = $("#password").val();
                console.log(username, password);
                Rho.RhoConnectClient.login(username, password, function (err) {
                    //The operation has been failed
                    if (err.error_code != 0) {
                        var errorText = err.error_message != "" ? err.error_message : "Unknown error with code: " + err.error_code;
                        that.showAlert(errorText);
                    }
                    else {
                        //the operation has been completed successfully
                        history.back();
                    }
                });
            });
        }
    }
};