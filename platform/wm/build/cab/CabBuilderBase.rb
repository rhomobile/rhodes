
class CabBuilderBase

  @@inf_file = nil  
  @@sections = nil
  @@extension_files = nil
  @@extension_dlls = nil
  
  @@src_disk_names = nil # Array<Hash<id, path>>; 
  @@src_disk_files = nil # Array<Hash<id, name>>
  @@dst_disk_names = nil #
  
  @@settings = 
   [{:sdk => 'wm6',   :path => 'Windows Mobile 6 Professional SDK (ARMV4I)', :minver => 'VersionMin=5.02', :maxver => 'VersionMax=7.99'},
    {:sdk => 'wm653', :path => 'Windows Mobile 6.5.3 Professional DTK (ARMV4I)', :minver => 'VersionMin=5.02', :maxver => 'VersionMax=7.99'},
    {:sdk => 'ce5',   :path => 'MC3000c50b (ARMV4I)', :minver => 'VersionMin=5.00', :maxver => 'VersionMax=7.99'},
    {:sdk => 'ce7',   :path => 'WT41N0c70PSDK (ARMV4I)', :minver => 'VersionMin=5.00', :maxver => 'VersionMax=7.99'}]

  ###################################################################
      
  def initialize(app_name, setup_paths, hidden_app, run_on_startup, additional_dlls_paths, regs_dlls, regkeys)
    puts "@@setup_paths= " +  setup_paths.to_s     
    puts "@@hidden_app=" + hidden_app.to_s
    
    @@setup_paths           = setup_paths # hash of :webkit_data, :vcbin, :src
    @@app_name              = app_name
    @@hidden_app            = hidden_app   
    @@run_on_startup        = run_on_startup
    @@additional_dlls_paths = additional_dlls_paths
    @@regs_dlls             = regs_dlls
    @@regkeys               = regkeys
    
    @@is_icon          = File.exist? File.join(@@setup_paths[:src], "icon", "icon.ico")
    @@is_custom_config = File.exist? File.join(@@setup_paths[:src], "apps", "Config.xml")
  end
  
  ###################################################################
    
  def getDirsForParse
    sources = Array.new
    
    source = Hash.new
    source[:id]       = "db"
    source[:path]     = "..\\..\\..\\platform\\shared\\db\\res\\db"
    source[:dst_path] = "rho"
    source[:filter]   = "*"
    sources << source
    
    source = Hash.new
    source[:id]       = "lib"
    source[:path]     = File.join @@setup_paths[:src], "lib"
    source[:dst_path] = "rho"
    source[:filter]   = "*"
    sources << source
    
    source = Hash.new
    source[:id]       = "apps"
    source[:path]     = File.join @@setup_paths[:src], "apps"
    source[:dst_path] = "rho\\apps"
    source[:filter]   = "*"
    sources << source
    
    path_idx = 1
    
    @@additional_dlls_paths.each { |path|
      source = Hash.new
      source[:id]       = "add" + path_idx.to_s 
      source[:path]     = path
      source[:dst_path] = ""
      source[:filter] = "*"
        
      sources << source
      
      path_idx = path_idx + 1      
    }
    
    source = Hash.new
    source[:id]       = ""
    source[:path]     = @@setup_paths[:vcbin]
    source[:dst_path] = ""
    source[:filter]   = "*.dll"
    sources << source
    
    source = Hash.new
    source[:id]       = ""
    source[:path]     = @@setup_paths[:vcbin]
    source[:dst_path] = ""
    source[:filter]   = @@app_name + ".exe"
    sources << source

    source = Hash.new
    source[:id]       = ""
    source[:path]     = @@setup_paths[:vcbin]
    source[:dst_path] = ""
    source[:filter]   = "RhoLaunch.exe"
    sources << source
            
    source = Hash.new
    source[:id]       = "Config"
    source[:path]     = File.join @@setup_paths[:webkit_data], "Config"
    source[:dst_path] = "Config"
    source[:filter]   = "*"
    sources << source
    
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
  
  def parseDirsReqursive(dir, relative_path, filter, dst_path, disk_names, disk_files, dst_disk_names, dst_disk_files, dir_idx)
    curr_dir_idx = dir_idx + 1        
    curr_dir     = Dir.pwd
    
    puts "dir=" + dir.to_s
    if File.exist? dir
      
      dir_src_hash = Hash.new
      dir_src_hash[:number] = dir_idx
      dir_src_hash[:path]   = dir
      
      disk_names << dir_src_hash
      
      chdir dir
      
      if !dst_disk_files.nil? && dst_disk_files.kind_of?(Hash) && !dst_disk_files.has_key?("files")
        dst_disk_files[:files] = Array.new
      end
              
      Dir.glob(filter).each { |f|

        next if f == "." || f == ".."
        
        if File.directory?(f)
          dir_name = f
          rel_path = File.join relative_path, f
                         
          dst_disk_hash = Hash.new     
          dst_disk_hash[:name]   = rel_path.clone.gsub("/", "_")
          dst_disk_hash[:path]   = File.join dst_path, rel_path
          dst_disk_hash[:number] = curr_dir_idx

          dst_disk_names << dst_disk_hash
          
          curr_dir_idx = parseDirsReqursive(File.join(dir, f), rel_path, filter, dst_path, disk_names, disk_files, dst_disk_names, dst_disk_hash, curr_dir_idx)
        else
          file_src_hash = Hash.new
           
          file_src_hash[:name]   = f.to_s
          file_src_hash[:number] = dir_idx
          
          disk_files << file_src_hash 
          
          if !dst_disk_files.nil? && dst_disk_files.kind_of?(Hash)
            dst_disk_files[:files] << f.to_s     
          end                   
        end
      }
    end
    
    chdir curr_dir
    
    return curr_dir_idx
  end
  
  def parseDirs(dirs_for_parse)
    @@src_disk_names = Array.new
    @@src_disk_files = Array.new    
    @@dst_disk_names = Array.new
    
    curr_dir_idx = 0
    curr_dir     = Dir.pwd
    
    if dirs_for_parse.kind_of?(Array)
      dirs_for_parse.each { |dir|
        dst_disk_hash = nil
        
        if !dir[:dst_path].empty?
          dst_disk_hash = Hash.new     
          dst_disk_hash[:name]   = "" 
          dst_disk_hash[:path]   = dir[:dst_path]
          dst_disk_hash[:number] = curr_dir_idx
            
          @@dst_disk_names << dst_disk_hash
        end
        
        curr_dir_idx = parseDirsReqursive(dir[:path], "", dir[:filter], dir[:dst_path], @@src_disk_names, @@src_disk_files, @@dst_disk_names, dst_disk_hash, curr_dir_idx)
      }  
    end
    
    chdir curr_dir
  end
  
  #################################################################################
  
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
    print("AppName=\"" + @@app_name + "\"")
    print("InstallDir=%CE1%\\%AppName%")
  end
 
  def fillCeDevice
    print("[CEDevice]")
    print("VersionMin=5.00")
    print("VersionMax=7.99")
    print("BuildMax=0xE0000000")
  end
 
  def fillPredefineFileCopies
    return "CopyToInstallDir,CopySystemFiles"
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
    
    copy_files_string = fillPredefineFileCopies
    
    @@dst_disk_names.each { |disk|
      if !disk[:files].nil? && !disk[:files].empty?   
        copy_files_string = copy_files_string + ",copyfiles_add" + disk[:number].to_s + disk[:name]
      end
    }
     
    shortcut_string = ""
    shortcut_string = shortcut_string + "Shortcuts"         if @@hidden_app == false
    shortcut_string = shortcut_string + ",ShortcutsAutorun" if @@run_on_startup == true

    print("CEShortcuts=" + shortcut_string) 
    print("AddReg=RegKeys")
    print("CopyFiles=" + copy_files_string)
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
    print("Shortcuts=0,\"%CE11%\"")       if @@hidden_app == false
    print("ShortcutsAutorun=0,\"%CE4%\"") if @@run_on_startup == true
    print("CopySystemFiles=0,\"%CE2%\"");
    print("CopyToInstallDir=0,\"%InstallDir%\"")
    
    @@dst_disk_names.each { |disk|      
      if !disk[:files].nil? && !disk[:files].empty?    
        print "copyfiles_add" + disk[:number].to_s + disk[:name] + "=0,\"" + File.join("%InstallDir%", disk[:path] + "\"").gsub("/", "\\")
      end
    }
    
  end
  
  def fillCopyFilesSections
    fillCopyToInstallDir
    print ""
    fillCopySystemFiles
    print ""

    @@dst_disk_names.each { |disk|      
      if !disk[:files].nil? && !disk[:files].empty?       
        print "[copyfiles_add" + disk[:number].to_s + disk[:name] + "]"
          
        disk[:files].each { |file|
          print "\"" + file + "\",\"" + file + "\",,0"
        }
        
        print ""
      end        
    }
  end
  
  def fillCopyToInstallDir
    print("[CopyToInstallDir]")
  end
  
  def fillCopyToInstallDir
    print("[CopyToInstallDir]")
    print("\"" + @@app_name + ".exe\",\"" + @@app_name + ".exe\",,0");
    print("\"" + "RhoLaunch" + ".exe\",\"" + "RhoLaunch" + ".exe\",,0");
    print("\"license_rc.dll\",\"license_rc.dll\",,0");
  end
    
  def fillCopySystemFiles
    print("[CopySystemFiles]")
    print("\"prtlib.dll\",\"prtlib.dll\",,0")
  end
   
  def fillRegKeys
    print("[RegKeys]")
    
    return if @@regkeys.nil?
    
    @@regkeys.each { |key|
      print(key)      
    }
  end
     
end