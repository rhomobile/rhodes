require 'fileutils'
include FileUtils

mv File.dirname(__FILE__) +"/../rakefile.rb", File.dirname(__FILE__) +"/../Rakefile"

cp File.dirname(__FILE__) +"/../rhobuild.yml.example", File.dirname(__FILE__) +"/../rhobuild.yml" unless File.exists? File.dirname(__FILE__) +"/../rhobuild.yml"

#This is the hack, we make all the things to make it look like an extension has compiled

    File.open('Makefile', 'w') { |f| f.write "all:\n\ninstall:\n\n" }
    File.open('make', 'w') do |f|
      f.write '#!/bin/sh'
      f.chmod f.stat.mode | 0111
    end
    File.open('rhodes_postinstallhack' + '.so', 'w') {}
    File.open('rhodes_postinstallhack' + '.dll', 'w') {}
    File.open('nmake.bat', 'w') { |f| }