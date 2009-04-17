require File.join(File.dirname(__FILE__),'..','jake.rb')

namespace "config" do
  task :wm => :common do

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :wm =>  ["config:wm", "loadframework", "makedirs"] do

    rm_rf $srcdir
    rm_rf $bindir
    mkdir_p $srcdir
    mkdir_p $bindir
    mkdir_p $targetdir
    mkdir_p File.join($srcdir,'apps')

    rubypath =  File.join($res,'RhoRuby.exe')
    compileERB = File.join($compileERBbase,'default.rb')
    compileRB = File.join($compileRBbase,'compileRB.rb')

    dest = $srcdir
    src = $rhodeslib
    cp_r src,dest

    src = File.join($rhodeslib,"..","res","sqlite3")
    cp_r src,File.join(dest,"lib")
    
    chdir dest
    Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
    Dir.glob("**/erb.rb").each {|f| rm f}
    Dir.glob("**/find.rb").each {|f| rm f}
    $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

    chdir $basedir
    cp_r 'app',File.join($srcdir,'apps')
    cp_r 'public', File.join($srcdir,'apps')
    cp   'config.rb', File.join($srcdir,'apps')

    cp   $appmanifest, $srcdir
    puts `#{rubypath} -R#{$rhodeslib} #{$srcdir}/createAppManifest.rb` 
    rm   File.join($srcdir,'createAppManifest.rb')
    cp   compileERB, $srcdir
    puts `#{rubypath} -R#{$rhodeslib} #{$srcdir}/default.rb` 

    rm "#{$srcdir}/default.rb"
    
    cp   compileRB, $srcdir
    puts `#{rubypath} -R#{$rhodeslib} #{$srcdir}/compileRB.rb` 

    chdir $srcdir
    Dir.glob("**/*.rb") { |f| rm f }
    Dir.glob("**/*.erb") { |f| rm f }
#throw "ME"

    chdir $basedir
    
  #  rm_rf $srcdir

  end
end

namespace "device" do
  desc "Create downloadable app for windows mobile"
  task :wm => "bundle:wm" do
      src = File.join($prebuilt,"wm","wm6.7z")

      seven = File.join($res,"7z.exe")
      chdir $bindir

      puts `#{seven} x #{src}`

      chdir $basedir

      src = File.join($prebuilt,"common","db")
      dest = $srcdir

      cp_r src,dest

      src = File.join($res,"build_inf.js")
      dest = $basedir
      cp src,dest

      chdir $basedir
      puts `cscript build_inf.js rhodes.inf wm6`

      cabwiz = $config["env"]["paths"]["cabwiz"]
      puts `#{cabwiz} rhodes.inf`

	  puts `cscript cleanup.js`
	  rm "cleanup.js"

      mv "rhodes.inf", $bindir
      mv "rhodes.cab", $targetdir

  end
end

namespace "run" do
  namespace "wm" do
  
    desc "Run app in WM Sim"
    task :app  => ["device:wm"] do
 


      Thread.new { system($config["env"]["paths"]["wmemu"]) }

      puts "BUILD COMPLETE"
      puts "*** Your .cab file is located in #{$targetdir}\\rhodes.cab"
      puts "*** The device emulator has been started for you, select a windows mobile 6 device"
      puts "*** Then you must cradle the device and can drop the cab in the Mobile Device folder in My computer"
    end
  

  end
end
