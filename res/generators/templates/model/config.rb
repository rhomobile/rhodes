require 'rho'

Rho::RhoConfig::add_source("<%=class_name%>", {"url"=>"<%=@model_sync_server%>"<% if type and type == 'ask' %>, ", "type"=>"ask"<%end%><% if priority %>, "priority"=><%=priority%><%end%>})