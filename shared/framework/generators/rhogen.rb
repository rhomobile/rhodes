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
      Generates a new rhodes application.  This will create a new directory with two files:
      application.rb and index.html
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "application name"

    template :application do |template|
      template.source = 'application.rb'
      template.destination = "#{name.camel_case}/application.rb"
    end

    template :index do |template|
      template.source = 'index.erb'
      template.destination = "#{name.camel_case}/index.erb"
    end

    template :layout do |template|
      template.source = 'layout.erb'
      template.destination = "#{name.camel_case}/layout.erb"
    end

  end

  class ModelGenerator < BaseGenerator
    include ActiveSupport::Inflector

    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'model')
    end

    desc <<-DESC
      Generates a new model for a given source.  You must specify name, source_url, and source_id.
      You can also specify an optional attribute list in the form: 'attribute1', 'attribute2', 'attribute3'...
    DESC

    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'

    first_argument :name, :required => true, :desc => "model name"
    second_argument :source_url, :required => true, :desc => "source url"
    third_argument :source_id, :required => true, :desc => "source id"
    fourth_argument :attributes, :as => :array, :default => [], :required => false, :desc => "array of attributes (only string suppported right now)"

    template :config do |template|
      template.source = 'config.rb'
      template.destination = "#{name.camel_case}/config.rb"
    end

    template :index do |template|
      template.source = 'index.erb'
      template.destination = "#{name.camel_case}/index.erb"
    end

    template :edit do |template|
      template.source = 'edit.erb'
      template.destination = "#{name.camel_case}/edit.erb"
    end

    template :new do |template|
      template.source = 'new.erb'
      template.destination = "#{name.camel_case}/new.erb"
    end
    
    template :new do |template|
      template.source = 'show.erb'
      template.destination = "#{name.camel_case}/show.erb"
    end

    template :controller do |template|
      template.source = 'controller.rb'
      template.destination = "#{name.camel_case}/controller.rb"
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
      Generates a new source adapter with the given name and attributes.  You must specify name.
      You can also specify an optional attribute list in the form: 'attribute1', 'attribute2', 'attribute3'...
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