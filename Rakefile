require 'find'
require 'erb'
require 'rake/rdoctask'
require 'digest/sha2'
require 'rexml/document'

#Look, another big fat hack. Make it so we can remove tasks from rake -T by setting comment to nil
module Rake
  class Task
    attr_accessor :comment
  end
end

$app_basedir = pwd
chdir File.dirname(__FILE__)

require File.join(pwd, 'lib/build/jake.rb')

load File.join(pwd, 'platform/bb/build/bb.rake')
load File.join(pwd, 'platform/android/build/android.rake')
load File.join(pwd, 'platform/iphone/rbuild/iphone.rake')
load File.join(pwd, 'platform/wm/build/wm.rake')
load File.join(pwd, 'platform/linux/tasks/linux.rake')

def get_dir_hash(dir, init = nil)
  hash = init
  hash = Digest::SHA2.new if hash.nil?
  Dir.glob(dir + "/**/*").each do |f|
    hash << f
    hash.file(f) if File.file? f
  end
  hash
end

namespace "framework" do
  task :spec do
    loadpath = $LOAD_PATH.inject("") { |load_path,pe| load_path += " -I" + pe }

    rhoruby = ""

    if RUBY_PLATFORM =~ /(win|w)32$/
      rhoruby = 'res\\build-tools\\RhoRuby'
    elsif RUBY_PLATFORM =~ /darwin/
      rhoruby = 'res/build-tools/RubyMac'
    else
      rhoruby = 'res/build-tools/rubylinux'
    end
   
    puts `#{rhoruby}  -I#{File.expand_path('spec/framework_spec/app/')} -I#{File.expand_path('lib/framework')} -I#{File.expand_path('lib/test')} -Clib/test framework_test.rb`
  end
end


namespace "config" do
  task :common do
    $startdir = File.dirname(__FILE__)
    $binextensions = []
    buildyml = 'rhobuild.yml'

    buildyml = ENV["RHOBUILD"] unless ENV["RHOBUILD"].nil?
    $config = Jake.config(File.open(buildyml))
    $config["platform"] = $current_platform if $current_platform
    
    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
      $rubypath = "res/build-tools/RhoRuby.exe"
    else
      $all_files_mask = "*"
      if RUBY_PLATFORM =~ /darwin/
        $rubypath = "res/build-tools/RubyMac"
      else
        $rubypath = "res/build-tools/rubylinux"
      end
    end
	
    if $app_path.nil? #if we are called from the rakefile directly, this wont be set
      #load the apps path and config

      $app_path = $config["env"]["app"]
      unless File.exists? $app_path
        puts "Could not find rhodes application. Please verify your application setting in #{File.dirname(__FILE__)}/rhobuild.yml"
        exit 1
      end
      $app_config = Jake.config(File.open(File.join($app_path, "build.yml")))

    end
    Jake.set_bbver($app_config["bbver"].to_s)
    
    extpaths = []

    extpaths << $app_config["paths"]["extensions"] if $app_config["paths"] and $app_config["paths"]["extensions"]
    extpaths << $config["env"]["paths"]["extensions"] if $config["env"]["paths"]["extensions"]
    extpaths << File.join($app_path, "extensions")
    extpaths << "lib/extensions"
    $app_config["extpaths"] = extpaths

    if $app_config["build"] and $app_config["build"] == "release"
      $debug = false
    else
      $debug = true
    end
    
    extensions = []
    extensions += $app_config["extensions"] if $app_config["extensions"] and
       $app_config["extensions"].is_a? Array
    extensions += $app_config[$config["platform"]]["extensions"] if $app_config[$config["platform"]] and
       $app_config[$config["platform"]]["extensions"] and $app_config[$config["platform"]]["extensions"].is_a? Array
    $app_config["extensions"] = extensions
  end

  out = `javac -version 2>&1`
  puts "\n\nYour java bin folder does not appear to be on your path.\nThis is required to use rhodes.\n\n" unless $? == 0
end

def copy_assets(asset)
  
  dest = File.join($srcdir,'apps/public')
  
  cp_r asset + "/.", dest, :preserve => true, :remove_destination => true 
  
end

def clear_linker_settings
  if $config["platform"] == "iphone"
#    outfile = ""
#    IO.read($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj").each_line do |line|
#      if line =~ /EXTENSIONS_LDFLAGS = /
#        outfile << line.gsub(/EXTENSIONS_LDFLAGS = ".*"/, 'EXTENSIONS_LDFLAGS = ""')
#      else
#        outfile << line
#      end
#    end
#    File.open($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj","w") {|f| f.write outfile}
#    ENV["EXTENSIONS_LDFLAGS"] = ""

    $ldflags = ""
  end

end

def add_linker_library(libraryname)
#  if $config["platform"] == "iphone"
#    outfile = ""
#    IO.read($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj").each_line do |line|
#      if line =~ /EXTENSIONS_LDFLAGS = /
#        outfile << line.gsub(/";/, " $(TARGET_TEMP_DIR)/#{libraryname}\";")
#      else
#        outfile << line
#      end
#    end
#    File.open($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj","w") {|f| f.write outfile}
#  end
      simulator = $sdk =~ /iphonesimulator/

      if ENV["TARGET_TEMP_DIR"] and ENV["TARGET_TEMP_DIR"] != ""
        tmpdir = ENV["TARGET_TEMP_DIR"]
      else
        tmpdir = $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
          ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      end
  $ldflags << "#{tmpdir}/#{libraryname}\n" unless $ldflags.nil?
end

def set_linker_flags
  if $config["platform"] == "iphone"
      simulator = $sdk =~ /iphonesimulator/
      if ENV["TARGET_TEMP_DIR"] and ENV["TARGET_TEMP_DIR"] != ""
        tmpdir = ENV["TARGET_TEMP_DIR"]
      else
        tmpdir = $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
          ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      end
      mkdir_p tmpdir unless File.exist? tmpdir
      File.open(tmpdir + "/rhodeslibs.txt","w") { |f| f.write $ldflags }
#    ENV["EXTENSIONS_LDFLAGS"] = $ldflags
#    puts `export $EXTENSIONS_LDFLAGS`
  end

end

def add_extension(path,dest)
  start = pwd
  chdir path if File.directory?(path)

  Dir.glob("*").each { |f| cp_r f,dest unless f =~ /^ext(\/|(\.yml)?$)/ }

  chdir start
end

def init_extensions(startdir, dest)
  extentries = []
  extlibs = [] 
  extpaths = $app_config["extpaths"]
  
  $app_config["extensions"].each do |extname|
    extpath = nil
    extpaths.each do |p|
      ep = File.join(p, extname)
      if File.exists? ep
        extpath = ep
        break
      end
    end

    if extpath.nil?
      begin
        $rhodes_extensions = nil
        require extname
        extpath = $rhodes_extensions[0] unless $rhodes_extensions.nil?
      rescue Exception => e
        
      end

    end

    unless extpath.nil?
      add_extension(extpath, dest)

      if $config["platform"] != "bb"
          extyml = File.join(extpath, "ext.yml")
          if File.file? extyml
            extconf = Jake.config(File.open(extyml))
            entry = extconf["entry"]
            extentries << entry unless entry.nil?
            libs = extconf["libraries"]
            libs = [] unless libs.is_a? Array
            if $config["platform"] == "wm" || $config["platform"] == "win32"
              libs.map! { |lib| lib + ".lib" }
            else
              libs.map! { |lib| "lib" + lib + ".a" }
            end
            extlibs += libs
          end
      end    
    end

  end

  if $config["platform"] != "bb"
      exts = File.join($startdir, "platform", "shared", "ruby", "ext", "rho", "extensions.c")
      exists = []
      File.new(exts, "r").read.split("\n").each do |line|
        next if line !~ /^\s*extern\s+void\s+([A-Za-z_][A-Za-z0-9_]*)/
        exists << $1
      end

      if exists.sort! != extentries.sort!
        File.open(exts, "w") do |f|
          f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
          f.puts "// Generated #{Time.now.to_s}"
          if $config["platform"] == "wm" || $config["platform"] == "win32"
            # Add libraries through pragma
            extlibs.each do |lib|
              f.puts "#pragma comment(lib, \"#{lib}\")"
            end
          end
          extentries.each do |entry|
            f.puts "extern void #{entry}(void);"
          end
          f.puts "void Init_Extensions(void) {"
          extentries.each do |entry|
            f.puts "    #{entry}();"
          end
          f.puts "}"
        end
      end

      extlibs.each { |lib| add_linker_library(lib) }

      set_linker_flags
  end
  
  unless $app_config["constants"].nil?
    File.open("rhobuild.rb","w") do |file|
      file << "module RhoBuild\n"
      $app_config["constants"].each do |key,value|
        value.gsub!(/"/,"\\\"")
        file << "  #{key.upcase} = \"#{value}\"\n"
      end
      file << "end\n"
    end
  end

  if $excludeextlib  
      chdir dest
      $excludeextlib.each {|e| Dir.glob(e).each {|f| rm f}}
  end
end

def common_bundle_start(startdir, dest)
  app = $app_path
  rhodeslib = "lib/framework"

  rm_rf $srcdir
  mkdir_p $srcdir
  mkdir_p dest if not File.exists? dest
  mkdir_p File.join($srcdir,'apps')

  start = pwd
  chdir rhodeslib

  Dir.glob("*").each { |f| cp_r f,dest, :preserve => true }

  chdir dest
  Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
  Dir.glob("**/erb.rb").each {|f| rm f}
  Dir.glob("**/find.rb").each {|f| rm f}
  $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

  chdir start
  clear_linker_settings

  init_extensions(startdir, dest)
  
  chdir startdir
  #throw "ME"
  cp_r app + '/app',File.join($srcdir,'apps'), :preserve => true
  cp_r app + '/public', File.join($srcdir,'apps'), :preserve => true if File.exists? app + '/public'
  cp   app + '/rhoconfig.txt', File.join($srcdir,'apps'), :preserve => true

  app_version = "\r\napp_version='#{$app_config["version"]}'"  
  File.open(File.join($srcdir,'apps/rhoconfig.txt'), "a"){ |f| f.write(app_version) }
  
  unless $debug
    rm_rf $srcdir + "/apps/app/test"
    rm_rf $srcdir + "/apps/app/SpecRunner"
    rm_rf $srcdir + "/apps/app/mspec"
    rm_rf $srcdir + "/apps/app/mspec.rb"
    rm_rf $srcdir + "/apps/app/spec_runner.rb"
  end


  copy_assets($assetfolder) if ($assetfolder and File.exists? $assetfolder)

  chdir File.join($srcdir,'apps')

  replace_platform = $config['platform']
  replace_platform = "bb6" if $bb6
  Dir.glob("**/*.#{replace_platform}.*").each do |file|
    oldfile = file.gsub(Regexp.new(Regexp.escape('.') + replace_platform + Regexp.escape('.')),'.')
    rm oldfile if File.exists? oldfile
    mv file,oldfile
  end
  
  Dir.glob("**/*.wm.*").each { |f| rm f }
  Dir.glob("**/*.iphone.*").each { |f| rm f }
  Dir.glob("**/*.bb.*").each { |f| rm f }
  Dir.glob("**/*.bb6.*").each { |f| rm f }
  Dir.glob("**/*.android.*").each { |f| rm f }
  Dir.glob("**/.svn").each { |f| rm_rf f }
  Dir.glob("**/CVS").each { |f| rm_rf f }

end

def create_manifest
  dir = File.join($srcdir, 'apps')
  #fname = "config.rb"
  fappManifest = File.new( File.join(dir,'app_manifest.txt'), "w")
  dir = File.join($srcdir, 'apps/app')

  Find.find(dir) do |path|

    strDir = File.dirname(path)
    next unless dir == File.dirname(strDir) #one level only
    
    if File.basename(path) == "config.rb"
        puts "******ERROR enumerating models***********"
        puts "Model definition has changed and doesn't use config.rb anymore: '#{path}' "
        puts "You should replace config.rb with <model_name>.rb file as described: "
        puts "http://wiki.rhomobile.com/index.php/Rhom#Rhom_Models"
        puts "*****************************************"
        exit 1
    end
    
    fname = File.dirname(path)
    fname = File.basename(fname)
    modelname = fname.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
    fname = modelname + ".rb"
  
    if File.basename(path) == fname

      relPath = path[dir.length+1, File.dirname(path).length-1]   #relative path
      relPath = relPath[0, relPath.length-3] #remove .rb extension and app
      relPath = File.join(File.dirname(relPath), modelname )
      fappManifest.puts( relPath )

    end
  end

  fappManifest.close()
  
end

def process_exclude_folders
  excl = []

  if $app_config["excludedirs"]
      excl << $app_config["excludedirs"]['all'] if $app_config["excludedirs"]['all']
      excl << $app_config["excludedirs"][$config["platform"]] if $app_config["excludedirs"][$config["platform"]]
  end
      
  if  $config["excludedirs"]    
      excl << $config["excludedirs"]['all'] if $config["excludedirs"]['all']
      excl << $config["excludedirs"][$config["platform"]] if $config["excludedirs"][$config["platform"]]
  end  
  
  if excl
      chdir File.join($srcdir, 'apps')
  
      excl.each do |mask|
        Dir.glob(mask).each {|f| rm_rf f}
      end
  end

end
  
namespace "build" do
  namespace "bundle" do
    task :xruby do
      #needs $config, $srcdir, $excludelib, $bindir
      app = $app_path
      startdir = pwd
      dest =  $srcdir
      xruby =  File.dirname(__FILE__) + '/res/build-tools/xruby-0.3.3.jar'
      compileERB = "lib/build/compileERB/bb.rb"
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"
      
      common_bundle_start(startdir,dest)

      process_exclude_folders()
      
      cp_r File.join(startdir, "platform/shared/db/res/db"), File.join($srcdir, 'apps')
      
      chdir startdir
      
      #create manifest
      create_manifest
      
      #"compile ERB"
      #ext = ".erb"
      #Find.find($srcdir) do |path|
      #  if File.extname(path) == ext
      #    rbText = ERB.new( IO.read(path) ).src
      #    newName = File.basename(path).sub('.erb','_erb.rb')
      #    fName = File.join(File.dirname(path), newName)
      #    frb = File.new(fName, "w")
      #    frb.write( rbText )
      #    frb.close()
      #  end
      #end
      cp   compileERB, $srcdir
      puts "Running bb.rb"

      puts `#{$rubypath} -I#{rhodeslib} "#{$srcdir}/bb.rb"`
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/bb.rb"

      chdir $bindir
      # -n#{$bundleClassName}
      output = `java -jar "#{xruby}" -v -c RhoBundle 2>&1`
      output.each_line { |x| puts ">>> " + x  }
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end
      chdir startdir
      chdir $srcdir
  
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
=begin
      # RubyIDContainer.* files takes half space of jar why we need it?
      Jake.unjar("../RhoBundle.jar", $tmpdir)
      Dir.glob($tmpdir + "/**/RubyIDContainer.class") { |f| rm f }
      rm "#{$bindir}/RhoBundle.jar"
      chdir $tmpdir
      puts `jar cf #{$bindir}/RhoBundle.jar #{$all_files_mask}`      
      rm_rf $tmpdir
      mkdir_p $tmpdir
      chdir $srcdir
=end      
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
      process_exclude_folders
      chdir startdir
      
      create_manifest
      
      cp   compileERB, $srcdir
      puts "Running default.rb"

      puts `#{$rubypath} -I#{rhodeslib} "#{$srcdir}/default.rb"`
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/default.rb"

      cp   compileRB, $srcdir
      puts "Running compileRB"
      puts `#{$rubypath} -I#{rhodeslib} "#{$srcdir}/compileRB.rb"`
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end

      chdir $srcdir
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
  
      chdir startdir

      cp_r "platform/shared/db/res/db", $srcdir 
    end
  end
end


# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

#desc "Get versions"
task :get_version do

  #genver = "unknown"
  iphonever = "unknown"
  #symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"
  

  # File.open("res/generators/templates/application/build.yml","r") do |f|
  #     file = f.read
  #     if file.match(/version: (\d+\.\d+\.\d+)/)
  #       genver = $1
  #     end
  #   end

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

  gemver = "unknown"
  rhodesver = "unknown"
  frameworkver = "unknown"

  File.open("lib/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      gemver =  $1
    end
  end

  File.open("lib/framework/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rhodesver =  $1
    end
  end

  File.open("lib/framework/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      frameworkver =  $1
    end
  end

  

  puts "Versions:"
  #puts "  Generator:        " + genver
  puts "  iPhone:           " + iphonever
  #puts "  Symbian:          " + symver
  #puts "  WinMo:            " + wmver
  puts "  Android:          " + androidver
  puts "  Gem:              " + gemver
  puts "  Rhodes:           " + rhodesver
  puts "  Framework:        " + frameworkver
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

  # File.open("res/generators/templates/application/build.yml","r") { |f| origfile = f.read }
  #   File.open("res/generators/templates/application/build.yml","w") do |f|
  #     f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  #   end
  

  File.open("platform/iphone/Info.plist","r") { |f| origfile = f.read }
  File.open("platform/iphone/Info.plist","w") do |f| 
    f.write origfile.gsub(/CFBundleVersion<\/key>(\s+)<string>(\d+\.\d+\.*\d*)<\/string>/, "CFBundleVersion</key>\n\t<string>#{verstring}</string>")
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
  ["lib/rhodes.rb","lib/framework/rhodes.rb","lib/framework/version.rb"].each do |versionfile|
  
    File.open(versionfile,"r") { |f| origfile = f.read }
    File.open(versionfile,"w") do |f|
      origfile.gsub!(/^(\s*VERSION) = '(\d+\.\d+\.*\d*)'/, '\1 = \''+ verstring + "'")
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
#          Jake.reconfig($config)
 
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
  Dir.glob("**/*") do |fname| 
    # TODO: create exclusion list
    out << fname + "\n" if File.file? fname and not fname =~ /rhosync-client/
  end
  File.open("Manifest.txt",'w') {|f| f.write(out)}

  puts "Loading gemspec"
  spec = Gem::Specification.load('rhodes.gemspec')

  puts "Building gem"
  gemfile = Gem::Builder.new(spec).build
end

task :tasks do
  Rake::Task.tasks.each {|t| puts t.to_s.ljust(27) + "# " + t.comment.to_s}
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


Rake::RDocTask.new do |rd|
  rd.main = "README.textile"
  rd.rdoc_files.include("README.textile", "lib/framework/**/*.rb")
end
Rake::Task["rdoc"].comment=nil
Rake::Task["rerdoc"].comment=nil

task :rdocpush => :rdoc do
  puts "Pushing RDOC. This may take a while"
  `scp -r html/* dev@dev.rhomobile.com:dev.rhomobile.com/rhodes/`
end

namespace "build" do
    #    desc "Build rhosync-client package"
    task :rhosync_client do

        ver = File.read("rhosync-client/version").chomp #.gsub(".", "_")
        zip_name = "rhosync-client-"+ver+".zip"

        bin_dir = "rhosync-client-bin"
        src_dir = bin_dir + "/rhosync-client-"+ver #"/src"        
        shared_dir = src_dir + "/platform/shared"        
        rm_rf bin_dir
        rm    zip_name if File.exists? zip_name
        mkdir_p bin_dir
        mkdir_p src_dir

        cp_r 'rhosync-client', src_dir, :preserve => true
        
        mv src_dir+"/rhosync-client/license", src_dir
        mv src_dir+"/rhosync-client/README.textile", src_dir
        mv src_dir+"/rhosync-client/version", src_dir
        mv src_dir+"/rhosync-client/changelog", src_dir
                
        Dir.glob(src_dir+"/rhosync-client/**/*").each do |f|
		    #puts f
            rm_rf f if f.index("/build/") || f.index(".DS_Store")         
 
        end
		
        mkdir_p shared_dir
        
        Dir.glob("platform/shared/*").each do |f|
            next if f == "platform/shared/ruby" || f == "platform/shared/rubyext" || f == "platform/shared/xruby" || f == "platform/shared/shttpd" ||
                f == "platform/shared/stlport"        
 
            #puts f                
            cp_r f, shared_dir #, :preserve => true                        
        end
        startdir = pwd
        chdir bin_dir
        puts `zip -r #{File.join(startdir, zip_name)} *`
                
        chdir startdir
        
        rm_rf bin_dir
    end
end