class JsonTest
    def initialize
    end

    def parse_test
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        
        Test_equal( parsed.is_a?(Array), true )
        Test_equal( parsed.length(), 3 )
        Test_equal( parsed[0]['count'], 6 )
        Test_equal( parsed[1][0], 'item1' )
        Test_equal( parsed[1][2], 3.2 )
        Test_equal( parsed[2]['object_value'].is_a?(Hash), true )
        Test_equal( parsed[2]['object_value']['attrib'], "xform" )
    end

    def circular_test
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'circtest.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        Test_equal( parsed.is_a?(Hash), true )
        Test_equal( parsed.length(), 9 )
        Test_equal( parsed['ObjectUrl'], "http://192.168.1.104:3000/mapi/trips/83.xml" )
        Test_equal( parsed['Name'], "My New Trip" )

        require 'json'
        
        gen_content = ::JSON.generate(parsed)
        parsed2 = Rho::JSON.parse(gen_content)
        Test_equal( parsed, parsed2 )
        
    end
    
    def parseseparator_test
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'septest.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        puts "parsed: #{parsed}"
        
        Test_equal( parsed.is_a?(Hash), true )
        Test_equal( parsed["Manufacturer List"].is_a?(Array), true )
        Test_equal( parsed["Category List"].is_a?(Array), true )
    end
    
    def webservice_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get( :url => 'http://rhostore.heroku.com/products.json' )
        puts "res : #{res}"  
        Test_equal(res['status'],'ok')
        
        parsed = res['body']
        Test_equal( parsed.is_a?(Array), true )
        Test_equal( parsed[0].is_a?(Hash), true )    
    end
    
    def generate_test
        require 'json'
        
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        
        gen_content = ::JSON.generate(parsed)
        parsed2 = Rho::JSON.parse(gen_content)
        Test_equal( parsed, parsed2 )
        
    end
    
    def clear
    end
end    