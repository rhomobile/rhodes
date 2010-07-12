class RhoFileSpec
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

    def binary_readwrite_test
        file_testname = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.png')
        test_content = File.binread(file_testname)
        Test_equal(File.size(file_testname), test_content.length)
    
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'temp.png')
        File.delete(file_name) if File.exists?(file_name)
        Test_equal(File.exists?(file_name), false)

        f = File.new(file_name, "wb")
        f.write(test_content)
        f.close        
        
        content = File.binread(file_name)
        Test_equal( content, test_content )
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
	    if  dd == 2
	        return "limit"
	    else
	        # if less than 2, save record of file in model Save, open a new file, write content to file.
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
        
        (1..2).each do |n|
            res = create_file_in_cache(dir_name, "cache_test", n.to_s())
            Test_equal( res, "saved" )
        end
        
        res = create_file_in_cache(dir_name, "cache_test", "1")
        Test_equal( res, "exist" )
        
        res = create_file_in_cache(dir_name, "cache_test", "3")
        Test_equal( res, "limit" )
        
    end

    def isfileexist_test
        Test_equal( File.exist?(Rho::RhoApplication::get_model_path('app', 'Spec')), true ) if System.get_property('platform') != 'Blackberry'
        Test_equal( File.exist?(Rho::RhoApplication::get_blob_folder()), true )
        Test_equal( File.exist?( File.join( __rhoGetCurrentDir(), 'RhoLog.txt')),  true )
        
        bExc = false
        begin
            File.exist?(nil)
        rescue Exception => e
            bExc = e.is_a?(TypeError)
        end
        
        Test_equal( bExc, true )
        Test_equal( File.exist?(""), false )        
    end
    
    def readnonexistfile_test
        file_name = Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_345'
        Test_equal( File.exist?(file_name), false )

        #TODO: crash after this exception on windows mobile(rb_sys_fail)
        #https://www.pivotaltracker.com/story/show/4164945
        if System.get_property('platform') != 'WINDOWS'    
            bExc = false
            begin
                File.binread(file_name)
            rescue Exception => e
                bExc = e.is_a?(SystemCallError)
            end
            Test_equal( bExc, true )
        end    
    end
    
    def clear
        (1..2).each do |n|
    	    file_name = File.join(Rho::RhoApplication::get_model_path('app', 'cache'), "cache_test"+ n.to_s())
            File.delete(file_name) if File.exists?(file_name)
        end
    
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'temp.txt')
        File.delete(file_name) if File.exists?(file_name)
    end
end    
