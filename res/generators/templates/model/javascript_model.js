var model = Rho.ORM.getModel("<%= name.camelize %>");
if (model == null) {
    model = Rho.ORM.addModel(function (model) {
        model.modelName("<%= name.camelize %>");
        <% attributes.each do |attribute| %>
        model.property("<%= attribute %>", "string");
        <% end %>
    });
}