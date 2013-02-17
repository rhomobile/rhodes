require 'rubygems'
require 'templater'
require 'thread'
#TODO: This is temporary, see https://www.pivotaltracker.com/story/show/3399292
gem "activesupport", "~> 2.3.5"
require 'active_support'
require 'uuid'
require 'yaml'
require 'rexml/document'
require 'fileutils'
require File.join(File.dirname(__FILE__),'/../../lib/build/jake.rb')

require File.dirname(__FILE__) + '/../../lib/rhodes'



module Rhogen
  extend Templater::Manifold

  desc <<-DESC
    Generate rhodes application
  DESC

  class BaseGenerator < Templater::Generator
    def class_name
      name.gsub('-', '_').camel_case
    end

    alias_method :module_name, :class_name
  end

  class AppGenerator < BaseGenerator

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'application')
    end

    desc <<-DESC
      Generates a new rhodes application.

      Options:
        --norhoconnect - don't include rhoconnect-client in application
      
      Required:
        name        - application name
        
      Optional:
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292/application")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :norhoconnect, :desc=> '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => "application name"
    second_argument :syncserver, :required => false, :desc => 'url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/")'
    third_argument :zip_url, :required => false, :desc => "optional url to zipfile download of bundle"

    template :config do |template|
      zip_url ||= ''
      syncserver ||= ''
      template.source = 'rhoconfig.txt'
      template.destination = "#{name}/rhoconfig.txt"
    end

    template :buildyml do |template|
      @sdk_path = File.expand_path(File.join(File.dirname(__FILE__),'..','..'))
      @sdk_path.gsub!("\\","/")
      @app_name = name
      @app_name_cleared = name.downcase.split(/[^a-zA-Z0-9\.\-]/).map{|w| w.downcase}.join("")
      puuid = UUID.new
      generated_uuid = puuid.generate
      @productid = generated_uuid
      @uid = '0x'+(0xE0000000 + rand(0xFFFFFFF)).to_s(16)
      @rhoconnectclient_ext = ', "rhoconnect-client"' unless norhoconnect
      template.source = 'build.yml'
      template.destination = "#{name}/build.yml"
    end
    
    template :gitignore do |template|
      template.source = 'gitignore'
      template.destination = "#{name}/.gitignore"
    end
    
    template :application do |template|
      template.source = 'app/application.rb'
      template.destination = "#{name}/app/application.rb"
    end

    template :index do |template|
      template.source = 'app/index.erb'
      template.destination = "#{name}/app/index.erb"
    end
=begin
    template :bb_index do |template|
      template.source = 'app/index.bb.erb'
      template.destination = "#{name}/app/index.bb.erb"
    end   
=end

    template :layout do |template|
      template.source = 'app/layout.erb'
      template.destination = "#{name}/app/layout.erb"
    end
    
    template :loading do |template|
      template.source = 'app/loading.html'
      template.destination = "#{name}/app/loading.html"
    end

    file :loadingpng do |file|
      file.source = 'app/loading.png'
      file.destination = "#{name}/app/loading.png"
    end

    file :loadingpng5 do |file|
      file.source = 'app/loading-568h@2x.png'
      file.destination = "#{name}/app/loading-568h@2x.png"
    end

    file :loadingpngl do |file|
      file.source = 'app/loading-Landscape.png'
      file.destination = "#{name}/app/loading-Landscape.png"
    end

    file :loadingpngll do |file|
      file.source = 'app/loading-LandscapeLeft.png'
      file.destination = "#{name}/app/loading-LandscapeLeft.png"
    end

    file :loadingpnglr do |file|
      file.source = 'app/loading-LandscapeRight.png'
      file.destination = "#{name}/app/loading-LandscapeRight.png"
    end

    file :loadingpngp do |file|
      file.source = 'app/loading-Portrait.png'
      file.destination = "#{name}/app/loading-Portrait.png"
    end

    file :loadingpngpud do |file|
      file.source = 'app/loading-PortraitUpsideDown.png'
      file.destination = "#{name}/app/loading-PortraitUpsideDown.png"
    end

    file :loadingpngx do |file|
      file.source = 'app/loading@2x.png'
      file.destination = "#{name}/app/loading@2x.png"
    end

    
    directory :helpers do |directory|
      directory.source = 'app/helpers'
      directory.destination = "#{name}/app/helpers"
    end

    directory :icon do |directory|
      directory.source = 'icon'
      directory.destination = "#{name}/icon"
    end

    template :settings1 do |template|
      template.source = 'app/Settings/controller.rb'
      template.destination = "#{name}/app/Settings/controller.rb"
    end
    template :settings2 do |template|
      template.source = 'app/Settings/err_sync.erb'
      template.destination = "#{name}/app/Settings/err_sync.erb"
    end
    template :settings3 do |template|
      template.source = 'app/Settings/home.erb'
      template.destination = "#{name}/app/Settings/home.erb"
    end
    template :settings4 do |template|
      template.source = 'app/Settings/index.erb'
      template.destination = "#{name}/app/Settings/index.erb"
    end
    template :settings5 do |template|
      template.source = 'app/Settings/login.erb'
      template.destination = "#{name}/app/Settings/login.erb"
    end
    template :settings6 do |template|
      template.source = 'app/Settings/reset.erb'
      template.destination = "#{name}/app/Settings/reset.erb"
    end
    template :settings7 do |template|
      template.source = 'app/Settings/wait.erb'
      template.destination = "#{name}/app/Settings/wait.erb"
    end
    
=begin    
    directory :settings do |directory|
      directory.source = 'app/Settings'
      directory.destination = "#{name}/app/Settings/"
    end
=end
    directory :public do |directory|
      directory.source = 'public'
      directory.destination = "#{name}/public/"
    end
 
    template :rakefile do |template|
      template.source = 'Rakefile'
      template.destination = "#{name}/Rakefile"
    end
  end

  class ModelGenerator < BaseGenerator
    include ActiveSupport::Inflector

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'model')
    end

    desc <<-DESC
      Generates a new model for a rhodes application.
      
      Required:
        name        - model name
        attributes  - list of one or more string attributes (i.e. name,industry,progress), NO spaces between attributes
        
      Optional:
        priority    - sync priority (i.e. 100)
        type        - DEPRECATED: type of model (i.e. "ask" for an ask model). This will be removed in 1.5, instead use 
                      search method.
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "model name"
    second_argument :attributes, :as => :array, :required => true, :desc => "list of one or more string attributes (i.e. name,industry,progress), NO spaces between attributes"
    third_argument :priority, :required => false, :desc => "optional sync priority (i.e. 100)"
    fourth_argument :type, :required => false, :desc => "optional type (i.e. \"ask\" for an ask model)"

    #template :config do |template|
    #  @model_sync_server = syncserver_exists? ? class_name : ''
    #  template.source = 'config.rb'
    #  template.destination = "app/#{name.camel_case}/config.rb"
    #end

    template :index do |template|
      template.source = 'index.erb'
      template.destination = "app/#{name.camel_case}/index.erb"
    end

    template :edit do |template|
      template.source = 'edit.erb'
      template.destination = "app/#{name.camel_case}/edit.erb"
    end

    template :new do |template|
      template.source = 'new.erb'
      template.destination = "app/#{name.camel_case}/new.erb"
    end

    template :new do |template|
      template.source = 'show.erb'
      template.destination = "app/#{name.camel_case}/show.erb"
    end
=begin    
    template :bb_index do |template|
      template.source = 'index.bb.erb'
      template.destination = "app/#{name.camel_case}/index.bb.erb"
    end

    template :bb_edit do |template|
      template.source = 'edit.bb.erb'
      template.destination = "app/#{name.camel_case}/edit.bb.erb"
    end

    template :bb_new do |template|
      template.source = 'new.bb.erb'
      template.destination = "app/#{name.camel_case}/new.bb.erb"
    end
    
    template :bb_show do |template|
      template.source = 'show.bb.erb'
      template.destination = "app/#{name.camel_case}/show.bb.erb"
    end
=end

    template :controller do |template|
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
      template.source = 'controller.rb'
      template.destination = "app/#{name.camel_case}/#{underscore_name}_controller.rb"
    end

    template :model do |template|
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
      template.source = 'model.rb'
      template.destination = "app/#{name.camel_case}/#{underscore_name}.rb"
    end

    template :spec do |template|
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
      template.source = 'spec.rb'
      template.destination = "app/test/#{underscore_name}_spec.rb"
    end

    def attributes?
      self.attributes && !self.attributes.empty?
    end
    
    def syncserver_exists?
      found = true
      File.open('rhoconfig.txt').each do |line|
        found = false if line.match("syncserver\ =\ ''") or line.match("syncserver\ =\ \"\"") or line.match("syncserver\ =\ nil")
      end
      found
    end
  end

  class SpecGenerator < BaseGenerator

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'spec')
    end

    desc <<-DESC
      Adds spec framework to rhodes application.
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    #first_argument :name, :required => true, :desc => "application name"
    #second_argument :syncserver, :required => false, :desc => 'url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/")'
    #third_argument :zip_url, :required => false, :desc => "optional url to zipfile download of bundle"


    directory :specrunner do |directory|
      directory.source = 'app/SpecRunner'
      directory.destination = "app/SpecRunner"
    end
    #directory :mspec do |directory|
    #  directory.source = 'app/mspec'
    #  directory.destination = "app/mspec"
    #end
    #directory :spec do |directory|
    #  directory.source = 'app/spec'
    #  directory.destination = "app/spec"
    #end
    #template :fileutils do |template|
    #  template.source = 'app/fileutils.rb'
    #  template.destination = "app/fileutils.rb"
    #end
    template :mspecrb do |template|
      template.source = 'app/mspec.rb'
      template.destination = "app/mspec.rb"
    end
    template :specrunnerrb do |template|
      template.source = 'app/spec_runner.rb'
      template.destination = "app/spec_runner.rb"
    end

  end



  class ExtensionGenerator < BaseGenerator

	@@noapp = false

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'extension')
    end

    desc <<-DESC
      Adds native extension to Rhodes based application.
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'
	option :noapp, :desc=> '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => "extension name"


    def namefixed
        return name.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("")
    end    

    template :ext do |template|
      template.source = 'extensions/montana/ext.yml'
      template.destination = "extensions/#{namefixed.downcase}/ext.yml"
	  puuid = UUID.new
      generated_uuid = puuid.generate
      @productid = generated_uuid
	  @@noapp = noapp
    end

    template :extension_ruby do |template|
      template.source = 'extensions/montana/montana.rb'
      template.destination = "extensions/#{namefixed.downcase}/#{namefixed.downcase}.rb"
    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
        # change atribbutes in build script file to executable
        File.chmod(0755, $build_script_full_path) 
		Dir.chdir("extensions/#{namefixed.downcase}/ext")
		args = []
		args << "api"
		args << Dir.pwd+"/#{namefixed.downcase}.xml"
    ## TODO: fix it - next line do not work on Mac OS !
		## puts Jake.run(source_root+"/../../../../bin/rhogen", args)
    ##
    end

	def callback_after_delete_testapp(template)
		if @@noapp == true
			Dir.chdir("../../../")
			FileUtils.rm_rf "app"
			FileUtils.cp_r("extensions/#{namefixed.downcase}", Dir.pwd)
			FileUtils.rm_rf "extensions"
		end
	end

	template :extension_apigen_xml do |template|
        template.source = 'extensions/montana/ext/montana.xml'
        template.destination = "extensions/#{namefixed.downcase}/ext/#{namefixed.downcase}.xml"
    end

    template :build do |template|
      template.source = 'extensions/montana/ext/build'
      template.destination = "extensions/#{namefixed.downcase}/ext/build"
      $build_script_full_path = template.destination   
      template.options = { :after => :callback_after_make_build}  
    end

    template :build_bat do |template|
      template.source = 'extensions/montana/ext/build.bat'
      template.destination = "extensions/#{namefixed.downcase}/ext/build.bat"
    end

    template :extension_iphone_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Rakefile'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/Rakefile"
    end

    template :extension_iphone_pch do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Montana_Prefix.pch'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/#{namefixed.camel_case}_Prefix.pch"
    end

    template :extension_iphone_project do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Montana.xcodeproj/project.pbxproj'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/#{namefixed.camel_case}.xcodeproj/project.pbxproj"
    end

    template :extension_iphone_src_h do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Classes/Montana.h'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/Classes/#{namefixed.camel_case}.h"
    end

    template :extension_iphone_src_m do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Classes/Montana.m'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/Classes/#{namefixed.camel_case}.m"
    end

    template :extension_android_ext_build do |template|
        template.source = 'extensions/montana/ext/platform/android/ext_build.files'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/android/ext_build.files"
    end

    template :extension_android_rakefile do |template|
        template.source = 'extensions/montana/ext/platform/android/Rakefile'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/android/Rakefile"
    end

    template :extension_android_cpp do |template|
        template.source = 'extensions/montana/ext/platform/android/jni/src/montana.cpp'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/android/jni/src/#{namefixed.downcase}.cpp"
    end

    template :extension_android_java do |template|
        template.source = 'extensions/montana/ext/platform/android/src/com/montana/Montana.java'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/android/src/com/#{namefixed.downcase}/#{namefixed.camel_case}.java"
    end


    #template :extension_wm_rakefile do |template|
    #    template.source = 'extensions/montana/ext/platform/wm/Rakefile'
    #    template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/Rakefile"
    #end

    template :extension_wm_vcsol do |template|
        template.source = 'extensions/montana/ext/platform/wm/Montana.sln'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/#{namefixed.camel_case}.sln"
    end

    template :extension_wm_vcproject do |template|
        template.source = 'extensions/montana/ext/platform/wm/Montana.vcproj'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/#{namefixed.camel_case}.vcproj"
    end

    template :extension_wm_vcprojectprops do |template|
        template.source = 'extensions/montana/ext/platform/wm/montana.vsprops'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/#{namefixed.camel_case}.vsprops"
    end

    template :extension_wm_impl_cpp do |template|
        template.source = 'extensions/montana/ext/platform/wm/src/montana_impl.cpp'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/src/#{namefixed.camel_case}_impl.cpp"
    end

	template :extension_wp8_vcproject do |template|
        template.source = 'extensions/montana/ext/platform/wp8/Montana.vcxproj'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wp8/#{namefixed.camel_case}.vcxproj"
    end

    template :extension_wp8_vcprojectprops do |template|
        template.source = 'extensions/montana/ext/platform/wp8/montana.props'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wp8/#{namefixed.camel_case}.props"
    end

	template :extension_wp8_vcprojectfilters do |template|
        template.source = 'extensions/montana/ext/platform/wp8/montana.vcxproj.filters'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wp8/#{namefixed.camel_case}.vcxproj.filters"
    end

	template :extension_wp8_impl_cpp do |template|
        template.source = 'extensions/montana/ext/platform/wp8/src/montana_impl.cpp'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wp8/src/#{namefixed.camel_case}_impl.cpp"
    end

    template :extension_bb_files do |template|
        template.source = 'extensions/montana/ext/platform/bb/Montana.files'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/bb/#{namefixed.camel_case}.files"
    end

    template :extension_bb_jgp do |template|
        template.source = 'extensions/montana/ext/platform/bb/montana.jdp'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/bb/#{namefixed.downcase}.jdp"
    end

    template :extension_bb_Rakefile do |template|
        template.source = 'extensions/montana/ext/platform/bb/Rakefile'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/bb/Rakefile"
    end

    template :extension_bb_java do |template|
        template.source = 'extensions/montana/ext/platform/bb/src/com/montana/Montana.java'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/bb/src/com/#{namefixed.downcase}/#{namefixed.camel_case}.java"
    end
  
    template :extension_test_controller do |template|
	  template.source = 'app/MontanaTest/controller.rb'
      template.destination = "app/#{namefixed.camel_case}Test/controller.rb"
    end

    template :extension_test_index do |template|
      template.source = 'app/MontanaTest/index.erb'
      template.destination = "app/#{namefixed.camel_case}Test/index.erb"
	  template.options = { :after => :callback_after_delete_testapp}  
	end



    #<%= name.downcase %>
    #<%= name.camel_case %>

    #template :spec do |template|
    #  underscore_name = name.camel_case.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
    #  template.source = 'spec.rb'
    #  template.destination = "app/test/#{underscore_name}_spec.rb"
    #end

    #file :loadingpng do |file|
    #  file.source = 'app/loading.png'
    #  file.destination = "#{name}/app/loading.png"
    #end
    
    #directory :helpers do |directory|
    #  directory.source = 'app/helpers'
    #  directory.destination = "#{name}/app/helpers"
    #end


    def attributes?
      self.attributes && !self.attributes.empty?
    end

  end


  class ApiGenerator < BaseGenerator

    Templater::CLI::Generator.class_eval do

      def old_step_through_templates
        @generator.all_actions.each do |action|
          if @options[:delete]
            action.revoke! unless @options[:pretend]
            say_status('deleted', action, :red)
          else
            if action.identical?
              say_status('identical', action, :blue)
            elsif action.exists?
              if @options[:force]
                action.invoke! unless @options[:pretend]
                say_status('forced', action, :yellow)
              elsif @options[:skip]
                say_status('skipped', action, :yellow)
              else
                say_status('conflict', action, :red)
                conflict_menu(action)
              end
            else
              action.invoke! unless @options[:pretend]
              say_status('added', action, :green)
            end
          end
        end
      end

      def step_through_templates
        if @generator.respond_to? :setup_xml
          if @generator.setup_xml
            $modules.each do |module_item|
               $cur_module = module_item
               old_step_through_templates
            end
          end
        else
          old_step_through_templates
        end
      end
    end

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'api')
    end

    def destination_root
        if File.dirname(@@xml_filepath)!= '.'
            File.dirname(@@xml_filepath).gsub("\\", "/")
        else
            self.class.superclass.instance_method(:destination_root).bind(self).call
        end
    end

    desc <<-DESC
      Generate API sources from XML API file

      Options:
        --skipwarnings - do not stop generation process by any XML parsing warnings

      Required:
        xml_path        - path to XML with API description

    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :skipwarnings, :desc=> 'skip any XML warnings, by default - generation break by any XML parse warning', :as => :boolean, :default => true
    first_argument :xmlpath, :required => true, :desc => "path to XML with API description"

    $xml = nil

    # array of arrays

    class ModuleAlias

      def initialize
        @existing_name = ''
        @new_name = ''
        @is_ruby_only = false
        @deprecated = false
        @reverseLogic = false
      end

      attr_accessor :existing_name
      attr_accessor :new_name
      attr_accessor :is_ruby_only
      attr_accessor :deprecated
      attr_accessor :reverseLogic

    end

    class MethodParam

      TYPE_STRING = "STRING"
      TYPE_ARRAY = "ARRAY"
      TYPE_HASH = "HASH"
      TYPE_INT = "INTEGER"
      TYPE_BOOL = "BOOLEAN"
      TYPE_DOUBLE = "FLOAT"
      TYPE_SELF = 'SELF_INSTANCE'

      BASE_TYPES = [TYPE_STRING, TYPE_ARRAY, TYPE_HASH, TYPE_INT, TYPE_BOOL, TYPE_DOUBLE]

      def initialize
        @name = ''
        @type = TYPE_STRING
        @can_be_nil = false
      end

      attr_accessor :name
      #attr_accessor :type
      attr_reader :type
      attr_accessor :can_be_nil

      def type=(value)
        up_value = value.upcase
        BASE_TYPES.each do |t|
            if up_value == t 
                @type = up_value
                return     
            end
        end
        
        if up_value == TYPE_SELF
            @type = up_value
        else
            @type = value
        end
        
      end

    end

    class MethodResult

      def initialize
        @type = MethodParam::TYPE_STRING
        @item_type = MethodParam::TYPE_STRING
      end

      attr_accessor :type
      attr_accessor :item_type
    end


    class ModuleConstant
      def initialize
        @name = ''
        @type = MethodParam::TYPE_STRING
        @value = ''
      end

      attr_accessor :name
      attr_accessor :type
      attr_accessor :value

    end

    class ModuleProperty

      USE_PROPERTY_BAG_MODE_NONE = "NONE"
      USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS = "PROPERTY_BAG_VIA_ACCESSORS"
      USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG = "ACCESSORS_VIA_PROPERTY_BAG"

      def initialize
        @name = ''
        @type = MethodParam::TYPE_STRING
        @native_name = ''
        @readonly = false
        @generate_accessors = true
        @use_property_bag_mode = USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
        @default_value = ''
        
        #used if getter and setter are present - this used with generation of property bag function implementation
        @getter = nil
        @setter = nil
      end

      attr_accessor :name
      attr_accessor :type
      attr_accessor :native_name
      attr_accessor :readonly
      attr_accessor :generate_accessors
      attr_accessor :use_property_bag_mode
      attr_accessor :default_value
      attr_accessor :getter
      attr_accessor :setter

    end

    class ModuleMethod

      ACCESS_STATIC = "STATIC"
      ACCESS_INSTANCE = "INSTANCE"

      CALLBACK_MANDATORY = "MANDATORY"
      CALLBACK_OPTIONAL = "OPTIONAL"
      CALLBACK_NONE = "NONE"

      RUN_IN_THREAD_NONE = "NONE"
      RUN_IN_THREAD_MODULE = "MODULE"
      RUN_IN_THREAD_SEPARATED = "SEPARATED"
      RUN_IN_THREAD_UI = "UI"

      SPECIAL_BEHAVIOUR_NONE = "NONE"
      SPECIAL_BEHAVIOUR_GETTER = "GETTER"
      SPECIAL_BEHAVIOUR_SETTER = "SETTER"

      def initialize
        @name = ''
        @native_name = ''
        @params = [] # array of MethodArgument
        @run_in_thread = RUN_IN_THREAD_NONE
        @is_factory_method = false
        @is_return_value = false
        @access = ACCESS_INSTANCE
        @has_callback = CALLBACK_NONE
        @result = nil
        @is_deprecated = false

        # name of template produced this method if applicable
        @generated_by_template = ''

        @linked_property = nil # use only if method is setter or getter
        #if this property is not NONE then generator should prepare special function code. For example setter or getter have already prepared code
        @special_behaviour = SPECIAL_BEHAVIOUR_NONE

        #use for store any data by string key. For example I save to this bag a prepared iOS full function signature for do not rebuild it everytime
        @cached_data = {}
      end

      attr_accessor :name
      attr_accessor :native_name
      attr_accessor :params
      attr_accessor :run_in_thread
      attr_accessor :is_factory_method
      attr_accessor :is_return_value
      attr_accessor :access
      attr_accessor :has_callback
      attr_accessor :result
      attr_accessor :is_deprecated
      attr_accessor :generated_by_template
      attr_accessor :linked_property
      attr_accessor :special_behaviour
      attr_accessor :cached_data
    end

    class ModuleItem
      def initialize
        @name = ''

        # array of parents from top to lowest
        @parents = []
        @methods = []
        @properties = []
        @constants = []
        @method_aliases = []
        @property_aliases = []
        @generateUnderscoreRubyNames = false
        @is_template_singletone_id = false
        @is_template_default_instance = false
        @is_template_propertybag = false
        @has_instance_methods = false
        @has_factory_methods = false
        @is_property_bag_limit_to_only_declared_properties = false
        @use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
        @properties_access = ModuleMethod::ACCESS_INSTANCE
      end

      def getPropAliases(name)
        ar = []
        property_aliases.each do|alias_item|
            ar << alias_item.new_name if alias_item.existing_name == name
        end
        
        ar
      end
      
      attr_accessor :name
      attr_accessor :parents
      attr_accessor :methods
      attr_accessor :properties
      attr_accessor :constants
      attr_accessor :method_aliases
      attr_accessor :property_aliases
      attr_accessor :generateUnderscoreRubyNames
      attr_accessor :is_template_singletone_id
      attr_accessor :is_template_default_instance
      attr_accessor :is_template_propertybag
      attr_accessor :has_instance_methods
      attr_accessor :has_factory_methods
      attr_accessor :is_property_bag_limit_to_only_declared_properties
      attr_accessor :use_property_bag_mode
      attr_accessor :properties_access
    end

    TEMPLATE_NAME = "TEMPLATE_NAME"
    TEMPLATE_PROPERTY_BAG = "property_bag"
    TEMPLATE_SINGLETONE_INSTANCES = "singleton_instances"

    $modules = []

    $cur_module = nil



    $possible_attributes = {}
    $possible_attributes["MODULE"] = ["name", "parent", "generateUnderscoreRubyNames"]
    $possible_attributes["CONSTANT"] = ["name", "value", "type"]
    $possible_attributes["PROPERTIES"] = ["usePropertyBag", "readOnly", "generateAccessors", "limitPropertyBag"]
    $possible_attributes["PROPERTY"] = ["name", "type", "usePropertyBag", "readOnly", "generateAccessors", "default"]
    $possible_attributes["VALUE"] = ["constName", "value", "type"]
    $possible_attributes["ALIAS"] = ["new", "existing", "reverseLogic", "deprecated", "rubyOnly"]
    $possible_attributes["METHODS"] = ["access", "hasCallback", "factory", "runInThread", "deprecated"]
    $possible_attributes["METHOD"] = ["name", "access", "hasCallback", "factory", "runInThread", "nativeName", "deprecated"]
    $possible_attributes["PARAM"] = ["name", "nativeName", "type"]
    $possible_attributes["RETURN"] = ["type", "itemType"]

    $possible_children = {}
    $possible_children["API"] = ["MODULE"]
    $possible_children["MODULE"] = ["HELP_OVERVIEW", "MORE_HELP", "TEMPLATES", "CONSTANTS", "PROPERTIES", "METHODS", "USER_OVERVIEW", "VER_INTRODUCED", "PLATFORM"]
    $possible_children["TEMPLATES"] = ["SINGLETON_INSTANCES", "DEFAULT_INSTANCE", "PROPERTY_BAG"]
    $possible_children["CONSTANTS"] = ["CONSTANT"]
    $possible_children["CONSTANT"] = ["DESC"]
    $possible_children["PROPERTIES"] = ["DESC", "PROPERTY", "ALIASES"]
    $possible_children["PROPERTY"] = ["DESC", "VALUES"]
    $possible_children["VALUES"] = ["VALUE"]
    $possible_children["VALUE"] = ["DESC"]
    $possible_children["ALIASES"] = ["ALIAS"]
    $possible_children["ALIAS"] = ["DESC"]
    $possible_children["METHODS"] = ["METHOD", "ALIASES", ]
    $possible_children["METHOD"] = ["DESC", "PARAMS", "RETURN"]
    $possible_children["PARAMS"] = ["PARAM"]
    $possible_children["PARAM"] = ["DESC", "CAN_BE_NIL"]
    $possible_children["RETURN"] = ["DESC"]
    $possible_children["CAN_BE_NIL"] = ["DESC"]

    $possible_values = {}
    $possible_values["generateUnderscoreRubyNames"] = ["true", "false"]
    $possible_values["readonly"] = ["true", "false"]
    $possible_values["usePropertyBag"] = ["none", "accessorsViaPropertyBag", "PropertyBagViaAccessors"]
    $possible_values["access"] = ["STATIC", "INSTANCE"]
    $possible_values["factory"] = ["true", "false"]
    $possible_values["runInThread"] = ["none", "ui", "module", "separate"]
    $possible_values["hasCallback"] = ["none", "mandatory", "optional"]
    $possible_values["deprecated"] = ["true", "false"]
    $possible_values["reverseLogic"] = ["true", "false"]
    $possible_values["rubyOnly"] = ["true", "false"]
    $possible_values["generateAccessors"] = ["true", "false"]
    $possible_values["generateUnderscoreRubyNames"] = ["true", "false"]
    $possible_values["generateUnderscoreRubyNames"] = ["true", "false"]


    def xml_error(error_desc, path)
       puts 'XML WARNING: [ '+error_desc+' ] in Element '+path
    end


    def check_xml_element_for_children_and_attributes(xml_element, parent_path)
       all_is_ok = true
       this_name = xml_element.name
       cur_path = ''
       if xml_element.attribute("name") != nil
          cur_path = parent_path + "<"+this_name+' name="'+xml_element.attribute("name").to_s+'">'
       else
          cur_path = parent_path + "<"+this_name+">"
       end

       # puts cur_path
       # check properties
       poss_attributes = $possible_attributes[this_name]
       if (xml_element.attributes.to_a.size > 0)
          if poss_attributes == nil
             all_is_ok = false
             xml_error("element should not have any attributes !", cur_path)
          else
            xml_element.attributes.to_a.each do |item_attribute|

               attribute_name = item_attribute.name.to_s
               # puts '           '+attribute_name

               # check possibility
               if !(poss_attributes.include?(attribute_name))
                   all_is_ok = false
                   xml_error( 'unrecognized attribute - "'+attribute_name+'"', cur_path)
               else
                  attribute_value = item_attribute.value.to_s
                  if $possible_values[attribute_name] != nil
                      if !($possible_values[attribute_name].include?(attribute_value))
                          all_is_ok = false
                          xml_error('unrecognized value "'+attribute_value+'" for attribute "'+attribute_name+'"', cur_path)
                      end
                  end
               end
            end
          end
       end


       # check children
       poss_children = $possible_children[this_name]
       if xml_element.elements.to_a.size > 0
          if poss_children == nil
              all_is_ok = false
              xml_error("element should not have any children !", cur_path)
          else
              xml_element.elements.to_a.each do |child_element|
                  element_name = child_element.name
                  if !(poss_children.include?(element_name))
                      all_is_ok = false
                      xml_error('unrecognized child <"'+element_name+'"> !', cur_path)
                  else
                      if !check_xml_element_for_children_and_attributes(child_element, cur_path)
                         all_is_ok = false
                      end
                  end
              end
          end
       end
       return all_is_ok
    end


    def namefixed(name)
        return name.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("")
    end

    def convertCamelToUnderscore ( s )
      res = ''
      last_char_is_big = false
      for i in 0..s.length
         if (s[i..i]).downcase != s[i..i]
            if (res.size > 0) && (res[(res.length-1)..(res.length-1)] != '_') && (!last_char_is_big)
                res = res + '_' + s[i..i].downcase
            else
                res = res + s[i..i].downcase
            end
            last_char_is_big = true
         else
            last_char_is_big = false
            res = res + s[i..i]
         end
      end
      return res
    end

    def apply_templates_to_module(xml_module_item, template_name)
      xml_f = File.new(File.join(File.dirname(__FILE__), 'templates', 'api', "xml_templates", template_name+".xml"))
      template_xml = REXML::Document.new(xml_f)
      xml_f.close

      methods_item = template_xml.elements["*/METHODS"]
      if methods_item != nil
         methods_item.elements.each("METHOD") do |method|
            method.add_attribute TEMPLATE_NAME, template_name
         end
         xml_module_item.add_element methods_item
      end
    end

    def addAlias(xml_alias, dst_collection)
      module_alias = ModuleAlias.new()

      module_alias.existing_name = xml_alias.attribute("existing").to_s
      module_alias.new_name = xml_alias.attribute("new").to_s
      if xml_alias.attribute("deprecated") != nil
          module_alias.deprecated = xml_alias.attribute("deprecated").to_s.downcase != "false"
      end
      if xml_alias.attribute("reverseLogic") != nil
          module_alias.reverseLogic = xml_alias.attribute("reverseLogic").to_s.downcase != "false"
      end
      if xml_alias.attribute("rubyOnly") != nil
          module_alias.is_ruby_only = xml_alias.attribute("rubyOnly").to_s.downcase != "false"
      end
      dst_collection << module_alias
    end

    def setup_xml
    
      require File.dirname(__FILE__) + '/templates/api/helpers/api_helper'
    
      if $xml != nil
        return true
      end
      # force changes in generated files !
      @options[:force] = true

      @@xml_filepath = xmlpath
      xml_f = File.new(@@xml_filepath)
      $xml = REXML::Document.new(xml_f)
      xml_f.close
      xml_api_root = $xml.elements["API"]

      if !check_xml_element_for_children_and_attributes(xml_api_root, '')
          if skipwarnings == false
             puts 'ERROR: XML has parsing WARNINGS - stop generation process ! Use "--skipwarnings" option when you run generator for skip parsing warnings.'
             return false
          end
      end


      xml_api_root.elements.each("MODULE") do |xml_module_item|
         module_item = ModuleItem.new()

         module_item.name = xml_module_item.attribute("name").to_s
         if xml_module_item.attribute("parent") != nil
            parents = xml_module_item.attribute("parent").to_s.split('.')
            module_item.parents = parents
         end

         if xml_module_item.attribute("generateUnderscoreRubyNames") != nil
            module_item.generateUnderscoreRubyNames = xml_module_item.attribute("generateUnderscoreRubyNames").to_s.downcase != "false"
         end

         module_item.is_template_default_instance = (xml_module_item.elements["TEMPLATES/DEFAULT_INSTANCE"] != nil)
         module_item.is_template_singletone_id = (xml_module_item.elements["TEMPLATES/SINGLETON_INSTANCES"] != nil)
         module_item.is_template_propertybag = (xml_module_item.elements["TEMPLATES/PROPERTY_BAG"] != nil)

         if module_item.is_template_propertybag
             apply_templates_to_module(xml_module_item, TEMPLATE_PROPERTY_BAG)
         end
         if module_item.is_template_singletone_id
             apply_templates_to_module(xml_module_item, TEMPLATE_SINGLETONE_INSTANCES)
         end
         if module_item.is_template_default_instance
             #apply_templates_to_module(xml_module_item, "default_instance")
         end


         #constants in module
         xml_module_item.elements.each("CONSTANTS/CONSTANT") do |xml_constant|
            module_constant = ModuleConstant.new()
            module_constant.name = xml_constant.attribute("name").to_s
            module_constant.value = xml_constant.attribute("value").to_s
            if xml_constant.attribute("type") != nil
               module_constant.type = xml_constant.attribute("type").to_s.upcase
            end
            module_item.constants << module_constant
         end

         #constants in properties
         xml_module_item.elements.each("PROPERTIES/PROPERTY/VALUES/VALUE") do |xml_property_value|
            module_constant = ModuleConstant.new()
            module_constant.name = xml_property_value.attribute("constName").to_s
            module_constant.value = xml_property_value.attribute("value").to_s
            if xml_property_value.attribute("type") != nil
               module_constant.type = xml_property_value.attribute("type").to_s.upcase
            end
            module_item.constants << module_constant
         end


         #properties
         xml_properties = xml_module_item.elements["PROPERTIES"]
         if xml_properties != nil
            if xml_properties.attribute("limitPropertyBag") != nil
              module_item.is_property_bag_limit_to_only_declared_properties = xml_properties.attribute("limitPropertyBag").to_s.downcase != "false"
            end
            
            if xml_properties.attribute("access") != nil
              if xml_properties.attribute("access").to_s.downcase == "static"
                 module_item.properties_access = ModuleMethod::ACCESS_STATIC
              end
            end
            
         end
         
         xml_module_item.elements.each("PROPERTIES/PROPERTY") do |xml_module_property|
            module_property = ModuleProperty.new()
            module_property.name = xml_module_property.attribute("name").to_s
            module_property.native_name = module_property.name.split(/[^a-zA-Z0-9\_]/).map{|w| w}.join("")
            if xml_module_property.attribute("generateAccessors") != nil
               module_property.generate_accessors =  xml_module_property.attribute("generateAccessors").to_s.downcase != "false"
            else
                if xml_properties.attribute("generateAccessors") != nil
                   module_property.generate_accessors =  xml_properties.attribute("generateAccessors").to_s.downcase != "false"
                end
            end

            if xml_module_property.attribute("type") != nil
               module_property.type = xml_module_property.attribute("type").to_s.upcase
            end
            if xml_module_property.attribute("default") != nil
               module_property.default_value = xml_module_property.attribute("default").to_s
            end
            if xml_module_property.attribute("readOnly") != nil
              module_property.readonly = xml_module_property.attribute("readOnly").to_s.downcase != "false"
            else
              if xml_properties.attribute("readOnly") != nil
                 module_property.readonly = xml_properties.attribute("readOnly").to_s.downcase != "false"
              end
            end
            if xml_module_property.attribute("usePropertyBag") != nil
               if xml_module_property.attribute("usePropertyBag").to_s.downcase == "none".downcase
                  module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_NONE
               end
               if xml_module_property.attribute("usePropertyBag").to_s.downcase == "accessorsViaPropertyBag".downcase
                  module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
               end
               if xml_module_property.attribute("usePropertyBag").to_s.downcase == "PropertyBagViaAccessors".downcase
                  module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
                  module_item.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
               end
            else
              if xml_properties.attribute("usePropertyBag") != nil
                 if xml_properties.attribute("usePropertyBag").to_s.downcase == "none".downcase
                    module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_NONE
                 end
                 if xml_properties.attribute("usePropertyBag").to_s.downcase == "accessorsViaPropertyBag".downcase
                    module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
                 end
                 if xml_properties.attribute("usePropertyBag").to_s.downcase == "PropertyBagViaAccessors".downcase
                    module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
                    module_item.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
                 end
              end
            end

            module_item.properties << module_property

         end

         #prepare getters and setters for property
        module_item.properties.each do |module_property|
           if module_property.generate_accessors
              getter_method = ModuleMethod.new()

              getter_method.name = module_property.name
              getter_method.native_name = 'get' + module_property.native_name[0..0].upcase + module_property.native_name[1..module_property.native_name.length-1]

              getter_method.params = []
              getter_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE
              getter_method.is_factory_method = false
              getter_method.is_return_value = true
              getter_method.access = module_item.properties_access
              getter_method.has_callback = ModuleMethod::CALLBACK_NONE
              getter_method.linked_property = module_property
              getter_method.special_behaviour = ModuleMethod::SPECIAL_BEHAVIOUR_GETTER
              module_property.getter = getter_method
              module_item.methods << getter_method

              if !module_property.readonly
                setter_method = ModuleMethod.new()

                setter_method.name = module_property.name + "="
                setter_method.native_name = 'set' + module_property.native_name[0..0].upcase + module_property.native_name[1..module_property.native_name.length-1]

                param = MethodParam.new()
                param.name = "value"
                param.can_be_nil = false
                param.type = module_property.type
                setter_method.params = [param]
                setter_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE
                setter_method.is_factory_method = false
                setter_method.is_return_value = false
                setter_method.access = module_item.properties_access
                setter_method.has_callback = ModuleMethod::CALLBACK_NONE
                setter_method.linked_property = module_property
                setter_method.special_behaviour = ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
                module_property.setter = setter_method

                module_item.methods << setter_method
              end
           end
        end


         #methods
         xml_methods_item = xml_module_item.elements["METHODS"]
         xml_module_item.elements.each("METHODS/METHOD") do |xml_module_method|
            module_method = ModuleMethod.new()

            module_method.name = xml_module_method.attribute("name").to_s

            module_method.native_name = module_method.name.split(/[^a-zA-Z0-9\_]/).map{|w| w}.join("")

            if xml_module_method.attribute("nativeName") != nil
               module_method.native_name = xml_module_method.attribute("nativeName").to_s
            end

            if xml_module_method.attribute(TEMPLATE_NAME) != nil
               module_method.generated_by_template = xml_module_method.attribute(TEMPLATE_NAME).to_s
            end

            if xml_module_method.attribute("access") != nil
               if xml_module_method.attribute("access").to_s.downcase == "static"
                   module_method.access = ModuleMethod::ACCESS_STATIC
               else
                   module_item.has_instance_methods = true
               end
            else
               if xml_methods_item.attribute("access") != nil
                 if xml_methods_item.attribute("access").to_s.downcase == "static"
                     module_method.access = ModuleMethod::ACCESS_STATIC
                 else
                     module_item.has_instance_methods = true
                 end
               end
            end

            if xml_module_method.attribute("deprecated") != nil
               module_method.is_deprecated = xml_module_method.attribute("deprecated").to_s.downcase != "false"
            else
              if xml_methods_item.attribute("deprecated") != nil
                 module_method.is_deprecated = xml_methods_item.attribute("deprecated").to_s.downcase != "false"
              end
            end

            if xml_module_method.attribute("factory") != nil
               if xml_module_method.attribute("factory").to_s.downcase != "false"
                   module_method.is_factory_method = true
                   module_item.has_factory_methods = true
               end
            end
            if xml_module_method.attribute("runInThread") != nil
               if xml_module_method.attribute("runInThread").to_s.downcase == "none".downcase
                   module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE
               end
               if xml_module_method.attribute("runInThread").to_s.downcase == "module".downcase
                   module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_MODULE
               end
               if xml_module_method.attribute("runInThread").to_s.downcase == "separate".downcase
                   module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_SEPARATED
               end
               if xml_module_method.attribute("runInThread").to_s.downcase == "ui".downcase
                   module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_UI
               end
            else
              if xml_methods_item.attribute("runInThread") != nil
                 if xml_methods_item.attribute("runInThread").to_s.downcase == "none".downcase
                     module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE
                 end
                 if xml_methods_item.attribute("runInThread").to_s.downcase == "module".downcase
                     module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_MODULE
                 end
                 if xml_methods_item.attribute("runInThread").to_s.downcase == "separate".downcase
                     module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_SEPARATED
                 end
                 if xml_methods_item.attribute("runInThread").to_s.downcase == "ui".downcase
                     module_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_UI
                 end
               end
            end


            if xml_module_method.attribute("hasCallback") != nil
              if xml_module_method.attribute("hasCallback").to_s.downcase == "none".downcase
                 module_method.has_callback = ModuleMethod::CALLBACK_NONE
              end
              if xml_module_method.attribute("hasCallback").to_s.downcase == "mandatory".downcase
                 module_method.has_callback = ModuleMethod::CALLBACK_MANDATORY
              end
              if xml_module_method.attribute("hasCallback").to_s.downcase == "optional".downcase
                 module_method.has_callback = ModuleMethod::CALLBACK_OPTIONAL
              end
            else
              if xml_methods_item.attribute("hasCallback") != nil
                if xml_methods_item.attribute("hasCallback").to_s.downcase == "none".downcase
                   module_method.has_callback = ModuleMethod::CALLBACK_NONE
                end
                if xml_methods_item.attribute("hasCallback").to_s.downcase == "mandatory".downcase
                   module_method.has_callback = ModuleMethod::CALLBACK_MANDATORY
                end
                if xml_methods_item.attribute("hasCallback").to_s.downcase == "optional".downcase
                   module_method.has_callback = ModuleMethod::CALLBACK_OPTIONAL
                end
              end
            end

            if xml_module_method.elements["RETURN"] != nil
                module_method.is_return_value = true
                method_result = MethodResult.new()
                result_type = xml_module_method.elements["RETURN"].attribute("type")
                result_item_type = xml_module_method.elements["RETURN"].attribute("itemType")
                if result_type != nil
                    method_result.type = result_type.to_s
                end
                if result_item_type != nil
                    method_result.item_type = result_item_type.to_s
                end
                module_method.result = method_result
            end

            param_index = 1

            xml_module_method.elements.each("PARAMS/PARAM") do |xml_method_param|
               method_param = MethodParam.new()

               if xml_method_param.attribute("name") != nil
                  method_param.name = xml_method_param.attribute("name").to_s
               else
                  method_param.name = 'param'+ param_index.to_s
               end

               if xml_method_param.attribute("type") != nil
                  method_param.type = xml_method_param.attribute("type").to_s #.upcase
               else
                  raise "parameter in method must have specified type ! Module[#{module_item.name}].method[#{module_method.name}].param_index[#{param_index.to_s}]"
               end

               method_param.can_be_nil = (xml_method_param.elements["CAN_BE_NIL"] != nil)

               param_index = param_index + 1
               module_method.params << method_param
            end


            module_item.methods << module_method
         end

         if module_item.has_instance_methods && (!module_item.has_factory_methods && !module_item.is_template_default_instance)
             raise "If module has instance methods, then module should have factory methods for produce instances ! Module[#{module_item.name}]"
         end

         xml_module_item.elements.each("PROPERTIES/ALIASES/ALIAS") do |xml_property_alias|
            addAlias(xml_property_alias, module_item.property_aliases)
         end

         xml_module_item.elements.each("METHODS/ALIASES/ALIAS") do |xml_method_alias|
           addAlias(xml_method_alias, module_item.method_aliases)
         end


         if module_item.generateUnderscoreRubyNames
            # auto generate aliases
            # setup aliases hashes

            property_aliases_hash = {}
            module_item.property_aliases.each do |property_alias|
               property_aliases_hash[property_alias.new_name] = property_alias
            end
            method_aliases_hash = {}
            module_item.property_aliases.each do |method_alias|
               method_aliases_hash[method_alias.new_name] = method_alias
            end

            # methods
            module_item.methods.each do |module_method|
               fixed_name = convertCamelToUnderscore(module_method.name)
               if (fixed_name != module_method.name) && (method_aliases_hash[fixed_name] == nil)
                  method_alias = ModuleAlias.new()
                  method_alias.existing_name = module_method.name
                  method_alias.new_name = fixed_name
                  method_alias.is_ruby_only = true
                  module_item.method_aliases << method_alias
               end
            end

            # properties
            module_item.properties.each do |module_property|
               fixed_name = convertCamelToUnderscore(module_property.name)
               if (fixed_name != module_property.name) && (property_aliases_hash[fixed_name] == nil)
                  property_alias = ModuleAlias.new()
                  property_alias.existing_name = module_property.name
                  property_alias.new_name = fixed_name
                  property_alias.is_ruby_only = true
                  module_item.property_aliases << property_alias
               end
            end


         end



         $modules << module_item
      end

      $cur_module = $modules[0]
      return true
    end


    template :shared_01 do |template|
      setup_xml
      template.source = 'shared/generated/montana_api_init.cpp'
      template.destination = "shared/generated/#{namefixed($cur_module.name)}_api_init.cpp"
    end

    template :shared_02 do |template|
      template.source = 'shared/generated/montana_ruby_api.c'
      template.destination = "shared/generated/#{namefixed($cur_module.name)}_ruby_api.c"
    end
    
    template :shared_03 do |template|
      template.source = 'shared/generated/montana_js_api.cpp'
      template.destination = "shared/generated/#{namefixed($cur_module.name)}_js_api.cpp"
    end

    template :iphone_api do |template|
      template.source = 'platform/iphone/Classes/api/IMontana.h'
      template.destination = "platform/iphone/Classes/api/I#{$cur_module.name}.h"
    end

    template :iphone_api_readme do |template|
      template.source = 'platform/iphone/Classes/api/readme.txt'
      template.destination = "platform/iphone/Classes/api/readme.txt"
    end

    template :iphone_ruby_wrapper do |template|
      template.source = 'platform/iphone/Classes/ruby_wrapper/montana_ruby_wrap.m'
      template.destination = "platform/iphone/Classes/ruby_wrapper/#{namefixed($cur_module.name)}_ruby_wrap.m"
    end

    template :iphone_ruby_wrapper_readme do |template|
      template.source = 'platform/iphone/Classes/ruby_wrapper/readme.txt'
      template.destination = "platform/iphone/Classes/ruby_wrapper/readme.txt"
    end

    template :iphone_js_wrapper do |template|
      template.source = 'platform/iphone/Classes/js_wrapper/montana_js_wrap.mm'
      template.destination = "platform/iphone/Classes/js_wrapper/#{namefixed($cur_module.name)}_js_wrap.mm"
    end

    template :iphone_js_wrapper_readme do |template|
      template.source = 'platform/iphone/Classes/js_wrapper/readme.txt'
      template.destination = "platform/iphone/Classes/js_wrapper/readme.txt"
    end

    template :iphone_impl_readme do |template|
      template.source = 'platform/iphone/Classes/impl/readme.txt'
      template.destination = "platform/iphone/Classes/impl/readme.txt"
    end

    template :iphone_base_impl_01 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaBase.h'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}Base.h"
    end

    template :iphone_base_impl_02 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaBase.m'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}Base.m"
    end

    template :iphone_base_impl_03 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaFactoryBase.h'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}FactoryBase.h"
    end

    template :iphone_base_impl_04 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaFactoryBase.m'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}FactoryBase.m"
    end

    template :iphone_base_impl_05 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaSingletonBase.h'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}SingletonBase.h"
    end

    template :iphone_base_impl_06 do |template|
      template.source = 'platform/iphone/Classes/base_impl/MontanaSingletonBase.m'
      template.destination = "platform/iphone/Classes/base_impl/#{$cur_module.name}SingletonBase.m"
    end

    template :iphone_base_impl_readme do |template|
      template.source = 'platform/iphone/Classes/base_impl/readme.txt'
      template.destination = "platform/iphone/Classes/base_impl/readme.txt"
    end

    template :iphone_stub_impl_01 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/Montana.h'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}.h"
    end

    template :iphone_stub_impl_02 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/Montana.m'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}.m"
    end

    template :iphone_stub_impl_03 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/MontanaFactorySingleton.m'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}FactorySingleton.m"
    end

    template :iphone_stub_impl_04 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/MontanaSetup.m'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}Setup.m"
    end

    template :iphone_stub_impl_05 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/MontanaSingleton.h'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}Singleton.h"
    end

    template :iphone_stub_impl_06 do |template|
      template.source = 'platform/iphone/Classes/stub_impl/MontanaSingleton.m'
      template.destination = "platform/iphone/Classes/stub_impl/#{$cur_module.name}Singleton.m"
    end

    template :iphone_stub_impl_readme do |template|
      template.source = 'platform/iphone/Classes/stub_impl/readme.txt'
      template.destination = "platform/iphone/Classes/stub_impl/readme.txt"
    end


    template :cpp_api do |template|
      template.source = 'cpp/IMontana.h'
      template.destination = "shared/generated/cpp/I#{$cur_module.name}.h"
    end

    template :cpp_api1 do |template|
      template.source = 'cpp/MontanaBase.h'
      template.destination = "shared/generated/cpp/#{$cur_module.name}Base.h"
    end

    template :cpp_api2 do |template|
      template.source = 'cpp/MontanaBase.cpp'
      template.destination = "shared/generated/cpp/#{$cur_module.name}Base.cpp"
    end

    template :cpp_api3 do |template|
      template.source = 'cpp/montana_ruby_wrap.cpp'
      template.destination = "shared/generated/cpp/#{$cur_module.name}_ruby_wrap.cpp"
    end

    template :cpp_api4 do |template|
      template.source = 'cpp/montana_js_wrap.cpp'
      template.destination = "shared/generated/cpp/#{$cur_module.name}_js_wrap.cpp"
    end

    template :android_java_obj_interface do |template|
      template.source = 'platform/android/gen/src/IMontana.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/I#{$cur_module.name}.java"
    end

    template :android_java_factory_interface do |template|
      template.source = 'platform/android/gen/src/IMontanaFactory.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/I#{$cur_module.name}Factory.java"
    end

    template :android_java_singleton_interface do |template|
      template.source = 'platform/android/gen/src/IMontanaSingleton.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/I#{$cur_module.name}Singleton.java"
    end

    template :android_java_obj_base do |template|
      template.source = 'platform/android/gen/src/MontanaBase.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/#{$cur_module.name}Base.java"
    end

    template :android_java_singleton_base do |template|
      template.source = 'platform/android/gen/src/MontanaSingletonBase.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/#{$cur_module.name}SingletonBase.java"
    end

    template :android_java_factory_singleton do |template|
      template.source = 'platform/android/gen/src/MontanaFactorySingleton.java'
      template.destination = "platform/android/generated/src/#{api_generator_java_makePackagePath($cur_module)}/#{$cur_module.name}FactorySingleton.java"
    end

    template :android_cpp_api_header do |template|
      template.source = 'platform/android/gen/jni/Montana.h'
      template.destination = "platform/android/generated/jni/#{$cur_module.name}.h"
    end

    template :android_cpp_api_impl do |template|
      template.source = 'platform/android/gen/jni/Montana.cpp'
      template.destination = "platform/android/generated/jni/#{$cur_module.name}.cpp"
    end

    template :android_cpp_ruby_wrapper do |template|
      template.source = 'platform/android/gen/jni/montana_ruby_wrap.cpp'
      template.destination = "platform/android/generated/jni/#{$cur_module.name.downcase}_ruby_wrap.cpp"
    end

    template :android_cpp_js_wrapper do |template|
      template.source = 'platform/android/gen/jni/montana_js_wrap.cpp'
      template.destination = "platform/android/generated/jni/#{$cur_module.name.downcase}_js_wrap.cpp"
    end

    def attributes?
      self.attributes && !self.attributes.empty?
    end

  end

class ApiMegatestGenerator < BaseGenerator


   def self.source_root
     File.join(File.dirname(__FILE__), 'templates', 'api_test')
   end

   desc <<-DESC
     Generate megatest XML for API generator
   DESC

   #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

   #first_argument :xmlpath, :required => true, :desc => "path to XML with API description"


   template :megatest_01 do |template|
     template.source = 'taho.xml'
     template.destination = "Megatest.xml"
   end


   def attributes?
     self.attributes && !self.attributes.empty?
   end

 end



 add :app, AppGenerator
 add :model, ModelGenerator
 add :spec, SpecGenerator
 add :extension, ExtensionGenerator
 add :api, ApiGenerator
 add :api_test, ApiMegatestGenerator

 end






=begin
# Stub this method to force 1.8 compatibility (come on templater!)
class Encoding
  def find
    "utf-8"
  end

  def dummy?
    false
  end
end

class String
  def force_encoding(enc)
    return self
  end
  def encoding
    if RUBY_VERSION =~ /1\.8/ and Encoding.responds_to?('new')
      Encoding.new
    else
      Encoding.default_external
    end
  end
end


Rhogen.run_cli(Dir.pwd, 'rhodes', Rhodes::VERSION, ARGV)

=end
