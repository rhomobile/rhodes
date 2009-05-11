require File.join(File.dirname(__FILE__),'..','jake.rb')

namespace "config" do
  task :linux => :common do
    $homedir = `echo ~`.to_s.strip

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $applog = File.join($homedir,$config["env"]["applog"])
  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :linux =>  ["config:linux", "loadframework", "makedirs"] do
    rm_rf $srcdir
    mkdir_p $srcdir
    mkdir_p File.join($srcdir,'apps')

    rubypath = `which rubylinux`.chomp!
    raise RuntimeError "no rubylinux on your path!" if rubypath.nil? or rubypath.empty?
    compileERB = File.join($compileERBbase,'default.rb')
    compileRB = File.join($compileRBbase,'compileRB.rb')

    dest = $srcdir
    src = $rhodeslib
    cp_r src,dest

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
    puts `#{rubypath} -I#{$rhodeslib} #{$srcdir}/createAppManifest.rb`
    rm   File.join($srcdir,'createAppManifest.rb')
    cp   compileERB, $srcdir
    puts `#{rubypath} -I#{$rhodeslib} #{$srcdir}/default.rb`

    rm "#{$srcdir}/default.rb"

    cp   compileRB, $srcdir
    puts `#{rubypath} -I#{$rhodeslib} #{$srcdir}/compileRB.rb`

    chdir $srcdir
    Dir.glob("**/*.rb") { |f| rm f }
    Dir.glob("**/*.erb") { |f| rm f }
    chdir $basedir
  end
  
  task :desktop => "bundle:linux" do
    chdir $srcdir
    rm_f File.join($srcdir,"rhobundle.zip")
    puts `zip rhobundle.zip -r *`
  end
end

namespace "desktop" do
  task :win32, [:simpath] => "bundle:linux" do |t,args|
    chdir $srcdir
    rm_f File.join($srcdir,"rhodes-simulator.zip")
    mkdir "tmp"
    cp_r "#{args.simpath}/rho", "tmp/"
    cp_r "#{$srcdir}/apps", "tmp/rho/"
    cp_r "#{$srcdir}/lib", "tmp/rho/"
    cp_r "#{args.simpath}/rhodes.exe", "tmp/"
    chdir "tmp"
    puts `zip ../rhodes-simulator.zip -r *`
    chdir $srcdir
    rm_rf "tmp"
  end
end