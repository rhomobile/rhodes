require 'pathname'
require 'yaml'

#TODO check generate api 
class GeneratorTimeChecker
  
  @@latest_update_time = nil
  @@app_path           = nil
  @@is_run_always      = false
  @@cached_time        = nil
  @@do_cache           = false
   
  def find_latest_modified_date(path)
    #puts 'find_latest_modified_date.path=' + path.to_s
    
    latest_mod_time = nil      
    templates_dir   = Dir.new(path)
    
    templates_dir.each { |item|       
      if item == '.' || item == '..'
        next        
      end
  
      full_path = File.join(path, item)
      mod_time  = nil
      
      if File.directory?(full_path)
        mod_time = find_latest_modified_date(full_path)
        
        if mod_time.nil?
          next
        end            
      else
        mod_time = File.mtime(full_path)
      end
      
      if latest_mod_time.nil?
        latest_mod_time = mod_time
      else
        if latest_mod_time < mod_time
          latest_mod_time = mod_time
        end
      end
    }
    
    return latest_mod_time
  end
  
  def init(startdir, app_path)
    @@app_path       = app_path
    templates_path   = File.join(startdir, "res", "generators", "templates", "api")
    rhogen_path      = File.join(startdir, "res", "generators", "rhogen.rb")
    
    api_time         = find_latest_modified_date(templates_path)
    rhogen_time      = File.mtime(rhogen_path)
    last_update_time = nil
    
    last_change_data = find_latest_modified_date(templates_path)
    time_cache_path  = File.join(@@app_path, "bin", "tmp", "rhogen.time")
     
    if last_change_data.nil?
      @@is_run_always = true
    end
    
    if File.exist? time_cache_path
      time_cache_file = File.new(time_cache_path)
      @@cached_time   = Time.parse(time_cache_file.gets)
      time_cache_file.close
      
      @@latest_update_time = rhogen_time >= api_time ? rhogen_time : api_time    
      puts 'latest_update_time=' + @@latest_update_time.to_s
      puts "cached_time : #{@@cached_time}"
            
      if @@cached_time < @@latest_update_time
        @@is_run_always = true
      end       
    else
      @@is_run_always = true
    end
    
    @@do_cache = false
  end
  
  def check(xmlpath)
    #@@do_cache = false
    generate_xml = false
    
    extpath  = File.dirname(xmlpath)
    xml_time = File.mtime(File.new(xmlpath))
    
    #puts "xmlpath: #{xmlpath}; xml_time : #{xml_time}"
    # for generate in first time
    if @@is_run_always
      @@do_cache = true
      generate_xml = true
    elsif !(File.exist? File.join(extpath, "shared", "generated"))              ||
       !(File.exist? File.join(extpath, "platform", "android", "generated")) ||
       !(File.exist? File.join(extpath, "platform", "iphone", "generated"))  ||
#       !(File.exist? File.join(extpath, "platform", "osx", "generated"))     ||
#       !(File.exist? File.join(extpath, "platform", "wm", "generated"))      ||
#       !(File.exist? File.join(extpath, "platform", "wp8", "generated"))     ||
       !(File.exist? File.join(extpath, "..", "public", "api", "generated"))
      @@do_cache = true
      generate_xml = true
    elsif @@cached_time < xml_time
      puts "!!!"
      @@do_cache = true
      generate_xml = true
    end 
    
    generate_xml
  end
  
  def update()
    if @@do_cache == false
      #puts "@@do_cache is FALSE"
      return
    end
    
    time_cache_path  = File.join(@@app_path, "bin", "tmp", "rhogen.time")

    FileUtils.mkdir(File.join(@@app_path, "bin") ) unless File.exist? File.join(@@app_path, "bin")
    FileUtils.mkdir(File.join(@@app_path, "bin", "tmp") ) unless File.exist? File.join(@@app_path, "bin", "tmp")
    
    time_cache_file = File.new(time_cache_path, "w+")
    time_cache_file.puts Time.new
    time_cache_file.close()
  end
end