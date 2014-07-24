
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
    sources = super
    
    source = Hash.new
    source[:id]       = "NPAPI"
    source[:path]     = File.join @@setup_paths[:webkit_data], "NPAPI"
    source[:dst_path] = "NPAPI"
    source[:filter]   = "*"
    sources << source

    source = Hash.new
    source[:id]       = "WebKitBin"
    source[:path]     = @@setup_paths[:webkit_data]
    source[:dst_path] = ""
    source[:filter]   = "*"
    sources << source
        
    return sources 
  end
  
  def fillDstDirs        
    super    
    print("CopyWebKitBin=0,\"%InstallDir%\"")
    print("CopyNPAPI=0,\"%InstallDir%\\NPAPI\"")
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
  
  def fillCopyFilesSections
    super
    fillCopyNPAPI
  end
   
end