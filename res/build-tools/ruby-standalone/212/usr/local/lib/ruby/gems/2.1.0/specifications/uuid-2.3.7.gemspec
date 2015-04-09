# -*- encoding: utf-8 -*-
# stub: uuid 2.3.7 ruby lib

Gem::Specification.new do |s|
  s.name = "uuid"
  s.version = "2.3.7"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Assaf Arkin", "Eric Hodel"]
  s.date = "2013-02-18"
  s.description = "UUID generator for producing universally unique identifiers based on RFC 4122\n(http://www.ietf.org/rfc/rfc4122.txt).\n"
  s.email = "assaf@labnotes.org"
  s.executables = ["uuid"]
  s.extra_rdoc_files = ["README.rdoc", "MIT-LICENSE"]
  s.files = ["MIT-LICENSE", "README.rdoc", "bin/uuid"]
  s.homepage = "http://github.com/assaf/uuid"
  s.rdoc_options = ["--main", "README.rdoc", "--title", "UUID generator", "--line-numbers"]
  s.rubygems_version = "2.2.2"
  s.summary = "UUID generator"

  s.installed_by_version = "2.2.2" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<macaddr>, ["~> 1.0"])
    else
      s.add_dependency(%q<macaddr>, ["~> 1.0"])
    end
  else
    s.add_dependency(%q<macaddr>, ["~> 1.0"])
  end
end
