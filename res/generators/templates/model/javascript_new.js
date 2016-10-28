window.addEventListener("load", function () {

    var model = Rho.ORM.getModel("<%= name.camelize %>");

    $("#cancelBtn").on("click", function () {
        history.back();
    });

    $("#submitBtn").on("click", function () {
        var properties = {};
        <% attributes.each do |attribute| %>
        properties.<%= attribute %> = $("#<%= name.downcase %><%= attribute.capitalize %>").val();
        <% end %>

        model.create(properties);
        history.back();
    });



});
