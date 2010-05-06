#

def freplace( fname, pattern, str )
  f = File.open( fname )
  strings = f.read
  f.close

  strings.gsub!( pattern, str )

  f = File.new( fname, "w" )
  f.print strings
  f.close
end

def startmds
  mdshome =  $config["env"]["paths"][$bbver]["mds"]
  args = []
  args << "/c"
  args << "run.bat"

  Jake.run("cmd.exe",args, mdshome,true)
end 

def stopmds
  mdshome =  $config["env"]["paths"][$bbver]["mds"]
  args = []
  args << "/c"
  args << "shutdown.bat"

  Jake.run("cmd.exe",args, mdshome, true, true)
end 

def startsim
  sim = $config["env"]["paths"][$bbver]["sim"]
  jde = $config["env"]["paths"][$bbver]["jde"]
    
  command =  jde + "/simulator/fledge.exe"
  args = []
  args << "/app=Jvm.dll"
  args << "/handheld=" + sim
  args << "/session=" + sim
  args << "/app-param=DisableRegistration"
  args << "/app-param=JvmAlxConfigFile:"+sim+".xml"
  args << "/data-port=0x4d44"
  args << "/data-port=0x4d4e"
  args << "/pin=0x2100000A"
  args << "/no-compact-filesystem"
  args << "/JvmDisableBacklightTimeout"
  #args << "/keep-lcd-on"
    
  if $bbver !~ /^4\.[012](\..*)?$/
    args << "/sdcard-inserted=true"
    args << "/fs-sdcard=true"
  end
  if $bbver !~ /^4\.[01235](\..*)?$/
    args << "/no-guibacklight"
  end
        
  args << "/app-param=JvmDebugFile:"+Jake.get_absolute($app_config["applog"])

  Jake.run2 command, args, {:directory => jde + "/simulator", :nowait => true}
end

def stopsim
  sim = $config["env"]["paths"][$bbver]["sim"]
  jde = $config["env"]["paths"][$bbver]["jde"]
    
  command = jde + "/simulator/fledgecontroller.exe"
  args = []
  args << "/session="+sim
  args << "/execute=Exit(true)"
  Jake.run2 command, args, {:directory => jde + "/simulator", :nowait => true}
end

def manualsign
  java = $config["env"]["paths"]["java"] + "/java.exe"
  jde = $config["env"]["paths"][$bbver]["jde"]

  args = []
  args << "-jar"
  args << jde + "/bin/SignatureTool.jar"
  args << "-r"
  args << $targetdir

  puts Jake.run(java,args)
  $stdout.flush

end

def autosign
  java = $config["env"]["paths"]["java"] + "/java.exe"
  jde = $config["env"]["paths"][$bbver]["jde"]

  args = []
  args << "-jar"
  args << jde + "/bin/SignatureTool.jar"
  args << "-c"
  args << "-a"
  args << "-p"
  args << $config["build"]["bbsignpwd"]
  args << "-r"
  args << $targetdir

  puts Jake.run(java,args)
  $stdout.flush

end

def create_alx_file(src,trg)
    rbText = ERB.new( IO.read($builddir + "/" + src + "Alx.erb") ).result
	fAlx = File.new($targetdir + "/" + trg + ".alx", "w")
    fAlx.write(rbText)
    fAlx.close()
end


namespace "config" do
  task :bb => ["config:common"] do
    $config["platform"] = "bb"

    $rubypath = "res/build-tools/RhoRuby.exe" #path to RhoRuby

    $bbver = $app_config["bbver"].to_s
    $builddir = $config["build"]["bbpath"] + "/build"
    $bindir = $app_path + "/bin"
    $rhobundledir =  $app_path + "/RhoBundle"
    $srcdir =  $bindir + "/RhoBundle"
    $preverified = $app_path + "/preverified"
    $targetdir = $bindir + "/target/" + $bbver
    $rubyVMdir = $app_path + "/RubyVM"
    $excludelib = ['**/singleton.rb','**/rational.rb','**/rhoframework.rb','**/dateOrig.rb']
    $excludeextlib = ['rexml/parsers/baseparser.rb', 'rexml/set.rb']
    $compileERB = $app_path + "/build/compileERB.rb"
    $tmpdir =  $bindir +"/tmp"

    $assetfolder = $app_path + "/public-" + "bb-" + $bbver

    $appname = $app_config["name"].nil? ? "rhodesApp" : $app_config["name"]
    $outfilebase = $appname.gsub(/[^A-Za-z_0-9]/, '_')
    $bundleClassName = $outfilebase + '_'  unless $bundleClassName
    
    $rhobundleimplib = $config["env"]["paths"][$bbver]["jde"] + "/lib/net_rim_api.jar;" +
      $preverified+"/RubyVM.jar"
    $rhodesimplib = $rhobundleimplib + ";"+ $preverified+"/RhoBundle.jar"
    

    needsclean = true
    if File.exists? "#{$bindir}/lastver"
      lastver = IO.read("#{$bindir}/lastver").strip
      needsclean = false if $bbver == lastver
    end

    Rake::Task["clean:bb:all"].invoke if needsclean
    
    mkdir_p $bindir unless File.exists? $bindir
    rm_rf $tmpdir
    mkdir_p $tmpdir unless File.exists? $tmpdir
    mkdir_p  $targetdir if not FileTest.exists?  $targetdir

    File.open("#{$bindir}/lastver", "w") {|f| f.write($bbver)}

  end
end

namespace "build" do
  namespace "bb" do
    task :alx => ["config:bb"] do
        create_alx_file('rhodesApp', $outfilebase)
        create_alx_file('RhoBundle', 'RhoBundle')
    end
    
    def runPreverify(args)  
      jdehome = $config["env"]["paths"][$bbver]["jde"]
    
      startdir = pwd      
      chdir $tmpdir
      puts Jake.run(jdehome + "/bin/preverify.exe",args)
      chdir startdir
      
      unless $? == 0
        rm_rf $tmpdir
        puts "Error preverifying"
        exit 1
      end
      $stdout.flush
    end
    
#    desc "Build rhoBundle"
    #XXX change to ns build, rhobundle
    task :rhobundle => :rubyvm do
      java = $config["env"]["paths"]["java"] + "/java.exe"
      jdehome = $config["env"]["paths"][$bbver]["jde"]
      jarexe =  $config["env"]["paths"]["java"] + "/jar.exe"
      
      #common bundle task goes here#
      Rake::Task["build:bundle:xruby"].execute

      rm_rf $srcdir
      rm_rf $preverified + "/RhoBundle.jar" if File.exists? $preverified + "/RhoBundle.jar"
      rm_rf $rhobundledir + "/RhoBundle.jar" if File.exists? $rhobundledir + "/RhoBundle.jar"

      #XXX make preverify function in Jake
      args = []
      #args << "-verbose"
      args << "-classpath"
      args << jdehome + "/lib/net_rim_api.jar;"+$preverified+"/RubyVM.jar"
      args << "-d"
      args << $preverified
      args << $bindir + "/RhoBundle.jar"
      runPreverify(args)
      
      mkdir_p $rhobundledir unless File.exists? $rhobundledir
      cp $preverified + "/RhoBundle.jar", $rhobundledir + "/RhoBundle.jar"
      
    end

    task :set_dev_outname do
        $bundleClassName = "rhodes_"
    end
    
    task :devrhobundle => [:set_dev_outname,:rhobundle] do
      cp $preverified + "/RhoBundle.jar", "platform/bb/RhoBundle/RhoBundle.jar"
      
      sdcardpath = $config["env"]["paths"][$bbver]["jde"] +"/simulator/sdcard/Rho/rhodes/apps/rhoconfig.txt"
      cp $app_path+"/rhoconfig.txt", sdcardpath if File.exists? sdcardpath
    end

    task :gensources => "config:bb" do
      caps = $app_config["capabilities"]
      caps = [] if caps.nil?
      caps = [] unless caps.is_a? Array

      has_push = caps.index("push") != nil

      puts "Modify rhodes.jdp"
      $stdout.flush
      jdp = File.join($builddir, "..", "..", "..", "platform", "bb", "rhodes", "rhodes.jdp")
      File.open(File.join($tmpdir, "rhodes.jdp"), "w") do |r|
        File.open(jdp, "r") do |f|
          while line = f.gets
            line = "RunOnStartup=#{has_push ? 1 : 0}" if line =~ /^\s*RunOnStartup\s*=/
            r.puts line
          end
        end
      end
      rm_f jdp
      cp File.join($tmpdir, "rhodes.jdp"), jdp

      puts "Modify Capabilities.java"
      $stdout.flush
      capabilities = File.join($builddir, "..", "..", "..", "platform", "shared", "rubyJVM", "src", "com", "rho", "Capabilities.java")
      File.open(capabilities, 'w') do |f|
        f.puts "package com.rho;"
        f.puts ""
        f.puts "public class Capabilities {"
        f.puts "  public static final boolean ENABLE_PUSH = #{has_push.to_s};"
        f.puts "  public static final boolean RUNAS_SERVICE = #{has_push.to_s};"
        f.puts "}"
      end
    end
    
#    desc "Build RubyVM"
    task :rubyvm => [:gensources, "config:bb"] do
      javac = $config["env"]["paths"]["java"] + "/javac.exe"
      jdehome = $config["env"]["paths"][$bbver]["jde"]

      rubyvmfiles = File.readlines($builddir + '/RubyVM_build.files').map { |l| l.gsub!(/\\/,'/').strip! }

      if not uptodate?($preverified + '/RubyVM.jar',rubyvmfiles)

        mkdir_p $tmpdir + "/RubyVM" if not FileTest.exists? $tmpdir + "/RubyVM"
        mkdir_p  $targetdir if not FileTest.exists?  $targetdir
        mkdir_p  $preverified if not FileTest.exists?  $preverified

        args = []
        args << "-g"
        args << "-d"
        args << $tmpdir + '/RubyVM'
        args << "-bootclasspath"
        args << $config["env"]["paths"][$bbver]["jde"] + '/lib/net_rim_api.jar'
        args << "-source"
        args << "1.3"
        args << "-target"
        args << "1.3"
        args << "-nowarn"
        args << "@#{$builddir}/RubyVM_build.files"
        puts Jake.run(javac,args)
        unless $? == 0
          puts "Error compiling java code"
          exit 1
        end
        $stdout.flush
        #XXX Move to task/function
        args = []
        args << "-classpath"
        args << jdehome + "/lib/net_rim_api.jar"
        args << "-d"
        args << $tmpdir + "/RubyVM.preverify"
        args << $tmpdir + "/RubyVM"
        runPreverify(args)

        Jake.jar($preverified+"/RubyVM.jar", $builddir + "/RubyVM_manifest.mf", $tmpdir + "/RubyVM.preverify",true)
        $stdout.flush
      else
        puts 'RubyVM.jar is up to date'
        $stdout.flush
      end

      rm_rf $tmpdir
      mkdir_p $tmpdir
    end

#    desc "Build rhodes"
    task :rhodes => [ :rubyvm, :rhobundle ] do
      javac = $config["env"]["paths"]["java"] + "/javac.exe"
      jde =  $config["env"]["paths"][$bbver]["jde"]
      jdehome = $config["env"]["paths"][$bbver]["jde"]

      sources = Dir.glob($builddir + "/../rhodes/resources/**/*") |
      File.readlines($builddir + '/hsqldb_build.files').map { |l| l.gsub!(/\\/,'/').strip! } |
      File.readlines($builddir +'/rhodes_build.files').map { |l| l.gsub!(/\\/,'/').strip! }

      sources.delete(nil)

      if not FileUtils.uptodate?($preverified + "/rhodes.jar",sources)

        $tmpdir.gsub!(/\\/, '/')
        vsrclist = $tmpdir + "/vsrc_build.files"

#        vsrclist = $builddir + "/../bin/vsrc_build.files"
#        mkdir_p $builddir + "/../bin" unless File.exists? $builddir + "/../bin"

        vsrcdir = $tmpdir + "/vsrc"
        mkdir_p vsrcdir
        cp_r $builddir + "/../rhodes/platform/common/.", vsrcdir
        if File.exist?( $builddir + "/../rhodes/platform/" + $bbver )
          cp_r $builddir + "/../rhodes/platform/" + $bbver + "/.", vsrcdir, :remove_destination => true
        end

        # Modify sources to get different class names due to BB limitation -
        # there can not be two or more applications installed which contains the same
        # class names which implements Persistable interface. See details here -
        # http://supportforums.blackberry.com/rim/board/message?board.id=java_dev&thread.id=11152
        mkdir_p vsrcdir + "/com/rho/file"
        cp_r $builddir + "/../rhodes/src/com/rho/file/PersistRAFileImpl.java", vsrcdir + "/com/rho/file"
        freplace( vsrcdir + "/com/rho/file/PersistRAFileImpl.java", /FileInfoWrapper/, $outfilebase + "_FileInfoWrapper" )
        freplace( vsrcdir + "/com/rho/file/PersistRAFileImpl.java", /PageWrapper/, $outfilebase + "_PageWrapper" )

        fvsrc = File.new( vsrclist, "w" )
        Dir.glob( vsrcdir + "/**/*.java" ).each do |line|
          fvsrc.puts line
        end
        fvsrc.close

        args = []
        args << "-g"
        args << "-d"
        args << $tmpdir
        args << "-classpath"
        args << $bindir + "/RhoBundle.jar;"+$preverified+"/RubyVM.jar"
        args << "-bootclasspath"
        args << jde + "/lib/net_rim_api.jar"
        args << "-source"
        args << "1.3"
        args << "-target"
        args << "1.3"
        args << "-nowarn"
        args << "@#{vsrclist}"
        #args << "@RubyVM_build.files"
        args << "@#{$builddir}/hsqldb_build.files"
        args << "@#{$builddir}/rhodes_build.files"
        puts "\texecuting javac"
        puts Jake.run(javac,args)
        unless $? == 0
          puts "Error compiling java code"
          exit 1
        end
        $stdout.flush

        rm_rf vsrcdir

        cp_r $builddir + "/../rhodes/resources", $tmpdir + "/resources"
        cp $app_path + "/icon/icon.png", $tmpdir +"/resources"
        
        Jake.jar($bindir + "/rhodes.jar", $builddir + "/manifest.mf", $tmpdir,true)
        $stdout.flush
        args = []
        args << "-classpath"
        args << $rhodesimplib
        args << "-d"
        args << $preverified
        args << $bindir + "/rhodes.jar"
        runPreverify(args)
      else
        puts "rhodes up to date"
      end

    end
  end
end

namespace "package" do
  namespace "bb" do
#    desc "Package rhoBundle"
    task :rhobundle => ["build:bb:rhobundle"] do
      Jake.rapc("RhoBundle",
        $targetdir,
        $rhobundleimplib ,
        $preverified + "/RhoBundle.jar",
        "RhoBundle",
        $app_config["vendor"],
        $app_config["version"]
      )
      unless $? == 0
        puts "Error in RAPC"
        exit 1
      end
      
      create_alx_file('RhoBundle', 'RhoBundle')
    end

#    desc "Package rubyVM"
    task :rubyvm => "build:bb:rubyvm" do
      jdehome = $config["env"]["paths"][$bbver]["jde"]

      if not FileUtils.uptodate?($targetdir + '/RubyVM.cod',$preverified + "/RubyVM.jar")
        Jake.rapc("RubyVM",
          $targetdir,
          jdehome + "/lib/net_rim_api.jar",
          $preverified + "/RubyVM.jar",
          "RubyVM",
          $app_config["vendor"],
          $app_config["version"]
        )
        unless $? == 0
          puts "Error in RAPC"
          exit 1
        end
        $stdout.flush
      else
        puts 'RubyVM .cod files are up to date'
        $stdout.flush
      end

    end

#    desc "Package rhodesApp"
    task :rhodes => ["build:bb:rhodes"] do
      if not FileUtils.uptodate?($targetdir + '/' + $outfilebase + '.cod',$preverified + "/rhodes.jar")
        Jake.rapc($outfilebase,
          $targetdir,
          $rhodesimplib,
          $preverified + "/rhodes.jar",
          $appname,
          $app_config["vendor"],
          $app_config["version"],
          "resources/icon.png",
          false,
          true
        )
        unless $? == 0
          puts "Error in RAPC"
          exit 1
        end
        $stdout.flush
        
        create_alx_file('rhodesApp', $outfilebase)
      else
        puts 'rhodes .cod files are up to date'
        $stdout.flush
      end
          
    end

#    desc "Package all production (all parts in one package)"
    task :production => ["build:bb:rhodes"] do
      jdehome = $config["env"]["paths"][$bbver]["jde"]
      rm_rf $tmpdir
      mkdir_p $tmpdir

      rm_rf $targetdir
      mkdir_p $targetdir

      Jake.unjar($preverified + "/RubyVM.jar", $tmpdir)
      Jake.unjar($preverified + "/RhoBundle.jar", $tmpdir)
      Jake.unjar($preverified + "/rhodes.jar", $tmpdir)

      if $bbver =~ /^4\.[012](\..*)$/
        max_size = 65536
        Dir.glob( $tmpdir + "/**/*" ).each do |f|
          if File.size( f ) > max_size
            puts "File size of " + f + " is more than " + max_size.to_s + " bytes"
            puts "There is no ability to pack this file into .cod file for BB " + $bbver
            puts "Please reduce its size and try again"
            $stdout.flush
            Process.exit
          end
        end
      end

      Jake.jar($bindir + "/" + $outfilebase + ".jar",$builddir + "/manifest.mf",$tmpdir,true)

      Jake.rapc($outfilebase,
        $targetdir,
        jdehome + "/lib/net_rim_api.jar",
        $bindir + "/" + $outfilebase + ".jar",
        $appname,
        $app_config["vendor"],
        $app_config["version"],
        "resources/icon.png",
        false,
        true
      )
      unless $? == 0
        puts "Error in RAPC"
        exit 1
      end
      $stdout.flush
      
      create_alx_file('rhodesApp', $outfilebase)
    end

    task :set_dev_build do
        $dev_build = true
    end
    
#    desc "Package all dev (each part in separate package)"
    task :dev => [ :set_dev_build, :rubyvm,:rhobundle,:rhodes] do
    end
  end
end




namespace "device" do
  namespace "bb" do
    desc "Build debug for device"
    task :debug => "package:bb:dev" do

      #make into functions
      if $config["build"]["bbsignpwd"] and $config["build"]["bbsignpwd"] != ""
        autosign
      else
        manualsign
      end

      rm_rf $targetdir + "/web"
      mkdir_p $targetdir + "/web"

      cp $targetdir + "/RhoBundle.jad", $targetdir + "/web"
      cp $targetdir + "/"+$outfilebase+".jad", $targetdir + "/web"
      cp $targetdir + "/RubyVM.jad", $targetdir + "/web"

      Jake.unjar($targetdir + "/RhoBundle.cod", $targetdir + "/web")
      Jake.unjar($targetdir + "/"+$outfilebase+".cod", $targetdir + "/web")
      Jake.unjar($targetdir + "/RubyVM.cod", $targetdir + "/web")

    end

#    desc "Build and package dev rhobundle for device"
    task :rhobundle => "package:bb:rhobundle" do

      if $config["build"]["bbsignpwd"] and $config["build"]["bbsignpwd"] != ""
        autosign
      else
        manualsign
      end

      rm_rf Dir.glob($targetdir + "/web/RhoBundle*.*")

      cp $targetdir + "/RhoBundle.jad", $targetdir + "/web"

      Jake.unjar($targetdir + "/RhoBundle.cod", $targetdir + "/web")

    end

    desc "Build production for device"
    task :production => "package:bb:production" do

      if $config["build"]["bbsignpwd"] and $config["build"]["bbsignpwd"] != ""
        autosign
      else
        manualsign
      end

      rm_rf $targetdir + "/ota-web"
      mkdir_p $targetdir + "/ota-web"

      cp $targetdir + "/"+$outfilebase+".jad", $targetdir + "/ota-web"

      Jake.unjar($targetdir + "/"+$outfilebase+".cod", $targetdir + "/ota-web")
      
      rm_rf Dir.glob($targetdir + "/*.debug")

    end

  end
end

namespace "clean" do
  desc "Clean bb"
  task :bb => "clean:bb:all"
  namespace "bb" do
#    desc "Clean preverified jars"
    task :preverified => "config:bb" do
      rm_rf $preverified if File.exists? $preverified
    end

#    desc "Clean packaged files"
    task :packaged => "config:bb" do
      rm_rf $targetdir +"/../"
    end

#    desc "Clean temp dir"
    task :tempdir => "config:bb" do
      rm_rf $tmpdir
    end

#    desc "Clean all"
    task :all => [:preverified,:packaged,:tempdir] do
      rm_rf $bindir
      rm_rf $rhobundledir
    end

  end
end

namespace "run" do
  namespace "bb" do
      task :stopmdsandsim => ["config:bb"] do
        stopsim  
        stopmds
      end

#      desc "Starts mds and sim"
      task :startmdsandsim => ["config:bb"] do
        startmds
        startsim
      end
  end
  
  desc "Builds everything, loads and starts bb sim and mds"
  task :bb => ["run:bb:stopmdsandsim", "package:bb:production"] do
    jde = $config["env"]["paths"][$bbver]["jde"]
    
    cp_r File.join($targetdir,"/."), jde + "/simulator"
    
    startmds
    startsim
    $stdout.flush
  end

  desc "Same as run:bb, but only supports one app at a time and works faster"
  task :bbdev => ["run:bb:stopmdsandsim", "package:bb:dev"] do
    jde = $config["env"]["paths"][$bbver]["jde"]
    
    cp_r File.join($targetdir,"/."), jde + "/simulator"
    
    startmds
    startsim
    $stdout.flush
  end
  
end

namespace "config" do
  desc "Check local blackberry configuration"
  task :checkbb => ["config:bb"] do
    javahome = $config["env"]["paths"]["java"]
    jdehome = $config["env"]["paths"][$bbver]["jde"]
    mdshome = $config["env"]["paths"][$bbver]["mds"]

    puts "BBVER: " + $bbver
    puts "JAVAHOME: " + javahome
    puts "JDEHOME: " + jdehome
    puts "MDSHOME: " + mdshome

    if not FileTest.exists? javahome
      puts "JAVAHOME does not exist. Make sure you have the Java SDK installed and that build.yml has the correct path"
      throw "JAVAHOME missing"
    end

    if not FileTest.exists? javahome + "/javac.exe"
      puts "javac.exe not found. Make sure JAVAHOME points to a valid Java SDK"
      throw "javac missing"
    end

    if not FileTest.exists? javahome + "/java.exe"
      puts "java.exe not found. Make sure JAVAHOME points to a valid Java SDK"
      throw "java missing"
    end

    if not FileTest.exists? javahome + "/jar.exe"
      puts "jar.exe not found. Make sure JAVAHOME points to a valid Java SDK"
      throw "jar missing"
    end

    if not FileTest.exists? jdehome
      puts "JDEHOME does not exist. Make sure you have the Blackberry JDK installed and that build.yml has the correct path"
      throw "JDEHOME missing"
    end
    if not FileTest.exists? mdshome
      puts "MDSHOME does not exist. Make sure you have the Blackberry JDK installed and that build.yml has the correct path"
      throw "MDSHOME missing"
    end

    if not FileTest.exists? jdehome + "/bin/preverify.exe"
      puts "preverify.exe not found. Make sure JDEHOME points to a valid Blackberry JDK"
      throw "preverify missing"
    end

    if not FileTest.exists? jdehome + "/bin/rapc.jar"
      puts "rapc.jar not found. Make sure JDEHOME points to a valid Blackberry JDK"
      throw "rapc missing"
    end

    puts "Config appears valid"
  end

end
