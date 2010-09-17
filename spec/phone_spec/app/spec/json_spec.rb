describe "Json" do
    it "should parse" do
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        
        parsed.is_a?(Array).should == true
        parsed.length().should ==  3
        parsed[0]['count'].should ==  6 
        parsed[1][0].should ==  'item1'
        parsed[1][2].should ==  3.2
        parsed[2]['object_value'].is_a?(Hash).should ==  true
        parsed[2]['object_value']['attrib'].should ==  "xform"
    end
    it "should parse big json" do
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'big_test.json')
        content = File.read(file_name)
        
        #GC.stress=true
        parsed = nil
        #(1..10).each do |i|
            #puts "i : #{i}"
            #GC.start
            parsed = Rho::JSON.parse(content)
            
            #puts "#{parsed[0]['coupleName']}"
        #end    

        parsed.is_a?(Array).should == true
        
        #GC.stress=false
    end

    it "should parse circular" do
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'circtest.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        parsed.is_a?(Hash).should ==  true 
        parsed.length().should ==  9 
        parsed['ObjectUrl'].should ==  "http://192.168.1.104:3000/mapi/trips/83.xml" 
        parsed['Name'].should ==  "My New Trip" 

        require 'json'
        
        gen_content = ::JSON.generate(parsed)
        parsed2 = Rho::JSON.parse(gen_content)
        parsed.should == parsed2
        
    end
    
    it "should parse with separator" do
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'septest.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        #puts "parsed: #{parsed}"
        
        parsed.is_a?(Hash).should ==  true 
        parsed["Manufacturer List"].is_a?(Array).should ==  true 
        parsed["Category List"].is_a?(Array).should ==  true 
    end

    it "should parse many times" do
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'perftest.json')
        content = File.read(file_name)
        
        (1..100).each do |i|
            Rho::JSON.parse(content)
        end    
    end

    it "should webservice" do
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get( :url => 'http://rhostore.heroku.com/products.json' )
        #puts "res : #{res}"  
        res['status'].should == 'ok'
        
        parsed = res['body']
        parsed.is_a?(Array).should ==  true
        parsed[0].is_a?(Hash).should ==  true 
    end

    it "should generate" do
        require 'json'
        
        file_name = File.join(Rho::RhoApplication::get_model_path('app','Data'), 'test.json')
        content = File.read(file_name)
        parsed = Rho::JSON.parse(content)
        
        gen_content = ::JSON.generate(parsed)
        parsed2 = Rho::JSON.parse(gen_content)
        parsed.should == parsed2
        
    end

    it "should generate rhom object" do
        require 'json'

        prod = Product.create("test"=>"123")
        parsed = Product.find(prod.object)
        gen_content = ::JSON.generate(parsed)
        gen_content.should == "{\"source_id\":#{prod.source_id},\"object\":\"#{prod.object}\",\"test\":\"123\"}"
        
    end    
    
=begin
    def webservice2_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get( :url => 'http://www.glutenfreechecklist.com/Mob_API/GetCat_MnfsList.aspx?checkListId=17&CategoryID=0&ManfName=&startindex=1&endindex=10' )
        puts "res : #{res}"  
        res['status'].should == 'ok'
        
        parsed = res['body']
        parsed.is_a?(Hash).should ==  true
        parsed["Manufacturer List"].is_a?(Array).should ==  true
        
        manf =  parsed["Manufacturer List"][10]["Manufacturer"]
        puts "manf : #{manf}"
        
        url2 = "http://www.glutenfreechecklist.com/Mob_API/GetCat_MnfsList.aspx?checkListId=17&CategoryID=0&ManfName=#{Rho::RhoSupport.url_encode(manf)}&startindex=1&endindex=10"
        
        res = Rho::AsyncHttp.get( :url => url2 )
        puts "res : #{res}"  
        res['status'].should == 'ok'
        
        parsed = res['body']
        parsed.is_a?(Hash).should ==  true
        parsed["Manufacturer List"].is_a?(Array).should ==  true
        
    end

    def webservice3_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get( :url => 'http://api.foursquare.com/v1/venues.json?geolat=37.331689&geolong=-122.030731' )
        puts "res : #{res}"  
        res['status'].should == 'ok'
        
        #@places = Array.new
        #res['body'].each do |attributes|
        #    puts "attributes : #{attributes}"
        #    @places << Place.new(attributes)
        #end        
    end
=end    
    
end    