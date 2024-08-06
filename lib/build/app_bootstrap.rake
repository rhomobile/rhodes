require 'yaml'

unless File.exist? "build.yml"
  puts "Cannot find build.yml"
  exit 1
end

$app_config = YAML::load_file("build.yml")
$app_path = Dir.getwd

if ENV["RHO_HOME"].nil?
  if $app_config["sdk"]
    rakefilepath = File.join(File.expand_path($app_config['sdk']), 'Rakefile')
  else
    begin
      rakefilepath = File.expand_path( File.join(File.dirname(__FILE__), '..', '..', 'Rakefile' ) )
    rescue
      rakefilepath  = ""	
    end
  end  
else
  rakefilepath = "#{ENV["RHO_HOME"]}/Rakefile"
end

unless File.exist? rakefilepath
  puts "\nCannot find your Rhodes gem or source path: #{rakefilepath}"
  puts "\nIf you have the sdk on your path or have installed the gem this"
  puts "can be resolved by running 'set-rhodes-sdk'"
  puts "\nYou can also set this manually by modifying your build.yml or"
  puts "setting the environment variable RHO_HOME"
  exit 1
end

load rakefilepath