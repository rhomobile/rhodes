
class CabBuilder

  @@inf_file = nil  
  
  def saveInfFile(filepath)
    FileUtils.rm_f filepath

    f = File.new(filepath, "w+")


    
    f.close
  end
  
  def fill 
    fillVersion
    fillStrings
    fillCeStrings("")
    fillCeDevice
    fillDefInstall
    fillSourceDiskNames
    fillSourceDiskFiles
    fillDstDirs
    fillCopyFilesSections
    fillRegKeys
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
    print("InstallDir=%CE1%\%AppName%")
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
      CESelfRegister=CeODAX.dll,NoSIP.dll,PocketBrowser.dll
    end
    
    print("CEShortcuts=Shortcuts")
    print("AddReg=RegKeys")
    
    
    #CopyFiles=CopyToInstallDir,CopyConfig,CopySystemFiles,copyfiles_db,copyfiles_lib,copyfiles_apps,copyfiles_apps_public,copyfiles_apps_public_api,copyfiles_apps_public_jquery,copyfiles_apps_public_re1,copyfiles_add13,copyfiles_add13_Bin,copyfiles_add13_HTML,copyfiles_add13_HTML_images,copyfiles_add13_Image,copyfiles_add13_MPM,copyfiles_add13_Plugin

  end
  
  def fillSourceDiskNames
  end
  
  def fillSourceDiskFiles
  end
  

  def fillDstDirs
  end
  
  def fillCopyFilesSections
  end
   
  def fillRegKeys
  end
     
end