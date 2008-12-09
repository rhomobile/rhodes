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

task :test do
end

$:.unshift(File.join(File.dirname(__FILE__), '..'))
require 'spec/stubs'

desc "Run the specs under spec/models"
Spec::Rake::SpecTask.new do |t|
  t.spec_opts = ['--options', "spec/spec.opts"]
  t.spec_files = FileList['spec/**/*_spec.rb']
  t.rcov = true
  t.rcov_opts = ['--include',
                 '"lib/rhom/*,lib/rho/*"',
                 '-x',
                 'spec,gems,sqlite3/*,lib/date.rb,lib/rational.rb,lib/time.rb,lib/find.rb,config.rb,lib/erb.rb,lib/singleton.rb,lib/pairparser.rb',
                 '--text-report',
                 '--html']
end