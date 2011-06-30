# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "rhomobile-debug"
  s.version = "1.0"
  s.authors = ["Rhomobile"]
  s.date    = Time.now.strftime('%Y-%m-%d')
  s.description = %q{Rhomobile debugger extension}
  s.email = %q{dev@rhomobile.com}
  s.homepage    = %q{http://rhomobile.com}
  s.summary     = %q{Rhomobile debugger extension for RhoStudio}

  s.rubyforge_project = nil
  s.rubygems_version = %q{1.5.0}
  s.require_paths = ["lib"]

  s.extra_rdoc_files = ["README.md", "LICENSE"]
  s.files = Array.new
  s.files << "lib/debugger.rb"
  s.files << "CHANGELOG"
end

