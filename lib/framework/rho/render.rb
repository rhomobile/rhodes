#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

require 'rho/rhoapplication'
require 'rho/rhocontroller'
require 'rho/rho'

module Rho
  class RhoController
    begin
      is_translator_exist = true
      is_translator_exist = (Rho::file_exist?( File.join(__rhoGetRuntimeDir(), 'lib/rhodes_translator' + RHO_RB_EXT) ) || Rho::file_exist?( File.join(__rhoGetRuntimeDir(), 'lib/rhodes_translator' + RHO_RB_EXT + RHO_ENCRYPTED_EXT) )) if !Rho::System.isRhoSimulator

      if is_translator_exist
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
      begin
          res = ""

          if filename.end_with?(RHO_ERB_EXT) || filename.end_with?(RHO_RB_EXT + RHO_ENCRYPTED_EXT)
            if RhoApplication::current_controller()
                puts "reuse current action controller."
                res = RhoApplication::current_controller().inst_render_index(filename, req, res)
            else
                res = (RhoController.new).inst_render_index(filename, req, res)
            end
          else
            res = IO.read(filename)
          end

          if System.get_property('platform') != 'WP8' && System.get_property('platform') != 'UWP'
            RhoController.start_objectnotify()
            RhoController.start_geoview_notification()
          end

          res
      rescue Exception => exception

        raise
      end
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
          layout = File.dirname(filename) + "/layout" + RHO_ERB_EXT
          if !Rho::file_exist?(layout)
             layout = layout + RHO_ENCRYPTED_EXT
          end
          @content = eval_compiled_file(layout, getBinding() ) if Rho::file_exist?(layout)
      else
          if @request["headers"]["Transition-Enabled"] == "true"
            #puts "add 'div' in inst_render_index"
            @content = "<div>#{@content}</div>"
          end
      end

      @content
    end

    def getBinding
        binding
    end

    @@cached_metadata = {}
    def self.cached_metadata
        @@cached_metadata
    end

    def self.clean_cached_metadata
        @@cached_metadata.clear()
        #puts "meta deleted"
    end

    def __get_model
        model = nil
        begin
            model = Object.const_get(@request['model'].to_sym()) if Object.const_defined?(@request['model'].to_sym() )
        rescue Exception => exc
        end
    end

    def render(options = nil)
      if @params['rho_callback']
        rho_error( "render call in callback. Call WebView.navigate instead" )
        return ""
      end

      RhoProfiler.start_counter('ERB_RENDER')

      options = {} if options.nil? or !options.is_a?(Hash)
      options = options.symbolize_keys

      metaenabled = false

      action = nil
      action = options[:action] if options[:action]
      action = @request['action'].nil? ? default_action : @request['action'] unless action

      if @request['model'] != nil

        model = __get_model()

        if model && model.respond_to?( :metadata ) and model.metadata != nil
          if $".include?( "rhodes_translator")
            metaenabled = model.metadata[action.to_s] != nil
          else
            rho_error( "unable to load rhodes_translator gem." )
          end
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
          fname = @request[:modelpath]+action.to_s+RHO_ERB_EXT
          if Rho::file_exist?(fname)
              @content = eval_compiled_file(fname, getBinding() )
          else
              fname = fname+RHO_ENCRYPTED_EXT
              if Rho::file_exist?(fname)
                  @content = eval_compiled_file(fname, getBinding() )
              else
                  @content = ""
              end
          end
        else
          options[:file] = options[:file].gsub(/\.erb$/,"").gsub(/^\/app/,"")
          fpath = RhoApplication::get_app_path(@request['application'])+options[:file]+RHO_ERB_EXT
          if !Rho::file_exist?(fpath)
             fpath = fpath + RHO_ENCRYPTED_EXT
          end
          @content = eval_compiled_file(fpath, getBinding() )
          options[:layout] = false if options[:layout].nil?
        end
      end
      #rho_info 'render content: ' + @content.length.to_s
      if xhr? and options[:use_layout_on_ajax] != true
        options[:layout] = false
        if options[:partial].nil? && @request["headers"]["Transition-Enabled"] == "true"
          #puts "add 'div' in render"
          if !(@request["headers"]["Accept"] =~ /^\*\/\*/ || @request["headers"]["Accept"] =~ /^text\/html/)
            @response["headers"]["Content-Type"] = @request["headers"]["Accept"].gsub(/\,.*/, '')
          else
            @content = "<div>#{@content}</div>"
          end
        end
      elsif options[:layout].nil? or options[:layout] == true
        options[:layout] = self.class.get_layout_name
      end

      if options[:layout] != false
        layoutfile = RhoApplication::get_app_path(@request['application']) + options[:layout].to_s + RHO_ERB_EXT
        if !Rho::file_exist?(layoutfile)
           layoutfile = layoutfile + RHO_ENCRYPTED_EXT
        end
        @content = eval_compiled_file(layoutfile, binding ) if Rho::file_exist?(layoutfile)
        rho_info 'Layout file: ' + layoutfile + '. Content size: ' + @content.length.to_s
      end

      RhoController.start_objectnotify()
      RhoController.start_geoview_notification()
      @back_action = options[:back] if options[:back]
      @rendered = true

      RhoProfiler.stop_counter('ERB_RENDER')

      @content
    end

    def render_metadata(action = nil,metadata = nil)
      if metadata.nil?
        model = nil
        model = Object.const_get(@request['model'].to_sym) if Object.const_defined?(@request['model'].to_sym)

        if model && model.respond_to?( :metadata ) and model.metadata != nil
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

      data["self"] = self

      prepared = bind(data,metadata[action])

      translate(prepared,action)

    end

    def render_partial(options = nil)
      options = {} if options.nil? or !options.is_a?(Hash)
      options = options.symbolize_keys

      splitpartial = options[:partial].split('/')
      partial_name = splitpartial[-1]
      model = nil
      if splitpartial.length > 1
        model = splitpartial[-2]
      end

      options[:locals] = {} if options[:locals].nil? or !options[:locals].is_a?(Hash)
       
      content = ""
      if options[:collection].nil?
        locals = self.clone

        class << locals

          def set_vars(obj = nil)
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

        locals.set_vars(options[:locals])

        modelpath = @request[:modelpath]
        modelpath = Rho::RhoFSConnector.get_model_path("app",model) if model
        fpath = modelpath+'_' + partial_name.to_s+RHO_ERB_EXT
        if !Rho::file_exist?(fpath)
           fpath = fpath + RHO_ENCRYPTED_EXT
        end
        content = eval_compiled_file(fpath, locals.get_binding )
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

      if arg.is_a?(Array)
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
      if defined?(RHOCONNECT_CLIENT_PRESENT)
          Rho::RhoConnectClient.clean_objectnotify()
      end

      return unless @@m_arObjectNotify && @@m_arObjectNotify.length > 0 

      if defined?(RHOCONNECT_CLIENT_PRESENT)
          0.upto(@@m_arObjectNotify.length()-1) do |i|
            Rho::RhoConnectClient::add_objectnotify(@@m_arSrcIDNotify[i], @@m_arObjectNotify[i])
          end
      end
      
      @@m_arObjectNotify = []
      @@m_arSrcIDNotify = []
      
    end

  end # RhoController
end # Rho
