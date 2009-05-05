require File.join(File.dirname(__FILE__),'..','jake.rb')

namespace "config" do
  task :symbian => :common do

    $targetdir =  File.join($bindir,'target','symbian')

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :symbian =>  ["config:symbian", "loadframework", "makedirs"] do

    rm_rf $srcdir
    mkdir_p $srcdir
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
    cp   'rhoconfig.txt', File.join($srcdir,'apps')

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
    
      src = File.join($prebuilt,"common","db")
      dest = $srcdir

      cp_r src,dest

      src = File.join($prebuilt,"symbian")
      dest = $tmpdir

      cp_r File.join(src,"Epoc32"), dest
      
      cp File.join(src,"backup_registration.xml"), dest
      cp File.join(src,"rhodes.hlp"), dest
      cp File.join(src,"rhologpath.txt"), dest
      cp File.join(src,"rhodes_device.pkg"), File.join(dest, "rhodes_tmp.pkg")
      cp File.join($res,"insertRhoBundle2pkg.rb"), dest

      rubypath =  File.join($res,'RhoRuby.exe')
      puts `#{rubypath} -R#{$rhodeslib} #{$tmpdir}/insertRhoBundle2pkg.rb` 

      tools = $config["env"]["paths"]["symbiantools"]

      chdir $tmpdir
      makesis = File.join(tools,"makesis.exe")
      pkgfile = ""
      File.open("rhodes_tmp.pkg","r") { |f| pkgfile = f.read }
      modifiedpkg = ""
      pkgfile.each do |line|
        modifiedpkg << line if not line.match(/^;/)
      end 
      File.open("rhodes_tmp.pkg","w") { |f| f.write modifiedpkg }
      puts `"#{makesis}" rhodes_tmp.pkg`
    chdir $basedir

  end
end

namespace "device" do
  desc "Create downloadable app for symbian mobile"
  task :symbian => "bundle:symbian" do
      tools = $config["env"]["paths"]["symbiantools"]
      chdir $tmpdir
      certdir = File.join($tmpdir,"cert")
      certpath = File.join(certdir,$config["env"]["symbian"]["certname"])
      keypath = File.join(certdir,$config["env"]["symbian"]["certkey"])
      pass = $config["env"]["symbian"]["selfsignpwd"]
      name = $config["env"]["symbian"]["selfsignname"]

      mkdir_p certdir

      cp File.join($res,"rhodes_pid.key"),certdir
      makekeys = File.join(tools,"makekeys.exe")

      puts `"#{makekeys}" -cert -password "#{pass}" -len 2048 -dname "#{name}" "#{keypath}" "#{certpath}"`

      signsis = File.join(tools,"signsis.exe")

      puts `#{signsis} rhodes_tmp.sis rhodes.sisx #{certpath} #{keypath} #{pass}`

      mv "rhodes.sisx",$targetdir
  end
end

namespace "run" do
  namespace "symbian" do
  
    desc "Run app in symbian Sim"
    task :app  => ["bundle:symbian"] do
    
    symroot = $config["env"]["paths"]["symroot"]

    appsroot = File.join(symroot,"/epoc32/winscw/c/Data/Rho")
    rm_rf appsroot
    mkdir_p appsroot if not File.exists? appsroot
    cp_r $srcdir + "/lib", appsroot
    cp_r $srcdir + "/apps", appsroot
    cp_r $srcdir + "/db", appsroot
    
    cp_r File.join($prebuilt,"symbian","Epoc32"), symroot

    executable = File.join(symroot,"epoc32","release","winscw","udeb","rhodes.exe")
    Thread.new { system(executable) }
		


    end
  

  end
end
