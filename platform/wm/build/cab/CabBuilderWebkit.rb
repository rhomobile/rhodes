
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderWebkit

  def initialize(app_name, 
                 srcdir, 
                 hidden_app, 
                 wk_data_dir, 
                 run_on_startup, 
                 additional_dlls_persistent_paths, 
                 regs_dlls)
                 
    super(app_name, srcdir, hidden_app, wk_data_dir, run_on_startup, additional_dlls_persistent_paths, regs_dlls)
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
    print(webkit_file + "," + webkit_file + ",,0");
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