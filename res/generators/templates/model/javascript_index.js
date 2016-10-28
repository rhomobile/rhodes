window.addEventListener("load", function () {

    $("#homeBtn").attr({"href": Rho.Application.startURI});

    var model = Rho.ORM.getModel("<%= name.camelize %>");
    var items = model.find("all");
    var container = $("[data-role=item-list]");
    items.forEach(function (each) {
        var span = $("<span>", {"class": "glyphicon glyphicon-chevron-right pull-right", "aria-hidden": true});
        var anchor = $("<a>", {"class": "list-group-item", "href": "show.html?" + each.object()});
        anchor.text(each.get("<%= attributes[0] %>"));
        anchor.append(span);
        container.append(anchor);
    });

});
