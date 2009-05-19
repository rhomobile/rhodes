require 'rho'

Rho::RhoConfig::add_source("<%= class_name %>", {"url"=>"<%= source_url %>", "source_id"=><%= name.camel_case.hash.abs % 10000 %><% if type and type == "ask" %>, "type"=>"ask"<%end%>})