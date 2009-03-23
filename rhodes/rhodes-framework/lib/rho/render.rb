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
      	if File.extname(filename) == '.iseq'
	    	render_index(filename)
	  	else
        	IO.read(filename)
		end
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

		if xhr? and options[:use_layout_on_ajax] != true
			options[:layout] = false
		elsif options[:layout].nil? or options[:layout] == true
			options[:layout] = self.class.get_layout_name
		end

		if options[:layout] != false
			layoutfile = RhoApplication::get_app_path(@request['application']) + options[:layout].to_s + "_erb.iseq"
			puts 'Layout file: ' + layoutfile
			@content = eval_compiled_file(layoutfile, binding ) if File.exist?(layoutfile)
		end

		@content
    end

  end # RhoController
end # Rho