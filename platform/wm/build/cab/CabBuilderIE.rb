
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
  
  def fillCopySystemFiles
    #print("[CopySystemFiles]")
    #print("\"prtlib.dll\",\"prtlib.dll\",,0");                    
  end
     
end