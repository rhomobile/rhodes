describe "URI" do

    before(:all) do
        @@file_name = File.join(Rho::RhoApplication::get_base_app_path(), 'local_file.txt')
        unless File.exist?(@@file_name)
            write_data  = "this is rhodes test"
            f = File.new(@@file_name, "w")
            f.write(write_data)
            f.close        
        end
    end

    after(:all) do
unless System.get_property('platform') == 'WINDOWS'
        File.delete(@@file_name) if File.exist?(@@file_name)
end        
    end

    it "test navigate local page" do
        WebView.navigate "/app"
    end

    it "test navigate external http" do
        WebView.navigate "http://www.google.com/images/icons/product/chrome-48.png?rho_open_target=_blank"
    end

    it "test navigate local file" do
        puts "runnuni open local file: #{@@file_name}"
        WebView.navigate "file://" + @@file_name
    end

    it "test navigate phone uri" do
        WebView.navigate "tel:1234567"
    end

    it "test navigate sms uri" do
        WebView.navigate "sms:+1234567?body=test%20sms%20message"
    end

    it "test navigate mailto URI" do
        WebView.navigate "mailto:dev@rhomobile.com?body=test%20phone%20spec%20mail%20message"
    end

    it "test navigate video" do
        WebView.navigate "video.avi"
    end

    it "test open local page" do
        WebView.navigate "/app"
    end

    it "test open external http" do
        WebView.navigate "http://www.google.com/images/icons/product/chrome-48.png?rho_open_target=_blank"
    end

    it "test open local file" do
        puts "runnuni open local file: #{@@file_name}"
        System.open_url "file://" + @@file_name
    end

    it "test open phone uri" do
        System.open_url "tel:1234567" unless System.get_property('platform') == 'WINDOWS_DESKTOP'
    end

    it "test open sms uri" do
        System.open_url "sms:+1234567?body=test%20sms%20message" unless System.get_property('platform') == 'WINDOWS_DESKTOP'
    end

    it "test open mailto URI" do
        System.open_url "mailto:dev@rhomobile.com?body=test%20phone%20spec%20mail%20message" unless System.get_property('platform') == 'WINDOWS_DESKTOP'
    end

    it "test open video" do
        System.open_url "video.avi" unless System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP'
    end

end
