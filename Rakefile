# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

chdir 'rhodes', :verbose => false
load 'Rakefile'
chdir '..', :verbose => false

desc "Get versions"
task :get_version do
  bbver = "unknown"
  iphonever = "unknown"
  symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"
  

  File.open("platform/bb/build/build.yml","r") do |f|
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

  File.open("platform/symbian/build/release.properties","r") do |f|
    file = f.read
    major = ""
    minor = ""
    build = ""

    if file.match(/release\.major=(\d+)/)
      major =  $1
    end
    if file.match(/release\.minor=(\d+)/)
      minor =  $1
    end
    if file.match(/build\.number=(\d+)/)
      build =  $1
    end

    symver = major + "." + minor + "." + build
  end

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

  File.open("rhodes/rhodes-build/lib/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rbuildver =  $1
    end
  end

  File.open("rhodes/rhodes-framework/lib/version.rb","r") do |f|
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
  puts "  Symbian:          " + symver
  puts "  WinMo:            " + wmver
  puts "  Android:          " + androidver
  puts "  Rhodes:           " + rhodesver
  puts "  Rhodes Build:     " + rbuildver
  puts "  Rhodes Framework: " + rframever
  puts "  Rhodes Generator: " + rgenver
end

desc "Set version"
task :set_version, [:version] do |t,args|
  throw "You must pass in version" if args.version.nil?
  ver = args.version.split(/\./)
  major = ver[0]
  minor = ver[1]
  build = ver[2]
  
  throw "Invalid version format. Must be in the format of: major.minor.build" if major.nil? or minor.nil? or build.nil?

  verstring = major+"."+minor+"."+build
  origfile = ""

  File.open("platform/bb/build/build.yml","r") { |f| origfile = f.read }
  File.open("platform/bb/build/build.yml","w") do |f| 
    f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  end

  File.open("platform/iphone/Info.plist","r") { |f| origfile = f.read }
  File.open("platform/iphone/Info.plist","w") do |f| 
    f.write origfile.gsub(/CFBundleVersion<\/key>(\s+)<string>(\d+\.\d+\.*\d*)<\/string>/, "CFBundleVersion</key>\n  <string>#{verstring}</string>")
  end

  File.open("platform/symbian/build/release.properties","r") { |f| origfile = f.read }
  File.open("platform/symbian/build/release.properties","w") do |f|
    origfile.gsub!(/release\.major=(\d+)/,"release.major=#{major}")
    origfile.gsub!(/release\.minor=(\d+)/,"release.minor=#{minor}")
    origfile.gsub!(/build\.number=(\d+)/,"build.number=#{build}")
    f.write origfile
  end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") { |f| origfile = f.read }
  File.open("platform/android/Rhodes/AndroidManifest.xml","w") do |f|
    origfile.match(/versionCode="(\d+)"/)
    vercode = $1.to_i + 1
    origfile.gsub!(/versionCode="(\d+)"/,"versionCode=\"#{vercode}\"")
    origfile.gsub!(/versionName="(\d+\.\d+\.*\d*)"/,"versionName=\"#{verstring}\"")

    f.write origfile
  end

  ["rhodes/rhodes/lib/rhodes.rb",
   "rhodes/rhodes-build/lib/version.rb",
   "rhodes/rhodes-framework/lib/version.rb",
   "rhodes/rhodes-framework/lib/rhodes.rb",
   "rhodes/rhodes-generator/lib/version.rb"].each do |versionfile|
  
    File.open(versionfile,"r") { |f| origfile = f.read }
    File.open(versionfile,"w") do |f|
      origfile.gsub!(/VERSION = '(\d+\.\d+\.*\d*)'/, "VERSION = '#{verstring}'")
      origfile.gsub!(/DBVERSION = '(\d+\.\d+\.*\d*)'/, "DBVERSION = '#{verstring}'")
      f.write origfile
    end
  end
  
end
