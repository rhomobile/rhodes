
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderIE < CabBuilderBase

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
    p("\"Config.xml\",\"Config.xml\",,0");
    p("\"Plugin.xml\",\"Plugin.xml\",,0");
    p("\"RegEx.xml\",\"RegEx.xml\",,0");
  end
  
  def fillCopySystemFiles
    print("[CopySystemFiles]")
    print("\"prtlib.dll\",\"prtlib.dll\",,0");                    
  end
     
end