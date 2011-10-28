require 'rubygems'
require 'templater'
require 'thread'
#TODO: This is temporary, see https://www.pivotaltracker.com/story/show/3399292
gem "activesupport", "~> 2.3.5"
require 'active_support'
require 'uuid'
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
      
      Required:
        name        - application name
        
      Optional:
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292/application")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

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

    template :ext do |template|
      template.source = 'extensions/montana/ext.yml'
      template.destination = "extensions/#{name.downcase}/ext.yml"
    end

    template :extension_ruby do |template|
      template.source = 'extensions/montana/montana.rb'
      template.destination = "extensions/#{name.downcase}/#{name.downcase}.rb"
    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
        # change atribbutes in build script file to executable
        File.chmod(0755, $build_script_full_path) 
    end    

    template :build do |template|
      template.source = 'extensions/montana/ext/build'
      template.destination = "extensions/#{name.downcase}/ext/build"
      $build_script_full_path = template.destination   
      template.options = { :after => :callback_after_make_build}  
    end

    template :build_bat do |template|
      template.source = 'extensions/montana/ext/build.bat'
      template.destination = "extensions/#{name.downcase}/ext/build.bat"
    end

    template :extension_i do |template|
      template.source = 'extensions/montana/ext/montana/shared/ruby/montana.i'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/shared/ruby/#{name.downcase}.i"
    end

    template :extension_wrap do |template|
      template.source = 'extensions/montana/ext/montana/shared/ruby/montana_wrap.c'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/shared/ruby/#{name.downcase}_wrap.c"
    end

    template :extension_c do |template|
      template.source = 'extensions/montana/ext/montana/shared/src/montana.c'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/shared/src/#{name.downcase}.c"
    end

    template :extension_iphone_rakefile do |template|
      template.source = 'extensions/montana/ext/montana/platform/iphone/Rakefile'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/iphone/Rakefile"
    end

    file :extension_iphone_pch do |file|
      file.source = 'extensions/montana/ext/montana/platform/iphone/Montana_Prefix.pch'
      file.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/iphone/#{name.camel_case}_Prefix.pch"
    end

    template :extension_iphone_project do |template|
      template.source = 'extensions/montana/ext/montana/platform/iphone/Montana.xcodeproj/project.pbxproj'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/iphone/#{name.camel_case}.xcodeproj/project.pbxproj"
    end

    template :extension_iphone_src_h do |template|
      template.source = 'extensions/montana/ext/montana/platform/iphone/Classes/Montana.h'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/iphone/Classes/#{name.camel_case}.h"
    end

    template :extension_iphone_src_m do |template|
      template.source = 'extensions/montana/ext/montana/platform/iphone/Classes/Montana.m'
      template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/iphone/Classes/#{name.camel_case}.m"
    end

    template :extension_android_ext_build do |template|
        template.source = 'extensions/montana/ext/montana/platform/android/ext_build.files'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/android/ext_build.files"
    end

    template :extension_android_rakefile do |template|
        template.source = 'extensions/montana/ext/montana/platform/android/Rakefile'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/android/Rakefile"
    end

    template :extension_android_cpp do |template|
        template.source = 'extensions/montana/ext/montana/platform/android/jni/src/montana.cpp'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/android/jni/src/#{name.downcase}.cpp"
    end

    template :extension_android_java do |template|
        template.source = 'extensions/montana/ext/montana/platform/android/src/com/montana/Montana.java'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/android/src/com/#{name.downcase}/#{name.camel_case}.java"
    end


    template :extension_wm_rakefile do |template|
        template.source = 'extensions/montana/ext/montana/platform/wm/Rakefile'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/wm/Rakefile"
    end

    template :extension_wm_vcproject do |template|
        template.source = 'extensions/montana/ext/montana/platform/wm/Montana.vcproj'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/wm/#{name.camel_case}.vcproj"
    end

    template :extension_wm_h do |template|
        template.source = 'extensions/montana/ext/montana/platform/wm/src/montana_wm.h'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/wm/src/#{name.downcase}_wm.h"
    end

    template :extension_wm_cpp do |template|
        template.source = 'extensions/montana/ext/montana/platform/wm/src/montana_wm.cpp'
        template.destination = "extensions/#{name.downcase}/ext/#{name.downcase}/platform/wm/src/#{name.downcase}_wm.cpp"
    end



    template :extension_test_controller do |template|
      template.source = 'app/MontanaTest/controller.rb'
      template.destination = "app/#{name.camel_case}Test/controller.rb"
    end

    template :extension_test_index do |template|
      template.source = 'app/MontanaTest/index.erb'
      template.destination = "app/#{name.camel_case}Test/index.erb"
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




  add :app, AppGenerator
  add :model, ModelGenerator
  add :spec, SpecGenerator
  add :extension, ExtensionGenerator

end
