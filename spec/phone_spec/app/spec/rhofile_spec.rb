describe "RhoFile" do
    
    before(:all) do
        clear()
		dir_name = Rho::RhoApplication::get_app_path('DataTemp')
		Dir.mkdir(dir_name) unless Dir.exist?(dir_name)
    end
    
    it "should write" do
        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'temp.txt')
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false

        write_data  = "this is rhodes test"
        #File.open(file_name, "w"){|file| file.write(write_data)}
        f = File.new(file_name, "w")
        f.write(write_data)
        f.close        
        
        content = File.read(file_name)
        content.should ==  write_data 

        write_data1  = "one more test"
        File.open(file_name, "w"){|file| file.write(write_data1)}
        content = File.read(file_name)
        content.should ==  write_data1 
        
        write_data2  = ";add more test"
        File.open(file_name, "a"){|file| file.write(write_data2)}
        content = File.read(file_name)
        content.should ==  write_data1 + write_data2 
        
    end
if !defined?(RHO_WP7)
    it "should binary read/write" do
        file_testname = File.join(Rho::RhoApplication::get_model_path('app', 'Data'), 'test.png')
        test_content = File.binread(file_testname)
        File.size(file_testname).should == test_content.length
    
        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'temp.png')
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false

        f = File.new(file_name, "wb")
        f.write(test_content)
        f.close        
        
        content = File.binread(file_name)
        content.should == test_content
    end
end
    def create_file_in_cache(dir_name, file, ext)
	    # get full file path
	    f = File.join(dir_name, "#{file}"+ "#{ext}")
        #check if file exists and return to fileName action if yes.
	    return "exist" if File.exist?(f)
	   
        dd = 0    
if !defined?(RHO_WP7)     
        # if no, get number of files in saving location. call delete action if equals 14
        files = Dir.entries(dir_name)
        #puts "files: #{files}"
	    dd = files.size - 2 #skip . and ..
end        
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

    it "should dir" do
        dir_name = Rho::RhoApplication::get_app_path('cache')
        Dir.mkdir(dir_name) unless Dir.exist?(dir_name)
        
        (1..2).each do |n|
            res = create_file_in_cache(dir_name, "cache_test", n.to_s())
            res.should ==  "saved"
        end
        
        res = create_file_in_cache(dir_name, "cache_test", "1")
        res.should ==  "exist" 
if !defined?(RHO_WP7)         
        res = create_file_in_cache(dir_name, "cache_test", "3")
        res.should ==  "limit" 
end        
        
    end

    it "should isfileexist" do
        File.exist?(Rho::RhoApplication::get_model_path('app', 'spec')).should == true if System.get_property('platform') != 'Blackberry'
        File.exist?(Rho::RhoApplication::get_blob_folder()).should ==  true 
        File.exist?( File.join( __rhoGetCurrentDir(), 'rholog.txt')).should ==  true 
        
        bExc = false
        begin
            File.exist?(nil)
        rescue Exception => e
            bExc = e.is_a?(TypeError)
        end        
        
        bExc.should ==  true  
    
        File.exist?("").should == false
    end
    
   if !defined?(RHO_WP7)   
    it "should readnonexistfile" do
        file_name = Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_345'
        File.exist?(file_name).should ==  false 

        #TODO: crash after this exception on windows mobile(rb_sys_fail)
        #https://www.pivotaltracker.com/story/show/4164945
        if System.get_property('platform') != 'WINDOWS' && System.get_property('platform') != 'WINDOWS_DESKTOP'
            bExc = false
            begin
                File.binread(file_name)
            rescue Exception => e
                bExc = e.is_a?(SystemCallError)
            end
            bExc.should ==  true
        end    
    end
    end
    
    def clear
        (1..2).each do |n|
    	    file_name = File.join(Rho::RhoApplication::get_app_path('cache'), "cache_test"+ n.to_s())
            File.delete(file_name) if File.exist?(file_name)
        end
    
        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'temp.txt')
        File.delete(file_name) if File.exist?(file_name)
    end
end    
