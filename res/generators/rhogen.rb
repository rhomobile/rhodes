require 'rubygems'
require 'templater'
require 'thread'
#TODO: This is temporary, see https://www.pivotaltracker.com/story/show/3399292
gem 'activesupport', '~> 2.3.5'
require 'active_support'
require 'uuid'
require 'yaml'
require 'rexml/document'
require 'fileutils'
require File.join(File.dirname(__FILE__), '/../../lib/build/jake.rb')

require File.dirname(__FILE__) + '/../../lib/rhodes'

# =================================================================
# helper utilities

class Array
  def stable_sort
    n = -1
    if block_given?
      collect { |x| n += 1; [x, n]
      }.sort! { |a, b|
        c = yield a[0], b[0]
        if c.nonzero? then
          c
        else
          a[1] <=> b[1]
        end
      }.collect! { |x| x[0] }
    else
      sort_by { |x| n += 1; [x, n] }
    end
  end

  def stable_sort_by
    block_given? or return enum_for(:stable_sort_by)
    n = -1
    sort_by { |x| n += 1; [(yield x), n] }
  end
end

class String
  def capitalize_first
    self.slice(0, 1).capitalize + self.slice(1..-1)
  end

  def underscore
    self.gsub(/::/, '/').
        gsub(/([A-Z]+)([A-Z][a-z])/,'\1_\2').
        gsub(/([a-z\d])([A-Z])/,'\1_\2').
        tr('- ', '_').
        downcase
  end

  def black;          "\033[30m#{self}\033[0m" end
  def red;            "\033[31m#{self}\033[0m" end
  def green;          "\033[32m#{self}\033[0m" end
  def brown;          "\033[33m#{self}\033[0m" end
  def blue;           "\033[34m#{self}\033[0m" end
  def magenta;        "\033[35m#{self}\033[0m" end
  def cyan;           "\033[36m#{self}\033[0m" end
  def gray;           "\033[37m#{self}\033[0m" end
  def bg_black;       "\033[40m#{self}\0330m"  end
  def bg_red;         "\033[41m#{self}\033[0m" end
  def bg_green;       "\033[42m#{self}\033[0m" end
  def bg_brown;       "\033[43m#{self}\033[0m" end
  def bg_blue;        "\033[44m#{self}\033[0m" end
  def bg_magenta;     "\033[45m#{self}\033[0m" end
  def bg_cyan;        "\033[46m#{self}\033[0m" end
  def bg_gray;        "\033[47m#{self}\033[0m" end
  def bold;           "\033[1m#{self}\033[22m" end
  def underline;      "\033[4m#{self}\033[24m" end
  def reverse_color;  "\033[7m#{self}\033[27m" end

end

# =================================================================
# Generator

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
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :norhoconnect, :desc => '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => 'application name'
    second_argument :syncserver, :required => false, :desc => 'url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/")'
    third_argument :zip_url, :required => false, :desc => 'optional url to zipfile download of bundle'

    template :config do |template|
      zip_url ||= ''
      syncserver ||= ''
      template.source = 'rhoconfig.txt'
      template.destination = "#{name}/rhoconfig.txt"
    end

    template :buildyml do |template|
      @sdk_path = File.expand_path(File.join(File.dirname(__FILE__), '..', '..'))
      @sdk_path.gsub!('\\', '/')
      @app_name = name
      @app_name_cleared = name.downcase.split(/[^a-zA-Z0-9\.\-]/).map { |w| w.downcase }.join("")
      puuid = UUID.new
      generated_uuid = puuid.generate
      @productid = generated_uuid
      @uid = '0x'+(0xE0000000 + rand(0xFFFFFFF)).to_s(16)
      @rhoconnectclient_ext = '"rhoconnect-client"' unless norhoconnect
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
    
    file :androidmanifesterb do |file|
      file.source = 'AndroidManifest.erb'
      file.destination = "#{name}/AndroidManifest.erb"
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

    first_argument :name, :required => true, :desc => 'model name'
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
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
      template.source = 'controller.rb'
      template.destination = "app/#{name.camel_case}/#{underscore_name}_controller.rb"
    end

    template :model do |template|
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
      template.source = 'model.rb'
      template.destination = "app/#{name.camel_case}/#{underscore_name}.rb"
    end

    template :spec do |template|
      underscore_name = name.camel_case.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
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
      directory.destination = 'app/SpecRunner'
    end
    #directory :mspec do |directory|
    #  directory.source = 'app/mspec'
    #  directory.destination = 'app/mspec'
    #end
    #directory :spec do |directory|
    #  directory.source = 'app/spec'
    #  directory.destination = 'app/spec'
    #end
    #template :fileutils do |template|
    #  template.source = 'app/fileutils.rb'
    #  template.destination = 'app/fileutils.rb'
    #end
    template :mspecrb do |template|
      template.source = 'app/mspec.rb'
      template.destination = 'app/mspec.rb'
    end
    template :specrunnerrb do |template|
      template.source = 'app/spec_runner.rb'
      template.destination = 'app/spec_runner.rb'
    end

  end




  class IphoneProjectGenerator < BaseGenerator

    @@noapp = false

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'iphone_project')
    end

    desc <<-DESC
      Adds XCode application project to Rhodes based application.
    DESC

    first_argument :name, :required => true, :desc => 'application name'
    second_argument :rhodes_root, :required => true, :desc => 'path to rhodes'

    def namefixed
      return name.downcase.split(/[^a-zA-Z0-9]/).map { |w| w.downcase }.join("")
    end

    def namecamelcase
      return name.split(/[^a-zA-Z0-9]/).map { |w| (w.capitalize_first) }.join("")
    end

    def rhodes_root_path
      return rhodes_root
    end

    directory :root do |directory|
      @options[:force] = true
      directory.source = 'root'
      directory.destination = 'project/iphone'
      if File.exists?(directory.destination)
        directory.destination = 'project/iphone/toremoved'
      end
    end

    directory :classes do |directory|
      #@options[:force] = true
      directory.source = 'Classes'
      directory.destination = 'project/iphone/Classes'
      if File.exists?(directory.destination)
        directory.destination = 'project/iphone/toremoved'
      end
    end

    directory :resources do |directory|
      #@options[:force] = true
      directory.source = 'Resources'
      directory.destination = 'project/iphone/Resources'
      if File.exists?(directory.destination)
        directory.destination = 'project/iphone/toremoved'
      end
    end

    #directory :settings do |directory|
    #  #@options[:force] = true
    #  directory.source = 'Settings.bundle'
    #  directory.destination = 'project/iphone/Settings.bundle'
    #  if File.exists?(directory.destination)
    #    directory.destination = 'project/iphone/toremoved'
    #  end
    #end

    template :project do |template|
      #@options[:force] = true
      #@options[:skip] = true
      template.source = 'Bremen.xcodeproj/project.pbxproj'
      template.destination = "project/iphone/#{namecamelcase}.xcodeproj/project.pbxproj"
      if File.exists?(template.destination)
        #puts '$$$$$$$$$$$$$$$$ EXIST'+template.destination
        template.destination = 'project/iphone/toremovef'
      else
        #puts '$$$$$$$$$$$$$$$$ NOT EXIST'+template.destination
      end
    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
      # change atribbutes in build script file to executable
      File.chmod(0755, $build_script_full_path)
    end


    template :build do |template|
      @options[:force] = true
      template.source = 'buildRhoBundle'
      template.destination = "project/iphone/buildRhoBundle"
      $build_script_full_path = template.destination
      template.options = {:after => :callback_after_make_build}
    end

    template :rhodes_project do |template|
      @options[:force] = true
      template.source = 'Rhodes/Rhodes.xcodeproj/project.pbxproj'
      template.destination = "project/iphone/Rhodes/Rhodes.xcodeproj/project.pbxproj"
    end

    template :rhodes_project_01 do |template|
      template.options[:force] = true
      template.source = 'Rhodes/Rhodes/Rhodes-Prefix.pch'
      template.destination = "project/iphone/Rhodes/Rhodes/Rhodes-Prefix.pch"
    end

    template :rhodes_project_02 do |template|
      template.options[:force] = true
      template.source = 'Rhodes/RhodesBaseDelegate.h'
      template.destination = "project/iphone/Rhodes/RhodesBaseDelegate.h"
    end

    template :rhodes_project_03 do |template|
      template.options[:force] = true
      template.source = 'Rhodes/RhodesBaseDelegate.m'
      template.destination = "project/iphone/Rhodes/RhodesBaseDelegate.m"
    end


    def attributes?
      self.attributes && !self.attributes.empty?
    end



  end

  class IphoneProjectPrebuildGenerator < BaseGenerator

    @@noapp = false

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'iphone_project')
    end

    desc <<-DESC
      Adds XCode application project with using of prebuild data to Rhodes based application.
    DESC

    first_argument :name, :required => true, :desc => 'application name'
    second_argument :rhodes_root, :required => true, :desc => 'path to rhodes'

    def namefixed
      return name.downcase.split(/[^a-zA-Z0-9]/).map { |w| w.downcase }.join("")
    end

    def namecamelcase
      return name.split(/[^a-zA-Z0-9]/).map { |w| (w.capitalize_first) }.join("")
    end

    def rhodes_root_path
      return rhodes_root
    end

    directory :root do |directory|
      @options[:force] = true
      directory.source = 'root'
      directory.destination = 'project/iphone'
      if File.exists?(directory.destination)
        #directory.options[:skip] = 'project/iphone/toremoved'
        directory.destination = 'project/iphone/toremoved'
      end
    end

    directory :classes do |directory|
      #@options[:force] = true
      directory.source = 'Classes'
      directory.destination = 'project/iphone/Classes'
      if File.exists?(directory.destination)
        directory.destination = 'project/iphone/toremoved'
      end
    end

    directory :resources do |directory|
      #@options[:force] = true
      directory.source = 'Resources'
      directory.destination = 'project/iphone/Resources'
      if File.exists?(directory.destination)
        directory.destination = 'project/iphone/toremoved'
      end
    end

    #directory :settings do |directory|
    #  #@options[:force] = true
    #  directory.source = 'Settings.bundle'
    #  directory.destination = 'project/iphone/Settings.bundle'
    #  if File.exists?(directory.destination)
    #    directory.destination = 'project/iphone/toremoved'
    #  end
    #end

    template :project do |template|
      #@options[:force] = true
      #@options[:skip] = true
      template.source = 'Bremen_prebuild.xcodeproj/project.pbxproj'
      template.destination = "project/iphone/#{namecamelcase}.xcodeproj/project.pbxproj"
      if File.exists?(template.destination)
        template.destination = 'project/iphone/toremovef'
      end
    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
      # change atribbutes in build script file to executable
      File.chmod(0755, $build_script_full_path)
    end


    template :build do |template|
      @options[:force] = true
      template.source = 'buildRhoBundle'
      template.destination = "project/iphone/buildRhoBundle"
      $build_script_full_path = template.destination
      template.options = {:after => :callback_after_make_build}
    end

    template :rhodes_project_02 do |template|
      template.options[:force] = true
      template.source = 'Rhodes/RhodesBaseDelegate.h'
      template.destination = "project/iphone/Rhodes/RhodesBaseDelegate.h"
    end

    template :rhodes_project_03 do |template|
      template.options[:force] = true
      template.source = 'Rhodes/RhodesBaseDelegate.m'
      template.destination = "project/iphone/Rhodes/RhodesBaseDelegate.m"
    end


    def attributes?
      self.attributes && !self.attributes.empty?
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
    option :noapp, :desc => '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => 'extension name'


    def namefixed
      return name.downcase.split(/[^a-zA-Z0-9]/).map { |w| w.downcase }.join("")
    end

    def namecamelcase
      return name.split(/[^a-zA-Z0-9]/).map { |w| (w.capitalize_first) }.join("")
    end

    template :ext do |template|
      template.source = 'extensions/montana/ext.yml'
      template.destination = "extensions/#{name}/ext.yml"
      puuid = UUID.new
      generated_uuid = puuid.generate
      @productid = generated_uuid
      @wp8LibGUID = puuid.generate
      @wp8StubImplGUID = puuid.generate
      @@noapp = noapp
    end

#    template :extension_ruby do |template|
#      template.source = 'extensions/montana/montana.rb'
#      template.destination = "extensions/#{name}/#{name}.rb"
#    end

    $build_script_full_path = ''

    def callback_after_make_build(template)
      # change atribbutes in build script file to executable
      File.chmod(0755, $build_script_full_path)
      Dir.chdir("extensions/#{name}/ext")
      args = []
      args << 'api'
      args << Dir.pwd+"/#{namecamelcase}.xml"
      Jake.run(source_root+'/../../../../bin/rhogen', args)
    end

    def callback_after_delete_testapp(template)
      if @@noapp == true
        Dir.chdir('../../../')
        FileUtils.rm_rf 'app'
        FileUtils.cp_r("extensions/#{name}", Dir.pwd)
        FileUtils.rm_rf 'extensions'
      end
    end

    template :extension_apigen_xml do |template|
      template.source = 'extensions/montana/ext/montana.xml'
      template.destination = "extensions/#{name}/ext/#{namecamelcase}.xml"
    end

    template :build do |template|
      template.source = 'extensions/montana/ext/build'
      template.destination = "extensions/#{name}/ext/build"
      $build_script_full_path = template.destination
      template.options = {:after => :callback_after_make_build}
    end

    template :build_bat do |template|
      template.source = 'extensions/montana/ext/build.bat'
      template.destination = "extensions/#{name}/ext/build.bat"
    end

    template :extension_iphone_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Rakefile'
      template.destination = "extensions/#{name}/ext/platform/iphone/Rakefile"
    end

    template :extension_iphone_pch do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Montana_Prefix.pch'
      template.destination = "extensions/#{name}/ext/platform/iphone/#{namecamelcase}_Prefix.pch"
    end

    template :extension_iphone_project do |template|
      template.source = 'extensions/montana/ext/platform/iphone/Montana.xcodeproj/project.pbxproj'
      template.destination = "extensions/#{name}/ext/platform/iphone/#{namecamelcase}.xcodeproj/project.pbxproj"
    end

    template :extension_iphone_src_1 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/Montana.h'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}.h"
    end

    template :extension_iphone_src_2 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/Montana.m'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}.m"
    end

    template :extension_iphone_src_3 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/MontanaFactorySingleton.m'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}FactorySingleton.m"
    end

    template :extension_iphone_src_4 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/MontanaSetup.m'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}Setup.m"
    end

    template :extension_iphone_src_5 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/MontanaSingleton.h'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}Singleton.h"
    end

    template :extension_iphone_src_6 do |template|
      template.source = 'extensions/montana/ext/platform/iphone/impl/MontanaSingleton.m'
      template.destination = "extensions/#{name}/ext/platform/iphone/impl/#{namecamelcase}Singleton.m"
    end

    template :extension_android_ext_java do |template|
      template.source = 'extensions/montana/ext/platform/android/ext_java.files'
      template.destination = "extensions/#{name}/ext/platform/android/ext_java.files"
    end

    template :extension_android_ext_native do |template|
      template.source = 'extensions/montana/ext/platform/android/ext_native.files'
      template.destination = "extensions/#{name}/ext/platform/android/ext_native.files"
    end

    template :extension_android_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/android/Rakefile'
      template.destination = "extensions/#{name}/ext/platform/android/Rakefile"
    end

    template :extension_android_cpp do |template|
      template.source = 'extensions/montana/ext/platform/android/jni/Montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/android/jni/#{namecamelcase}_impl.cpp"
    end

    template :extension_android_module_module_java do |template|
      template.source = 'extensions/montana/ext/platform/android/src/Montana.java'
      template.destination = "extensions/#{name}/ext/platform/android/src/com/rho/#{namefixed}/#{namecamelcase}.java"
    end

    template :extension_android_module_singleton_java do |template|
      template.source = 'extensions/montana/ext/platform/android/src/MontanaSingleton.java'
      template.destination = "extensions/#{name}/ext/platform/android/src/com/rho/#{namefixed}/#{namecamelcase}Singleton.java"
    end

    template :extension_android_module_module_java do |template|
      template.source = 'extensions/montana/ext/platform/android/src/MontanaFactory.java'
      template.destination = "extensions/#{name}/ext/platform/android/src/com/rho/#{namefixed}/#{namecamelcase}Factory.java"
    end

    #template :extension_wm_rakefile do |template|
    #    template.source = 'extensions/montana/ext/platform/wm/Rakefile'
    #    template.destination = "extensions/#{name}/ext/platform/wm/Rakefile"
    #end

    template :extension_wm_vcsol do |template|
      template.source = 'extensions/montana/ext/platform/wm/Montana.sln'
      template.destination = "extensions/#{name}/ext/platform/wm/#{namecamelcase}.sln"
    end

    template :extension_wm_vcproject do |template|
      template.source = 'extensions/montana/ext/platform/wm/Montana.vcproj'
      template.destination = "extensions/#{name}/ext/platform/wm/#{namecamelcase}.vcproj"
    end

    template :extension_wm_vcprojectprops do |template|
      template.source = 'extensions/montana/ext/platform/wm/montana.vsprops'
      template.destination = "extensions/#{name}/ext/platform/wm/#{namecamelcase}.vsprops"
    end

    template :extension_wm_impl_cpp do |template|
      template.source = 'extensions/montana/ext/platform/wm/src/montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/wm/src/#{namecamelcase}_impl.cpp"
    end

    template :extension_wp8_vcproject do |template|
      template.source = 'extensions/montana/ext/platform/wp8/Montana.vcxproj'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}.vcxproj"
    end

    template :extension_wp8_vcprojectprops do |template|
      template.source = 'extensions/montana/ext/platform/wp8/montana.props'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}.props"
    end

    template :extension_wp8_vcprojectfilters do |template|
      template.source = 'extensions/montana/ext/platform/wp8/montana.vcxproj.filters'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}.vcxproj.filters"
    end

    template :extension_wp8_impl_cpp do |template|
      template.source = 'extensions/montana/ext/platform/wp8/src/montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/wp8/src/#{namecamelcase}_impl.cpp"
    end

    # wp8: c++ -> c# bridging

    template :extension_wp8_impl_targets do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaImpl.targets'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Impl.targets"
    end

    template :extension_wp8_impl_csproj do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaImpl.csproj'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Impl.csproj"
    end

    template :extension_wp8_impl_props do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaImpl.props'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Impl.props"
    end

    template :extension_wp8_impl_cs do |template|
      template.source = 'extensions/montana/ext/platform/wp8/src/Montana_impl.cs'
      template.destination = "extensions/#{name}/ext/platform/wp8/src/#{namecamelcase}_impl.cs"
    end

    template :extension_wp8_impl_assemblyinfo do |template|
      template.source = 'extensions/montana/ext/platform/wp8/src/Properties/AssemblyInfo.cs'
      template.destination = "extensions/#{name}/ext/platform/wp8/src/Properties/AssemblyInfo.cs"
    end

    template :extension_wp8_lib_vcxproj do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaLib.vcxproj'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Lib.vcxproj"
    end

    template :extension_wp8_lib_vcxproj_filters do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaLib.vcxproj.filters'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Lib.vcxproj.filters"
    end

    template :extension_wp8_lib_props do |template|
      template.source = 'extensions/montana/ext/platform/wp8/MontanaLib.props'
      template.destination = "extensions/#{name}/ext/platform/wp8/#{namecamelcase}Lib.props"
    end


    template :extension_qt_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/qt/Rakefile'
      template.destination = "extensions/#{name}/ext/platform/qt/Rakefile"
    end

    template :extension_qt_pro do |template|
      template.source = 'extensions/montana/ext/platform/qt/Montana.pro'
      template.destination = "extensions/#{name}/ext/platform/qt/#{namecamelcase}.pro"
    end

    template :extension_qt_src_impl do |template|
      template.source = 'extensions/montana/ext/platform/qt/src/Montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/qt/src/#{namecamelcase}_impl.cpp"
    end


    template :extension_bb_files do |template|
      template.source = 'extensions/montana/ext/platform/bb/Montana.files'
      template.destination = "extensions/#{name}/ext/platform/bb/#{namecamelcase}.files"
    end

    template :extension_bb_jgp do |template|
      template.source = 'extensions/montana/ext/platform/bb/montana.jdp'
      template.destination = "extensions/#{name}/ext/platform/bb/#{namefixed.downcase}.jdp"
    end

    template :extension_bb_Rakefile do |template|
      template.source = 'extensions/montana/ext/platform/bb/Rakefile'
      template.destination = "extensions/#{name}/ext/platform/bb/Rakefile"
    end

    template :extension_bb_java do |template|
      template.source = 'extensions/montana/ext/platform/bb/src/com/montana/Montana.java'
      template.destination = "extensions/#{name}/ext/platform/bb/src/com/#{namefixed}/#{namecamelcase}.java"
    end

    template :extension_test_controller do |template|
      template.source = 'app/MontanaTest/controller.rb'
      template.destination = "app/#{namecamelcase}Test/controller.rb"
    end

    template :extension_test_index do |template|
      template.source = 'app/MontanaTest/index.erb'
      template.destination = "app/#{namecamelcase}Test/index.erb"
      template.options = {:after => :callback_after_delete_testapp}
    end


    #<%= name.downcase %>
    #<%= name.camel_case %>

    #template :spec do |template|
    #  underscore_name = name.camel_case.split(/(?=[A-Z])/).map{|w| w.downcase}.join('_')
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

  # =================================================================
  # ApiGenerator

  class ApiGenerator < BaseGenerator

    Templater::CLI::Generator.class_eval do

      def old_step_through_templates
        @generator.all_actions.each do |action|
          begin
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
          rescue 
            puts "Error processing: #{action.source.inspect}".bold.red
            raise
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
        File.dirname(@@xml_filepath).gsub('\\', '/')
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

    option :skipwarnings, :desc => 'skip any XML warnings, by default - generation break by any XML parse warning', :as => :boolean, :default => true
    first_argument :xmlpath, :required => true, :desc => 'path to XML with API description'

    $xml = nil

    # array of arrays

    class ModuleAlias

      def initialize
        @existing_name = ''
        @new_name = ''
        @is_ruby_only = false
        @deprecated = false
        @reverseLogic = false
        @is_method_static = false
        @is_method_instance = false
      end

      attr_accessor :existing_name
      attr_accessor :new_name
      attr_accessor :is_ruby_only
      attr_accessor :deprecated
      attr_accessor :reverseLogic
      attr_accessor :is_method_static
      attr_accessor :is_method_instance

    end

    class MethodParam

      TYPE_STRING = 'STRING'
      TYPE_ARRAY = 'ARRAY'
      TYPE_HASH = 'HASH'
      TYPE_INT = 'INTEGER'
      TYPE_BOOL = 'BOOLEAN'
      TYPE_DOUBLE = 'FLOAT'
      TYPE_CALLBACK = 'CALLBACK'
      TYPE_SELF = 'SELF_INSTANCE'

      SIMPLE_TYPES = [TYPE_STRING, TYPE_INT, TYPE_BOOL, TYPE_DOUBLE]
      BASE_TYPES = [TYPE_STRING, TYPE_ARRAY, TYPE_HASH, TYPE_INT, TYPE_BOOL, TYPE_DOUBLE]
      ALL_TYPES = [TYPE_STRING, TYPE_ARRAY, TYPE_HASH, TYPE_INT, TYPE_BOOL, TYPE_DOUBLE, TYPE_CALLBACK, TYPE_SELF]

      TYPE_TRAITS = {
          TYPE_STRING => {'default' => '""', 'cpp_type' => 'rho::String'},
          TYPE_INT => {'default' => '0', 'cpp_type' => 'int'},
          TYPE_BOOL => {'default' => 'false', 'cpp_type' => 'bool'},
          TYPE_DOUBLE => {'default' => '0.0', 'cpp_type' => 'double'}
      }

      def initialize
        @name = ''
        @type = TYPE_STRING
        @sub_param = nil
        @sub_params = nil
        @can_be_nil = false
        @desc = ''
        @default_value = nil
        @is_property_hash = false
        @linked_field = nil
        @is_generated = false
      end

      attr_accessor :name
      #attr_accessor :type
      attr_reader :type

      # for ARRAY
      attr_accessor :sub_param

      #for HASH
      attr_accessor :sub_params

      attr_accessor :can_be_nil
      attr_accessor :desc
      attr_accessor :default_value
      attr_accessor :is_property_hash
      attr_accessor :linked_field
      attr_accessor :is_generated

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

        # if type=ARRAY
        @sub_param = nil

        # if type=HASH
        @sub_params = nil
        @desc = ''
      end

      attr_accessor :type
      attr_accessor :item_type
      attr_accessor :sub_param
      attr_accessor :sub_params
      attr_accessor :desc
    end


    class ModuleConstant
      def initialize
        @name = ''
        @type = MethodParam::TYPE_STRING
        @value = ''
        @desc = ''
      end

      attr_accessor :name
      attr_accessor :type
      attr_accessor :value
      attr_accessor :desc
    end

    class ModuleHashKey
      def initialize
        @name = ''
        @const_tag = ''
        @deprecated = false
      end

      attr_accessor :name
      attr_accessor :const_tag
      attr_accessor :deprecated
    end

    class ModuleProperty

      USE_PROPERTY_BAG_MODE_NONE = 'NONE'
      USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS = 'PROPERTY_BAG_VIA_ACCESSORS'
      USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG = 'ACCESSORS_VIA_PROPERTY_BAG'

      def initialize
        @name = ''
        @type = MethodParam::TYPE_STRING
        @param = nil
        @native_name = ''
        @readonly = false
        @writeonly = false
        @generate_accessors = true
        @use_property_bag_mode = USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
        @default_value = nil
        @access = ModuleMethod::ACCESS_INSTANCE
        @is_deprecated = false

        #used if getter and setter are present - this used with generation of property bag function implementation
        @getter = nil
        @setter = nil
        @generateAPI = true
        @generateDoc = true
        @run_in_thread = ModuleMethod::RUN_IN_THREAD_UNDEFINED
      end

      attr_accessor :name
      attr_accessor :type
      attr_accessor :param
      attr_accessor :native_name
      attr_accessor :readonly
      attr_accessor :writeonly
      attr_accessor :generate_accessors
      attr_accessor :use_property_bag_mode
      attr_accessor :default_value
      attr_accessor :access
      attr_accessor :getter
      attr_accessor :setter
      attr_accessor :generateAPI
      attr_accessor :generateDoc
      attr_accessor :run_in_thread
      attr_accessor :is_deprecated

    end

    class ModuleMethod
      ACCESS_STATIC = 'STATIC'
      ACCESS_INSTANCE = 'INSTANCE'
      ACCESS_MODULE = 'MODULE'

      CALLBACK_MANDATORY = 'MANDATORY'
      CALLBACK_OPTIONAL = 'OPTIONAL'
      CALLBACK_NONE = 'NONE'

      ATTRIBUTE_CALLBACK_MAPPING = {
          'none'.downcase => CALLBACK_NONE,
          'mandatory'.downcase => CALLBACK_MANDATORY,
          'optional'.downcase => CALLBACK_OPTIONAL
      }

      RUN_IN_THREAD_UNDEFINED = 'UNDEFINED'
      RUN_IN_THREAD_NONE = 'NONE'
      RUN_IN_THREAD_MODULE = 'MODULE'
      RUN_IN_THREAD_SEPARATED = 'SEPARATED'
      RUN_IN_THREAD_UI = 'UI'

      ATTRIBUTE_THREAD_MAPPING = {
          'undefined'.downcase => RUN_IN_THREAD_UNDEFINED,
          'none'.downcase => RUN_IN_THREAD_NONE,
          'module'.downcase => RUN_IN_THREAD_MODULE,
          'separate'.downcase => RUN_IN_THREAD_SEPARATED,
          'ui'.downcase => RUN_IN_THREAD_UI
      }

      SPECIAL_BEHAVIOUR_NONE = 'NONE'
      SPECIAL_BEHAVIOUR_GETTER = 'GETTER'
      SPECIAL_BEHAVIOUR_SETTER = 'SETTER'

      def initialize
        @name = ''
        @native_name = ''
        @binding_name = ''
        @params = [] # array of MethodArgument
        @run_in_thread = RUN_IN_THREAD_UNDEFINED
        @is_factory_method = false
        @is_return_value = false
        @access = ACCESS_INSTANCE
        @has_callback = CALLBACK_NONE
        @result = nil
        @is_deprecated = false
        @is_constructor = false
        @is_destructor = false
        @is_generated = false
        @is_static_for_entity = false
        @generateNativeAPI = true

        # name of template produced this method if applicable
        @generated_by_template = ''

        @linked_property = nil # use only if method is setter or getter
        #if this property is not NONE then generator should prepare special function code. For example setter or getter have already prepared code
        @special_behaviour = SPECIAL_BEHAVIOUR_NONE

        @linked_entity = nil

        #use for store any data by string key. For example I save to this bag a prepared iOS full function signature for do not rebuild it everytime
        @cached_data = {}
        @generateAPI = true
        @generateDoc = true
        @desc = ''
      end

      attr_reader :name
      attr_accessor :native_name
      attr_accessor :binding_name
      attr_accessor :params
      attr_accessor :run_in_thread
      attr_accessor :is_factory_method
      attr_accessor :is_return_value
      attr_accessor :access
      attr_accessor :has_callback
      attr_accessor :result
      attr_accessor :is_deprecated
      attr_accessor :is_generated
      attr_accessor :is_static_for_entity
      attr_accessor :generated_by_template
      attr_accessor :linked_property
      attr_accessor :linked_entity
      attr_accessor :special_behaviour
      attr_accessor :cached_data
      attr_accessor :generateAPI
      attr_accessor :generateDoc
      attr_accessor :desc
      attr_accessor :is_constructor
      attr_accessor :is_destructor
      attr_accessor :generateNativeAPI

      def name=(val)
        @name = val
        @binding_name = val
      end

      def is_callback_accessor
        (@linked_property != nil && @linked_property.type == MethodParam::TYPE_CALLBACK )
      end

      def is_accessor
        (@special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER) || (@special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER) || is_callback_accessor
      end
    end

    class EntityField
      ACCESS_READ_ONLY = 'READ_ONLY'
      ACCESS_WRITE_ONLY = 'WRITE_ONLY'
      ACCESS_READ_WRITE = 'READ_WRITE'

      ACCESS_TYPES = [ACCESS_READ_ONLY, ACCESS_WRITE_ONLY, ACCESS_READ_WRITE]

      CONSTRUCTOR_PARAM_NONE = 'NONE'
      CONSTRUCTOR_PARAM_MANDATORY = 'MANDATORY'
      CONSTRUCTOR_PARAM_OPTIONAL = 'OPTIONAL'

      CONSTRUCTOR_PARAM_TYPES = [CONSTRUCTOR_PARAM_NONE, CONSTRUCTOR_PARAM_MANDATORY, CONSTRUCTOR_PARAM_OPTIONAL]

      def initialize
        @field_index = 0
        @type = MethodParam::TYPE_STRING
        @cpptype = MethodParam::TYPE_TRAITS[@type]['cpp_type']
        @type_index = 0
        @constructor_param = 0
        @binding = false
        @const = false
        @name = ''
        @access = ACCESS_READ_WRITE
        @default_value = ''
        @desc = ''

        @parent_entity = nil
        @getter = nil
        @setter = nil
        @param = nil
      end

      attr_accessor :field_index
      attr_reader :type
      attr_reader :cpptype
      attr_reader :constructor_param
      attr_reader :type_index
      attr_accessor :binding
      attr_accessor :const
      attr_accessor :name
      attr_accessor :access
      attr_accessor :default_value
      attr_accessor :desc

      attr_accessor :parent_entity
      attr_accessor :getter
      attr_accessor :setter
      attr_accessor :param

      def type=(value)
        up_value = value.upcase
        type_index = MethodParam::SIMPLE_TYPES.index(up_value)

        if index != nil
          @type = up_value
          @cpptype = MethodParam::TYPE_TRAITS[@type]['cpp_type']
          @type_index = index
          @default_value = MethodParam::TYPE_TRAITS[@type]['default']
          return
        end
      end

      def index
        return ((binding ? 0 : 2) + (const ? 0 : 1))
      end

      def is_readable
        return @access == ACCESS_READ_ONLY || @access == ACCESS_READ_WRITE
      end

      def is_writeable
        return (@access == ACCESS_WRITE_ONLY || @access == ACCESS_READ_WRITE) && !const
      end

      def constructor_param=(value)
        idx = CONSTRUCTOR_PARAM_TYPES.index(value.upcase)
        if idx != nil
          @constructor_param = idx
        end
      end
    end

    class ModuleEntity
      def initialize
        @name = ''
        @native_name = ''
        @fields = []
        @binding_fields = []
        @normal_fields = []
        @addr_fields = []
        @methods = []
        @init_method = nil
        @update_method = nil
        @desc = ''
        @run_in_thread = ModuleMethod::RUN_IN_THREAD_UNDEFINED
      end

      attr_accessor :name
      attr_accessor :native_name
      attr_accessor :fields
      attr_accessor :binding_fields
      attr_accessor :normal_fields
      attr_accessor :addr_fields
      attr_accessor :methods
      attr_accessor :init_method
      attr_accessor :update_method
      attr_accessor :desc
      attr_accessor :run_in_thread
    end

    class ModuleItem
      def initialize
        @name = ''

        # array of parents from top to lowest
        @parents = []
        @methods = []
        @methods_without_generated_code = []
        @properties = []
        @constants = []
        @entities = []
        @method_aliases = []
        @property_aliases = []
        @module_aliases = []
        @hash_key_names = []

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
        property_aliases.each do |alias_item|
          ar << alias_item.new_name if alias_item.existing_name == name
        end

        ar
      end

      attr_accessor :name
      attr_accessor :parents
      attr_accessor :methods
      attr_accessor :methods_without_generated_code
      attr_accessor :properties
      attr_accessor :constants
      attr_accessor :entities
      attr_accessor :method_aliases
      attr_accessor :property_aliases
      attr_accessor :module_aliases
      attr_accessor :hash_key_names
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

    TEMPLATE_NAME = 'TEMPLATE_NAME'
    TEMPLATE_PROPERTY_BAG = 'property_bag'
    TEMPLATE_SINGLETONE_INSTANCES = 'singleton_instances'

    $modules = []

    $cur_module = nil


    $possible_attributes = {}
    $possible_attributes['MODULE'] = ['name', 'parent', 'generateUnderscoreRubyNames']
    $possible_attributes['CONSTANT'] = ['name', 'value', 'type']
    $possible_attributes['PROPERTIES'] = ['usePropertyBag', 'readOnly', 'writeOnly', 'generateAccessors', 'limitPropertyBag', 'generateAPI', 'generateDoc', 'access']
    $possible_attributes['PROPERTY'] = ['name', 'type', 'usePropertyBag', 'readOnly', 'writeOnly', 'generateAccessors', 'default', 'generateAPI', 'generateDoc', 'access', 'runInThread', 'deprecated']
    $possible_attributes['VALUE'] = ['constName', 'value', 'type']
    $possible_attributes['ALIAS'] = ['new', 'existing', 'reverseLogic', 'deprecated', 'rubyOnly']
    $possible_attributes['METHODS'] = ['access', 'hasCallback', 'factory', 'runInThread', 'deprecated', 'generateAPI', 'generateDoc']
    $possible_attributes['METHOD'] = ['name', 'access', 'hasCallback', 'factory', 'runInThread', 'nativeName', 'deprecated', 'generateAPI', 'generateDoc', 'constructor', 'destructor', 'generateNativeAPI']
    $possible_attributes['PARAM'] = ['name', 'nativeName', 'type', 'propertyHash', 'default', 'rhoAction']
    $possible_attributes['RETURN'] = ['type']
    $possible_attributes['CALLBACK'] = ['type']
    $possible_attributes['APPLIES'] = ['msiOnly', 'rubyOnly', 'jsOnly']
    $possible_attributes['ENTITY'] = ['name', 'runInThread']
    $possible_attributes['FIELD'] = ['name', 'type', 'binding', 'readOnly']
    $possible_attributes['INCLUDE'] = ['path']

    $possible_children = {}
    $possible_children['API'] = ['MODULE']
    $possible_children['MODULE'] = ['HELP_OVERVIEW', 'MORE_HELP', 'TEMPLATES', 'CONSTANTS', 'PROPERTIES', 'METHODS', 'ENTITIES', 'USER_OVERVIEW', 'VER_INTRODUCED', 'PLATFORM', 'ALIASES', 'EXAMPLES']
    $possible_children['TEMPLATES'] = ['SINGLETON_INSTANCES', 'DEFAULT_INSTANCE', 'PROPERTY_BAG', 'INCLUDE']
    $possible_children['CONSTANTS'] = ['CONSTANT']
    $possible_children['CONSTANT'] = ['DESC']
    $possible_children['PROPERTIES'] = ['DESC', 'PROPERTY', 'ALIASES']
    $possible_children['PROPERTY'] = ['DESC', 'VALUES', 'PLATFORM', 'MORE_HELP', 'APPLIES', 'VER_INTRODUCED', 'APPLIES', 'PARAM']
    $possible_children['VALUES'] = ['VALUE']
    $possible_children['VALUE'] = ['DESC', 'PLATFORM']
    $possible_children['ALIASES'] = ['ALIAS']
    $possible_children['ALIAS'] = ['DESC']
    $possible_children['METHODS'] = ['METHOD', 'ALIASES']
    $possible_children['FIELDS'] = ['FIELD']
    $possible_children['METHOD'] = ['DESC', 'PARAMS', 'RETURN', 'CALLBACK', 'PLATFORM', 'MORE_HELP', 'APPLIES', 'VER_INTRODUCED', 'BACKWARDS_COMPATIBILITY', 'APPLIES']
    $possible_children['PARAMS'] = ['PARAM']
    $possible_children['PARAM'] = ['DESC', 'PARAM', 'PARAMS', 'VALUES', 'CAN_BE_NIL', 'APPLIES', 'PLATFORM']
    $possible_children['RETURN'] = ['DESC', 'PARAM', 'PARAMS']
    $possible_children['CAN_BE_NIL'] = ['DESC']
    $possible_children['CALLBACK'] = ['DESC', 'PARAM', 'PARAMS']
    $possible_children['ENTITIES'] = ['ENTITY']
    $possible_children['ENTITY'] = ['DESC', 'FIELDS', 'METHODS', 'PLATFORM', 'MORE_HELP', 'APPLIES', 'VER_INTRODUCED', 'APPLIES']


    $possible_values = {}
    $possible_values['generateUnderscoreRubyNames'] = ['true', 'false']
    $possible_values['readOnly'] = ['true', 'false']
    $possible_values['writeOnly'] = ['true', 'false']
    $possible_values['usePropertyBag'] = ['none', 'accessorsViaPropertyBag', 'PropertyBagViaAccessors']
    $possible_values['access'] = ['STATIC', 'INSTANCE']
    $possible_values['factory'] = ['true', 'false']
    $possible_values['runInThread'] = ['none', 'ui', 'module', 'separate']
    $possible_values['hasCallback'] = ['none', 'mandatory', 'optional']
    $possible_values['deprecated'] = ['true', 'false']
    $possible_values['reverseLogic'] = ['true', 'false']
    $possible_values['rubyOnly'] = ['true', 'false']
    $possible_values['generateAccessors'] = ['true', 'false']
    $possible_values['generateAPI'] = ['true', 'false']
    $possible_values['generateDoc'] = ['true', 'false']
    $possible_values['constructor'] = ['true', 'false']
    $possible_values['destructor'] = ['true', 'false']
    $possible_values['generateNativeAPI'] = ['true', 'false']

    def xml_error(error_desc, path)
      puts "XML WARNING: #{error_desc.bold} in Element #{path.bold}"
    end


    def check_xml_element_for_children_and_attributes(xml_element, parent_path)
      all_is_ok = true
      this_name = xml_element.name
      cur_path = ''
      if xml_element.attribute('name') != nil
        cur_path = parent_path + "<"+this_name+' name="'+xml_element.attribute('name').to_s+'">'
      else
        cur_path = parent_path + "<"+this_name+'>'
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
              xml_error('unrecognized attribute - "'+attribute_name+'"', cur_path)
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
      return name.downcase.split(/[^a-zA-Z0-9]/).map { |w| w.downcase }.join("")
    end

    def convertCamelToUnderscore (s)
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


    def foundMethodByName(module_item, method_name)
      module_item.methods.each do |method_item|
        if method_item.name == method_name
          return method_item
        end
      end
      return nil
    end

    def apply_templates_to_module(xml_module_item, template_name)
      xml_f = File.new(File.join(File.dirname(__FILE__), 'templates', 'api', 'xml_templates', template_name+'.xml'))
      template_xml = REXML::Document.new(xml_f)
      xml_f.close

      elements = xml_module_item.get_elements('METHODS')

      base_section = nil

      if elements.size == 0
        puts "Module have no its own METHODS section but is using templates".red
      else
        base_section = elements[0]
        if elements.size > 1
          puts "Module have more than one METHODS section, using first one to insert template methods"
        end
      end

      methods_item = template_xml.elements['*/METHODS']

      if methods_item != nil
        methods_item.elements.each('METHOD') do |method|
          method.add_attribute(TEMPLATE_NAME, template_name)

          if !base_section.nil?
            base_section.add_element method
          end
        end
        if base_section.nil?
          xml_module_item.add_element methods_item
        end
      end
    end

    def add_alias(xml_alias, dst_collection)
      module_alias = ModuleAlias.new()

      module_alias.existing_name = xml_alias.attribute('existing').to_s
      module_alias.new_name = xml_alias.attribute('new').to_s
      if xml_alias.attribute('deprecated') != nil
        module_alias.deprecated = xml_alias.attribute('deprecated').to_s.downcase != 'false'
      end
      if xml_alias.attribute('reverseLogic') != nil
        module_alias.reverseLogic = xml_alias.attribute('reverseLogic').to_s.downcase != 'false'
      end
      if xml_alias.attribute('rubyOnly') != nil
        module_alias.is_ruby_only = xml_alias.attribute('rubyOnly').to_s.downcase != 'false'
      end
      dst_collection << module_alias
    end


    def prepare_rho_api_param_string(str)
      if str == nil
        return '0'
      end
      if str.size <= 0
        return '0'
      end
      return '"'+str+'"'
    end

    #return string {...}
    def prepare_rho_api_param_structure(param, name_prefix, param_index, lines)
      str = ''
      if MethodParam::SIMPLE_TYPES.include?(param.type)
        str = '{'
        str = str + 'RHO_API_'+param.type+', '
        str = str + '0, '
        str = str + prepare_rho_api_param_string(param.name)+', '
        str = str + '0, 0 }'
      else
        if param.type == MethodParam::TYPE_ARRAY
          str = '{'
          str = str + 'RHO_API_ARRAY, '
          str = str + '0, '
          str = str + prepare_rho_api_param_string(param.name)+', '
          str = str + '1, '+name_prefix+'_param'+param_index.to_s
          str = str + ' }'
          tmp_ar = []
          tmp_ar << param.sub_param if param.sub_param
          prepare_rho_api_params_structure_line(tmp_ar, name_prefix+'_param'+param_index.to_s, lines)
        else
          if param.type == MethodParam::TYPE_HASH
            str = '{'
            str = str + 'RHO_API_HASH, '
            str = str + '0, '
            str = str + prepare_rho_api_param_string(param.name)+', '
            if param.sub_params != nil
              str = str + param.sub_params.size.to_s + ', '+name_prefix+'_param'+param_index.to_s
              str = str + ' }'
              prepare_rho_api_params_structure_line(param.sub_params, name_prefix+'_param'+param_index.to_s, lines)
            else
              str = str + '0, 0 }'
            end
          else
            #custom type - object
            str = '{'
            str = str + 'RHO_API_OBJECT, '
            str = str + '"' + param.type + '", '
            str = str + prepare_rho_api_param_string(param.name)+', '
            str = str + '0, 0 }'
          end
        end
      end
      return str
    end

    def prepare_rho_api_params_structure_line(params, name, lines)
      index = 0
      line = 'static RHO_API_PARAM '+name+'[] = { '
      params.each do |param|
        line = line + prepare_rho_api_param_structure(param, name, index, lines)
        index = index + 1
        if index < params.size
          line = line + ', '
        end
      end
      line = line + ' };'
      lines << line
    end

    #return array of strings
    def prepare_rho_api_params_structure_lines(root_params, name)
      tmp_lines = []
      prepare_rho_api_params_structure_line(root_params, name, tmp_lines)
      return tmp_lines
    end

    #return MethodParam object
    def process_param(xml_param_item, predefined_name, module_item, method_name, param_index, self_type)
      param = MethodParam.new()

      if xml_param_item.attribute('name') != nil
        param.name = xml_param_item.attribute('name').to_s
      else
        param.name = predefined_name
      end
      if xml_param_item.attribute('default') != nil
        param.default_value = xml_param_item.attribute('default').to_s
      end
      if xml_param_item.attribute('propertyHash') != nil
        param.is_property_hash = (xml_param_item.attribute('propertyHash').to_s == 'true')
      end

      xml_param_item.elements.each('DESC') do |xml_desc|
        param.desc = xml_desc.text
      end

      if xml_param_item.attribute('type') != nil
        ttype = xml_param_item.attribute('type').to_s #.upcase
        if ttype == MethodParam::TYPE_SELF
          ttype = self_type
        end
        param.type = ttype

        if ttype == MethodParam::TYPE_ARRAY
          # looking for <PARAM> child
          xml_param_item.elements.each('PARAM') do |xml_method_subparam|
            if xml_method_subparam.parent == xml_param_item
              param.sub_param = process_param(xml_method_subparam, 'array_param', module_item, method_name, param_index, self_type)
            end
          end
          if param.sub_param == nil
            param.sub_param = MethodParam.new()
            puts "WARNING: <ARRAY> do not have specified item type - set to STRING by default ! in Module[#{module_item.name.bold}].method[#{method_name.bold}].param_index[#{param_index.to_s.bold}]".brown
          end
        end

        if ttype == MethodParam::TYPE_HASH
          xml_param_item.elements.each('PARAMS') do |xml_method_subparams|
            if xml_method_subparams.parent == xml_param_item
              param.sub_params = process_params(xml_method_subparams, module_item, method_name+'_'+param.name, self_type)
            end
          end

          #if param.sub_params == nil
          #  puts "WARNING: you use HASH type without specified items ! Module[#{module_item.name}].method[#{method_name}].param_index[#{param_index.to_s}]"
          #end

        end

      else
        raise "ERROR: #{"Parameter in method must have specified type".red} ! Module[#{module_item.name}].method[#{method_name.bold}].param_index[#{param_index.to_s.bold}]"
      end

      xml_param_item.elements.each('CAN_BE_NIL') do |canbenil|
        if canbenil.parent == xml_param_item
          param.can_be_nil = true
        end
      end

      return param
    end

    #return array of MethodParam objects
    def process_params(xml_params_item, module_item, method_name, self_type)
      params = []

      param_index = 1
      xml_params_item.elements.each('PARAM') do |xml_param|
        if xml_param.parent == xml_params_item
          param = process_param(xml_param, 'param'+ param_index.to_s, module_item, method_name, param_index, self_type)
          param_index = param_index + 1
          params << param
        end
      end

      return params
    end

    def param_list_from_fields(fields, can_be_nil)
      field_list = []
      fields.each do |field|
        param_item = MethodParam.new()
        param_item.name = field.name
        param_item.type = field.type
        param_item.default_value = field.default_value
        param_item.can_be_nil = can_be_nil && !field.binding
        param_item.linked_field = field
        field_list << param_item
      end

      field_list
    end

    def create_param_from_list(field_list, param_name, can_be_nil, can_be_simplified)
      param = nil
      if field_list.size > 0
        fields_param_list = param_list_from_fields(field_list, can_be_nil)
        if can_be_simplified && fields_param_list.size == 1
          param = fields_param_list.at(0)
        else
          param = MethodParam.new()
          param.name = param_name
          param.type = MethodParam::TYPE_HASH
          param.sub_params = fields_param_list
        end
      end

      param
    end

    def check_unique_names(element_type, elements)
      element_mapping = {}
      elements.each do |x|
        #check for elements without names
        next if !x.value || (x.value.strip.empty?)

        if !element_mapping.has_key?(x.name)
          element_mapping[x.name] = [x.value]
        else
          element_mapping[x.name] << x.value
        end
      end

      element_mapping.each do |k,v|
        if v.size > 1
          unique_values = v.uniq
          if unique_values.size > 1
            puts "ERROR: #{element_type} with name #{k.bold} has several definitions with distinct values: #{v.join(', ').bold}".red
            has_error = true
          else
            puts "WARNING: #{element_type} with name #{k.bold} is defined #{v.size.to_s.bold} times".brown
          end
        end
      end
    end

    def check_unique_names_and_values(element_type, elements)
      element_mapping = {}
      value_mapping = {}
      elements.each do |x|
        #check for constnants without names
        if !x.name || (x.name.strip.empty?)
          if !x.value || (x.value.strip.empty?)
            next
          else
            x.name = x.value.underscore.upcase
            puts "Notification: #{element_type} with value #{x.value.bold} had no name, set name to: #{x.name.bold}"
          end
        end
        if !element_mapping.has_key?(x.name)
          element_mapping[x.name] = [x.value]
        else
          element_mapping[x.name] << x.value
        end
        if !value_mapping.has_key?(x.value)
          value_mapping[x.value] = [x.name]
        else
          value_mapping[x.value] << x.name
        end

      end

      has_error = false;

      element_mapping.each do |k,v|
        if v.size > 1
          unique_values = v.uniq
          if unique_values.size > 1
            puts "ERROR: #{element_type} with name #{k.bold} has several definitions with distinct values: #{v.join(', ').bold}".red
            has_error = true
          else
            puts "WARNING: #{element_type} with name #{k.bold} is defined #{v.size.to_s.bold} times".brown
          end
        end
      end


      value_mapping.each do |k,v|
        if v.size > 1
          unique_values = v.uniq
          if unique_values.size > 1
            wrong_paste = []

            v.each do |elem|
              if !((elem.downcase.include?(k.downcase)) || (k.downcase.include?(elem.downcase)))
                wrong_paste << elem
              end
            end
            if wrong_paste.size >= 1
              puts "WARNING: #{element_type} value #{k.bold} is probably wrong for elements with name #{wrong_paste.join(', ').to_s.bold} ".magenta
            end
          end
        end
      end

      if has_error
        raise "#{element_type} with multiple values defined"
      end
    end


    def check_duplicate_descriptions(xml_module_item, element)
      descriptions = {}
      xml_module_item.elements.each(".//#{element.upcase}/DESC") do |constant|
        const_desc = constant.text
        next if const_desc.nil?
        next if const_desc.strip.empty?

        if !descriptions.has_key?(const_desc)
          descriptions[const_desc] = [constant]
        else
          descriptions[const_desc] << constant
        end
      end

      descriptions.each do |k, v|
        if v.size > 1
          constant_names = v.map{|c| c.parent.attribute('name').to_s.downcase + (c.parent.attribute(TEMPLATE_NAME).nil? ? '' : " (#{c.parent.attribute(TEMPLATE_NAME).to_s})")}
          constant_names.uniq!
          if constant_names.size > 1
            puts "Warning: Elements #{element.upcase.bold} with different names: #{constant_names.join(', ').bold} have same description \"#{k.bold}\"".magenta
          end
        end
      end
    end



    def process_constants(supported_simple_types, xml_module_item)
      constants = []

      xml_module_item.elements.each('CONSTANTS/CONSTANT') do |xml_constant|
        module_constant = ModuleConstant.new()
        module_constant.name = xml_constant.attribute('name').to_s
        module_constant.value = xml_constant.attribute('value').to_s
        if xml_constant.attribute('type') != nil
          module_constant.type = xml_constant.attribute('type').to_s.upcase
        end
        xml_constant.elements.each('DESC') do |xml_desc|
          module_constant.desc = xml_desc.text
        end
        constants << module_constant
      end

      #constants in properties
      xml_module_item.elements.each('PROPERTIES/PROPERTY') do |xml_property|
        default_type = nil
        if xml_property.attribute('type') != nil
          type = xml_property.attribute('type').to_s.upcase
          if supported_simple_types.include?(type)
            default_type = type
          end
        end
        xml_property.elements.each('VALUES/VALUE') do |xml_property_value|
          module_constant = ModuleConstant.new()
          module_constant.name = xml_property_value.attribute('constName').to_s
          module_constant.value = xml_property_value.attribute('value').to_s
          if xml_property_value.attribute('type') != nil
            module_constant.type = xml_property_value.attribute('type').to_s.upcase
          elsif default_type != nil
            module_constant.type = default_type
          end
          xml_property_value.elements.each('DESC') do |xml_desc|
            module_constant.desc = xml_desc.text
          end
          constants << module_constant
        end
      end

      #constants in param
      xml_module_item.elements.each('.//PARAM/VALUES/VALUE') do |xml_param_value|
        module_constant = ModuleConstant.new()
        module_constant.name = xml_param_value.attribute('constName').to_s
        module_constant.value = xml_param_value.attribute('value').to_s
        if xml_param_value.attribute('type') != nil
          module_constant.type = xml_param_value.attribute('type').to_s.upcase
        end
        xml_param_value.elements.each('DESC') do |xml_desc|
          module_constant.desc = xml_desc.text
        end
        constants << module_constant
      end

      check_unique_names_and_values("Constant", constants)

      #leave only unique consants
      return constants.uniq { |x| x.name }.sort{ |a,b|  a.name <=> b.name}
    end

    def process_hash_keys(xml_module_item)
      # gather all possible elements with hash keys
      hash_elems = []

      xml_module_item.elements.each('.//PARAMS/PARAM') do |xml_param|
        if xml_param.attribute('type').to_s.upcase == 'HASH'
          hash_elems << xml_param
        end
      end

      xml_module_item.elements.each('.//RETURN') do |xml_param|
        if xml_param.attribute('type').to_s.upcase == 'HASH'
          hash_elems << xml_param
        end
      end

      xml_module_item.elements.each('.//CALLBACK') do |xml_param|
        if xml_param.attribute('type').to_s.upcase == 'HASH'
          hash_elems << xml_param
        end
      end

      # process hash key names
      hash_objs = []
      hash_elems.each do |xml_param|
        xml_param.elements.each('.//PARAM') do |param_key|
          name_attr = param_key.attribute('name')
          next if name_attr.nil?
          next if name_attr.to_s.strip.empty?

          key = ModuleHashKey.new()
          key.name = name_attr.to_s

          if (param_key.attribute('deprecated').to_s.downcase == 'true')
            key.deprecated = true
          end

          # transform hash key name to constant (camelcase to underscore)
          const_key = 'HK_' + key.name.dup.underscore + ( key.deprecated ? '_DEPRECATED' : '' )
          const_key.upcase!
          key.const_tag = const_key

          hash_objs << key
        end
      end

      # process property names
      xml_module_item.elements.each('.//PROPERTY') do |xml_property|
        name_attr = xml_property.attribute('name')
        next if name_attr.nil?
        next if name_attr.to_s.strip.empty?

        key = ModuleHashKey.new()
        key.name = name_attr.to_s

        if (xml_property.attribute('deprecated').to_s.downcase == 'true')
          key.deprecated = true
        end

        # transform hash key name to constant (camelcase to underscore)
        const_key = 'PROPERTY_' + key.name.dup.underscore + ( key.deprecated ? '_DEPRECATED' : '' )
        const_key.upcase!
        key.const_tag = const_key

        hash_objs << key
      end

      hash_objs.uniq! { |p| p.const_tag }
      hash_objs.sort! { |a, b| a.const_tag <=> b.const_tag }
      hash_objs
    end

    # retun hash filled with attribute=value pairs
    # if include_inherited is true then all partent node attributes will be also included
    # if include_inherited is an array of strings then only attributes with names in that array will be included from parent node
    def collect_inherited_attributes(item, include_inherited = false)
      attributes = {}
      if !item.nil?
        if include_inherited.is_a?(Array)
          item.parent.attributes.each do |k, v|
            if include_inherited.index(k)
              attributes[k] = v.to_s
            end
          end
        else
          if include_inherited
            item.parent.attributes.each do |k, v|
              attributes[k] = v.to_s
            end
          end
        end
        item.attributes.each do |k, v|
          attributes[k] = v.to_s
        end
      end

      attributes
    end

    def process_method(xml_module_method, module_item, self_type)
      module_method = ModuleMethod.new()

      item_attributes = collect_inherited_attributes(xml_module_method, ['deprecated', 'generateAPI','generateDoc','runInThread', 'hasCallback'])

      module_method.name = xml_module_method.attribute('name').to_s
      module_method.native_name = module_method.name.split(/[^a-zA-Z0-9\_]/).map { |w| w }.join("")

      if item_attributes['nativeName'] != nil
        module_method.native_name = item_attributes['nativeName']
      end

      if item_attributes['deprecated'] != nil
        module_method.is_deprecated = item_attributes['deprecated'].downcase != 'false'
      end

      if item_attributes['generateAPI'] != nil
        module_method.generateAPI = item_attributes['generateAPI'].downcase != 'false'
      end

      if item_attributes['generateDoc'] != nil
        module_method.generateDoc = item_attributes['generateDoc'].downcase != 'false'
      end

      attr_run_in_thread = item_attributes['runInThread']
      if attr_run_in_thread != nil
        run_in_thread = ModuleMethod::ATTRIBUTE_THREAD_MAPPING[attr_run_in_thread.downcase]

        if run_in_thread != nil
          module_method.run_in_thread = run_in_thread
        else
          puts "Invalid attribute 'run_in_thread' == #{attr_run_in_thread.bold} for method #{module_method.name.bold}".red
        end
      end

      attr_has_callback = item_attributes['hasCallback']
      if attr_has_callback != nil
        has_callback = ModuleMethod::ATTRIBUTE_CALLBACK_MAPPING[attr_has_callback.downcase]

        if has_callback != nil
          module_method.has_callback = has_callback
        else
          puts "Invalid attribute 'has_callback' == #{attr_run_in_thread.bold} for method #{module_method.name.bold}".red
        end
      end

      xml_module_method.elements.each('DESC') do |xml_desc|
        module_method.desc = xml_desc.text
      end

      if (xml_module_method.elements['RETURN'] != nil) || xml_module_method.elements['CALLBACK'] != nil

        ret_el = xml_module_method.elements['RETURN']
        if ret_el == nil
          ret_el = xml_module_method.elements['CALLBACK']
        end

        module_method.is_return_value = true
        method_result = MethodResult.new()
        result_type = ret_el.attribute('type')
        if result_type == nil
          puts "you use RETURN/CALLBACK without specified type - use default STRING type ! Module[#{module_item.name}].method[#{module_method.name}]"
          result_type = MethodParam::TYPE_STRING
        else
          result_type = result_type.to_s
        end
        if result_type == ""
          puts "you use RETURN/CALLBACK with invalid type - use default STRING type ! Module[#{module_item.name}].method[#{module_method.name}]"
          result_type = MethodParam::TYPE_STRING
        end
        #result_item_type = xml_module_method.elements['RETURN'].attribute('itemType').to_s
        ret_el.elements.each('DESC') do |xml_desc|
          method_result.desc = xml_desc.text
        end
        if result_type != nil
          if result_type == MethodParam::TYPE_SELF
            result_type = self_type
          end
          method_result.type = result_type
        end
        #if result_item_type != nil
        #   if result_item_type == MethodParam::TYPE_SELF
        #     result_item_type = [module_item.parents.join('.'),module_item.name].join('.')
        #   end
        #   method_result.item_type = result_item_type
        #end

        xml_module_method.elements.each('RETURN/PARAM') do |return_param_xml|
          method_result.sub_param = process_param(return_param_xml, 'result', module_item, module_method.name+'_RETURN', 0, self_type)
          if method_result.sub_param != nil
            method_result.item_type = method_result.sub_param.type
          end
        end
        xml_module_method.elements.each('RETURN/PARAMS') do |return_params_xml|
          method_result.sub_params = process_params(return_params_xml, module_item, module_method.name+'_RETURN', self_type)
        end
        xml_module_method.elements.each('CALLBACK/PARAM') do |return_param_xml|
          method_result.sub_param = process_param(return_param_xml, 'result', module_item, module_method.name+'_RETURN', 0, self_type)
          if method_result.sub_param != nil
            method_result.item_type = method_result.sub_param.type
          end
        end
        xml_module_method.elements.each('CALLBACK/PARAMS') do |return_params_xml|
          method_result.sub_params = process_params(return_params_xml, module_item, module_method.name+'_RETURN', self_type)
        end

        if (method_result.type == MethodParam::TYPE_ARRAY) && (method_result.item_type == nil)
          # set default STRING type and show WARNING
          method_result.sub_param = MethodParam.new()
          method_result.item_type = MethodParam::TYPE_STRING
          puts "WARNING: Use of ARRAY type without specified item type - set to STRING by default ! Module[#{module_item.name.bold}].method[#{module_method.name.bold}].RETURN".brown
        end
        if (method_result.type == MethodParam::TYPE_HASH) && (method_result.sub_params == nil)
          puts "WARNING: Use of HASH type without specified items ! Module[#{module_item.name.bold}].method[#{module_method.name.bold}].RETURN".brown
        end

        module_method.result = method_result
      end


      xml_module_method.elements.each('PARAMS') do |xml_method_params|
        if xml_method_params.parent == xml_module_method
          module_method.params = process_params(xml_method_params, module_item, module_method.name, self_type)
        end
      end


      #param_index = 1
      #
      #xml_module_method.elements.each('PARAMS/PARAM') do |xml_method_param|
      #   method_param = MethodParam.new()
      #
      #   if xml_method_param.attribute('name') != nil
      #      method_param.name = xml_method_param.attribute('name').to_s
      #   else
      #      method_param.name = 'param'+ param_index.to_s
      #   end
      #
      #   if xml_method_param.attribute('type') != nil
      #      ttype = xml_method_param.attribute('type').to_s #.upcase
      #      if ttype == MethodParam::TYPE_SELF
      #         ttype = [module_item.parents.join('.'),module_item.name].join('.')
      #      end
      #      method_param.type = ttype
      #   else
      #      raise "parameter in method must have specified type ! Module[#{module_item.name}].method[#{entity_method.name}].param_index[#{param_index.to_s}]"
      #   end
      #
      #   method_param.can_be_nil = (xml_method_param.elements['CAN_BE_NIL'] != nil)
      #
      #   param_index = param_index + 1
      #   entity_method.params << method_param
      #end

      return module_method
    end

    def process_methods(module_item, xml_module_item)
      self_type = [module_item.parents.join('::'), module_item.name].join('::')
      xml_module_item.elements.each('METHODS/METHOD') do |xml_module_method|
        module_method = process_method(xml_module_method, module_item, self_type)

        method_attributes = collect_inherited_attributes(xml_module_method, ['access'])

        attr_access = method_attributes['access']
        if attr_access != nil
          if attr_access.downcase == 'static'
            module_method.access = ModuleMethod::ACCESS_STATIC
          else
            module_item.has_instance_methods = true
          end
        end

        if xml_module_method.attribute(TEMPLATE_NAME) != nil
          module_method.generated_by_template = xml_module_method.attribute(TEMPLATE_NAME).to_s
        end
        if method_attributes['constructor'] != nil
          module_method.is_constructor = method_attributes['constructor'].downcase != 'false'
        end
        if method_attributes['destructor'] != nil
          module_method.is_destructor = method_attributes['destructor'].downcase != 'false'
        end
        if method_attributes['generateNativeAPI'] != nil
          module_method.generateNativeAPI = method_attributes['generateNativeAPI'].downcase != 'false'
        end

        if method_attributes['factory'] != nil
          if method_attributes['factory'].downcase != 'false'
            module_method.is_factory_method = true
            module_item.has_factory_methods = true
          end
        end
        module_item.methods << module_method
      end

      if module_item.has_instance_methods && (!module_item.has_factory_methods && !module_item.is_template_default_instance)
        raise "If module has instance methods, then module should have factory methods for produce instances ! Module[#{module_item.name}]"
      end
    end

    def process_properties(module_item, unsupported_names, xml_module_item)
      properties = []

      xml_properties = xml_module_item.elements['PROPERTIES']

      if xml_properties != nil
        if xml_properties.attribute('limitPropertyBag') != nil
          module_item.is_property_bag_limit_to_only_declared_properties = xml_properties.attribute('limitPropertyBag').to_s.downcase != 'false'
        end
        if xml_properties.attribute('access') != nil
          if xml_properties.attribute('access').to_s.downcase == 'static'
            module_item.properties_access = ModuleMethod::ACCESS_STATIC
          end
        end
      end

      self_type = [module_item.parents.join('::'), module_item.name].join('::')

      xml_module_item.elements.each('PROPERTIES/PROPERTY') do |xml_module_property|
        module_property = ModuleProperty.new()
        module_property.name = xml_module_property.attribute('name').to_s

        property_attributes = collect_inherited_attributes(xml_module_property, ['generateAccessors', 'readOnly', 'writeOnly', 'generateAPI', 'generateDoc', 'access', 'runInThread', 'usePropertyBag'])


        if unsupported_names.include?(module_property.name.upcase)
          raise "Property have invalid name !\n Module[#{module_item.name}].property[#{module_property.name}] is in the list of forbidden names:\n '#{unsupported_names.join("','")}'"
        end
        module_property.native_name = module_property.name.split(/[^a-zA-Z0-9\_]/).map { |w| w }.join("")
        module_property.param = process_param(xml_module_property, module_property.native_name, module_item, 'property_'+module_property.name, 0, self_type)

        if xml_module_property.attribute('type') != nil
          module_property.type = xml_module_property.attribute('type').to_s.upcase
        end
        if xml_module_property.attribute('default') != nil
          module_property.default_value = xml_module_property.attribute('default').to_s
        end

        if property_attributes['generateAccessors'] != nil
          module_property.generate_accessors = property_attributes['generateAccessors'].downcase != 'false'
        end

        # callback is property with wrtieonly logic, it sets result with CALLBACK type
        if module_property.type != MethodParam::TYPE_CALLBACK
          if property_attributes['readOnly'] != nil
            module_property.readonly = property_attributes['readOnly'].downcase != 'false'
          end

          if property_attributes['writeOnly'] != nil
            module_property.writeonly = property_attributes['writeOnly'].downcase != 'false'
          end
        else
          module_property.writeonly = true
        end

        if property_attributes['generateAPI'] != nil
          module_property.generateAPI = property_attributes['generateAPI'].downcase != 'false'
        end

        if property_attributes['generateDoc'] != nil
          module_property.generateDoc =property_attributes['generateDoc'].downcase != 'false'
        end

        if property_attributes['access'] != nil
          if property_attributes['access'].downcase == 'static'
            module_property.access = ModuleMethod::ACCESS_STATIC
          end
        else
          module_property.access = module_item.properties_access
        end

        if property_attributes['runInThread'] != nil
          if property_attributes['runInThread'].downcase == 'none'.downcase
            module_property.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE
          end
          if property_attributes['runInThread'].downcase == 'module'.downcase
            module_property.run_in_thread = ModuleMethod::RUN_IN_THREAD_MODULE
          end
          if property_attributes['runInThread'].downcase == 'separate'.downcase
            module_property.run_in_thread = ModuleMethod::RUN_IN_THREAD_SEPARATED
          end
          if property_attributes['runInThread'].downcase == 'ui'.downcase
            module_property.run_in_thread = ModuleMethod::RUN_IN_THREAD_UI
          end
        end

        if property_attributes['usePropertyBag'] != nil
          if property_attributes['usePropertyBag'].downcase == 'none'.downcase
            module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_NONE
          end
          if property_attributes['usePropertyBag'].downcase == 'accessorsViaPropertyBag'.downcase
            module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
          end
          if property_attributes['usePropertyBag'].downcase == 'PropertyBagViaAccessors'.downcase
            module_property.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
            module_item.use_property_bag_mode = ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
          end
        end
        # if default is not set - set it to default for types
        if module_property.default_value == nil
          if module_property.type == MethodParam::TYPE_BOOL
            module_property.default_value = 'false'
          end
          if module_property.type == MethodParam::TYPE_INT
            module_property.default_value = '0'
          end
          if module_property.type == MethodParam::TYPE_DOUBLE
            module_property.default_value = '0'
          end
          if module_property.type == MethodParam::TYPE_STRING
            module_property.default_value = ''
          end
        end

        properties << module_property
      end
      properties
    end

    def generate_property_methods(properties)
      property_methods = []

      #prepare getters and setters for property
      properties.each do |module_property|
        if module_property.generate_accessors
          if module_property.type != MethodParam::TYPE_CALLBACK
            if  !module_property.writeonly
              getter_method = ModuleMethod.new()

              getter_method.name = module_property.name
              getter_method.native_name = 'get' + module_property.native_name[0..0].upcase + module_property.native_name[1..module_property.native_name.length-1]
              getter_method.desc = 'getter for "' + module_property.name + '" property'
              getter_method.params = []
              getter_method.run_in_thread = ModuleMethod::RUN_IN_THREAD_UNDEFINED
              getter_method.is_factory_method = false
              getter_method.is_return_value = true
              getter_method.access = module_property.access
              getter_method.has_callback = ModuleMethod::CALLBACK_NONE
              getter_method.linked_property = module_property
              getter_method.special_behaviour = ModuleMethod::SPECIAL_BEHAVIOUR_GETTER
              getter_method.generateAPI = module_property.generateAPI
              module_property.getter = getter_method
              property_methods << getter_method
            end

            if !module_property.readonly
              setter_method = ModuleMethod.new()

              setter_method.name = module_property.name + "="
              setter_method.native_name = 'set' + module_property.native_name[0..0].upcase + module_property.native_name[1..module_property.native_name.length-1]
              setter_method.desc = 'setter for "'+ module_property.name + '" property'
              unless (module_property.param)
                param = MethodParam.new()
                param.name = 'value'
                param.can_be_nil = false
                param.type = module_property.type
                module_property.param = param
              end
              setter_method.params = [module_property.param]
              setter_method.run_in_thread = module_property.run_in_thread
              setter_method.is_factory_method = false
              setter_method.is_return_value = false
              setter_method.access = module_property.access
              setter_method.has_callback = ModuleMethod::CALLBACK_NONE
              setter_method.linked_property = module_property
              setter_method.special_behaviour = ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
              setter_method.generateAPI = module_property.generateAPI
              module_property.setter = setter_method

              property_methods << setter_method
            end
          else
            setter_method = ModuleMethod.new()

            setter_method.name = module_property.name + "="
            setter_method.native_name = 'set' + module_property.native_name[0..0].upcase + module_property.native_name[1..module_property.native_name.length-1]
            setter_method.desc = 'setter for "'+ module_property.name + '" property'
            setter_method.params = []
            setter_method.run_in_thread = module_property.run_in_thread
            setter_method.is_factory_method = false
            setter_method.is_return_value = false
            setter_method.access = module_property.access
            setter_method.has_callback = ModuleMethod::CALLBACK_MANDATORY
            setter_method.linked_property = module_property
            setter_method.special_behaviour = ModuleMethod::SPECIAL_BEHAVIOUR_NONE
            setter_method.generateAPI = module_property.generateAPI
            module_property.setter = setter_method

            property_methods << setter_method
          end
        end
      end

      property_methods
    end


    def process_entities(module_item, unsupported_names, xml_module_item)
      xml_module_item.elements.each('ENTITIES/ENTITY') do |xml_module_entity|
        module_entity = ModuleEntity.new()
        module_item.entities << module_entity

        module_entity.name = xml_module_entity.attribute('name').to_s.capitalize_first
        if unsupported_names.include?(module_entity.name.upcase)
          raise "ModuleEntity have invalid name !\n Module[#{module_item.name}].entity[#{module_entity.name}] is in the list of forbidden names:\n '#{unsupported_names.join("','")}'"
        end
        module_entity.native_name = (module_entity.name).split(/[^a-zA-Z0-9\_]/).map { |w| w }.join("")
        self_type = [module_item.parents.join('::'), module_item.name, module_entity.name].join('::')

        module_entity.run_in_thread = ModuleMethod::RUN_IN_THREAD_NONE

        if xml_module_entity.attribute('runInThread') != nil
          module_entity.run_in_thread = xml_module_entity.attribute('runInThread').to_s
        end

        current_field_index = 0

        xml_module_entity.elements.each('FIELDS/FIELD') do |xml_entity_field|
          curr_name = xml_entity_field.attribute('name').to_s

          entity_field = EntityField.new()
          entity_field.field_index = current_field_index
          current_field_index = current_field_index + 1
          module_entity.fields << entity_field
          entity_field.name = curr_name
          entity_field.parent_entity = module_entity

          if xml_entity_field.attribute('type') != nil
            type = xml_entity_field.attribute('type').to_s.upcase

            unless MethodParam::SIMPLE_TYPES.include?(type)
              raise "ModuleEntity have invalid type !\n Module[#{module_item.name}].entity[#{module_entity.name}] = #{type}"
            end
            entity_field.type = type
          end

          if xml_entity_field.attribute('binding') != nil
            if xml_entity_field.attribute('binding').to_s.downcase != 'false'
              entity_field.binding = true
            end
          end

          if xml_entity_field.attribute('const') != nil
            if xml_entity_field.attribute('const').to_s.downcase != 'false'
              entity_field.const = true
            end
          end

          if xml_entity_field.attribute('default') != nil
            entity_field.default_value = xml_entity_field.attribute('default').to_s
          end

          if xml_entity_field.attribute('constructorParam') != nil
            entity_field.constructor_param = xml_entity_field.attribute('constructorParam').to_s
          end

          if entity_field.binding then
            module_entity.binding_fields << entity_field
          else
            module_entity.normal_fields << entity_field
          end
        end

        module_entity.fields.uniq! { |x| x.name }

        if module_entity.binding_fields.size > 0
          module_entity.addr_fields = module_entity.binding_fields
        else
          module_entity.addr_fields = module_entity.fields
        end

        if module_entity.fields.size > 0
          ##############################
          # constructor params with hash
          cnt_hash = []
          module_entity.fields.each do |field|
            result_item = MethodParam.new()
            result_item.name = field.name
            result_item.is_generated = true
            result_item.type = field.type
            result_item.default_value = field.default_value
            cnt_hash << result_item
          end
          cnt_param = MethodParam.new()
          cnt_param.is_generated = true
          cnt_param.type = MethodParam::TYPE_HASH
          cnt_param.sub_params = cnt_hash

          # access params
          binding_param = create_param_from_list(module_entity.binding_fields, 'binding', false, true)
          binding_param.is_generated = true unless binding_param.nil?

          store_fileds = module_entity.fields.select { |f| !f.const }
          store_param = create_param_from_list(store_fileds, 'init_hash', true, true)
          store_param.is_generated = true unless store_param.nil?

          update_fileds = module_entity.fields.select { |f| !f.const && !f.binding }
          update_param = create_param_from_list(update_fileds, 'updates', true, false)
          update_param.is_generated = true unless update_param.nil?

          ## constructor for entity
          #ntt_constructor = ModuleMethod.new()
          #ntt_constructor.name = module_entity.name + '.create'
          #ntt_constructor.native_name = 'create' + module_entity.native_name.capitalize_first
          #ntt_constructor.desc = 'constructor for "' + module_entity.name + '.'
          #//
          #const_fields = module_entity.fields.select { |f| f.constructor_param > 0 }
          #const_fields = const_fields.stable_sort { |a, b| a.constructor_param <=> b.constructor_param }
          #//
          #const_params = param_list_from_fields(const_fields, false)
          #const_params.each do |param|
          #  param.can_be_nil = param.can_be_nil || param.linked_field.constructor_param < 2
          #end
          #
          #ntt_constructor.params = const_params
          #ntt_constructor.run_in_thread = module_entity.run_in_thread
          #ntt_constructor.is_factory_method = false
          #ntt_constructor.is_return_value = true
          #ntt_constructor.access = ModuleMethod::ACCESS_STATIC
          #ntt_constructor.has_callback = ModuleMethod::CALLBACK_NONE
          #ntt_constructor.generateAPI = true
          #
          ## constructor result
          #constr_result = MethodResult.new()
          #constr_result.type = MethodParam::TYPE_HASH
          #constr_result.sub_params = cnt_hash
          #
          #ntt_constructor.result = constr_result
          #ntt_constructor.is_return_value = true
          #
          #module_item.methods << ntt_constructor
          #
          ################
          ## remove method
          #ntt_destructor = ModuleMethod.new()
          #ntt_destructor.name = module_entity.name + '.remove'
          #ntt_destructor.native_name = 'remove' + module_entity.native_name.capitalize_first
          #ntt_destructor.desc = 'remove for "' + module_entity.name
          #if binding_param != nil
          #  ntt_destructor.params = [binding_param]
          #else
          #  ntt_destructor.params = [create_param_from_list(module_entity.fields, true)]
          #end
          #ntt_destructor.run_in_thread = module_entity.run_in_thread
          #ntt_destructor.is_factory_method = false
          #ntt_destructor.is_return_value = true
          #ntt_destructor.access = ModuleMethod::ACCESS_STATIC
          #ntt_destructor.has_callback = ModuleMethod::CALLBACK_NONE
          #ntt_destructor.generateAPI = true
          #module_item.methods << ntt_destructor
        end

        ################################################
        # init method
        ntt_init= ModuleMethod.new()
        module_entity.methods << ntt_init
        ntt_init.access = ModuleMethod::ACCESS_STATIC
        ntt_init.is_static_for_entity = true
        ntt_init.linked_entity = module_entity
        ntt_init.is_generated = true
        module_entity.init_method = ntt_init
        module_item.methods << ntt_init

        ntt_init.name = 'init' + module_entity.name
        ntt_init.native_name = 'init' + module_entity.native_name.capitalize_first
        ntt_init.desc = 'init method for ' + module_entity.name

        if store_param
          ntt_init.params = [store_param]
        end

        ntt_init.run_in_thread = module_entity.run_in_thread
        ntt_init.is_factory_method = false
        ntt_init.is_return_value = true
        ntt_init.has_callback = ModuleMethod::CALLBACK_NONE
        ntt_init.generateAPI = true

        if module_entity.fields.size > 0
          init_result = MethodResult.new()
          init_result.type = MethodParam::TYPE_HASH
          init_result.sub_params = param_list_from_fields(module_entity.fields, true)
          ntt_init.result = init_result
          ntt_init.is_return_value = true
        end

        ################################################
        # update
        update_args = []
        if binding_param != nil
          update_args << binding_param
        end

        if update_param != nil
          update_args << update_param
        end

        if update_args.size > 0
          ntt_update = ModuleMethod.new()
          module_entity.methods << ntt_update
          ntt_update.access = ModuleMethod::ACCESS_STATIC
          ntt_update.is_static_for_entity = true
          ntt_update.linked_entity = module_entity
          ntt_update.is_generated = true
          module_entity.update_method = ntt_update
          module_item.methods << ntt_update

          ntt_update.name = 'update' + module_entity.name
          ntt_update.native_name = 'update' + module_entity.native_name.capitalize_first
          ntt_update.desc = 'update method for ' + module_entity.name

          ntt_update.params = update_args

          ntt_update.run_in_thread = module_entity.run_in_thread
          ntt_update.is_factory_method = false
          ntt_update.is_return_value = false

          if update_fileds != nil
            update_result = MethodResult.new()
            update_result.type = MethodParam::TYPE_HASH
            update_result.sub_params = param_list_from_fields(update_fileds, false)
            ntt_init.result = update_result
            ntt_init.is_return_value = true
          end
        end
        #
        #if (update_param != nil)
        #  ############
        #  # get method
        #  ntt_load = ModuleMethod.new()
        #  ntt_load.linked_entity = module_entity
        #  module_item.methods << ntt_load
        #
        #  ntt_load.name = 'load' + module_entity.name + 'Entity'
        #  ntt_load.native_name = 'load' + module_entity.native_name.capitalize_first
        #  ntt_load.desc = 'load for "' + module_entity.name
        #  if binding_param != nil
        #    ntt_load.params = [binding_param, update_param]
        #  else
        #    ntt_load.params = [create_param_from_list(module_entity.fields, true)]
        #  end
        #  ntt_load.run_in_thread = module_entity.run_in_thread
        #  ntt_load.is_factory_method = false
        #  ntt_load.is_return_value = true
        #  ntt_load.access = ModuleMethod::ACCESS_STATIC
        #  ntt_load.has_callback = ModuleMethod::CALLBACK_NONE
        #  ntt_load.generateAPI = true
        #  # constructor result
        #  get_result = MethodResult.new()
        #  get_result.type = MethodParam::TYPE_HASH
        #  get_result.sub_params = param_list_from_fields(module_entity.fields.select { |f| !f.const }, true)
        #  ntt_load.result = get_result
        #  ntt_load.is_return_value = true
        #end

        xml_entity_methods_item = xml_module_entity.elements['METHODS']
        xml_module_entity.elements.each('METHODS/METHOD') do |xml_module_method|

          entity_method = process_method(xml_module_method, module_item, self_type)
          module_entity.methods << entity_method
          entity_method.linked_entity = module_entity
          module_item.methods << entity_method

          # static methods are not taking hashes as parameters
          is_static = false
          method_attributes = collect_inherited_attributes(xml_module_method, ['access'])
          attr_access = method_attributes['access']

          if attr_access != nil
            if attr_access.downcase == 'static'
              is_static = true
            end
          end
          entity_method.is_static_for_entity = is_static

          if !is_static
            fields = module_entity.fields.select { |f| !f.const }
            params = nil
            if (module_entity.binding_fields.size != 0)
              params = module_entity.binding_fields
            else
              if fields.size != 0
                params = fields
              end
            end
            if params != nil
              hash_param = create_param_from_list(params, 'hash', false, true)
              hash_param.is_generated = true
              entity_method.params.insert(0, hash_param)
            end
          end

          # entity_method.name << module_entity.name + 'Entity'
          entity_method.native_name = (  entity_method.name + module_entity.name + (is_static ? "Static" : "") ).split(/[^a-zA-Z0-9\_]/).map { |w| w }.join("")
          entity_method.binding_name = (  entity_method.name + module_entity.name + (is_static ? "Static" : "") ).split(/[^a-zA-Z0-9\_]/).map { |w| w }.join("")
          entity_method.access = ModuleMethod::ACCESS_STATIC
        end

        # generated methods first, then static, then sort by name
        module_entity.methods = module_entity.methods.stable_sort do|a,b|
          if a.is_generated == b.is_generated
            if a.is_static_for_entity == b.is_static_for_entity
              a.name <=> b.name
            else
              a.is_static_for_entity ? 1 : -1
            end
          else
            a.is_generated ? 1 : -1
          end
        end
      end
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
      xml_api_root = $xml.elements['API']

      if !check_xml_element_for_children_and_attributes(xml_api_root, '')
        if skipwarnings == false
          puts 'ERROR: XML has parsing WARNINGS - stop generation process ! Use "--skipwarnings" option when you run generator for skip parsing warnings.'.red
          return false
        end
      end


      #get compund name for ease of comparation
      def compound_name(xml_item, default_access)
        if !xml_item.nil? && !xml_item.attribute('name').nil?
          name = xml_item.attribute('name').to_s

          if !default_access.nil?
            access = default_access
          else
            if xml_item.name == 'PROPERTY'
              access = ModuleMethod::ACCESS_INSTANCE
            else
              access = ModuleMethod::ACCESS_STATIC
            end
          end

          if !xml_item.attribute('access').nil?
            access = xml_item.attribute('access').to_s
          end

          "#{name}_#{access}"
        else
          'undefined'
        end
      end

      #check if element name is defined and not empty
      def valid_element_name?(current_item)
        name_attr = current_item.attribute('name')

        is_valid = !name_attr.nil? && !name_attr.to_s.strip.empty?
        if !is_valid
          puts "Warning: Element has no name and will be skipped: \n #{current_item.to_s.bold} ".red
        end

        is_valid
      end

      #put item from include_module to item_dict if it is not defined in current_module or item_dict
      def include_module_items(include_module, current_module, item_dict, section, subsection, template)
        # fill item_dict with items from curren_module in the first run
        if item_dict.size == 0
          current_module.elements.each("#{section}/#{subsection}") do |current_item|
            next if !valid_element_name?(current_item)

            default_access = nil
            if !current_item.parent.nil?
              default_access = current_item.parent.attribute('access')
            end

            item_name = compound_name(current_item, default_access)
            item_dict[item_name] = { :item => current_item, :index => item_dict.size, :current => true }
          end
        end

        include_module.elements.each("#{section}/#{subsection}") do |include_item|
          next if !valid_element_name?(include_item)

          default_access = nil
          if !include_item.parent.nil?
            default_access = include_item.parent.attribute('access')
          end

          include_item.add_attribute( TEMPLATE_NAME, template)
          include_item_name = compound_name(include_item, default_access)

          if !item_dict.has_key?(include_item_name)
            item_dict[include_item_name] = { :item => include_item, :index => item_dict.size, :current => false }
          end
        end
      end

      # add items from item_dict to current_module
      def update_current_module(current_module, item_dict, section, subsection)
        xml_module_item_methods = current_module.elements[section]
        existing_methods = []

        #check if xml_module_item_methods section exists
        if xml_module_item_methods.nil?
          xml_module_item_methods = current_module.add_element(section)  
        else
          existing_methods = current_module.get_elements("#{section}/#{subsection}")
        end

        # since we want to get base methods be be first, we need to remove existing methods and then insert them back in the last turn
        existing_methods.each do |method|
          xml_module_item_methods.delete(method)
        end

        list_size = item_dict.size
        # generate key/value pairs
        item_list = item_dict.sort_by { |k, v| v[:index] + (v[:current] ? list_size : 0) }

        item_list.each do |item|
          # get sorted value, not the key
          xml_module_item_methods.add_element item[1][:item]
        end
      end


      # ===============================================================
      # those property types could be mapped to corresponding constants
      supported_simple_types = MethodParam::SIMPLE_TYPES.map(&:upcase)

      unsupported_names = ['__proto__', 'break', 'case', 'catch', 'class', 'const', 'constructor', 'continue', 'debugger', 'default', 'delete', 'do', 'else', 'enum', 'export', 'extends', 'false', 'finally', 'for', 'function', 'if', 'implements', 'import', 'in', 'instanceof', 'interface', 'let', 'name', 'new', 'null', 'package', 'private', 'protected', 'prototype', 'public', 'return', 'static', 'super', 'switch', 'this', 'throw', 'true', 'try', 'typeof', 'var', 'void', 'while', 'with', 'yield'].map(&:upcase)

      xml_api_root.elements.each('MODULE') do |xml_module_item|
        module_item = ModuleItem.new()

        module_item.name = xml_module_item.attribute('name').to_s
        if xml_module_item.attribute('parent') != nil
          parents = xml_module_item.attribute('parent').to_s.split('.')
          module_item.parents = parents
        end

        if xml_module_item.attribute('generateUnderscoreRubyNames') != nil
          module_item.generateUnderscoreRubyNames = xml_module_item.attribute('generateUnderscoreRubyNames').to_s.downcase != 'false'
        end

        module_item.is_template_default_instance = (xml_module_item.elements['TEMPLATES/DEFAULT_INSTANCE'] != nil)
        module_item.is_template_singletone_id = (xml_module_item.elements['TEMPLATES/SINGLETON_INSTANCES'] != nil)
        module_item.is_template_propertybag = (xml_module_item.elements['TEMPLATES/PROPERTY_BAG'] != nil)

        ## process includes
        includes = xml_module_item.elements['TEMPLATES/INCLUDE']
        if (includes != nil)
          property_dict = {}
          const_dict = {}
          method_dict = {}

          xml_module_item.elements.each('TEMPLATES/INCLUDE') do |include_xml|
            base_path = include_xml.attribute('path').to_s
            include_file_name = File.expand_path(File.join(File.dirname(xmlpath),base_path))

            puts "Including #{base_path}"

            if File.exists?(include_file_name)
              xml_f = File.new(include_file_name)
              template_xml = REXML::Document.new(xml_f)
              xml_f.close
              base_modules = template_xml.elements['API'].get_elements('MODULE')

              base_modules.each do |base_module|
                if !base_module.nil?
                  include_module_items(base_module, xml_module_item, property_dict, 'PROPERTIES', 'PROPERTY', include_file_name)

                  include_module_items(base_module, xml_module_item, const_dict, 'CONSTANTS', 'CONSTANT', include_file_name)

                  include_module_items(base_module, xml_module_item, method_dict, 'METHODS', 'METHOD', include_file_name)
                end
              end
            else
              puts "Include file does not exists: #{include_file_name.bold}".red
              return false
            end
          end
          if property_dict.size > 0
            update_current_module(xml_module_item, property_dict, 'PROPERTIES', 'PROPERTY')
          end
          if const_dict.size > 0
            update_current_module(xml_module_item, const_dict, 'CONSTANTS', 'CONSTANT')
          end
          if method_dict.size > 0
            update_current_module(xml_module_item, method_dict, 'METHODS', 'METHOD')
          end
        end

        if module_item.is_template_propertybag
          apply_templates_to_module(xml_module_item, TEMPLATE_PROPERTY_BAG)
        end
        if module_item.is_template_singletone_id
          apply_templates_to_module(xml_module_item, TEMPLATE_SINGLETONE_INSTANCES)
        end
        if module_item.is_template_default_instance
          #apply_templates_to_module(xml_module_item, 'default_instance')
        end

        check_duplicate_descriptions(xml_module_item,'CONSTANT')
        check_duplicate_descriptions(xml_module_item,'PROPERTY')
        check_duplicate_descriptions(xml_module_item,'METHOD')

        #constants in module
        module_item.constants = process_constants(supported_simple_types, xml_module_item)

        # ===============================================================
        # hash keys
        module_item.hash_key_names = process_hash_keys(xml_module_item)

        # ===============================================================
        # properties
        properties = process_properties(module_item, unsupported_names, xml_module_item)
        module_item.properties.concat( properties )
        module_item.methods.concat( generate_property_methods(properties) )

        # ===============================================================
        # entities
        process_entities(module_item, unsupported_names, xml_module_item)

        # ===============================================================
        # methods
        process_methods(module_item, xml_module_item)

        all_methods = module_item.methods.map{|m| (m.access == ModuleMethod::ACCESS_STATIC ? "static method " : "") + m.binding_name}

        non_unique_methods = all_methods.group_by { |e| e }.select { |k, v| v.size > 1 }.map(&:first)
        if non_unique_methods.size > 0
          puts "ERROR: XML has following methods with the same name: #{non_unique_methods.join(', ').bold}".red
          return false
        end

        module_item.methods.each do |method|
          if method.has_callback != ModuleMethod::CALLBACK_NONE && method.result.nil?
            puts "WARNING: Method #{method.name.bold} has callback, but does not have definition of callback result".brown
          end
        end

        xml_module_item.elements.each('ALIASES/ALIAS') do |xml_module_alias|
          add_alias(xml_module_alias, module_item.module_aliases)
        end

        xml_module_item.elements.each('PROPERTIES/ALIASES/ALIAS') do |xml_property_alias|
          add_alias(xml_property_alias, module_item.property_aliases)
        end

        xml_module_item.elements.each('METHODS/ALIASES/ALIAS') do |xml_method_alias|
          add_alias(xml_method_alias, module_item.method_aliases)
        end


        if module_item.generateUnderscoreRubyNames
          # auto generate aliases
          # setup aliases hashes

          property_aliases_hash = {}
          module_item.property_aliases.each do |property_alias|
            property_aliases_hash[property_alias.new_name] = property_alias
          end
          method_aliases_hash = {}
          module_item.method_aliases.each do |method_alias|
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

        module_item.method_aliases.each do |method_alias|
          method_item = foundMethodByName(module_item, method_alias.existing_name)
          if method_item != nil
            if method_item.access == ModuleMethod::ACCESS_STATIC
              method_alias.is_method_static = true
            end
            if method_item.access == ModuleMethod::ACCESS_INSTANCE
              method_alias.is_method_instance = true
              if module_item.is_template_default_instance
                method_alias.is_method_static = true
              end
            end
          end
        end


        # remove methods with generateAPI=true from list of methods for generation
        tmp_methods = module_item.methods
        module_item.methods = []
        tmp_methods.each do |method_item|
          if method_item.generateAPI
            module_item.methods << method_item
          else
            module_item.methods_without_generated_code << method_item
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

    template :shared_stub_api_impl do |template|
      template.source = 'shared/generated/stub/montana_stub_impl.cpp'
      template.destination = "shared/generated/stub/#{namefixed($cur_module.name)}_stub_impl.cpp"
    end

    template :iphone_api do |template|
      template.source = 'platform/iphone/generated/IMontana.h'
      template.destination = "platform/iphone/generated/I#{$cur_module.name}.h"
    end

    template :iphone_ruby_wrapper do |template|
      template.source = 'platform/iphone/generated/ruby/montana_ruby_wrap.m'
      template.destination = "platform/iphone/generated/ruby/#{namefixed($cur_module.name)}_ruby_wrap.m"
    end

    template :iphone_js_wrapper do |template|
      template.source = 'platform/iphone/generated/javascript/montana_js_wrap.mm'
      template.destination = "platform/iphone/generated/javascript/#{namefixed($cur_module.name)}_js_wrap.mm"
    end

    template :iphone_impl_readme do |template|
      template.source = 'platform/iphone/impl/readme.txt'
      template.destination = 'platform/iphone/impl/readme.txt'
    end

    template :iphone_base_impl_01 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaBase.h'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}Base.h"
    end

    template :iphone_base_impl_02 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaBase.m'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}Base.m"
    end

    template :iphone_base_impl_03 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaFactoryBase.h'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}FactoryBase.h"
    end

    template :iphone_base_impl_04 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaFactoryBase.m'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}FactoryBase.m"
    end

    template :iphone_base_impl_05 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaSingletonBase.h'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}SingletonBase.h"
    end

    template :iphone_base_impl_06 do |template|
      template.source = 'platform/iphone/generated/base_impl/MontanaSingletonBase.m'
      template.destination = "platform/iphone/generated/base_impl/#{$cur_module.name}SingletonBase.m"
    end

    template :iphone_stub_impl_01 do |template|
      template.source = 'platform/iphone/generated/stub_impl/Montana.h'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}.h"
    end

    template :iphone_stub_impl_02 do |template|
      template.source = 'platform/iphone/generated/stub_impl/Montana.m'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}.m"
    end

    template :iphone_stub_impl_03 do |template|
      template.source = 'platform/iphone/generated/stub_impl/MontanaFactorySingleton.m'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}FactorySingleton.m"
    end

    template :iphone_stub_impl_04 do |template|
      template.source = 'platform/iphone/generated/stub_impl/MontanaSetup.m'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}Setup.m"
    end

    template :iphone_stub_impl_05 do |template|
      template.source = 'platform/iphone/generated/stub_impl/MontanaSingleton.h'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}Singleton.h"
    end

    template :iphone_stub_impl_06 do |template|
      template.source = 'platform/iphone/generated/stub_impl/MontanaSingleton.m'
      template.destination = "platform/iphone/generated/stub_impl/#{$cur_module.name}Singleton.m"
    end

    template :iphone_stub_impl_readme do |template|
      template.source = 'platform/iphone/generated/stub_impl/readme.txt'
      template.destination = 'platform/iphone/generated/stub_impl/readme.txt'
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


    # wp8
    template :wp8_stub_impl_montana_impl do |template|
      template.source = 'platform/wp8/stub_impl/Montana_impl.cs'
      template.destination = "platform/wp8/generated/stub_impl/#{$cur_module.name}_impl.cs"
    end

    template :wp8_runtime_montana_runtime_h do |template|
      template.source = 'platform/wp8/lib/MontanaRuntime.h'
      template.destination = "platform/wp8/generated/lib/#{$cur_module.name}Runtime.h"
    end

    template :wp8_runtime_montana_runtime_cpp do |template|
      template.source = 'platform/wp8/lib/MontanaRuntime.cpp'
      template.destination = "platform/wp8/generated/lib/#{$cur_module.name}Runtime.cpp"
    end

    template :wp8_lib_montana_factory_h do |template|
      template.source = 'platform/wp8/lib/MontanaFactory.h'
      template.destination = "platform/wp8/generated/lib/#{$cur_module.name}Factory.h"
    end

    template :wp8_lib_montana_impl do |template|
      template.source = 'platform/wp8/lib/Montana_impl.cpp'
      template.destination = "platform/wp8/generated/lib/#{$cur_module.name}_impl.cpp"
    end


    template :public_api_module do |template|
      template.source = 'js/Rho.Montana.js'
      module_name = $cur_module.parents.clone()
      template.destination = "../public/api/generated/#{module_name.push($cur_module.name).join('.')}.js"
    end

    template :monana_entities_rb do |template|
      template.source = 'rb/RhoMontanaEntities.rb'
      module_name = $cur_module.parents.clone().push($cur_module.name).join('')
      if $cur_module.entities.size > 0
        template.destination = "../#{module_name}Entities.rb"
      else
        template.destination = "shared/generated/stub/#{module_name}EntitiesStub.rb"
      end

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

    #first_argument :xmlpath, :required => true, :desc => 'path to XML with API description'


    template :megatest_01 do |template|
      template.source = 'taho.xml'
      template.destination = 'Megatest.xml'
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
  add :iphone_project, IphoneProjectGenerator
  add :iphone_project_prebuild, IphoneProjectPrebuildGenerator

end


=begin
# Stub this method to force 1.8 compatibility (come on templater!)
class Encoding
  def find
    'utf-8'
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
