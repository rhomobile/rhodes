require 'rubygems'
require 'templater'

module Rhogen
  
  extend Templater::Manifold
  
  desc <<-DESC
    Generate rhodes application
  DESC
  
  # class BaseGenerator < Templater::Generator
  #     def class_name
  #       name.gsub('-', '_').camel_case
  #     end
  #     
  #     alias_method :module_name, :class_name
  #   end
  
  class AppGenerator < Templater::Generator
    
    def class_name
      name.gsub('-', '_').camel_case
    end
    
    def self.source_root
      File.join(File.dirname(__FILE__), 'templates')
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
  
  class ModelGenerator < Templater::Generator
  end
  
  add :app, AppGenerator
  add :model, ModelGenerator
  
end