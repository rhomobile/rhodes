#!/usr/bin/env ruby

puts ">>>> Running rho with ruby: #{RbConfig.ruby}, version #{RUBY_VERSION}"
puts ">>>> Command line: #{ARGV}"

rhodir = File.expand_path( File.join( File.dirname(__FILE__),"../" ) )

rhodesbin = File.join( File.dirname(__FILE__), "rhodes" )
bootstrap = File.expand_path( File.join( File.dirname(__FILE__), '..', 'lib', 'build', 'app_bootstrap.rake' ) )

args = ''
ARGV.each { |arg|
  args += arg + ' '
}
args.strip!

rhodes_commands = [
  'api',
  'api_test',
  'app',
  'extension',
  'iphone_project',
  'iphone_project_prebuild',
  'model',
  'spec'
]

if ARGV.length > 0
  command = ARGV[0]
  require File.join( rhodir, 'lib', 'build', 'rho_packages.rb' )  
  DependencyResolver.resolveDependencies( command, 'TODO: GET PLATFORM NAME' )
end


if ARGV.length==0 or rhodes_commands.include?(ARGV[0])
  executable = rhodesbin
else
  executable = 'rake'
  args += " --rakefile \"#{bootstrap}\""
end

cmdline = "#{executable} #{args}"

exec( cmdline )