## systemu.gemspec
#

Gem::Specification::new do |spec|
  spec.name = "systemu"
  spec.version = "2.6.4"
  spec.platform = Gem::Platform::RUBY
  spec.summary = "systemu"
  spec.description = "universal capture of stdout and stderr and handling of child process pid for windows, *nix, etc."
  spec.license = "same as ruby's"

  spec.files =
["LICENSE",
 "README",
 "README.erb",
 "Rakefile",
 "lib",
 "lib/systemu.rb",
 "samples",
 "samples/a.rb",
 "samples/b.rb",
 "samples/c.rb",
 "samples/d.rb",
 "samples/e.rb",
 "samples/f.rb",
 "systemu.gemspec",
 "test",
 "test/systemu_test.rb",
 "test/testing.rb"]

  spec.executables = []
  
  spec.require_path = "lib"

  spec.test_files = nil

  

  spec.extensions.push(*[])

  spec.rubyforge_project = "codeforpeople"
  spec.author = "Ara T. Howard"
  spec.email = "ara.t.howard@gmail.com"
  spec.homepage = "https://github.com/ahoward/systemu"
end
