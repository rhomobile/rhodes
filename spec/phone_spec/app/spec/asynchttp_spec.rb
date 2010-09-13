require 'local_server'

describe "AsyncHttp" do

    after(:all) do
        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.jpg')
        File.delete(file_name) if File.exists?(file_name)
    end

    it "should http get" do
        return unless $is_network_available
        
        res = Rho::AsyncHttp.get(
          :url => 'http://www.apache.org/licenses/LICENSE-2.0' )
        
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        res['headers']['content-type'].should ==  'text/plain'
        res['body'].should_not be_nil

        # www.apache.org response with gzipped body if client declare 'Accept-Encoging: gzip'
        # (our network implementation for iPhone and Android does).
        # It means that content-length header will contain value less than
        # body length because body we have on application level is already decoded
        # This is why following two lines commented out
        #res['headers']['content-length'].should == 11358
        #res['body'].length.should == res['headers']['content-length'].to_i
        res['body'].length.should == 11358
    end

    it "should http post" do
        return unless $is_network_available    
        
        #TODO: post_test
    end
    
    it "should http download" do
        return unless $is_network_available

        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.jpg')
        File.delete(file_name) if File.exists?(file_name)
        File.exists?(file_name).should == false

        res = Rho::AsyncHttp.download_file(
          :url => 'http://rhomobile.com/wp-content/themes/rhomobile/img/imgs_21.jpg',
          :filename => file_name )
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        res['headers']['content-length'].to_i.should ==  8945
        res['headers']['content-type'].should == 'image/jpeg'

        File.exists?(file_name).should == true
        File.size(file_name).should == res['headers']['content-length'].to_i
    end

    it "should http upload" do
        return unless $is_network_available
        
        server = 'http://rhologs.heroku.com'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('app'), 'Data/test_log.txt')
        File.exists?(file_name).should ==  true

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :filename => file_name )
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        File.exists?(file_name).should ==  true
    end

    it "should http upload" do
        return unless $is_network_available
        
        server = 'http://rhologs.heroku.com'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('app'), 'Data/test_log.txt')
        File.open(file_name, "w"){|f| f.write("******************THIS IS TEST! REMOVE THIS FILE! *******************")}

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :filename => file_name )
          #optional parameters:
          #:filename_base => "phone_spec_file",
          #:name => "phone_spec_name" )
        
        res['status'].should == 'ok'
        File.exists?(file_name).should ==  true
    end

    it "should decode chunked body" do
      return unless $is_network_available

      if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'        
          host = SPEC_LOCAL_SERVER_HOST
          port = SPEC_LOCAL_SERVER_PORT
          puts "+++++++++++++++++++ chunked test: #{host}:#{port}"
          res = Rho::AsyncHttp.get :url => "http://#{host}:#{port}/chunked"
          res['status'].should == 'ok'
          res['body'].should_not be_nil
          res['body'].should == "1234567890"
      end
    end

=begin
    def upload_withbody_test
        return unless $is_network_available
        
        server = 'http://rhologs.heroku.com'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('app'), 'Data/test_log.txt')
        File.open(file_name, "w"){|f| f.write("******************THIS IS TEST! REMOVE THIS FILE! *******************")}

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :filename => file_name,
          :file_content_type => "application/octet-stream",
          :filename_base => "phone_spec_file",
          :name => "phone_spec_name",
          
          :body => "upload test",
          :headers => {"content-type"=>"plain/text"}
           )
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        File.exists?(file_name).should == true
    end

    def upload_multiple_test
        return unless $is_network_available
        
        server = 'http://rhologs.heroku.com'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('app'), 'Data/test_log.txt')
        File.open(file_name, "w"){|f| f.write("******************THIS IS TEST! REMOVE THIS FILE! *******************")}

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :multipart => [
              { 
                :filename => file_name,
                :filename_base => "phone_spec_file",
                :name => "phone_spec_name",
                :content_type => "application/octet-stream"
              },
              {
                :body => "upload test",
                :name => "phone_spec_bodyname",
                :content_type => "plain/text"
              }
           ]
        )
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        File.exists?(file_name).should == true
    end
=end  
# TODO: Fix this test!
=begin
    def httpsget_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get(
          :url => 'https://mail.google.com/' )
        
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        
        http_error = res['http_error'].to_i if res['http_error']
        if http_error == 301 || http_error == 302 #redirect
            res2 = Rho::AsyncHttp.get( :url => res['headers']['location'] )
            
            res2['status'].should == 'ok'
        end    
        
    end
=end
end    
