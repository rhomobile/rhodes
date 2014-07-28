
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderWebkit < CabBuilderBase

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
  
  def fillDstDirs        
    super    
    print("CopyWebKitBin=0,\"%InstallDir%\"")
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
   
end