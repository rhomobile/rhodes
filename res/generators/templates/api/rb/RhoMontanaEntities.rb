<% 
    def namespace(a_module)
        a_module.parents.clone().join("::")
    end

    def method_name(method)
      [$cur_module.parents.clone().push($cur_module.name).join("::"), method].join('.')
    end

    def entity_method_name(entity,method)
      [$cur_module.parents.clone().push($cur_module.name).push(entity.name).join("::"), method].join('.')
    end

    debug = true
if $cur_module.entities.size > 0 %>   
module <%= namespace($cur_module) %>
  class <%=$cur_module.name%>
<%
    $cur_module.entities.each do |entity| 
      have_init_fn = entity.fields.size > 0 
      updateable_fields = entity.fields.select { |f| !f.const } 
      updateable_binding_fields = entity.fields.select { |f| !f.const && f.binding }
      updateable_non_binding_fields = entity.fields.select { |f| !f.const && !f.binding }  
      binding_arg = entity.binding_fields.size > 0 ? "binding" : ""
      updateable_binding_arg = entity.binding_fields.size > 0 ? "binding" : ""

      is_update_used = (entity.binding_fields.size > 0) && (updateable_non_binding_fields.size > 0)
 
      init_args = []
      update_args = []
      have_update_fn = updateable_fields.size > 0 

      if (updateable_fields.size > 0)
        if (updateable_fields.size == 1)
            init_args << "@hash[:#{updateable_fields[0].name}]"
        else
            init_args << "@hash"
        end
      end

      update_args = []
      if entity.binding_fields.size > 0
        update_args << "@binding" 
        if updateable_non_binding_fields.size > 0
            update_args << "@to_update"
        end
      else
          update_args << "@hash"
      end
  %>
    # =================================================
    # Entity <%= entity.name %> <%
    entity.fields.each do |f|%>
    # <%= f.const ? "const " : "" %>field <%= f.name %> <%= f.type %> <%end%>

    class <%= entity.name %>
      def self.create_instance(param)
        <% if debug %>puts "<%= entity.name %>::create_instance  #{param.inspect}"<% end %>
        <%=entity_method_name(entity,'new')%>(param)
      end

      def initialize(fields = nil)
        <% if debug %>puts "<%= entity.name %>::initialize  #{fields.inspect}"<% end %>
        @hash = { <%= entity.fields.map{ |f| ":#{f.name} => #{f.default_value}" }.join(", ") %> }<% if (entity.binding_fields.size > 0) %>
        @binding = {} <% end 
            if is_update_used%>
        @is_changed = false
        @to_update = {}<% end %>
        if (!fields.nil?)
          do_init(fields)
        else
          @is_initialized = false
        end
      end 
<%  if entity.fields.size > 0 %>
      # ==============================
      # protected field access helpers <%     
      entity.fields.each do |f| %>
      def set<%=f.field_index%>(val) <% case f.type
            when RhogenCore::TYPE_STRING %>
        res = (val.is_a? String) ? val : val.to_s <%
            when RhogenCore::TYPE_INT %>
        res = (val.instance_of? Integer) ? val : Integer(val.to_i)<%
            when RhogenCore::TYPE_DOUBLE %>
        res = (val.instance_of? Float) ? val : val.to_f<%
            when RhogenCore::TYPE_BOOL %>
        res = (val.is_a?(TrueClass) || val.is_a?(FalseClass)) ? val : (val and !!val.match(/^(true|t|yes|y|1)$/i)) <%
         end %>
        <% if debug %>puts "<%= entity.name %>::set_<%=f.name%> : #{res}"<% end %>
        @hash[:<%=f.name%>] = res
        <% 
        if f.binding 
          if entity.binding_fields.size == 1 %>@binding = res<% 
          else %>@binding[:<%= f.name %>] = res <% 
          end 
        end %>
        res
      end 
<%    end  
    end%>
      # =======================================
      # protected fields initialization helper
      def do_init(fields)
        <% if debug %>puts "<%= entity.name %>::do_init  #{fields.inspect}"<% end %>
<%      entity.fields.each do |f| 
%>        set<%= f.field_index %>(fields["<%= f.name%>"]) if fields.has_key?("<%= f.name%>")
<%      end 
%>        @is_initialized = true
      end
<%        have_update_fn = updateable_fields.size > 0 
    if have_update_fn %>
      def update(fields)
        <% if debug %>puts "<%= entity.name %>::update  #{fields.inspect}"<% end %>
        if !fields.nil? 
<%      entity.fields.each do |f| 
            next if f.const 
%>            set<%= f.field_index %>(fields["<%= f.name%>"]) if fields.has_key?("<%= f.name%>")
<%      end 
%>        end
        @to_update = {}; @is_changed = false 
      end 
<%  end  %>
      # ============================
      # push field changes to native
      def save 
        <% if debug %>puts "<%= entity.name %>::save"<% end %>
        if @is_initialized 
<% if !entity.update_method.nil? 
%>          if @is_changed 
            <%= "updateobj = " if have_update_fn %><%=method_name(entity.update_method.binding_name)%>(<%= update_args.map{ |arg| "#{arg}" }.join(', ') %>)
            <% if have_update_fn 
            %>update(updateobj)
            <% if debug %>puts "<%= entity.name %>.updateobj  #{updateobj.inspect}"<% end %>
             <%  end %>
          end 
<% end %>        else 
          <% if entity.init_method != nil 
                %><%= "init_object = " if have_init_fn %><%=method_name(entity.init_method.binding_name)%>(<%=init_args.map{ |arg| "#{arg}" }.join(', ')%>) <% 
                if have_init_fn 
                    %>
          do_init(init_object) <% else %>
          @is_initialized = true <% end %><%
            end %>
        end
      end

      # ================
      # field accessors <%
   entity.fields.each do |f| 
      if f.is_readable %>
      def <%=f.name%>
        <% if debug %>puts "<%= entity.name %>::get.<%=f.name%>"<% end %>
        @hash[:<%=f.name%>]
      end <% end 
      if f.is_writeable 
        %>
      def <%=f.name%>=(val) 
        return if val.nil?
        <% if is_update_used %>last_val = @hash[:<%=f.name%>]
        new_val =  <% end %>set<%=f.field_index%>(val)
        <% if is_update_used %>
        if (last_val != new_val) 
          <% if debug %>puts "<%= entity.name %>::set.<%=f.name%>"<% end %>
            @to_update[:<%= f.name %>] = new_val; @is_changed = true 
        end
        <% end %>
      end <% end %>   

<%      end #fields %>
      # ==============
      # entity methods
<%  
    entity.methods.select{|m| !m.is_generated }.each do |entity_method|
        params = entity_method.params.map do |param|
            "/* #{CppGen::native_type_arg(param)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')

        call_params = []
        if ( !entity_method.is_static_for_entity)
            if entity.binding_fields.size > 0 
                call_params << "@binding"
            else
                if entity.fields.size > 0 
                    call_params << "@hash"
                end
            end
        end

%>      # function <%=entity_method.name.camelcase%> (<%= params %>)<%
    fn_call_params = entity_method.params.select{|param| !param.is_generated}.map{|param| "#{param.name}"} 
      fn_debug_msg = "#{entity.name}::#{entity_method.name.underscore}"
      fn_debug_msg += ", call_params: #{call_params.map{|x| x + '=#{'+x+'.class}:#{'+x+'}'}.join(', ')}" if call_params.size > 0
      fn_debug_msg += ", params: #{fn_call_params.map{|x| x + '=#{'+x+'.class}:#{'+x+'}'}.join(', ') }" if fn_call_params.size > 0%>
      def <%=entity_method.is_static_for_entity ? "self." : ""%><%=entity_method.name.underscore%>(<%= fn_call_params.join(', ')%>)
        <% if debug %>puts "<%=fn_debug_msg%>"<% end %>
        <%= "save(); " if !entity_method.is_static_for_entity %><%= 
        "return " if entity_method.is_return_value %><%=method_name(entity_method.binding_name)%>(<%= (call_params + fn_call_params).join(', ') %>)
      end

<%   end %><% 
    list_of_fns = []
    list_of_fns << ":do_init"
    list_of_fns << ":update" if have_update_fn
    list_of_fns.concat( entity.fields.map{ |f| ":set#{f.field_index}"} )

    if list_of_fns.size > 0 
%>      protected <%= list_of_fns.join(', ') %>
<%    end %>
    end # <%= entity.name %>
<%  end %>
  end # <%=$cur_module.name%>
end # Rho
<%end%>