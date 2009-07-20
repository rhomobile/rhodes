require 'rho'

Rho::RhoConfig::add_source("<%=class_name%>", {"url"=>"#{Rho::RhoConfig.syncserver}<%=class_name%>", "source_id"=><%= name.camel_case.hash.abs % 10000 %><% if type and type == "ask" %>, "type"=>"ask"<%end%>})