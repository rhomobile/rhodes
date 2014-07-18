
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderIE < CabBuilderBase

  def initialize(app_name, 
                 setup_paths, 
                 hidden_app, 
                 run_on_startup, 
                 additional_dlls_paths, 
                 regs_dlls,
                 regkeys)
                 
    super(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
  end

  def fillCopyToInstallDir
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".exe\",\"" + @@app_name + ".exe\",,0");
    print("\"" + "RhoLaunch" + ".exe\",\"" + "RhoLaunch" + ".exe\",,0");
    print("\"license_rc.dll\",\"license_rc.dll\",,0");
  end
  
  def fillCopyConfig
    print("[CopyConfig]")
    print("\"Config.xml\",\"Config.xml\",,0");
    print("\"Plugin.xml\",\"Plugin.xml\",,0");
    print("\"RegEx.xml\",\"RegEx.xml\",,0");
  end
  
  def fillCopySystemFiles
    print("[CopySystemFiles]")
    print("\"prtlib.dll\",\"prtlib.dll\",,0");                    
  end
     
end