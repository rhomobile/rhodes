# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|

  require 'FileUtils'
  FileUtils.cp "Rakefile", "rakefile.rb"

  s.name = %q{rhodes-core}
  s.version = File.read('version').chomp

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Rhomobile"]
  s.date = Time.now
  s.description = %q{Rhodes mobile framework core installer}
  s.email = %q{dev@rhomobile.com}
  s.extra_rdoc_files = ["README.md", "LICENSE"]
  
  folders_to_include = [
    "./bin",
    "./ext",
    "./lib/build",
    "./lib/rhodes",
    "./platform/android/build",
    "./platform/iphone/rbuild",
    "./platform/wm/build",
    "./platform/wp7/build",
    "./platform/wp8/build",
    "./platform/uwp/build",
    "./platform/linux/tasks/linux.rake",
    "./platform/osx/build/osx.rake",
    "./res/build-tools/ruby-standalone",
    "./res/generators"
  ]
  
  files = [
    'lib/rhodes.rb',
    'Rakefile',
    'rhobuild.yml.example',
    'platform/shared/common/RhoDefs.h'
  ]

  require 'find'

  folders_to_include.each { |folder|
    Find.find( folder ) { |file|
      files << file
    }
  }

  s.files =  files
  s.homepage = %q{http://www.rhomobile.com}

  s.require_paths = ["lib"]

  s.executables << 'rho'
  #this is a hack, we dont have an extension, but we want to run postinstall code. It's done via the 'building' of an extension
  s.extensions << 'ext/extconf-core.rb'
  s.summary = %q{The Rhodes framework is the easiest way to develop NATIVE apps with full device capabilities (GPS, PIM, camera, etc.) for any smartphone.}
end

