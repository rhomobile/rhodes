$VERBOSE = nil

if RUBY_PLATFORM =~ /(win|w)32$/
  system("set RUBYOPTS=")
else
  system("unset RUBYOPTS")
end

# Load Rails rakefile extensions
Dir["#{File.dirname(__FILE__)}/*.rake"].each { |ext| load ext }

