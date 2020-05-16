#!/usr/bin/env ruby

require 'yaml'

file = ARGV[0]
abis = ARGV[1].split(',')

cfg = YAML::load_file(file)

cfg['android']['abis'] = abis

File.write( file, cfg.to_yaml )
