require 'local_server'

describe "AsyncHttp" do

    after(:all) do
        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.jpg')
        File.delete(file_name) if File.exist?(file_name)

        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'test_log.txt')
        File.delete(file_name) if File.exist?(file_name)		
        
    end

    it "should http get" do
        res = Rho::AsyncHttp.get(
          :url => 'http://www.apache.org/licenses/LICENSE-2.0' )
        
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        res['headers']['content-type'].should ==  'text/plain; charset=utf-8'
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
        
        #TODO: post_test
    end

    it "should http download" do

        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.png')
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should == false

        reference_url = 'http://www.google.com/images/icons/product/chrome-48.png'
        reference_size = 1834

        res = Rho::AsyncHttp.download_file(
          :url => reference_url,
          :filename => file_name )
        puts "res : #{res}"  
        
        res['status'].should == 'ok'
        res['headers']['content-length'].to_i.should == reference_size
        res['headers']['content-type'].should == 'image/png'

        File.exist?(file_name).should == true
        orig_len = File.size(file_name)
        orig_len.should == res['headers']['content-length'].to_i

        #delete file for re-download  - see should http partial download
        #see another test should overwrite existing file on download, overwriteFile=true
        #File.delete(file_name) if File.exist?(file_name)

        #check that in case of one more download, files keeps the same        
        #res = Rho::AsyncHttp.download_file(
        #  :url => reference_url,
        #  :filename => file_name,
        #  :overwriteFile => true )
        #puts "res : #{res}"  
        
        #res['status'].should == 'ok'
        #res['headers']['content-length'].to_i.should == reference_size
        #res['http_error'].should == '200'

        #File.exist?(file_name).should == true
        #File.size(file_name).should == orig_len

        #check that in case of network error, files keeps the same        
        res = Rho::AsyncHttp.download_file(
          :url => 'http://www.google.com/images/icons/product/chrome-48_BAD.png',
          :filename => file_name,
          :overwriteFile => true )
        puts "res : #{res}"  
        res['status'].should == 'error'
        res['http_error'].should == '404'

        File.exist?(file_name).should == true
        File.size(file_name).should == orig_len
    end
#TODO: server should not return 503, it should just drop the connection somehow after writing part of the data    
=begin
    it "should http partial download" do
        
        host = SPEC_LOCAL_SERVER_HOST
        port = SPEC_LOCAL_SERVER_PORT
        url = "http://#{host}:#{port}/resume_download"
        
        
        filename = File.join(Rho::RhoApplication::get_base_app_path(), 'test.txt')
        tmpFilename = filename + '.rhodownload'
        modtimeFile = filename + '.modtime'
        
        File.delete(filename) if File.exist?(filename)
        File.delete(tmpFilename) if File.exist?(tmpFilename)
        File.delete(modtimeFile) if File.exist?(modtimeFile)

        File.exist?(filename).should == false
        File.exist?(tmpFilename).should == false
        File.exist?(modtimeFile).should == false
        
        res = Rho::AsyncHttp.download_file(:url => url, :filename => filename )
        
        puts "res : #{res}"  
        
        res['status'].should == 'error'
        res['http_error'].should == '503'
        res['headers']['content-length'].to_i.should == 10

        File.exist?(filename).should == false
        File.exist?(tmpFilename).should == true
        File.exist?(modtimeFile).should == true

        File.size(tmpFilename).should == 5
        
        #content = open(tmpFilename,'rb').read
        content = File.read(tmpFilename)
        
        content.should == '12345'
        
        res = Rho::AsyncHttp.download_file( :url => url, :filename => filename )
        
        puts "res : #{res}"
        
        res['status'].should == 'ok'
        res['headers']['content-length'].to_i.should == 5
        res['http_error'].should == '206'        
        
        File.exist?(filename).should == true
        File.exist?(tmpFilename).should == false
        File.exist?(modtimeFile).should == false
        
        File.size(filename).should == 10
        
        #content = open(filename,'rb').read
        content = File.read(filename)
        content.should == '1234567890'
    end
=end

    it "should http upload" do
        
        server = 'http://rhologs.heroku.com'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'test_log.txt')
        File.exist?(file_name).should ==  true

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :filename => file_name )
        #puts "res : #{res}"  
        
        res['status'].should == 'ok'
        File.exist?(file_name).should ==  true
    end

    it "should http upload" do
        
        server = 'http://rhologs.heroku.com'
		dir_name = Rho::RhoApplication::get_app_path('DataTemp')
		Dir.mkdir(dir_name) unless Dir.exist?(dir_name)
        
        file_name = File.join(dir_name, 'test_log.txt')
        puts " file_name : #{file_name}"
        File.open(file_name, "w"){|f| puts "OK"; f.write("******************THIS IS TEST! REMOVE THIS FILE! *******************")}

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?client_id=&device_pin=&log_name=uptest",
          :filename => file_name )
          #optional parameters:
          #:filename_base => "phone_spec_file",
          #:name => "phone_spec_name" )
        
        res['status'].should == 'ok'
        File.exist?(file_name).should ==  true
    end

#TODO: There are issues with connecting to localhost from WP8 emulator
if System.get_property('platform') != 'WP8'
    it "should decode chunked body" do

      host = SPEC_LOCAL_SERVER_HOST
      port = SPEC_LOCAL_SERVER_PORT
      puts "+++++++++++++++++++ chunked test: #{host}:#{port}"
      res = Rho::AsyncHttp.get :url => "http://#{host}:#{port}/chunked"
      res['status'].should == 'ok'
      res['body'].should_not be_nil
      res['body'].should == "1234567890"
    end
end

    it "should send custom command" do
        
        res = Rho::AsyncHttp.get(
          :url => 'http://www.apache.org/licenses/LICENSE-2.0',
          :http_command => 'PUT' )
        
        #puts "res : #{res}"  
        res['http_error'].should == '405'
        res['body'].index('The requested method PUT is not allowed for the URL').should_not be_nil
        
        res = Rho::AsyncHttp.post(
          :url => 'http://www.apache.org/licenses/LICENSE-2.0',
          :http_command => 'PUT' )
        
        #puts "res : #{res}"  
        res['http_error'].should == '405'
        res['body'].index('The requested method PUT is not allowed for the URL').should_not be_nil
        
    end    

    it "should upload with body" do
        
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
        File.exist?(file_name).should == true
    end

    it "should upload miltiple" do
        
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
        File.exist?(file_name).should == true
    end
    
if System.get_property('platform') != 'WP8'
    it "should send https request" do
            
        res = Rho::AsyncHttp.get(
          :url => 'https://rhologs.heroku.com' )
        
        puts "res : #{res}"  
        
        res['status'].should == 'ok'
        
        http_error = res['http_error'].to_i if res['http_error']
        if http_error == 301 || http_error == 302 #redirect
            res2 = Rho::AsyncHttp.get( :url => res['headers']['location'] )
            
            res2['status'].should == 'ok'
        end    
        
    end
end

    it "should overwrite existing file on download, overwriteFile=true" do
        targetFile = File.join(Rho::RhoApplication::get_base_app_path(), 'test.png')
        refFile = File.join(Rho::RhoApplication::get_base_app_path(), 'reference.dat')
        refUrl = 'http://www.google.com/images/icons/product/chrome-48.png'
        refSize = 1834
        
        File.delete(refFile) if File.exist?(refFile)
        Rho::Network::downloadFile( :url => refUrl, :filename => refFile )
        File.exist?(refFile).should == true
        File.size(refFile).should == refSize
        
        stamp = "#{Time.now}"
        
        File.delete(targetFile) if File.exist?(targetFile)
        File.open(targetFile,'w') { |outf| outf.write(stamp) }
        
        Rho::Network::downloadFile( :url => refUrl, :filename => targetFile, :overwriteFile => true )
        
        File.exist?(targetFile).should == true
        File.read(targetFile).should_not == stamp
        
        File.size(targetFile).should == refSize
        File.read(targetFile).should == File.read(refFile)
        
        File.delete(refFile)
        File.delete(targetFile)
    end

    it "should not download if file exists, overwriteFile=false" do
        
        refUrl = 'http://www.google.com/images/icons/product/chrome-48.png'
        targetFile = File.join(Rho::RhoApplication::get_base_app_path(), 'test.png')
        
        stamp = "#{Time.now}"
        
        File.delete(targetFile) if File.exist?(targetFile)
        File.open(targetFile,'w') { |outf| outf.write(stamp) }
        
        res = Rho::Network::downloadFile( :url => refUrl, :filename => targetFile, :overwriteFile => false )
        
        res["status"].should == "error"
        res["fileExists"].should == "true"
        
        File.exist?(targetFile).should == true
        File.size(targetFile).should == stamp.length
        File.read(targetFile).should == stamp
        
        File.delete(targetFile)
    end
    
    require 'fileutils'
    
    it "should create full path on download, createFolders=true" do
        
        refUrl = 'http://www.google.com/images/icons/product/chrome-48.png'
        targetDir = File.join(Rho::RhoApplication::get_base_app_path(), 'new_dir')
        targetFile = File.join(targetDir, 'test.png')
        refSize = 1834
        
        FileUtils.rm_rf(targetDir) if File.exist?(targetDir)
        
        Rho::Network::downloadFile( :url => refUrl, :filename => targetFile, :createFolders => true )
        
        File.exist?(targetFile).should == true
        File.size(targetFile).should == refSize
        
        FileUtils.rm_rf(targetDir)
    end
    
    it "should not download if path not exists, createFolders=false" do
        refUrl = 'http://www.google.com/images/icons/product/chrome-48.png'
        targetDir = File.join(Rho::RhoApplication::get_base_app_path(), 'new_dir')
        targetFile = File.join(targetDir, 'test.png')
        
        FileUtils.rm_rf(targetDir) if File.exist?(targetDir)
        
        Rho::Network::downloadFile( :url => refUrl, :filename => targetFile, :createFolders => false )
        
        File.exist?(targetFile).should == false
        File.exist?(targetDir).should == false
    end

end    
