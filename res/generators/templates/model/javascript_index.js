var App = {
    IndexPage: function () {
        var that = this;

        this.init = function () {
            $("#homeBtn").attr({"href": Rho.Application.startURI});

            var items = <%= name.camelize %>Model.find("all");
            var container = $(".list-group");
            items.forEach(function (each) {
                var span = $("<i data-feather=\"chevron-right\"></i>");
                var anchor = $("<a>", {"class": "list-group-item list-group-item-action d-flex justify-content-between", "href": "show.html?" + each.object()});
                anchor.text(each.get("<%= attributes[0] %>"));
                anchor.append(span);
                container.append(anchor);
                container.append(anchor);
            });
        };
        setTimeout(function(){feather.replace();}, 0);
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
        };
        feather.replace();
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
        };
        feather.replace();
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
            feather.replace();
        }
    }
}
