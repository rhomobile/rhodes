
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
  
  def getDirsForParse
    sources = super
    
    source = Hash.new
    source[:id]       = "prtlib"
    source[:path]     = File.join @@setup_paths[:webkit_data]
    source[:dst_path] = ""
    source[:filter]   = "prtlib.dll"
    sources << source
    
    source = Hash.new
    source[:id]       = "Config"
    source[:path]     = File.join @@setup_paths[:webkit_data], "Config"
    source[:dst_path] = "Config"
    source[:filter]   = "*"
    sources << source
    
    return sources
  end
     
end