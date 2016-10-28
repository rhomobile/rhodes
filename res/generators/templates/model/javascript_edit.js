window.addEventListener("load", function () {

    id = window.location.search.slice(1);
    var model = Rho.ORM.getModel("<%= name.camelize %>");
    var item = model.find("first", {object: id});

    $("#cancelBtn").on("click", function () {
        history.back();
    });

    $("#deleteBtn").on("click", function () {
        item.destroy();
        window.location = "index.html";
    });

    <% attributes.each do |attribute| %>
    $("#<%= name.downcase %><%= attribute.capitalize %>").attr("value", item.get("<%= attribute %>"));
    <% end %>

    $("#submitBtn").on("click", function () {
        var properties = {};
        <% attributes.each do |attribute| %>
        properties.<%= attribute %> = $("#<%= name.downcase %><%= attribute.capitalize %>").val();
        <% end %>

        item.updateAttributes(properties);
        history.back();
    });



});
