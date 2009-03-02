Gem::Specification.new do |s|
  s.name = %q{rhodes-generator}
  s.version = RhodesGenerator::VERSION
 
  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Rhomobile dev"]
  s.date = %q{2008-10-24}
  s.description = %q{The Rhodes application framework allows developers to create native mobile applications with the brevity, productivity and power of the Ruby programming language.}
  s.email = ["dev@rhomobile.com"]
  s.executables = []
  s.files = FileList['lib/**/*.rb', '[A-Z]*', '{spec,bin,generators}/**/*'].to_a
  s.has_rdoc = true
  s.homepage = %q{http://www.rhomobile.com/}
  s.rdoc_options = ["--main", "../../README.rdoc", "--line-numbers"]
  s.rubyforge_project = %q{rhodes-generator}
  s.rubygems_version = "#{RhodesGenerator::VERSION}"
  s.summary = "rhodes-generator #{RhodesGenerator::VERSION}"
end
