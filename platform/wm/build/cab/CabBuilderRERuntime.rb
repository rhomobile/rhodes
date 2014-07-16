
require File.join(File.dirname(__FILE__), 'CabBuilderBase.rb')

class CabBuilderRERuntime

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
    print("\"" + @@app_name + ".lnk\",\"" + @@app_name + ".lnk\",,0");    
  end
  
  def fillCopyConfig
    print("[CopyConfig]")
  end
  
  def fillCopySystemFiles
    print("[CopySystemFiles]")
  end
   
end