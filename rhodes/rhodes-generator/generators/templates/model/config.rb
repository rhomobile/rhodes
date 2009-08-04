require 'rho'

Rho::RhoConfig::add_source("<%=class_name%>", {"url"=>"<%=@model_sync_server%>", "source_id"=><%= name.camel_case.hash.abs % 10000 %><% if type and type == "ask" %>, "type"=>"ask"<%end%>})