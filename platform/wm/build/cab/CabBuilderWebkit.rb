
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderWebkit

  def initialize(app_name, 
                 setup_paths,
                 hidden_app, 
                 run_on_startup, 
                 additional_dlls_paths,
                 webkit_out_of_process, 
                 regs_dlls,
                 regkeys)
                 
    super(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
    
    @@webkit_out_of_process = webkit_out_of_process
  end

  def getDirsForParse
    puts "getDirsForParse for Webkit"
    sources = Array.new
    
    source = Hash.new
    source[:id] = "db"
    source[:path] = "..\\..\\..\\platform\\shared\\db\\res\\db"
    sources << source
    
    source = Hash.new
    source[:id] = "lib"
    source[:path] = File.join @@srcdir, "lib"
    sources << source
    
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
  
  def fillCopyToInstallDir
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".exe\",\"" + @@app_name + ".exe\",,0");
    print("\"" + "RhoLaunch" + ".exe\",\"" + "RhoLaunch" + ".exe\",,0");
    print("\"license_rc.dll\",\"license_rc.dll\",,0");
  end
  
  def fillCopyConfig
    print("[CopyConfig]")
    print("\"Config.xml\",\"Config.xml\",,0")
    print("\"Plugin.xml\",\"Plugin.xml\",,0")
    print("\"RegEx.xml\",\"RegEx.xml\",,0")
  end
  
  def fillCopySystemFiles
    print("[CopySystemFiles]")
    print("\"prtlib.dll\",\"prtlib.dll\",,0")
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
  
  def fillCopyNPAPI    
    print("[CopyNPAPI]");
    print("\"npwtg_jsobjects.dll\",\"npwtg_jsobjects.dll\",,0");
    print("\"bridge.dll\",\"bridge.dll\",,0");
    print("\"npwtg_legacy.dll\",\"npwtg_legacy.dll\",,0");
  end
   
end