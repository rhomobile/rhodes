window.addEventListener("load", function () {

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
                console.log(err);
                $("#alert").text(err.error_message).removeClass("hidden");
                setTimeout(function () {
                    $("#alert").empty().addClass("hidden");
                }, 5000)
            }
            else {
                //the operation has been completed successfully
                history.back();
            }
        });
    });
});