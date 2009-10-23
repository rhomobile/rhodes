require 'find'
require 'erb'

chdir File.dirname(__FILE__)

require 'lib/build/jake.rb'

load 'platform/bb/build/bb.rake'
load 'platform/android/build/android.rake'
load 'platform/iphone/rbuild/iphone.rake'
load 'platform/wm/build/wm.rake'
load 'platform/linux/tasks/linux.rake'

namespace "config" do
  task :common do
    $startdir = File.dirname(__FILE__)
    buildyml = 'rhobuild.yml'

    buildyml = ENV["RHOBUILD"] unless ENV["RHOBUILD"].nil?
    $config = Jake.config(File.open(buildyml))
    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
    else
      $all_files_mask = "*"
    end
    if $app_path.nil? #if we are called from the rakefile directly, this wont be set
      #load the apps path and config

      $app_path = $config["env"]["app"]
      $app_config = YAML::load_file($app_path + "/build.yml")

    end
    Jake.set_bbver($app_config["bbver"].to_s)
  end
end

def copy_assets(asset)
  
  dest = File.join($srcdir,'apps/public')
  
  cp_r asset + "/.", dest, :remove_destination => true 
  
end



def common_bundle_start(startdir, dest)
  app = $app_path
  rhodeslib = "lib/framework"

  rm_rf $srcdir
  mkdir_p $srcdir
  mkdir_p dest if not File.exists? dest
  mkdir_p File.join($srcdir,'apps')


  
  chdir rhodeslib
  Dir.glob("*").each { |f|
    src = f
    cp_r src,dest
  }
  chdir dest
  Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
  Dir.glob("**/erb.rb").each {|f| rm f}
  Dir.glob("**/find.rb").each {|f| rm f}
  $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

  chdir startdir
  #throw "ME"
  cp_r app + '/app',File.join($srcdir,'apps')
  cp_r app + '/public', File.join($srcdir,'apps')
  cp   app + '/rhoconfig.txt', File.join($srcdir,'apps')

  copy_assets($assetfolder) if ($assetfolder and File.exists? $assetfolder)
end

def create_manifest
  dir = File.join($srcdir, 'apps')
  fname = "config.rb"
  fappManifest = File.new( File.join(dir,'app_manifest.txt'), "w")

  Find.find(dir) do |path|
    if File.basename(path) == fname

      relPath = path[dir.length+1, File.dirname(path).length-1]   #relative path
      relPath = relPath[0, relPath.length-3] #remove .rb extension
      fappManifest.puts( relPath )

    end
  end

  fappManifest.close()
  
end
  
namespace "build" do
  namespace "bundle" do
    task :xruby do
      #needs $config, $srcdir, $excludelib, $bindir
      app = $app_path
      startdir = pwd
      dest =  $srcdir
      xruby =  File.dirname(__FILE__) + '/res/build-tools/xruby-0.3.3.jar'

      common_bundle_start(startdir,dest)

      if not $config["excludedirs"].nil?
        if $config["excludedirs"].has_key?($config["platform"])
          chdir File.join($srcdir, 'apps')

          excl = $config["excludedirs"][$config["platform"]]
          excl.each do |mask|
            Dir.glob(mask).each {|f| rm_rf f}
          end
        end
      end
      chdir startdir
      
      #create manifest
      create_manifest
      
      #"compile ERB"
      ext = ".erb"
      Find.find($srcdir) do |path|
        if File.extname(path) == ext
          rbText = ERB.new( IO.read(path) ).src
          newName = File.basename(path).sub('.erb','_erb.rb')
          fName = File.join(File.dirname(path), newName)
          frb = File.new(fName, "w")
          frb.write( rbText )
          frb.close()
        end
      end


      chdir $bindir
      puts `java -jar "#{xruby}" -v -c RhoBundle 2>&1`
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end
      chdir startdir
      chdir $srcdir
  
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
      
      puts `jar uf ../RhoBundle.jar apps/#{$all_files_mask}`
      unless $? == 0
        puts "Error creating Rhobundle.jar"
        exit 1
      end
      chdir startdir
    end

    task :noxruby do
      app = $app_path
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"
      compileERB = "lib/build/compileERB/default.rb"
      compileRB = "lib/build/compileRB/compileRB.rb"
      startdir = pwd
      dest = $srcdir + "/lib"      

      common_bundle_start(startdir,dest)

      create_manifest
      
      cp   compileERB, $srcdir
      puts "Running default.rb"

      puts `#{$rubypath} -I#{rhodeslib} #{$srcdir}/default.rb` 
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/default.rb"

      cp   compileRB, $srcdir
      puts "Running compileRB"
      puts `#{$rubypath} -I#{rhodeslib} #{$srcdir}/compileRB.rb` 
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end

      chdir $srcdir
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
  
      chdir startdir

      cp_r "res/build-tools/db", $srcdir 
    end
  end
end


# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

#desc "Get versions"
task :get_version do
  bbver = "unknown"
  iphonever = "unknown"
  #symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"
  

  File.open("rhobuild.yml","r") do |f|
    file = f.read
    if file.match(/version: (\d+\.\d+\.\d+)/)
      bbver = $1
    end
  end

  File.open("platform/iphone/Info.plist","r") do |f|
    file = f.read
    if file.match(/CFBundleVersion<\/key>\s+<string>(\d+\.\d+\.*\d*)<\/string>/)
      iphonever =  $1
    end
  end

  # File.open("platform/symbian/build/release.properties","r") do |f|
  #     file = f.read
  #     major = ""
  #     minor = ""
  #     build = ""
  # 
  #     if file.match(/release\.major=(\d+)/)
  #       major =  $1
  #     end
  #     if file.match(/release\.minor=(\d+)/)
  #       minor =  $1
  #     end
  #     if file.match(/build\.number=(\d+)/)
  #       build =  $1
  #     end
  # 
  #     symver = major + "." + minor + "." + build
  #   end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") do |f|
    file = f.read
    if file.match(/versionName="(\d+\.\d+\.*\d*)"/)
      androidver =  $1
    end
  end

  rhodesver = "unknown"
  rbuildver = "unknown"
  rframever = "unknown"
  rgenver = "unknown"

  File.open("rhodes/rhodes/lib/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rhodesver =  $1
    end
  end

  File.open("lib/build/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rbuildver =  $1
    end
  end

  File.open("lib/framework/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rframever =  $1
    end
  end

  File.open("rhodes/rhodes-generator/lib/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rgenver =  $1
    end
  end
  

  puts "Versions:"
  puts "  Blackberry:       " + bbver
  puts "  iPhone:           " + iphonever
  #puts "  Symbian:          " + symver
  puts "  WinMo:            " + wmver
  puts "  Android:          " + androidver
  puts "  Rhodes:           " + rhodesver
  puts "  Rhodes Build:     " + rbuildver
  puts "  Rhodes Framework: " + rframever
  puts "  Rhodes Generator: " + rgenver
end

#desc "Set version"
task :set_version, [:version] do |t,args|
  throw "You must pass in version" if args.version.nil?
  ver = args.version.split(/\./)
  major = ver[0]
  minor = ver[1]
  build = ver[2]
  
  throw "Invalid version format. Must be in the format of: major.minor.build" if major.nil? or minor.nil? or build.nil?

  verstring = major+"."+minor+"."+build
  origfile = ""

  File.open("rhobuild.yml","r") { |f| origfile = f.read }
  File.open("rhobuild.yml","w") do |f| 
    f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  end
  
  File.open("rhodes/rhodes-generator/generators/templates/application/build.yml","r") { |f| origfile = f.read }
  File.open("rhodes/rhodes-generator/generators/templates/application/build.yml","w") do |f| 
    f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  end

  File.open("platform/iphone/Info.plist","r") { |f| origfile = f.read }
  File.open("platform/iphone/Info.plist","w") do |f| 
    f.write origfile.gsub(/CFBundleVersion<\/key>(\s+)<string>(\d+\.\d+\.*\d*)<\/string>/, "CFBundleVersion</key>\n  <string>#{verstring}</string>")
  end

  # File.open("platform/symbian/build/release.properties","r") { |f| origfile = f.read }
  # File.open("platform/symbian/build/release.properties","w") do |f|
  #   origfile.gsub!(/release\.major=(\d+)/,"release.major=#{major}")
  #   origfile.gsub!(/release\.minor=(\d+)/,"release.minor=#{minor}")
  #   origfile.gsub!(/build\.number=(\d+)/,"build.number=#{build}")
  #   f.write origfile
  # end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") { |f| origfile = f.read }
  File.open("platform/android/Rhodes/AndroidManifest.xml","w") do |f|
    origfile.match(/versionCode="(\d+)"/)
    vercode = $1.to_i + 1
    origfile.gsub!(/versionCode="(\d+)"/,"versionCode=\"#{vercode}\"")
    origfile.gsub!(/versionName="(\d+\.\d+\.*\d*)"/,"versionName=\"#{verstring}\"")

    f.write origfile
  end

  ["rhodes/rhodes/lib/rhodes.rb",
    "lib/build/version.rb",
    "lib/framework/version.rb",
    "lib/framework/rhodes.rb",
    "rhodes/rhodes-generator/lib/version.rb"].each do |versionfile|
  
    File.open(versionfile,"r") { |f| origfile = f.read }
    File.open(versionfile,"w") do |f|
      origfile.gsub!(/VERSION = '(\d+\.\d+\.*\d*)'/, "VERSION = '#{verstring}'")
      origfile.gsub!(/DBVERSION = '(\d+\.\d+\.*\d*)'/, "DBVERSION = '#{verstring}'")
      f.write origfile
    end
  end
  Rake::Task[:get_version].invoke  
end



namespace "buildall" do
  namespace "bb" do
#    desc "Build all jdk versions for blackberry"
    task :production => "config:common" do
      $config["env"]["paths"].each do |k,v|
        if k.to_s =~ /^4/
          puts "BUILDING VERSION: #{k}"
          $app_config["bbver"] = k
          Jake.reconfig($config)
 
          #reset all tasks used for building
          Rake::Task["config:bb"].reenable
          Rake::Task["build:bb:rhobundle"].reenable
          Rake::Task["build:bb:rhodes"].reenable
          Rake::Task["build:bb:rubyvm"].reenable
          Rake::Task["device:bb:dev"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["device:bb:rhobundle"].reenable
          Rake::Task["package:bb:dev"].reenable
          Rake::Task["package:bb:production"].reenable
          Rake::Task["package:bb:rhobundle"].reenable
          Rake::Task["package:bb:rhodes"].reenable
          Rake::Task["package:bb:rubyvm"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["clean:bb:preverified"].reenable

          Rake::Task["clean:bb:preverified"].invoke
          Rake::Task["device:bb:production"].invoke
        end
      end

    end
  end
end

task :gem do
  puts "Removing old gem"
  rm_rf Dir.glob("*.gem")
  puts "Copying Rakefile"
  cp "Rakefile", "rakefile.rb"
  
  puts "Building manifest"
  out = ""
  Dir.glob("**/*") {|fname| out << fname + "\n" if File.file? fname}
  File.open("Manifest.txt",'w') {|f| f.write(out)}

  puts "Loading gemspec"
  spec = Gem::Specification.load('rhodes.gemspec')

  puts "Building gem"
  gemfile = Gem::Builder.new(spec).build
end

task :tasks do
  Rake::Task.tasks.each {|t| puts t.to_s}
end

task :switch_app => "config:common" do
  rhobuildyml = File.dirname(__FILE__) + "/rhobuild.yml"
  if File.exists? rhobuildyml
    config = YAML::load_file(rhobuildyml)
  else
    puts "Cant find rhobuild.yml"
    exit 1
  end
  config["env"]["app"] = $app_path.gsub(/\\/,"/")
  File.open(  rhobuildyml, 'w' ) do |out|
    YAML.dump( config, out )
  end
end
