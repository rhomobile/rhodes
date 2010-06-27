class AsyncHttpTest
    def initialize
    end

    def get_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get(
          :url => 'http://www.apache.org/licenses/LICENSE-2.0' )
        
        puts "res : #{res}"  
        
        Test_equal(res['status'],'ok')
        Test_equal(res['headers']['content-length'].to_i, 11358 )
        Test_equal(res['headers']['content-type'], 'text/plain' )
        Test_not_equal(res['body'], nil )
        Test_equal(res['body'].length, res['headers']['content-length'].to_i )
    end

    def post_test
        return unless $is_network_available    
        
        #TODO: post_test
    end
    
    def download_test
        return unless $is_network_available

        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.jpg')
        File.delete(file_name) if File.exists?(file_name)
        Test_equal(File.exists?(file_name), false)

        res = Rho::AsyncHttp.download_file(
          :url => 'http://rhomobile.com/wp-content/themes/rhomobile/img/imgs_21.jpg',
          :filename => file_name )
        puts "res : #{res}"  
        
        Test_equal(res['status'],'ok')
        Test_equal(res['headers']['content-length'].to_i, 8945 )
        Test_equal(res['headers']['content-type'], 'image/jpeg' )

        Test_equal(File.exists?(file_name), true)
        Test_equal(File.size(file_name), res['headers']['content-length'].to_i )
    end

    def upload_test
        return unless $is_network_available
        #TODO: upload_test
=begin
        server = 'http://logserver.staging.rhohub.com/log_handler'
        
        file_name = File.join(Rho::RhoApplication::get_app_path('app'), 'Data/test_log.txt')
        Test_equal(File.exists?(file_name), true)

        res = Rho::AsyncHttp.upload_file(
          :url => server + "/client_log?log_name=phone_test",
          :filename => file_name )
        puts "res : #{res}"  
        
        Test_equal(res['status'],'ok')

        Test_equal(File.exists?(file_name), true)
=end        
    end
  
# TODO: Fix this test!
=begin
    def httpsget_test
        return unless $is_network_available
            
        res = Rho::AsyncHttp.get(
          :url => 'https://mail.google.com/' )
        
        puts "res : #{res}"  
        
        Test_equal(res['status'],'ok')
        
        http_error = res['http_error'].to_i if res['http_error']
        if http_error == 301 || http_error == 302 #redirect
            res2 = Rho::AsyncHttp.get( :url => res['headers']['location'] )
            
            Test_equal(res2['status'],'ok')
        end    
        
    end
=end
    def clear
        file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'test.jpg')
        File.delete(file_name) if File.exists?(file_name)
    end
end    