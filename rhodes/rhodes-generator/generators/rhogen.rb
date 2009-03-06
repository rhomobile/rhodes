require 'rubygems'
require 'templater'
require 'activesupport'

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
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "application name"

    template :config do |template|
      template.source = 'config.rb'
      template.destination = "#{name}/config.rb"
    end

    template :buildyml do |template|
      template.source = 'build.yml'
      template.destination = "#{name}/build.yml"
    end
    
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
    
    directory :helpers do |directory|
      directory.source = 'app/helpers'
      directory.destination = "#{name}/app/helpers"
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
      
      You must specify the following:
        name - model name
        source_url - url to the source adapter (i.e. "" or "http://rhosync.rhohub.com/apps/myapp/sources/account")
        source_id - unique id for this model (i.e. 500, this is only used on the device to uniquely identify the source)
        
      Optional arguments:
        attributes - optional list of string attributes (i.e. name,industry,progress)
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "model name"
    second_argument :source_url, :required => true, :desc => "source url"
    third_argument :source_id, :required => true, :desc => "source id"
    fourth_argument :attributes, :as => :array, :default => [], :required => false, :desc => "array of attributes (only string suppported right now)"

    template :config do |template|
      template.source = 'config.rb'
      template.destination = "app/#{name.camel_case}/config.rb"
    end

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

    template :controller do |template|
      template.source = 'controller.rb'
      template.destination = "app/#{name.camel_case}/controller.rb"
    end



    def attributes?
      self.attributes && !self.attributes.empty?
    end

  end

  class SourceGenerator < BaseGenerator
    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'source')
    end

    desc <<-DESC
      Generates a new source adapter with the given name.
    DESC

    first_argument :name, :required => true, :desc => "model name"
    second_argument :attributes, :as => :array, :default => [], :required => false, :desc => "array of attributes (only string suppported right now)"

    template :config do |template|
      template.source = 'source_adapter.rb'
      template.destination = "lib/#{name.snake_case}.rb"
    end

  end

  add :app, AppGenerator
  add :model, ModelGenerator
  add :source, SourceGenerator

end
