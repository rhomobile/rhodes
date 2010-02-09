require 'rho/rhoapplication'
require 'rho/rhocontroller'

module Rho
  class RhoController

    def self.layout(name)
      @layout = name
    end

    def self.get_layout_name
      @layout.nil? ? 'layout' : @layout
    end

    def self.renderfile(filename)
      res = ""
      if File.extname(filename) == '.iseq'
        res = render_index(filename)
      else
        res = IO.read(filename)
      end
      RhoController.start_objectnotify()

      res
    end

    def inst_render_index(filename)
      rho_info 'inst_render_index'
      @request, @response = {}
      @params = {}
      require 'rho/rhoviewhelpers'
      layout = File.dirname(filename) + "/layout_erb.iseq"
      @content = eval_compiled_file(filename, getBinding() )
      @content = eval_compiled_file(layout, getBinding() ) if File.exist?(layout)
      @content
    end

    def self.render_index(filename)
      (RhoController.new).inst_render_index(filename)
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

      unless options[:string].nil?
        @content = options[:string]
        @back_action = options[:back] if options[:back]
        @rendered = true
        return @content
      end

      unless options[:partial].nil?  # render the file and return, don't set rendered true for a partial.
        @content = render_partial(options)
        return @content
      end

      if options[:file].nil? or !options[:file].is_a?(String)
        if options[:action].nil?
          called_action = @request['action'].nil? ? default_action : @request['action']
          if File.exist?(@request[:modelpath]+called_action.to_s+'_erb.iseq')
            @content = eval_compiled_file(@request[:modelpath]+called_action.to_s+'_erb.iseq', getBinding() )
          else
            @content = ""
          end
        else
          @content = eval_compiled_file(@request[:modelpath]+options[:action].to_s+'_erb.iseq', getBinding() )
        end
      else
        options[:file] = options[:file].gsub(/\.erb$/,"").gsub(/^\/app/,"")
        @content = eval_compiled_file(RhoApplication::get_app_path(@request['application'])+options[:file]+'_erb.iseq', getBinding() )
        options[:layout] = false if options[:layout].nil?
      end

      rho_info 'render content: ' + @content.length.to_s
      if xhr? and options[:use_layout_on_ajax] != true
        options[:layout] = false
      elsif options[:layout].nil? or options[:layout] == true
        options[:layout] = self.class.get_layout_name
      end

      if options[:layout] != false
        layoutfile = RhoApplication::get_app_path(@request['application']) + options[:layout].to_s + "_erb.iseq"
        @content = eval_compiled_file(layoutfile, binding ) if File.exist?(layoutfile)
        rho_info 'Layout file: ' + layoutfile + '. Content size: ' + @content.length.to_s
      end

      RhoController.start_objectnotify()
      @back_action = options[:back] if options[:back]
      @rendered = true
      @content
    end

    def render_partial(options = nil)
      options = {} if options.nil? or !options.is_a?(Hash)
      options = options.symbolize_keys

      localclass = Class.new do
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
            varname = name.to_s.gsub(/\=/,"")
            setting = (name.to_s =~ /=/)
            if setting
              @vars[varname.to_sym()] = args[0]
            else
              @vars[varname.to_sym()]
            end
          end
        end
        def get_binding
          binding
        end
      end

      partial_name = options[:partial].split('/')[-1]

      options[:locals] = {} if options[:locals].nil? or !options[:locals].is_a?(Hash)
       
      content = ""
      if options[:collection].nil?
        locals = localclass.new(options[:locals])
        content = eval_compiled_file(@request[:modelpath]+'_' + options[:partial].to_s+'_erb.iseq', locals.get_binding )
      else
        i = 0
        options[:collection].each do |x|
          options[:locals][partial_name] = x

          options[:locals][partial_name + '_counter'] = i
          i = i + 1
          #puts "render partial: #{x}"
          content += render_partial :partial => options[:partial], :locals => options[:locals]
        end
      end
      content
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
    end

  end # RhoController
end # Rho
