require File.join(File.dirname(__FILE__),'..','jake.rb')

namespace "config" do
  task :iphone do
    $config = Jake.config(File.open('build.yml'))  
    $basedir = pwd
    $homedir = `echo ~`.to_s.strip
    $deploydir = File.join($basedir,'deploy','bb')

    $bindir = File.join($basedir,'bin')
    $srcdir = File.join($bindir, '/RhoBundle')
    $tmpdir = File.join($bindir,'tmp')
    $targetdir =  File.join($bindir,'target')

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

    $compileERBbase = File.join(File.dirname(__FILE__),'..','compileERB')
    $compileRBbase = File.join(File.dirname(__FILE__),'..','compileRB')
    $appmanifest = File.join(File.dirname(__FILE__),'..','manifest','createAppManifest.rb')
    $res = File.join(File.dirname(__FILE__),'..','..','res')

    $prebuilt = File.join($res,'prebuilt')
    $simapp="#{$homedir}/Library/Application Support/iPhone Simulator/User/Applications"
    $simlink="#{$homedir}/Library/Application Support/iPhone Simulator/User/Library/Preferences"
    $sim="/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications"
    $guid="364FFCAF-C71D-4543-B293-9058E31CFFEE"
    $applog = File.join($homedir,$config["env"]["applog"])
    mkdir_p $bindir if not File.exists? $bindir
  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :iphone =>  ["config:iphone", "loadframework"] do

    rm_rf $srcdir
    mkdir_p $srcdir
    mkdir_p File.join($srcdir,'apps')

    rubypath =  File.join($res,'RubyMac')
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
  desc "Create downloadable app for BlackBerry"
  task :iphone => "bundle:iphone" do
    if $config["env"]["bbsignpwd"] and $config["env"]["bbsignpwd"] != ""
      Rake::Task["run:iphone:autosign"].execute
    else
      Rake::Task["run:iphone:manualsign"].execute
    end

    rm_rf $deploydir
    mkdir_p $deploydir 

    cp File.join($targetdir, "rhodesApp.jad"), $deploydir

    Jake.unjar(File.join($targetdir, "rhodesApp.cod"), $deploydir)


  end
end

namespace "run" do
  namespace "iphone" do
  
    desc "Run app in iphone Sim"
    task :app  => ["bundle:iphone"] do
       simrhodes = File.join($simapp,$guid)
       rm_rf simrhodes
     
       mkdir_p File.join(simrhodes,"Documents")
       mkdir_p File.join(simrhodes,"Library","Preferences")
       
       puts `cp -R -p "#{$prebuilt}/iphone/sim/rhorunner.app" "#{simrhodes}"`
       puts `cp -R -p "#{$srcdir}/apps" "#{simrhodes}/rhorunner.app"`
       puts `cp -R -p "#{$srcdir}/lib" "#{simrhodes}/rhorunner.app"`
       puts `ln -f -s "#{$simlink}/com.apple.PeoplePicker.plist" "#{simrhodes}/Library/Preferences/com.apple.PeoplePicker.plist"`
       puts `ln -f -s "#{$simlink}/.GlobalPreferences.plist" "#{simrhodes}/Library/Preferences/.GlobalPreferences.plist"`
 
       puts `echo "#{$applog}" > "#{simrhodes}/Documents/rhologpath.txt"`
       
       f = File.new("#{$simapp}/#{$guid}.sb","w")
       f << "(version 1)\n(debug deny)\n(allow default)\n"
       f.close
       
       system("open \"#{$sim}/iPhone Simulator.app\"")
       
    end
  

  end
end
