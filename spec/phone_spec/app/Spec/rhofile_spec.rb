class RhoFileTest
    def initialize
        clear()
    end

    def write_test
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'temp.txt')
        File.delete(file_name) if File.exists?(file_name)
        Test_equal(File.exists?(file_name), false)

        write_data  = "this is rhodes test"
        #File.open(file_name, "w"){|file| file.write(write_data)}
        f = File.new(file_name, "w")
        f.write(write_data)
        f.close        
        
        content = File.read(file_name)
        Test_equal( content, write_data )

        write_data1  = "one more test"
        File.open(file_name, "w"){|file| file.write(write_data1)}
        content = File.read(file_name)
        Test_equal( content, write_data1 )
        
        write_data2  = ";add more test"
        File.open(file_name, "a"){|file| file.write(write_data2)}
        content = File.read(file_name)
        Test_equal( content, write_data1 + write_data2 )
        
    end

    def create_file_in_cache(dir_name, file, ext)
	    # get full file path
	    f = File.join(dir_name, "#{file}"+ "#{ext}")
        #check if file exists and return to fileName action if yes.
	    return "exist" if File.exists?(f)
	    
        # if no, get number of files in saving location. call delete action if equals 14
        files = Dir.entries(dir_name)
        #puts "files: #{files}"
	    dd = files.size - 2 #skip . and ..
	    if  dd == 14
	        return "limit"
	    else
	        # if less than 14, save record of file in model Save, open a new file, write content to file.
            content = "TEST cache"
            content = File.new("#{f}", "w")
            content.write(content)
            content.close
            
            return "saved"
        end
        
    end
    
    def dir_test
        dir_name = Rho::RhoApplication::get_model_path('app', 'cache')
        Dir.mkdir(dir_name) unless Dir.exists?(dir_name)
        
        (1..14).each do |n|
            res = create_file_in_cache(dir_name, "cache_test", n.to_s())
            Test_equal( res, "saved" )
        end
        
        res = create_file_in_cache(dir_name, "cache_test", "1")
        Test_equal( res, "exist" )
        
        res = create_file_in_cache(dir_name, "cache_test", "15")
        Test_equal( res, "limit" )
        
    end
        
    def clear
        (1..14).each do |n|
    	    file_name = File.join(Rho::RhoApplication::get_model_path('app', 'cache'), "cache_test"+ n.to_s())
            File.delete(file_name) if File.exists?(file_name)
        end
    
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'temp.txt')
        File.delete(file_name) if File.exists?(file_name)
    end
end    