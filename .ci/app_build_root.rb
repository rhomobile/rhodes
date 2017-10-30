#!/usr/bin/env ruby

require 'yaml'

cfg = YAML.load_file( File.join( File.dirname( __FILE__ ), 'app_cfg.yml' ) )
puts cfg[ENV['RHO_APP']]['build_root']
