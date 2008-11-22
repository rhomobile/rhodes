require 'rubygems'
require 'templater'

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
    
    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'
    
    first_argument :name, :required => true, :desc => "application name"
    
    template :application do |template|
      template.source = 'application.rb'
      template.destination = "#{name}/application.rb"
    end
    
    template :index do |template|
      template.source = 'index.html'
      template.destination = "#{name}/index.html"
    end
    
  end
  
  class ModelGenerator < BaseGenerator
    
    def self.source_root
      File.join(File.dirname(__FILE__), 'templates', 'model')
    end
    
    #option :testing_framework, :desc => 'Specify which testing framework to use (spec, test_unit)'
    
    first_argument :name, :required => true, :desc => "model name"
    second_argument :source_url, :required => true, :desc => "source url"
    third_argument :source_id, :required => true, :desc => "source id"
    
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
    
    template :controller do |template|
      template.source = 'controller.rb'
      template.destination = "#{name.camel_case}/controller.rb"
    end
    
  end
  
  add :app, AppGenerator
  add :model, ModelGenerator
  
end