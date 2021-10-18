# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{rhodes}
  s.version = File.read('version').chomp
  s.licenses = ['MIT']
  #s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ['TAU Technologies']
  s.date = Time.now
  s.platform = Gem::Platform::RUBY
  s.description = 'Rhodes mobile framework'
  s.email = 'info@tau-platform.com'
  s.extra_rdoc_files = ["README.md", "LICENSE"]

  if File.file?("Manifest.txt") # manifest may be missing when running bundle install
    files = Array.new
    IO .read("Manifest.txt").each_line {|x| files << x.chomp}
    s.files =  files
  end

  s.homepage = 'http://tau-platform.com/'
  s.rdoc_options = ["--inline-source", "--charset=UTF-8", '--exclude=bin', '--exclude=doc', '--exclude=ext', '--exclude=installer', '--exclude=lib/build', '--exclude=lib/commonAPI', '--exclude=lib/extensions', '--exclude=lib/rhodes', '--exclude=lib/test', '--exclude=lib/rhodes.rb', '--exclude=platform', '--exclude=res', '--exclude=spec']
  s.require_paths = ["lib"]
  s.required_ruby_version = '>= 2.3'
  s.add_dependency('templater', '1.0.0')
  s.add_dependency('rake', '12.3.3')
  s.add_dependency('uuid', '2.3.7')
  s.add_dependency('systemu', '2.6.5')
  s.add_dependency('json', '1.8.6')
  s.add_dependency('rest-client', '2.0.2')
  s.add_dependency('net-scp','1.2.1')
  s.add_dependency('net-ssh','6.0.2')
  s.add_dependency('CFPropertyList', '2.2.8')
  s.add_dependency('naturally', '1.3.2')
  s.add_dependency('simctl', '1.6.8')
  s.add_dependency('listen', '3.0.6')
  s.add_dependency('rubyzip', '1.3.0')
  s.add_dependency('rdoc', '4.2.2')
  s.add_dependency('deep_merge','1.1.1')
  s.add_dependency('nokogiri', '1.10.10')

  s.post_install_message = "Thanks for installing Rhodes ! Please visit our website: http://tau-platform.com" if s.respond_to? :post_install_message=
  s.executables << 'rhogen'
  s.executables << 'rhodes'
  s.executables << 'upgrade-rhodes-app'
  s.executables << 'rhodes-setup'
  s.executables << 'set-rhodes-sdk'
  s.executables << 'migrate-rhodes-app'
  s.executables << 'get-rhodes-info'
  #this is a hack, we dont have an extension, but we want to run postinstall code. It's done via the 'building' of an extension
  s.extensions << 'ext/extconf.rb'
  s.summary = %q{The Rhodes framework is the easiest way to develop NATIVE apps with full device capabilities (GPS, PIM, camera, etc.) for any smartphone.}

  s.add_dependency(%q<diff-lcs>, "1.1.2")
end
