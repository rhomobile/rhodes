window.addEventListener("load", function () {

        Rho.NativeToolbar.create([
            {action: "back"},
            {action: "separator"},
            {action: "home"},
            {action: "options"}
        ], {});

        $("#logoutBtn").on("click", function () {
            if (Rho.RhoConnectClient == null) {
                return;
            }
            Rho.RhoConnectClient.logout();
            updateRhoClientControlButtons();
        });

        $("#syncBtn").on("click", function () {
            if (Rho.RhoConnectClient == null) {
                return;
            }
            Rho.RhoConnectClient.doSync(true);
            updateRhoClientControlButtons();
        });

        var updateRhoClientControlButtons = function () {
            if (Rho.RhoConnectClient == null) {
                return;
            }
            var loggedIn = Rho.RhoConnectClient.isLoggedIn();
            $("#logoutBtn").toggleClass("hidden", !loggedIn);
            $("#syncBtn").toggleClass("hidden", !loggedIn);
            $("#loginBtn").toggleClass("hidden", loggedIn);

        };

        updateRhoClientControlButtons();
    }
);