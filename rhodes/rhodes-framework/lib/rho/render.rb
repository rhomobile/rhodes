require 'rho/rhoapplication'

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

	def self.render_index(filename)
		layout = File.dirname(filename) + "/layout_erb.iseq"
		@content = eval_compiled_file(filename, binding)
		@content = eval_compiled_file(layout, binding) if File.exist?(layout)
	    @content
	end

    def render(options = nil)
    	options = {} if options.nil? or !options.is_a?(Hash)
		options[:action] = :index if options[:action].nil?

		@content = eval_compiled_file(@request[:modelpath]+options[:action].to_s+'_erb.iseq', binding )
        puts 'render content: ' + @content.length.to_s
		if xhr? and options[:use_layout_on_ajax] != true
			options[:layout] = false
		elsif options[:layout].nil? or options[:layout] == true
			options[:layout] = self.class.get_layout_name
		end

		if options[:layout] != false
			layoutfile = RhoApplication::get_app_path(@request['application']) + options[:layout].to_s + "_erb.iseq"
			@content = eval_compiled_file(layoutfile, binding ) if File.exist?(layoutfile)
			puts 'Layout file: ' + layoutfile + '. Content size: ' + @content.length.to_s
		end

        RhoController.start_objectnotify()
        
		@content
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