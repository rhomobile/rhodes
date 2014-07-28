
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderPersistentIE < CabBuilderBase

  def initialize(app_name, 
                 setup_paths, 
                 hidden_app, 
                 run_on_startup, 
                 additional_dlls_paths, 
                 regs_dlls,
                 regkeys)
                 
    super(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
  end
  
  def getDirsForParse
    sources = super
    
    source = Hash.new
    source[:id]       = "prtlib"
    source[:path]     = File.join @@setup_paths[:webkit_data]
    source[:dst_path] = ""
    source[:filter]   = "prtlib.dll"
    sources << source
    
    source = Hash.new
    source[:id]       = "Config"
    source[:path]     = File.join @@setup_paths[:webkit_data], "Config"
    source[:dst_path] = "Config"
    source[:filter]   = "*"
    sources << source
    
    return sources
  end
  
  def fillCeStrings(app_name)
    print("[CEStrings]")
    print("AppName=\"" + @@app_name + "\"")
  end
  
  def fillDstDirs    
    app_dir = File.join("\Application", @@app_name).gsub("/", "\\")
    
    print("[DestinationDirs]")
    print("Shortcuts=0,\"%CE11%\"")       if @@hidden_app == false
    print("ShortcutsAutorun=0,\"%CE4%\"") if @@run_on_startup == true
    print("CopySystemFiles=0,\"%CE2%\"");
    print("CopyToInstallDir=0,\"" + app_dir + "\"")
    print("CopyWebKitBin=0,\"" + app_dir + "\"")
    
    @@dst_disk_names.each { |disk|      
      if !disk[:files].nil? && !disk[:files].empty?    
        print "copyfiles_add" + disk[:number].to_s + disk[:name] + "=0,\"" + File.join(app_dir, disk[:path] + "\"").gsub("/", "\\")
      end
    }      
  end
     
end