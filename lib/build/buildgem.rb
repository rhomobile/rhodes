#!/usr/bin/env ruby

require 'fileutils'
require 'rake'
require_relative 'jake.rb'

puts "Removing old gem"
rm_rf Dir.glob("rhodes*.gem")
puts "Copying Rakefile"
cp "Rakefile", "rakefile.rb"

puts "Building manifest"
out = ""
Dir.glob("**/*") do |fname|
  # TODO: create exclusion list
  next unless File.file? fname
  next if fname =~ /rhoconnect-client/
  next if fname =~ /^spec\/api_generator_spec/
  next if fname =~ /ruby-standalone/

  out << fname + "\n"
end
File.open("Manifest.txt",'w') {|f| f.write(out)}

puts "Building gem"
Jake.run3('gem build rhodes.gemspec')
