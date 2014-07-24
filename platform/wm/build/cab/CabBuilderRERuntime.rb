
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderRERuntime

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
    
    sources = Array.new
    
    source = Hash.new
    source[:id]       = "db"
    source[:path]     = "..\\..\\..\\platform\\shared\\db\\res\\db"
    source[:dst_path] = "rho"
    source[:filter]   = "*"
    sources << source
    
    source = Hash.new
    source[:id]       = "lib"
    source[:path]     = File.join @@setup_paths[:src], "lib"
    source[:dst_path] = "rho"
    source[:filter]   = "*"
    sources << source
    
    source = Hash.new
    source[:id]       = "apps"
    source[:path]     = File.join @@setup_paths[:src], "apps"
    source[:dst_path] = "rho\\apps"
    source[:filter]   = "*"
    sources << source
    
    path_idx = 1
    
    @@additional_dlls_paths.each { |path|
      source = Hash.new
      source[:id]       = "add" + path_idx.to_s 
      source[:path]     = path
      source[:dst_path] = ""
      source[:filter] = "*"
        
      sources << source
      
      path_idx = path_idx + 1      
    }
    
    if @@is_icon
      source = Hash.new
      source[:id] = "icon"
      source[:path] = File.join @@srcdir, "icon"
      source[:dst_path] = ""
      source[:filter] = "*"
      sources << source
    end
    
    return sources
  end
  
  def fillDstDirs    
    print("[DestinationDirs]")
    print("Shortcuts=0,\"%CE11%\"")       if @@hidden_app == false
    print("ShortcutsAutorun=0,\"%CE4%\"") if @@run_on_startup == true
    print("CopyToInstallDir=0,\"%InstallDir%\"")
    #print("CopyConfig=0,\"%InstallDir%\\Config\"")
    
    @@dst_disk_names.each { |disk|      
      if !disk[:files].nil? && !disk[:files].empty?    
        print "copyfiles_add" + disk[:number].to_s + disk[:name] + "=0,\"" + File.join("%InstallDir%", disk[:path] + "\"").gsub("/", "\\")
      end
    }
  end
  
  def fillCopyToInstallDir      
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".lnk\",\"" + @@app_name + ".lnk\",,0");    
  end
    
  def fillCopySystemFiles
  end
   
end