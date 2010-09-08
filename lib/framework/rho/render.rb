require 'rho/rhoapplication'
require 'rho/rhocontroller'
require 'rho/rho'

module Rho
  class RhoController
    begin
      if Rho::file_exist? File.join(__rhoGetCurrentDir(), 'lib/rhodes_translator.iseq')
        require 'rhodes_translator'
        include RhodesTranslator::Translator
        include RhodesTranslator::Binding
        include RhodesTranslator::Validation
      end
    rescue Exception => e
    end

    def self.layout(name)
      @layout = name
    end

    def self.get_layout_name
      @layout.nil? ? 'layout' : @layout
    end

    def self.renderfile(filename, req = {}, res = {})
      res = ""
      if File.extname(filename) == '.iseq'
        res = (RhoController.new).inst_render_index(filename, req, res)
      else
        res = IO.read(filename)
      end
      RhoController.start_objectnotify()
      RhoController.start_geoview_notification()
      res
    end

    def inst_render_index(filename, req, res)
      rho_info 'inst_render_index'
      @request, @response = req, res
      @params = RhoSupport::query_params req
      
      #@request, @response = {}
      #@params = {}
      require 'rho/rhoviewhelpers'
      
      @content = eval_compiled_file(filename, getBinding() )
      if !xhr?
          rho_info 'index layout' 
          layout = File.dirname(filename) + "/layout_erb.iseq"
          @content = eval_compiled_file(layout, getBinding() ) if Rho::file_exist?(layout)
      end
          
      @content
    end

    def getBinding
        binding
    end
    
    def render(options = nil)
      if @params['rho_callback']
        rho_error( "render call in callback. Call WebView.navigate instead" ) 
        return ""
      end  

      options = {} if options.nil? or !options.is_a?(Hash)
      options = options.symbolize_keys

      metaenabled = false
      clean_metadata = false
      
      action = nil
      action = options[:action] if options[:action]
      action = @request['action'].nil? ? default_action : @request['action'] unless action

      if $".include?( "rhodes_translator") and @request['model'] != nil
        model = nil
        begin
          model = Object.const_get(@request['model'].to_sym)
        rescue
        end
        if model.respond_to?( :metadata ) and model.metadata != nil
          clean_metadata = true
          metaenabled = model.metadata[action.to_s] != nil
        end
      end

      if not options[:string].nil?
        @content = options[:string]
        @back_action = options[:back] if options[:back]
        options[:layout] = false if options[:layout].nil?

      elsif metaenabled
        @content = render_metadata(action,model.metadata)

      elsif not options[:partial].nil?  # render the file and return, don't set rendered true for a partial.
        @content = render_partial(options)
        options[:layout] = false
      else
        if options[:file].nil? or !options[:file].is_a?(String)
          if Rho::file_exist?(@request[:modelpath]+action.to_s+'_erb.iseq')
            @content = eval_compiled_file(@request[:modelpath]+action+'_erb.iseq', getBinding() )
          else
            @content = ""
          end
        else
          options[:file] = options[:file].gsub(/\.erb$/,"").gsub(/^\/app/,"")
          @content = eval_compiled_file(RhoApplication::get_app_path(@request['application'])+options[:file]+'_erb.iseq', getBinding() )
          options[:layout] = false if options[:layout].nil?
        end
      end
      #rho_info 'render content: ' + @content.length.to_s
      if xhr? and options[:use_layout_on_ajax] != true
        options[:layout] = false
        if @request["headers"]["Transition-Enabled"] == "true"
          @content = "<div>#{@content}</div>"
        end
      elsif options[:layout].nil? or options[:layout] == true
        options[:layout] = self.class.get_layout_name
      end

      if options[:layout] != false
        layoutfile = RhoApplication::get_app_path(@request['application']) + options[:layout].to_s + "_erb.iseq"
        @content = eval_compiled_file(layoutfile, binding ) if Rho::file_exist?(layoutfile)
        rho_info 'Layout file: ' + layoutfile + '. Content size: ' + @content.length.to_s
      end

      RhoController.start_objectnotify()
      RhoController.start_geoview_notification()
      @back_action = options[:back] if options[:back]
      @rendered = true
      model.clean_cached_metadata() if clean_metadata    
      @content
    end

    def render_metadata(action = nil,metadata = nil)
      if metadata.nil?
        begin
          model = Object.const_get(@request['model'].to_sym)
        rescue
        end
        if model.respond_to?( :metadata ) and model.metadata != nil
          metadata = model.metadata
        end
      end

      if action.nil?
          action = @request['action'].nil? ? default_action : @request['action']
      end

      return "" if metadata.nil?
      
      action = action.to_s
      data = {}
      self.instance_variables.each do |sym|
        data[sym.to_s] = self.instance_variable_get sym
      end

      prepared = bind(data,metadata[action])
      
      translate(prepared,action)

    end

    def render_partial(options = nil)
      options = {} if options.nil? or !options.is_a?(Hash)
      options = options.symbolize_keys

      localclass = Class.new do
        require 'helpers/application_helper'
        include ApplicationHelper
        def initialize(obj=nil)
          @vars = {}
          if obj
            obj.each do |key,value|
              @vars[key.to_sym()] = value if key && key.length > 0
            end
          end
        end
        def method_missing(name, *args)
            unless name == Fixnum
              if name[name.length()-1] == '='
                @vars[name.to_s.chop.to_sym()] = args[0]  
              else
                @vars[name]
              end
            end
        end
        def get_binding
          binding
        end
      end

      splitpartial = options[:partial].split('/')
      partial_name = splitpartial[-1]
      model = nil
      if splitpartial.length > 1
        model = splitpartial[-2]
      end

      options[:locals] = {} if options[:locals].nil? or !options[:locals].is_a?(Hash)
       
      content = ""
      if options[:collection].nil?
        locals = localclass.new(options[:locals])

        self.instance_variables.each do |ivar|
          locals.instance_variable_set(ivar,self.instance_variable_get(ivar))
        end
        modelpath = @request[:modelpath]
        modelpath = Rho::RhoFSConnector.get_model_path("app",model) if model
        content = eval_compiled_file(modelpath+'_' + partial_name.to_s+'_erb.iseq', locals.get_binding )
      else
        #xruby issue - https://www.pivotaltracker.com/story/show/3454121
        content = render_partial_collection(options,partial_name)
      end
      content
    end

    def render_partial_collection(options, partial_name)
        i = 0
        content = ""
        options[:collection].each do |x|
          options[:locals][partial_name] = x

          options[:locals][partial_name + '_counter'] = i
          i = i + 1
          #puts "render partial: #{x}"
          content += render_partial :partial => options[:partial], :locals => options[:locals]
        end
        
        content
    end

    @@m_geoview_callback = nil
    @@m_geoview_callback_data = nil
    @@m_geoview_timeout_sec = 0
    def set_geoview_notification(callback, callback_data="", timeout_sec=0)
        @@m_geoview_callback = callback
        @@m_geoview_callback_data = callback_data
        @@m_geoview_timeout_sec = timeout_sec
    end
    
    def self.start_geoview_notification()
        GeoLocation.set_view_notification(@@m_geoview_callback, @@m_geoview_callback_data, @@m_geoview_timeout_sec)
        @@m_geoview_callback = nil
        @@m_geoview_callback_data = nil
        @@m_geoview_timeout_sec = 0
    end

    @@m_arObjectNotify = []
    @@m_arSrcIDNotify = []
    def add_objectnotify(arg)
      return unless arg

      if arg.is_a? (Array)
        arg.each do |item|
          @@m_arObjectNotify.push(strip_braces(item.object))
          @@m_arSrcIDNotify.push(item.source_id)
        end
      else
        @@m_arObjectNotify.push(strip_braces(arg.object))
        @@m_arSrcIDNotify.push(arg.source_id)
      end

    end

    def self.start_objectnotify()
      SyncEngine::clean_objectnotify()

      return unless @@m_arObjectNotify && @@m_arObjectNotify.length > 0 

      0.upto(@@m_arObjectNotify.length()-1) do |i|
        SyncEngine::add_objectnotify(@@m_arSrcIDNotify[i], @@m_arObjectNotify[i])
      end
      
      @@m_arObjectNotify = []
      @@m_arSrcIDNotify = []
      
    end

  end # RhoController
end # Rho
