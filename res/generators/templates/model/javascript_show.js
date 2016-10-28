window.addEventListener("load", function () {

    $("#backBtn").on("click", function () {
        history.back();
    });

    id = window.location.search.slice(1);
    var model = Rho.ORM.getModel("<%= name.camelize %>");
    var item = model.find("first", {object: id});

    $("#title").text(item.get("<%= attributes[0] %>"));
    $("#editBtn").attr("href", "edit.html?" + item.object());

    <% attributes.each do |attribute| %>
    $("#<%= name.downcase %><%= attribute.capitalize %>").text(item.get("<%= attribute %>"));
    <% end %>
});
