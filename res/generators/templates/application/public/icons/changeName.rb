

basedir = "./"
contains = Dir.new(basedir).entries
count = 0;

contains.each { |filename|
  
  puts filename

    old_name = "#{filename}"
    
    new_name = nil
    old_name.gsub!(/\d+-(.*)/) {
        new_name = $1
    }
    
    
    if !new_name.nil?
        File.rename(filename,new_name)
    end
}