
class CabBuilderBase

  @@inf_file = nil  
  @@sections = nil
  @@extension_files = nil
  @@extension_dlls = nil
  
  @@src_disk_names = nil
  @@src_disk_files = nil
  @@dst_disk_names = nil
  @@dst_disk_files = nil
  
  def initialize(app_name, srcdir, hidden_app, wk_data_dir, run_on_startup, additional_dlls_persistent_paths, regs_dlls)
    @@app_name              = app_name
    @@srcdir                = srcdir
    @@hidden_app            = hidden_app
    @@wk_data_dir           = wk_data_dir    
    @@run_on_startup        = run_on_startup
    @@additional_dlls_paths = additional_dlls_paths
    @@regs_dlls             = regs_dlls
  end
  
  def saveInfFile(filepath)
    FileUtils.rm_f filepath if File.exists? filepath

    parseDirs    
    
    File.open(filepath, 'w') do |f|
      @@inf_file = f
      fillFile     
      @@inf_file.close
    end
  end
  
  def fillFile 
    fillVersion
    print("")
    fillStrings
    print("")
    fillCeStrings(@@regs_dlls)
    print("")
    fillCeDevice
    print("")
    fillDefInstall(nil)
    print("")
    fillSourceDiskNames
    print("")
    fillSourceDiskFiles
    print("")
    fillDstDirs
    print("")
    fillCopyFilesSections
    print("")
    fillRegKeys
  end
  
  def parseDirs
  end
  
  def print(data)
    if @@inf_file && @@inf_file.kind_of?(File)
      @@inf_file.puts(data)  
    end     
  end
  
  def fillVersion   
    print("[Version]")    
    print("Signature=\"$Windows NT$\"")
    print("Provider=\"rhomobile\"")
    print("CESignature=\"$Windows CE$\"")
  end
  
  def fillStrings
    print("[Strings]")    
    print("Manufacturer=\"rhomobile\"")
  end 
  
  def fillCeStrings(app_name)
    print("[CEStrings]")
    print("AppName=\" + app_name + \"")
    print("InstallDir=%CE1%\\%AppName%")
  end
  
  def fillCeDevice
    print("[CEDevice]") 
    print("VersionMin=5.00")
    print("VersionMax=7.99")
    print("BuildMax=0xE0000000")
  end
  
  def fillDefInstall(regs_dlls)
    
    print("[DefaultInstall]")
         
    if (!regs_dlls.nil? && regs_dlls.lenght > 0)
      regs_dlls_string = ""
      
      regs_dlls.each do |dll|
        regs_dlls_string += dll.to_s
        regs_dlls_string += "," 
      end
      
      print("CESelfRegister=" + regs_dlls_string)
    end
    
    print("CEShortcuts=Shortcuts")
    print("AddReg=RegKeys")
  end
  
  def fillSourceDiskNames
    print("[SourceDisksNames]")
    @@src_disk_names.each { |disk|
      print(disk[:number] + "=,\"\",,\"" + disk[:path])
    }
  end
  
  def fillSourceDiskFiles
    print("[SourceDisksFiles]")
    
    @@src_disk_files.each { |disk|
      print("\"" + disk[:name] + "\"=" + disk[:number])      
    }    
  end
  
  def fillDstDirs    
    print("[DestinationDirs]")
    #@@dst_disk_names = nil
  end
  
  def fillCopyFilesSections
    fillCopyToInstallDir
    print("")
    fillCopySystemFiles
    print("")
    fillCopyConfig
    print("")
    
    @@dst_disk_files = nil
  end
  
  def fillCopyToInstallDir
    print("[CopyToInstallDir]")
  end
  
  def fillCopyConfig
    print("[CopyConfig]")
  end
  
  def fillCopySystemFiles
    print("[CopySystemFiles]")
  end
   
  def fillRegKeys
  end
     
end