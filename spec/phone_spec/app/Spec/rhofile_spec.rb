class RhoFileTest
    def initialize
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
    
    def clear
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'temp.txt')
        File.delete(file_name) if File.exists?(file_name)
    end
end    