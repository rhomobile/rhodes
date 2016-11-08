var App = {
    IndexPage: function () {
        var that = this;

        this.init = function () {
            $("#homeBtn").attr({"href": Rho.Application.startURI});

            var items = <%= name.camelize %>Model.find("all");
            var container = $("[data-role=item-list]");
            items.forEach(function (each) {
                var span = $("<span>", {"class": "glyphicon glyphicon-chevron-right pull-right", "aria-hidden": true});
                var anchor = $("<a>", {"class": "list-group-item", "href": "show.html?" + each.object()});
                anchor.text(each.get("<%= attributes[0] %>"));
                anchor.append(span);
                container.append(anchor);
            });
        }
    },

    ShowPage: function () {
        var that = this;

        this.init = function () {
            $("#backBtn").on("click", function () {
                history.back();
            });

            var id = window.location.search.slice(1);
            var item = <%= name.camelize %>Model.find("first", {conditions: {object: id}});
            $("#title").text(item.get("<%= attributes[0] %>"));
            $("#editBtn").attr("href", "edit.html?" + item.object());
            <% attributes.each do |attribute| %>
            $("#<%= name.downcase %><%= attribute.capitalize %>").text(item.get("<%= attribute %>"));
            <% end %>
        }
    },

    NewPage: function () {
        var that = this;

        this.init = function () {
            $("#cancelBtn").on("click", function () {
                history.back();
            });
            $("#submitBtn").on("click", function () {
                var properties = {};
                <% attributes.each do |attribute| %>
                properties.<%= attribute %> = $("#<%= name.downcase %><%= attribute.capitalize %>").val();
                <% end %>
                <%= name.camelize %>Model.create(properties);
                history.back();
             });
        }
    },

    EditPage: function () {
        var that = this;

        this.init = function () {
            var id = window.location.search.slice(1);
            var item = <%= name.camelize %>Model.find("first", {conditions: {object: id}});
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
        }
    }
}