require 'rubygems'
require 'templater'
require 'thread'
require 'uuid'
require 'yaml'
require 'rexml/document'
require 'fileutils'
require File.join(File.dirname(__FILE__), '/../../lib/build/ExtendedString.rb')

require File.join(File.dirname(__FILE__), '/../../lib/build/jake.rb')
require File.join(File.dirname(__FILE__), 'rhogen_core.rb')

require File.dirname(__FILE__) + '/../../lib/rhodes'

DEBUG_TEMPLATE_RENDERER = false

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

# =================================================================
# Generator

module Rhogen
  extend Templater::Manifold

  desc <<-DESC
    Generate rhodes application
  DESC

  class BaseGenerator < Templater::Generator
    def class_name
      name.gsub('-', '_').modulize
    end

    alias_method :module_name, :class_name
  end

  class RubyAppGenerator < BaseGenerator

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'application')
    end

    desc <<-DESC
      Generates a new ruby-based rhodes application.

      Options:
        --rhoconnect - include rhoconnect-client in application

      Required:
        name        - application name

      Optional:
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :rhoconnect, :desc => '', :as => :boolean, :default => false

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
      @rhoconnectclient_ext = '"rhoconnect-client"' if rhoconnect
      template.source = 'build.yml'
      template.destination = "#{name}/build.yml"
    end

    template :gitignore do |template|
      template.source = 'gitignore'
      template.destination = "#{name}/.gitignore"
    end

    #template :rubyversion do |template|
    #  template.source = 'ruby-version'
    #  template.destination = "#{name}/.ruby-version"
    #end

    #template :gemfile do |template|
    #  template.source = 'Gemfile'
    #  template.destination = "#{name}/Gemfile"
    #end

    template :application do |template|
      template.source = 'app/application.rb'
      template.destination = "#{name}/app/application.rb"
    end

    template :index do |template|
      template.source = 'app/index.erb'
      template.destination = "#{name}/app/index.erb"
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

    directory :resources do |directory|
      directory.source = 'resources'
      directory.destination = "#{name}/resources"
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

  class JavascriptAppGenerator < BaseGenerator

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'application')
    end

    desc <<-DESC
      Generates a new javascript-based rhodes application.

      Options:
        --rhoconnect - include rhoconnect-client in application

      Required:
        name        - application name

      Optional:
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :rhoconnect, :desc => '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => 'application name'
    second_argument :syncserver, :required => false, :desc => 'url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/")'
    third_argument :zip_url, :required => false, :desc => 'optional url to zipfile download of bundle'

    invoke :appResources

    template :config do |template|
      zip_url ||= ''
      syncserver ||= ''
      template.source = 'javascript_rhoconfig.txt'
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
      @rhoconnectclient_ext = '"rhoconnect-client"' if rhoconnect
      template.source = 'javascript_build.yml'
      template.destination = "#{name}/build.yml"
    end

    template :gitignore do |template|
      template.source = 'gitignore'
      template.destination = "#{name}/.gitignore"
    end

    #template :rubyversion do |template|
    #  template.source = 'ruby-version'
    #  template.destination = "#{name}/.ruby-version"
    #end

    #template :gemfile do |template|
    #  template.source = 'Gemfile'
    #  template.destination = "#{name}/Gemfile"
    #end

    template :application do |template|
      template.source = 'app/javascript_index.html'
      template.destination = "#{name}/app/index.html"
    end

    file :applicationScript do |file|
      file.source = 'app/javascript_index.js'
      file.destination = "#{name}/app/index.js"
    end

    template :options do |template|
      template.source = 'app/Settings/javascript_index.html'
      template.destination = "#{name}/app/Settings/index.html"
    end

    template :login do |template|
      template.source = 'app/Settings/javascript_login.html'
      template.destination = "#{name}/app/Settings/login.html"
    end

    template :loading do |template|
      template.source = 'app/loading.html'
      template.destination = "#{name}/app/loading.html"
    end

    file :loadingpng do |file|
      file.source = 'app/loading.png'
      file.destination = "#{name}/app/loading.png"
    end

    directory :icon do |directory|
      directory.source = 'icon'
      directory.destination = "#{name}/icon"
    end

    directory :resources do |directory|
      directory.source = 'resources'
      directory.destination = "#{name}/resources"
    end

    file :androidmanifesterb do |file|
      file.source = 'AndroidManifest.erb'
      file.destination = "#{name}/AndroidManifest.erb"
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


  class NodeJSAppGenerator < BaseGenerator

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'application')
    end

    desc <<-DESC
      Generates a new Node.js-based rhodes application.

      Options:
        --rhoconnect - include rhoconnect-client in application

      Required:
        name        - application name

      Optional:
        syncserver  - url to the rhosync application (i.e. "http://localhost:9292")
        zip_url     - optional url to zipfile download of bundle (this can be your RhoHub Bundle URL)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    option :rhoconnect, :desc => '', :as => :boolean, :default => false

    first_argument :name, :required => true, :desc => 'application name'
    second_argument :syncserver, :required => false, :desc => 'url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/")'
    third_argument :zip_url, :required => false, :desc => 'optional url to zipfile download of bundle'

    invoke :appResources

    template :config do |template|
      zip_url ||= ''
      syncserver ||= ''
      template.source = 'nodejs_rhoconfig.txt'
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
      @rhoconnectclient_ext = '"rhoconnect-client"' if rhoconnect
      template.source = 'nodejs_build.yml'
      template.destination = "#{name}/build.yml"
    end

    template :gitignore do |template|
      template.source = 'gitignore'
      template.destination = "#{name}/.gitignore"
    end

    directory :resources do |directory|
      directory.source = 'resources'
      directory.destination = "#{name}/resources"
    end

    file :androidmanifesterb do |file|
      file.source = 'AndroidManifest.erb'
      file.destination = "#{name}/AndroidManifest.erb"
    end

    directory :public do |directory|
      directory.source = 'nodejs'
      directory.destination = "#{name}/nodejs/"
    end

    template :rakefile do |template|
      template.source = 'Rakefile'
      template.destination = "#{name}/Rakefile"
    end
  end

  class RubyModelGenerator < BaseGenerator
    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'model')
    end

    desc <<-DESC
      Generates a new ruby model for a ruby-based rhodes application.

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
    #  template.destination = "app/#{name.modulize}/config.rb"
    #end

    template :index do |template|
      template.source = 'index.erb'
      template.destination = "app/#{name.modulize}/index.erb"
    end

    template :edit do |template|
      template.source = 'edit.erb'
      template.destination = "app/#{name.modulize}/edit.erb"
    end

    template :new do |template|
      template.source = 'new.erb'
      template.destination = "app/#{name.modulize}/new.erb"
    end

    template :new do |template|
      template.source = 'show.erb'
      template.destination = "app/#{name.modulize}/show.erb"
    end

    template :controller do |template|
      underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
      template.source = 'controller.rb'
      template.destination = "app/#{name.modulize}/#{underscore_name}_controller.rb"
    end

    template :model do |template|
      underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
      template.source = 'model.rb'
      template.destination = "app/#{name.modulize}/#{underscore_name}.rb"
    end

    template :spec do |template|
      underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
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

  class JavascriptModelGenerator < BaseGenerator
    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'model')
    end

    desc <<-DESC
      Generates a new javascript model for a javascript-based rhodes application.

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

    template :javascript_index do |template|
      template.source = "javascript_index.html"
      template.destination = "app/#{name.modulize}/index.html"
    end

    template :javascript_index_script do |template|
      template.source = "javascript_index.js"
      template.destination = "app/#{name.modulize}/index.js"
    end

    template :javascript_model_script do |template|
      template.source = "javascript_model.js"
      template.destination = "app/#{name.modulize}/model.js"
    end

    template :javascript_new do |template|
      template.source = "javascript_new.html"
      template.destination = "app/#{name.modulize}/new.html"
    end

    template :javascript_show do |template|
      template.source = "javascript_show.html"
      template.destination = "app/#{name.modulize}/show.html"
    end

    template :javascript_edit do |template|
      template.source = "javascript_edit.html"
      template.destination = "app/#{name.modulize}/edit.html"
    end


    # template :index do |template|
    #   template.source = 'index.erb'
    #   template.destination = "app/#{name.modulize}/index.erb"
    # end
    #
    # template :edit do |template|
    #   template.source = 'edit.erb'
    #   template.destination = "app/#{name.modulize}/edit.erb"
    # end
    #
    # template :new do |template|
    #   template.source = 'new.erb'
    #   template.destination = "app/#{name.modulize}/new.erb"
    # end
    #
    # template :new do |template|
    #   template.source = 'show.erb'
    #   template.destination = "app/#{name.modulize}/show.erb"
    # end
    #
    # template :controller do |template|
    #   underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
    #   template.source = 'controller.rb'
    #   template.destination = "app/#{name.modulize}/#{underscore_name}_controller.rb"
    # end
    #
    # template :model do |template|
    #   underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
    #   template.source = 'model.rb'
    #   template.destination = "app/#{name.modulize}/#{underscore_name}.rb"
    # end
    #
    # template :spec do |template|
    #   underscore_name = name.modulize.split(/(?=[A-Z])/).map { |w| w.downcase }.join('_')
    #   template.source = 'spec.rb'
    #   template.destination = "app/test/#{underscore_name}_spec.rb"
    # end

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

    def load_plist(fname)
       require 'cfpropertylist'
       plist = CFPropertyList::List.new(:file => fname)
       data = CFPropertyList.native_types(plist.value)
       data
    end

	def get_xcode_version
  		info_path = '/Applications/XCode.app/Contents/version.plist'
  		ret_value = '0.0'
  		if File.exists? info_path
    		hash = load_plist(info_path)
    		ret_value = hash['CFBundleShortVersionString'] if hash.has_key?('CFBundleShortVersionString')
  		else
    		puts '$$$ can not find XCode version file ['+info_path+']'
  		end
  		puts '$$$ XCode version is '+ret_value
  		return ret_value
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

    directory :assets do |directory|
      directory.source = 'Media.xcassets'
      directory.destination = 'project/iphone/Media.xcassets'
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
      xcode_version = get_xcode_version
	  if xcode_version[0].to_i >= 7
        template.source = 'Bremen7.xcodeproj/project.pbxproj'
      end
      if xcode_version[0].to_i >= 8
        template.source = 'Bremen8.xcodeproj/project.pbxproj'
      end
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
      template.source = 'Bremen7_prebuild.xcodeproj/project.pbxproj'
      xcode_version = get_xcode_version
      if xcode_version[0].to_i >= 7
        template.source = 'Bremen7_prebuild.xcodeproj/project.pbxproj'
      end
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
      @uwpLibGUID = puuid.generate
      @uwpStubImplGUID = puuid.generate
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

    template :extension_uwp_vcproject do |template|
      template.source = 'extensions/montana/ext/platform/uwp/Montana.vcxproj'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}.vcxproj"
    end

    template :extension_uwp_vcprojectprops do |template|
      template.source = 'extensions/montana/ext/platform/uwp/montana.props'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}.props"
    end

    template :extension_uwp_vcprojectfilters do |template|
      template.source = 'extensions/montana/ext/platform/uwp/montana.vcxproj.filters'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}.vcxproj.filters"
    end

    template :extension_uwp_impl_cpp do |template|
      template.source = 'extensions/montana/ext/platform/uwp/src/montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/uwp/src/#{namecamelcase}_impl.cpp"
    end

    # uwp: c++ -> c# bridging

    template :extension_uwp_impl_targets do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaImpl.targets'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Impl.targets"
    end

    template :extension_uwp_impl_csproj do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaImpl.csproj'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Impl.csproj"
    end

    template :extension_uwp_impl_props do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaImpl.props'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Impl.props"
    end

    template :extension_uwp_impl_cs do |template|
      template.source = 'extensions/montana/ext/platform/uwp/src/Montana_impl.cs'
      template.destination = "extensions/#{name}/ext/platform/uwp/src/#{namecamelcase}_impl.cs"
    end

    template :extension_uwp_impl_assemblyinfo do |template|
      template.source = 'extensions/montana/ext/platform/uwp/src/Properties/AssemblyInfo.cs'
      template.destination = "extensions/#{name}/ext/platform/uwp/src/Properties/AssemblyInfo.cs"
    end

    template :extension_uwp_lib_vcxproj do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaLib.vcxproj'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Lib.vcxproj"
    end

    template :extension_uwp_lib_vcxproj_filters do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaLib.vcxproj.filters'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Lib.vcxproj.filters"
    end

    template :extension_uwp_lib_props do |template|
      template.source = 'extensions/montana/ext/platform/uwp/MontanaLib.props'
      template.destination = "extensions/#{name}/ext/platform/uwp/#{namecamelcase}Lib.props"
    end


    template :extension_qt_rakefile do |template|
      template.source = 'extensions/montana/ext/platform/qt/Rakefile'
      template.destination = "extensions/#{name}/ext/platform/qt/Rakefile"
    end

    template :extension_qt_pro do |template|
      template.source = 'extensions/montana/ext/platform/qt/Montana.pro'
      #template.destination = "extensions/#{name}/ext/platform/qt/#{namecamelcase}.pro"
      template.destination = "extensions/#{name}/ext/platform/qt/#{namecamelcase()}.pro"
    end

    template :extension_qt_src_impl do |template|
      template.source = 'extensions/montana/ext/platform/qt/src/Montana_impl.cpp'
      template.destination = "extensions/#{name}/ext/platform/qt/src/#{namecamelcase}_impl.cpp"
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
    #<%= name.modulize %>

    #template :spec do |template|
    #  underscore_name = name.modulize.split(/(?=[A-Z])/).map{|w| w.downcase}.join('_')
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
    include RhogenCore

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
            unless $cur_module.nil? || DEBUG_TEMPLATE_RENDERER != true
              data = File.split(action.relative_destination)
              puts "Rendering #{$cur_module.name} : #{action.relative_destination}"
              comosite_name = [$cur_module.name,data[0].gsub(/[\/\.]+/,'_'),data[1]].join('_')
              fname = File.join('/Users/snowyowl/work/rhomobile/rhodes/rendered/',comosite_name)
              File.open(fname,'w') { |io| io << action.render.to_lines.join() }
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
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/I#{$cur_module.name}.java"
    end

    template :android_java_factory_interface do |template|
      template.source = 'platform/android/gen/src/IMontanaFactory.java'
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/I#{$cur_module.name}Factory.java"
    end

    template :android_java_singleton_interface do |template|
      template.source = 'platform/android/gen/src/IMontanaSingleton.java'
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/I#{$cur_module.name}Singleton.java"
    end

    template :android_java_obj_base do |template|
      template.source = 'platform/android/gen/src/MontanaBase.java'
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/#{$cur_module.name}Base.java"
    end

    template :android_java_singleton_base do |template|
      template.source = 'platform/android/gen/src/MontanaSingletonBase.java'
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/#{$cur_module.name}SingletonBase.java"
    end

    template :android_java_factory_singleton do |template|
      template.source = 'platform/android/gen/src/MontanaFactorySingleton.java'
      template.destination = "platform/android/generated/src/#{JavaGen::make_package_path($cur_module)}/#{$cur_module.name}FactorySingleton.java"
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

 # uwp
    template :uwp_stub_impl_montana_impl do |template|
      template.source = 'platform/uwp/stub_impl/Montana_impl.cs'
      template.destination = "platform/uwp/generated/stub_impl/#{$cur_module.name}_impl.cs"
    end

    template :uwp_montana_base_cs do |template|
      template.source = 'platform/uwp/MontanaBase.cs'
      template.destination = "platform/uwp/generated/#{$cur_module.name}Base.cs"
    end

    template :uwp_runtime_montana_runtime_h do |template|
      template.source = 'platform/uwp/lib/MontanaRuntime.h'
      template.destination = "platform/uwp/generated/lib/#{$cur_module.name}Runtime.h"
    end

    template :uwp_runtime_montana_runtime_cpp do |template|
      template.source = 'platform/uwp/lib/MontanaRuntime.cpp'
      template.destination = "platform/uwp/generated/lib/#{$cur_module.name}Runtime.cpp"
    end

    template :uwp_lib_montana_factory_h do |template|
      template.source = 'platform/uwp/lib/MontanaFactory.h'
      template.destination = "platform/uwp/generated/lib/#{$cur_module.name}Factory.h"
    end

    template :uwp_lib_montana_impl do |template|
      template.source = 'platform/uwp/lib/Montana_impl.cpp'
      template.destination = "platform/uwp/generated/lib/#{$cur_module.name}_impl.cpp"
    end

    template :uwp_lib_montana_impl_h do |template|
      template.source = 'platform/uwp/lib/Montana_impl.h'
      template.destination = "platform/uwp/generated/lib/#{$cur_module.name}_impl.h"
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


  add :app, RubyAppGenerator
  add :jsapp, JavascriptAppGenerator
  add :nodejsapp, NodeJSAppGenerator
  add :model, RubyModelGenerator
  add :jsmodel, JavascriptModelGenerator
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
