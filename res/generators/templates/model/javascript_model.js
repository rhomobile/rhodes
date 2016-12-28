/**
* The model has already been created by the framework
* You can add more methods here
**/

var <%= name.camelize %>Model = Rho.ORM.getModel("<%= name.camelize %>");
if (<%= name.camelize %>Model == null) {
    <%= name.camelize %>Model = Rho.ORM.addModel(function (model) {
        model.modelName("<%= name.camelize %>");
        <% attributes.each do |attribute| %>
        model.property("<%= attribute %>", "string");
        //  model.enable("sync");
        //  add model specific code here
        <% end %>
    });
}