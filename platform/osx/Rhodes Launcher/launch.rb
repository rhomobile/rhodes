    require 'Find'
    require 'fileutils'
    include FileUtils

    chdir File.dirname(__FILE__)

    $homedir = `echo ~`.to_s.strip
    $simapp="#{$homedir}/Library/Application Support/iPhone Simulator/User/Applications"
    $simlink="#{$homedir}/Library/Application Support/iPhone Simulator/User/Library/Preferences"
    $sim="/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications"
    $guid="364FFCAF-C71D-4543-B293-9058E31CFFEE"
    $applog = File.join($homedir,"rholog.txt")

     `killall "iPhone Simulator"`


     Find.find($simapp) do |path|
       if File.basename(path) == "rhorunner.app"
         $guid = File.basename(File.dirname(path))
       end
     end

     simrhodes = File.join($simapp,$guid)
     rm_rf simrhodes

     mkdir_p File.join(simrhodes,"Documents")
     mkdir_p File.join(simrhodes,"Library","Preferences")

     puts `cp -R -p "rhorunner.app" "#{simrhodes}"`
     puts `ln -f -s "#{$simlink}/com.apple.PeoplePicker.plist" "#{simrhodes}/Library/Preferences/com.apple.PeoplePicker.plist"`
     puts `ln -f -s "#{$simlink}/.GlobalPreferences.plist" "#{simrhodes}/Library/Preferences/.GlobalPreferences.plist"`

     puts `echo "#{$applog}" > "#{simrhodes}/Documents/rhologpath.txt"`

     f = File.new("#{$simapp}/#{$guid}.sb","w")
     f << "(version 1)\n(debug deny)\n(allow default)\n"
     f.close

     system("open \"#{$sim}/iPhone Simulator.app\"")
