# -*- encoding: utf-8 -*-
# stub: templater 1.0.0 ruby lib

Gem::Specification.new do |s|
  s.name = "templater"
  s.version = "1.0.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Jonas Nicklas"]
  s.date = "2009-08-28"
  s.description = "Templater has the ability to both copy files from A to B and also to render templates using ERB. Templater consists of four parts:\n\n- Actions (File copying routines, templates generation and directories creation routines).\n- Generators (set of rules).\n- Manifolds (generator suites).\n- The command line interface.\n\nHierarchy is pretty simple: manifold has one or many public and private generators. Public ones are supposed to be called\nby end user. Generators have one or more action that specify what they do, where they take files, how they name resulting\nfiles and so forth."
  s.email = ["jonas.nicklas@gmail.com"]
  s.extra_rdoc_files = ["History.txt", "Manifest.txt"]
  s.files = ["History.txt", "Manifest.txt"]
  s.homepage = "http://github.com/jnicklas/templater"
  s.rdoc_options = ["--main", "README.rdoc"]
  s.rubyforge_project = "templater"
  s.rubygems_version = "2.2.2"
  s.summary = "Templater has the ability to both copy files from A to B and also to render templates using ERB"

  s.installed_by_version = "2.2.2" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<highline>, [">= 1.4.0"])
      s.add_runtime_dependency(%q<diff-lcs>, [">= 1.1.2"])
      s.add_runtime_dependency(%q<extlib>, [">= 0.9.5"])
      s.add_development_dependency(%q<rspec>, [">= 1.2.8"])
      s.add_development_dependency(%q<hoe>, [">= 2.3.3"])
    else
      s.add_dependency(%q<highline>, [">= 1.4.0"])
      s.add_dependency(%q<diff-lcs>, [">= 1.1.2"])
      s.add_dependency(%q<extlib>, [">= 0.9.5"])
      s.add_dependency(%q<rspec>, [">= 1.2.8"])
      s.add_dependency(%q<hoe>, [">= 2.3.3"])
    end
  else
    s.add_dependency(%q<highline>, [">= 1.4.0"])
    s.add_dependency(%q<diff-lcs>, [">= 1.1.2"])
    s.add_dependency(%q<extlib>, [">= 0.9.5"])
    s.add_dependency(%q<rspec>, [">= 1.2.8"])
    s.add_dependency(%q<hoe>, [">= 2.3.3"])
  end
end
