
class CabBuilderBase

  @@inf_file = nil  
  @@sections = nil
  @@extension_files = nil
  @@extension_dlls = nil
  
  @@src_disk_names = nil # Array<Hash<id, path>>; 
  @@src_disk_files = nil # Array<Hash<id, name>>
  @@dst_disk_names = nil #
  @@dst_disk_files = nil #
  
  def initialize(app_name, srcdir, hidden_app, wk_data_dir, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
    @@app_name              = app_name
    @@srcdir                = srcdir
    @@hidden_app            = hidden_app
    @@wk_data_dir           = wk_data_dir    
    @@run_on_startup        = run_on_startup
    @@additional_dlls_paths = additional_dlls_paths
    @@regs_dlls             = regs_dlls
    @@regkeys               = regkeys
    
    @@is_icon          = File.exist? File.join(srcdir, "icon", "icon.ico")
    @@is_custom_config = File.exist? File.join(srcdir, "apps", "Config.xml")
  end
  
  def getDirsForParse
    sources = Array.new
    
    source = Hash.new
    source[:id]   = "db"
    source[:path] = "..\\..\\..\\platform\\shared\\db\\res\\db"
    sources << source
    
    source = Hash.new
    source[:id]   = "lib"
    source[:path] = File.join @@srcdir, "lib"
    sources << source
    
    source = Hash.new
    source[:id]   = "apps"
    source[:path] = File.join @@srcdir, "apps"
    sources << source
    
    path_idx = 1
    
    @@additional_dlls_paths.each { |path|
      source = Hash.new
      source[:id]   = "add" +path_idx.to_s 
      source[:path] = path
      sources << source
      
      path_idx = path_idx + 1      
    }
    
    return sources
  end
  
  def saveInfFile(filepath)
    FileUtils.rm_f filepath if File.exists? filepath

    parseDirs(getDirsForParse)    
    
    File.open(filepath, 'w') do |f|
      @@inf_file = f
      fillFile     
      @@inf_file.close
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
  
  def parseDirsReqursive(dir, disk_names, disk_files, dir_idx)
    curr_dir_idx = dir_idx + 1        
    curr_dir     = Dir.pwd
    
    if File.exist? dir
      
      dir_hash = Hash.new
      dir_hash[:number] = curr_dir_idx
      dir_hash[:path]   = dir
      
      disk_names << dir_hash 
      
      chdir dir
      
      Dir.glob("*").each { |f|

        next if f == "." || f == ".."
        
        if File.directory?(f)
          curr_dir_idx = parseDirsReqursive(File.join(dir, f), disk_names, disk_files, curr_dir_idx)
        else
          file_hash = Hash.new
           
          file_hash[:name]   = f.to_s
          file_hash[:number] = curr_dir_idx
          
          disk_files << file_hash
        end
      }
    end      
    
    chdir curr_dir
    
    return curr_dir_idx
  end
  
  def parseDirs(dirs_for_parse)
    @@src_disk_names = Array.new
    @@src_disk_files = Array.new
    
    curr_dir_idx = 0
    curr_dir = Dir.pwd
    
    if dirs_for_parse.kind_of?(Array)
      dirs_for_parse.each { |dir|
        curr_dir_idx = parseDirsReqursive(dir[:path], @@src_disk_names, @@src_disk_files, curr_dir_idx)
      }  
    end
    
    chdir curr_dir
  end
  def print(data)
    if @@inf_file && @@inf_file.kind_of?(File)
      @@inf_file.puts(data)  
    end     
  end
  
  def fillSourceDiskNames
    print("[SourceDisksNames]")
    @@src_disk_names.each { |disk|
      print(disk[:number].to_s + "=,\"\",,\"" + disk[:path].to_s + "\"")
    }
  end
  
  def fillSourceDiskFiles
    print("[SourceDisksFiles]")
    
    @@src_disk_files.each { |disk|
      print("\"" + disk[:name].to_s + "\"=" + disk[:number].to_s)      
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
    
    #@@dst_disk_files.each { |files|
      
    #}
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
    print("[RegKeys]")
    
    return if @@regkeys.nil?
    
    @@regkeys.each { |key|
      print(key)      
    }
  end
     
end