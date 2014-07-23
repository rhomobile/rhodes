
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
    source[:id] = "db"
    source[:path] = "..\\..\\..\\platform\\shared\\db\\res\\db"
    sources << source
    
    if @@is_icon
      source = Hash.new
      source[:id] = "icon"
      source[:path] = File.join @@srcdir, "icon"
      sources << source
    end
    
    source = Hash.new
    source[:id] = "apps"
    source[:path] = File.join @@srcdir, "apps"
    sources << source
    
    path_idx = 1
    
    @@additional_dlls_paths.each { |path|
      source = Hash.new
      source[:id] = "add" + path_idx.to_s 
      source[:path] = path
      sources << source 
      
      path_idx = path_idx + 1     
    }
    
    return sources
  end
  
  def fillDstDirs    
    print("[DestinationDirs]")
    print("Shortcuts=0,\"%CE11%\"")       if @@hidden_app == false
    print("ShortcutsAutorun=0,\"%CE4%\"") if @@run_on_startup == true
    print("CopyToInstallDir=0,\"%InstallDir%\"");
  end
  
  def fillCopyToInstallDir      
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".lnk\",\"" + @@app_name + ".lnk\",,0");    
  end
  
  def fillCopyConfig
    print("[CopyConfig]")
  end
  
  def fillCopySystemFiles
  end
   
end