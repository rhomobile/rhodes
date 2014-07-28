
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderPersistentWebkit < CabBuilderBase

  def initialize(app_name, 
                 setup_paths, 
                 hidden_app, 
                 run_on_startup, 
                 additional_dlls_paths, 
                 regs_dlls,
                 regkeys,
                 webkit_out_of_process)
                 
    super(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
    
    @@webkit_out_of_process = webkit_out_of_process
  end
  
  def fillPredefineFileCopies
    return "CopyToInstallDir,CopySystemFiles,CopyWebKitBin"
  end
  
  def getDirsForParse
    sources = super

    source = Hash.new
    source[:id]       = "WebKitBin"
    source[:path]     = @@setup_paths[:webkit_data]
    source[:dst_path] = ""
    source[:filter]   = "*"
    sources << source
        
    return sources 
  end
  
  def fillCeStrings(app_name)
    print("[CEStrings]")
    print("AppName=\"" + @@app_name + "\"")
  end
  
  def fillCopyWebKitBin
    print("[CopyWebKitBin]");
    print("\"eklibrary.dll\",\"eklibrary.dll\",,0");
    print("\"ipc_manager.dll\",\"ipc_manager.dll\",,0");
    
    if @@webkit_out_of_process
      print("\"OutProcessWK.exe\"" + "," + "\"OutProcessWK.exe\""+ ",,0");
    else
      print("\"WebkitPlatformDeliveryCompiledAsDLL.dll\"" + "," + "\"WebkitPlatformDeliveryCompiledAsDLL.dll\"" + ",,0");
    end
    
    print("\"openssl.dll\",\"openssl.dll\",,0");
    print("\"Ekioh.dll\",\"Ekioh.dll\",,0");     
  end
    
  def fillCopyFilesSections
    super
    print ""
    fillCopyWebKitBin
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