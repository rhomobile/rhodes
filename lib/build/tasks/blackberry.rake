#
require File.join(File.dirname(__FILE__),'..','jake.rb')

def freplace( fname, pattern, str )
  f = File.open( fname )
  strings = f.read
  f.close

  strings.gsub!( pattern, str )

  f = File.new( fname, "w" )
  f.print strings
  f.close
end

namespace "config" do
  task :bb => :common do

    $deploydir = File.join($basedir,'deploy','bb')
    $excludelib = ['**/singleton.rb','**/rational.rb','**/rhoframework.rb','**/date.rb']

    if $config["env"].has_key? "application-name"
      $appname = $config["env"]["application-name"]
    else
      $appname = "rhodesApp"
    end

  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :bb =>  ["config:bb", "loadframework", "makedirs"] do
    jdehome = $config["env"]["paths"][$config["env"]["bbver"]]["jde"]

    rm_rf $srcdir
    mkdir_p $srcdir
    mkdir_p File.join($srcdir,'apps')

    compileERB = File.join($compileERBbase,'bb.rb')
    #rubypath =  File.join($res,'RhoRuby.exe')
    xruby =  File.join($res,'xruby-0.3.3.jar')

    dest = $srcdir 
    chdir $rhodeslib
    Dir.glob("*").each { |f| 
      src = f
      cp_r src,dest
    }
    chdir dest
    Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
    Dir.glob("**/erb.rb").each {|f| rm f}
    Dir.glob("**/find.rb").each {|f| rm f}
    $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

    chdir $basedir
#throw "ME"
    cp_r 'app',File.join($srcdir,'apps')
    cp_r 'public', File.join($srcdir,'apps')
    cp   'rhoconfig.txt', File.join($srcdir,'apps')
    
    chdir File.join($srcdir,'apps/public')
    rm_rf 'js/iui'
    Dir.glob("js/jquery*").each {|f| rm_rf f}
    Dir.glob("js/prototype*").each {|f| rm f}
    chdir $basedir
    
    #cp   'index.erb', File.join($srcdir,'apps')
    #cp   'layout.erb', File.join($srcdir,'apps')
    #cp   'loading.html', File.join($srcdir,'apps')
    cp   $appmanifest, $srcdir
    puts `ruby #{$srcdir}/createAppManifest.rb`
    rm   File.join($srcdir,'createAppManifest.rb')
    cp   compileERB, $srcdir
    puts `ruby #{$srcdir}/bb.rb`

    chdir $bindir
    puts `java -jar "#{xruby}" -c RhoBundle`
    chdir $srcdir
    Dir.glob("**/*.rb") { |f| rm f }
    Dir.glob("**/*.erb") { |f| rm f }
    puts `jar uf ../RhoBundle.jar apps/*.*`
    chdir $basedir
    
    rm_rf $srcdir

    args = []
    args << "-classpath"
    args << '"' + jdehome + "/lib/net_rim_api.jar;" + File.join($prebuilt, "bb","RubyVM.jar") + '"'
    args << "-d"
    args << '"' + $bindir + '"'
    args << '"' + $bindir + "/RhoBundle.jar" +'"'
    puts Jake.run(jdehome + "/bin/preverify.exe",args)
    $stdout.flush

    rm_rf $tmpdir
    mkdir_p $tmpdir

    rm_rf $targetdir
    mkdir_p $targetdir

    Jake.unjar(File.join($prebuilt, "bb","RubyVM.jar"), $tmpdir) 
    Jake.unjar($bindir + "/RhoBundle.jar", $tmpdir) 
    Jake.unjar(File.join($prebuilt, "bb","rhodes.jar"), $tmpdir) 

    Jake.jar($bindir + "/" + $appname + ".jar",'"' + File.join($prebuilt, "bb","manifest.mf") + '"',$tmpdir,true)

    vendor = $config["env"]["vendor"]
    version = $config["env"]["version"]
    Jake.rapc($appname, 
           $targetdir,
           jdehome + "/lib/net_rim_api.jar",
           '"' + $bindir + "/" + $appname + ".jar" +'"',
           $appname,
           vendor,
           version,
           "resources/icon.png",
           false,
           true
      )
    $stdout.flush

    cp  File.join($prebuilt, "bb","rhodesApp.alx"), File.join($targetdir, $appname + ".alx")
    freplace( File.join( $targetdir, $appname + ".alx" ), /rhodesApp/, $appname )
  end
end

namespace "device" do
  desc "Create downloadable app for BlackBerry"
  task :bb => "bundle:bb" do
    if $config["env"]["bbsignpwd"] and $config["env"]["bbsignpwd"] != ""
      Rake::Task["run:bb:autosign"].execute
    else
      Rake::Task["run:bb:manualsign"].execute
    end

    webdir = File.join($targetdir, "web")  
    mkdir_p webdir

    cp File.join($targetdir, $appname + ".jad"), webdir

    Jake.unjar(File.join($targetdir, $appname + ".cod"), webdir)

    rm_rf $deploydir
    mkdir_p $deploydir 
    mv $targetdir,$deploydir
    
  end
end

namespace "run" do
  namespace "bb" do
    def startmds
        mdshome =  $config["env"]["paths"][$config["env"]["bbver"]]["mds"]
        args = []
        args << "/c"
        args << "run.bat"

        Jake.run("cmd.exe",args, mdshome,true)
    end 

    def stopmds
        mdshome =  $config["env"]["paths"][$config["env"]["bbver"]]["mds"]
        args = []
        args << "/c"
        args << "shutdown.bat"

        Jake.run("cmd.exe",args, mdshome,true)
    end 

    def startsim
        bbver = $config["env"]["bbver"]
        sim = $config["env"]["paths"][bbver]["sim"]
        jde = $config["env"]["paths"][bbver]["jde"]

        command =  '"' + jde + "/simulator/fledge.exe\""
        args = [] 
        args << "/app=Jvm.dll"
        args << "/handheld=" + sim
        args << "/session=" + sim
        args << "/app-param=DisableRegistration"
        args << "/app-param=JvmAlxConfigFile:"+sim+".xml"
        args << "/data-port=0x4d44"
        args << "/data-port=0x4d4e"
        args << "/pin=0x2100000A"

        if bbver !~ /^4\.[012](\..*)?$/
          args << "/fs-sdcard=true"
        end
            
        args << "\"/app-param=JvmDebugFile:"+Jake.get_absolute($config["env"]["applog"]) +'"'

        Thread.new { Jake.run(command,args,jde + "/simulator",true) }
        $stdout.flush
    end

    def stopsim
        bbver = $config["env"]["bbver"]
        sim = $config["env"]["paths"][bbver]["sim"]
        jde = $config["env"]["paths"][bbver]["jde"]

        command =  '"' + jde + "/simulator/fledgecontroller.exe\""
        args = []
        args << "/session="+sim
        args << "/execute=Exit(true)"
        Jake.run(command,args, jde + "/simulator")
    end
  
    task :stopmdsandsim => ["config:bb"] do
        stopsim  
        stopmds
    end

    desc "Run app in BlackBerry Sim"
    task :app => [:stopmdsandsim, "bundle:bb"] do
        jde = $config["env"]["paths"][$config["env"]["bbver"]]["jde"]

        cp_r File.join($targetdir,"/."), jde + "/simulator"

        startmds
        startsim

#        puts "sleeping to allow simulator to get started"
#        sleep 25

#        command = '"' + jde + "/simulator/fledgecontroller.exe\""
#        args = []
#        args << "/session="+sim
#        args << "\"/execute=LoadCod(" + File.join($targetdir,$appname + ".cod") + ")\""

#        Jake.run(command,args, jde + "/simulator")
        $stdout.flush
    end
  
    task :autosign do
      java = $config["env"]["paths"]["java"] + "/java.exe"
      jde = $config["env"]["paths"][$config["env"]["bbver"]]["jde"] 
    
      args = []
      args << "-jar"
      args << '"' + jde + "/bin/SignatureTool.jar\""
      args << "-c"
      args << "-a"
      args << "-p"
      args << '"' + $config["env"]["bbsignpwd"] +'"'
      args << "-r"
      args << $targetdir
  
      puts Jake.run(java,args)
      $stdout.flush
  
    end
  
    task :manualsign do
      java = $config["env"]["paths"]["java"] + "/java.exe"
      jde = $config["env"]["paths"][$config["env"]["bbver"]]["jde"] 
      
      args = []
      args << "-jar"
      args << '"' + jde + "/bin/SignatureTool.jar\""
      args << "-r"
      args << $targetdir
    
      puts Jake.run(java,args)
      $stdout.flush
    
    end
  end
end

namespace "check" do
  task :bb => "config:bb" do
    errors = Array.new
    
    begin
      javahome = $config["env"]["paths"]["java"]
      jdehome = $config["env"]["paths"][$config["env"]["bbver"]]["jde"]
      mdshome = $config["env"]["paths"][$config["env"]["bbver"]]["mds"]
    rescue
      puts " - Error parsing build.yml make sure you have all of the required fields (see generated build.yml)"
      errors << "invalid build.yml"
    end


    if not FileTest.exists? javahome
      puts " - JAVAHOME does not exist. Make sure you have the Java SDK installed and that build.yml has the correct path"
      errors << "JAVAHOME missing"
    end

    if not FileTest.exists? javahome + "/javac.exe"
      puts " - javac.exe not found. Make sure JAVAHOME in build.yml points to a valid Java SDK"
      errors << "javac missing"
    end

    if not FileTest.exists? javahome + "/java.exe"
      puts " - java.exe not found. Make sure JAVAHOME in build.yml points to a valid Java SDK"
      errors << "java missing"
    end

    if not FileTest.exists? javahome + "/jar.exe"
      puts " - jar.exe not found. Make sure JAVAHOME in build.yml points to a valid Java SDK"
      errors << "jar missing"
    end

    if not FileTest.exists? jdehome
      puts " - JDEHOME does not exist. Make sure you have the Blackberry JDK installed and that build.yml has the correct path"
      errors << "JDEHOME missing"
    end
    if not FileTest.exists? mdshome
      puts " - MDSHOME does not exist. Make sure you have the Blackberry JDK installed and that build.yml has the correct path"
      errors << "MDSHOME missing"
    end

    if not FileTest.exists? jdehome + "/bin/preverify.exe"
      puts " - preverify.exe not found. Make sure JDEHOME in build.yml points to a valid Blackberry JDK"
      errors << "preverify missing"
    end

    if not FileTest.exists? jdehome + "/bin/rapc.jar"
      puts " - rapc.jar not found. Make sure JDEHOME in build.yml points to a valid Blackberry JDK"
      errors << "rapc missing"
    end

    begin
      blah = `jar 2>&1`
    rescue
      puts " - jar is not on the path. Make sure you have the java sdk bin folder in your path"
      errors << "jar not on path"
    end

    begin
      blah = `java 2>&1`
    rescue
      puts " - java is not on the path. Make sure you have the java sdk bin folder in your path"
      errors << "java not on path"
    end

    puts "\nBBVER: " + $config["env"]["bbver"]
    puts "JAVAHOME: " + javahome
    puts "JDEHOME: " + jdehome
    puts "MDSHOME: " + mdshome

    if errors.size > 0
      puts "\nFound the following errors for blackberry: "
      errors.each { |error| puts "\t" + error.to_s }
    else
      puts "Blackberry config appears valid"
    end

  end
end
