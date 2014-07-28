
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderRERuntime < CabBuilderBase 

  def initialize(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)                 
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
       
    if @@is_icon
      source = Hash.new
      source[:id] = "icon"
      source[:path] = File.join @@setup_paths[:src], "icon"
      source[:dst_path] = ""
      source[:filter] = "*"
      sources << source
      
      source = Hash.new
      source[:id] = "icon"
      source[:path] = File.join @@setup_paths[:src], ".."
      source[:dst_path] = ""
      source[:filter] = @@app_name + ".lnk"
      sources << source
    end
    
    return sources
  end  
  
  def fillCopyToInstallDir      
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".lnk\",\"" + @@app_name + ".lnk\",,0");    
  end
    
  def fillCopySystemFiles
  end
   
end