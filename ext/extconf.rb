require 'fileutils'
include FileUtils

mv File.dirname(__FILE__) +"/../rakefile.rb", File.dirname(__FILE__) +"/../Rakefile"

cp File.dirname(__FILE__) +"/../rhobuild.yml.example", File.dirname(__FILE__) +"/../rhobuild.yml" unless File.exists? File.dirname(__FILE__) +"/../rhobuild.yml"

#hack upon hack. Since build files are modified, all the files need to be writable. 
#Only because iphone sim doesn work if you run it with sudo
chmod_R 0777, File.dirname(__FILE__) + "/.."

#This is the hack, we make all the things to make it look like an extension has compiled

    File.open('Makefile', 'w') { |f| f.write "all:\n\ninstall:\n\n" }
    File.open('make', 'w') do |f|
      f.write '#!/bin/sh'
      f.chmod f.stat.mode | 0111
    end
    File.open('rhodes_postinstallhack' + '.so', 'w') { |f| f.chmod 0777}
    File.open('rhodes_postinstallhack' + '.dll', 'w') { |f| f.chmod 0777}
    File.open('nmake.bat', 'w') { |f| f.write "ECHO \"Done\""
		f.chmod 0777}
