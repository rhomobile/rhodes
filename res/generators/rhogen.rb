require 'rubygems'
require 'templater'
require 'thread'
#TODO: This is temporary, see https://www.pivotaltracker.com/story/show/3399292
gem "activesupport", "~> 2.3.5"
require 'active_support'
require 'uuid'
require 'yaml'
require 'rexml/document'

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

    template :bb_index do |template|
      template.source = 'app/index.bb.erb'
      template.destination = "#{name}/app/index.bb.erb"
    end   

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

    
    directory :settings do |directory|
      directory.source = 'app/Settings'
      directory.destination = "#{name}/app/Settings/"
    end

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

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'extension')
    end

    desc <<-DESC
      Adds native extension to Rhodes based application.
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "extension name"


    def namefixed
        return name.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("")
    end    

    template :ext do |template|
      template.source = 'extensions/montana/ext.yml'
      template.destination = "extensions/#{namefixed.downcase}/ext.yml"
    end

    template :extension_ruby do |template|
      template.source = 'extensions/montana/montana.rb'
      template.destination = "extensions/#{namefixed.downcase}/#{namefixed.downcase}.rb"
    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
        # change atribbutes in build script file to executable
        File.chmod(0755, $build_script_full_path) 
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

    template :extension_i do |template|
      template.source = 'extensions/montana/ext/shared/ruby/montana.i'
      template.destination = "extensions/#{namefixed.downcase}/ext/shared/ruby/#{namefixed.downcase}.i"
    end

    template :extension_wrap do |template|
      template.source = 'extensions/montana/ext/shared/ruby/montana_wrap.c'
      template.destination = "extensions/#{namefixed.downcase}/ext/shared/ruby/#{namefixed.downcase}_wrap.c"
    end

    template :extension_c do |template|
      template.source = 'extensions/montana/ext/shared/src/montana.c'
      template.destination = "extensions/#{namefixed.downcase}/ext/shared/src/#{namefixed.downcase}.c"
    end

    template :extension_iphone_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Rakefile'
      template.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/Rakefile"
    end

    file :extension_iphone_pch do |file|
      file.source = 'extensions/montana/ext/platform/iphone/Montana_Prefix.pch'
      file.destination = "extensions/#{namefixed.downcase}/ext/platform/iphone/#{namefixed.camel_case}_Prefix.pch"
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

    template :extension_wm_h do |template|
        template.source = 'extensions/montana/ext/platform/wm/src/montana_wm.h'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/src/#{namefixed.downcase}_wm.h"
    end

    template :extension_wm_cpp do |template|
        template.source = 'extensions/montana/ext/platform/wm/src/montana_wm.cpp'
        template.destination = "extensions/#{namefixed.downcase}/ext/platform/wm/src/#{namefixed.downcase}_wm.cpp"
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

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'api')
    end

    desc <<-DESC
      Generate API sources from XML API file
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :xmlpath, :required => true, :desc => "path to XML with API description"


    $xml = nil
    $name = nil
    $xml_class = nil
    $xml_class_attributes_singletone_id = false
    $xml_class_attributes_default_instance = false
    $xml_class_attributes_propertybag = false

    # array of arrays

    class MethodArgument

      TYPE_STRING = "STRING"
      TYPE_ARRAY = "ARRAY"
      TYPE_HASH = "HASH"
      TYPE_BASE_OBJECT = "OBJECT" #if argument can be a different types - in this case we use base class and developer should recognize class byself

      @name = ''
      @type = TYPE_BASE_OBJECT
    end

    class ModuleMethod
      @name = ''
      @arguments = [] # array of MethodArgument
      @is_separated_thread = false
      @is_fabric_method = false
      @is_run_in_ui_thread = false
      @iphone_platform_specific_method_define_line = nil
      @android_platform_specific_method_define_line = nil
    end

    class ModuleItem
      @name = ''
      @parent = ''
      @methods = []
      @is_template_singletone_id = false
      @is_template_default_instance = false
      @is_template_propertybag = false
    end

    $modules = []


    def setup_xml
      if $xml != nil
        return
      end
      # force changes in generated files !
      @options[:force] = true

      xml_filepath = xmlpath
      xml_f = File.new(xml_filepath)
      $xml = REXML::Document.new(xml_f)
      xml_f.close
      $xml_class = $xml.elements["API/CLASS"]
      $name = $xml_class.attribute('name').to_s

      #enumerate attributes
      $xml.elements.each("*/CLASS/ATTRIBUTES/ATTRIBUTE") do |e|
        name = e.attribute("name").to_s
        if name == "SINGLETON_INSTANCES"
          if e.attribute("value") != nil
            if e.attribute("value").to_s.downcase != "false"
              $xml_class_attributes_singletone_id = true
            end
          end
        end
        if name == "DEFAULT_INSTANCE"
          if e.attribute("value") != nil
            if e.attribute("value").to_s.downcase != "false"
              $xml_class_attributes_default_instance = true
            end
          end
        end
        if name == "PROPERTY_BAG"
          if e.attribute("value") != nil
            if e.attribute("value").to_s.downcase != "false"
              $xml_class_attributes_propertybag = true
            end
          end
        end
      end

      #analyze methods
    end


    def namefixed
        setup_xml
        return $name.downcase.split(/[^a-zA-Z0-9]/).map{|w| w.downcase}.join("")
    end

    template :shared_01 do |template|
      template.source = 'montana/shared/generated/montana_api_init.cpp'
      template.destination = "#{namefixed.downcase}/shared/generated/#{namefixed.downcase}_api_init.cpp"
    end

    template :shared_02 do |template|
      template.source = 'montana/shared/generated/montana_ruby_api.c'
      template.destination = "#{namefixed.downcase}/shared/generated/#{namefixed.downcase}_ruby_api.c"
    end

    template :iphone_api do |template|
      template.source = 'montana/platform/iphone/Classes/api/IMontana.h'
      template.destination = "#{namefixed.downcase}/platform/iphone/Classes/api/I#{$name}.h"
    end

    template :iphone_api_readme do |template|
      template.source = 'montana/platform/iphone/Classes/api/readme.txt'
      template.destination = "#{namefixed.downcase}/platform/iphone/Classes/api/readme.txt"
    end

    template :iphone_ruby_wrapper do |template|
      template.source = 'montana/platform/iphone/Classes/ruby_wrapper/montana_ruby_wrap.m'
      template.destination = "#{namefixed.downcase}/platform/iphone/Classes/ruby_wrapper/#{namefixed.downcase}_ruby_wrap.m"
    end

    template :iphone_ruby_wrapper_readme do |template|
      template.source = 'montana/platform/iphone/Classes/ruby_wrapper/readme.txt'
      template.destination = "#{namefixed.downcase}/platform/iphone/Classes/ruby_wrapper/readme.txt"
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

end

