var App = {

    rhoClientErrorByCode: function (aString) {
        var code = parseInt(aString);
        if (code === 0) {
            return "ERR_NONE";
        }
        if (code === 1) {
            return "ERR_NETWORK";
        }
        if (code === 2) {
            return "ERR_REMOTESERVER";
        }
        if (code === 3) {
            return "ERR_RUNTIME";
        }
        if (code === 4) {
            return "ERR_UNEXPECTEDSERVERRESPONSE";
        }
        if (code === 5) {
            return "ERR_DIFFDOMAINSINSYNCSRC";
        }
        if (code === 6) {
            return "ERR_NOSERVERRESPONSE";
        }
        if (code === 7) {
            return "ERR_CLIENTISNOTLOGGEDIN";
        }
        if (code === 8) {
            return "ERR_CUSTOMSYNCSERVER";
        }
        if (code === 9) {
            return "ERR_UNATHORIZED";
        }
        return "Unknown error " + code;
    },

    IndexPage: function () {
        var messageTimeout;
        var that = this;

        this.syncNotify = function (result) {
            if (result.error_code) {
                that.showWarning("Error.", App.rhoClientErrorByCode(result.error_code));
            } else {
                that.showInfo("Success!", "Database has been synced");
            }
        };

        this.showWarning = function (title, text) {
            var duration = 5000;
            var alertElement = $("#alert");
            alertElement.addClass("alert-warning");
            $("#alert > #title").text(title);
            $("#alert > #text").text(text);
            alertElement.removeClass("hidden");

            if (messageTimeout != null) {
                clearTimeout(messageTimeout);
            }
            messageTimeout = setTimeout(function () {
                alertElement.removeClass("alert-warning");
                alertElement.addClass("hidden");
            }, duration);
        };
        this.showInfo = function (title, text) {
            var duration = 5000;
            var alertElement = $("#alert");
            alertElement.addClass("alert-info");
            $("#alert > #title").text(title);
            $("#alert > #text").text(text);
            alertElement.removeClass("hidden");

            if (messageTimeout != null) {
                clearTimeout(messageTimeout);
            }
            messageTimeout = setTimeout(function () {
                alertElement.removeClass("alert-info");
                alertElement.addClass("hidden");
            }, duration);
        };
        this.updateRhoClientControlButtons = function () {
            if (Rho.RhoConnectClient == null) {
                return;
            }
            var loggedIn = Rho.RhoConnectClient.isLoggedIn();
            $("#logoutBtn").toggleClass("hidden", !loggedIn);
            $("#syncBtn").toggleClass("hidden", !loggedIn);
            $("#loginBtn").toggleClass("hidden", loggedIn);

        };
        this.init = function () {

            Rho.NativeToolbar.create([
                {action: "back"},
                {action: "separator"},
                {action: "home"},
                {action: "options"},
                {action: "refresh"}
            ], {});

            if (Rho.RhoConnectClient != null) {
                Rho.RhoConnectClient.setNotification("*", that.syncNotify);
            }

            $("#logoutBtn").on("click", function () {
                Rho.RhoConnectClient.logout();
                that.updateRhoClientControlButtons();
            });

            $("#syncBtn").on("click", function () {
                Rho.RhoConnectClient.doSync();
                that.showInfo("Please wait.", "Sync has been triggered.");
            });

            that.updateRhoClientControlButtons();
        }
    },

    SettingsPage: function () {
        var messageTimeout;
        var that = this;

        this.showWarning = function (title, text) {
            var duration = 5000;
            var alertElement = $("#alert");
            alertElement.addClass("alert-warning");
            $("#alert > #title").text(title);
            $("#alert > #text").text(text);
            alertElement.removeClass("hidden");

            if (messageTimeout != null) {
                clearTimeout(messageTimeout);
            }
            messageTimeout = setTimeout(function () {
                alertElement.removeClass("alert-warning");
                alertElement.addClass("hidden");
            }, duration);
        };
        this.showInfo = function (title, text) {
            var duration = 5000;
            var alertElement = $("#alert");
            alertElement.addClass("alert-info");
            $("#alert > #title").text(title);
            $("#alert > #text").text(text);
            alertElement.removeClass("hidden");

            if (messageTimeout != null) {
                clearTimeout(messageTimeout);
            }
            messageTimeout = setTimeout(function () {
                alertElement.removeClass("alert-info");
                alertElement.addClass("hidden");
            }, duration);
        };
        this.syncNotify = function (result) {
            if (result.error_code) {
                that.showWarning("Error.", App.rhoClientErrorByCode(result.error_code));
            } else {
                that.showInfo("Success!", "Database has been synced");
            }
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
        this.init = function () {
            if (Rho.RhoConnectClient != null) {
                Rho.RhoConnectClient.setNotification("*", that.syncNotify);
            }
            that.updateUI();

            $("#back").on("click", function () {
                history.back();
            });

            $("#clientId").text(Rho.ORM.getClientId());

            $("#resetDatabaseItem").on("click", function () {
                $('#resetDatabaseModalDialog').modal('show');
            });

            $('#resetDatabaseModalDialog').on('hidden.bs.modal', function () {
                Rho.ORM.databaseFullReset();
                that.showInfo("", "Database has been reset.");
                Rho.RhoConnectClient.doSync();
            });

            $("#performSyncItem").on("click", function () {
                Rho.RhoConnectClient.doSync();
                that.showInfo("Please wait.", "Sync has been triggered.");
            });

            $("#logoutItem").on("click", function () {
                Rho.RhoConnectClient.logout();
                that.updateUI();
                that.showInfo("", "You have been logged out.");
            });
        }
    },

    LoginPage: function () {
        var messageTimeout;
        var that = this;

        this.showWarning = function (title, text) {
            var duration = 5000;
            var alertElement = $("#alert");
            alertElement.addClass("alert-warning");
            $("#alert > #title").text(title);
            $("#alert > #text").text(text);
            alertElement.removeClass("hidden");

            if (messageTimeout != null) {
                clearTimeout(messageTimeout);
            }
            messageTimeout = setTimeout(function () {
                alertElement.removeClass("alert-warning");
                alertElement.addClass("hidden");
            }, duration);
        };
        this.init = function () {
            $("#back").on("click", function () {
                history.back();
            });

            $("#loginBtn").on("click", function () {
                var username = $("#username").val();
                var password = $("#password").val();
                Rho.RhoConnectClient.login(username, password, function (err) {
                    //The operation has been failed
                    if (err.error_code != 0) {
                        var errorText = err.error_message != "" ? err.error_message : "Unknown error with code: " + err.error_code;
                        that.showWarning("Login error.", errorText);
                    }
                    else {
                        //the operation has been completed successfully
                        history.back();
                    }
                });
            });
        }
    }
}