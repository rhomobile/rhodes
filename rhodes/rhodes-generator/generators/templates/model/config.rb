require 'rho'

Rho::RhoConfig::add_source("<%= class_name %>", {"url"=>"<%= source_url %>", "source_id"=><%= source_id %><% if type and type == "ask" %>, "type"=>"ask"<%end%>})