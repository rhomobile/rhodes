#!/usr/bin/env ruby

require 'yaml'

file = ARGV[0]
abi = ARGV[1]

cfg = YAML::load_file(file)

cfg['android']['abis'] = [ abi ]

File.write( file, cfg.to_yaml )
