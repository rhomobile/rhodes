begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  require 'spec'
end
begin
  require 'spec/rake/spectask'
rescue LoadError
  puts <<-EOS
To use rspec for testing you must install rspec gem:
    gem install rspec
EOS
  exit(0)
end

desc "Run the specs under spec/models"
Spec::Rake::SpecTask.new do |t|
  t.spec_opts = ['--options', "spec/spec.opts"]
  t.spec_files = FileList['spec/**/*_spec.rb']
  t.rcov = true
  t.rcov_opts = ['--include',
                 '"lib/rhom/*,lib/rho/*"',
                 '--exclude',
                 '"spec/*,lib/date*,lib/rational*,lib/time*,lib/find.rb,config.rb, \
                 lib/pairparser.rb,/Library/Ruby/Gems/*,/var/lib/gems/*,/usr/lib/ruby"',
                 '--text-report',
                 '--html']
end
