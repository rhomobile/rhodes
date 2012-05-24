require "hmac-md5"
require "hmac-sha1"

describe "Hmac" do
    it "should pass s_digest" do
    
        key = "\x0b" * 16
        text = "Hi There"

        hmac = HMAC::MD5.new(key)
        hmac.update(text)

        hmac.digest.should == HMAC::MD5.digest(key, text)
    end

    it "should pass s_hexdigest" do
    
        key = "\x0b" * 16
        text = "Hi There"

        hmac = HMAC::MD5.new(key)
        hmac.update(text)

        hmac.hexdigest.should == HMAC::MD5.hexdigest(key, text)
    end
 
    it "should pass hmac_md5_1" do

        HMAC::MD5.hexdigest("\x0b" * 16, "Hi There").should == "9294727a3638bb1c13f48ef8158bfc9d"
    end

    it "should pass hmac_md5_2" do
        HMAC::MD5.hexdigest("Jefe", "what do ya want for nothing?").should == "750c783e6ab0b503eaa86e310a5db738"
    end

    it "should pass hmac_md5_3" do
        HMAC::MD5.hexdigest("\xaa" * 16, "\xdd" * 50).should == "56be34521d144c88dbb8c733f0e8b3f6"
    end

    it "should pass hmac_md5_4" do
        HMAC::MD5.hexdigest(["0102030405060708090a0b0c0d0e0f10111213141516171819"].pack("H*"), "\xcd" * 50).should == "697eaf0aca3a3aea3a75164746ffaa79"
    end

    it "should pass hmac_md5_5" do
        HMAC::MD5.hexdigest("\x0c" * 16, "Test With Truncation").should == "56461ef2342edc00f9bab995690efd4c"
    end

    it "should pass hmac_md5_7" do
        HMAC::MD5.hexdigest("\xaa" * 80, "Test Using Larger Than Block-Size Key - Hash Key First").should == "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd"
    end

    it "should pass hmac_md5_8" do
        HMAC::MD5.hexdigest("\xaa" * 80, "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data").should == "6f630fad67cda0ee1fb1f562db3aa53e"
    end

    it "should reset_key" do
        hmac = HMAC::MD5.new("key")
        hmac.reset_key
        
        bExc = false
        begin
            hmac.update("foo")
        rescue Exception => e
            bExc = e.is_a?(RuntimeError)
        end        
        bExc.should ==  true  
        
    end

    it "should pass set_key" do
        hmac = HMAC::MD5.new
        bExc = false
        begin
            hmac.update("foo")
        rescue Exception => e
            bExc = e.is_a?(RuntimeError)
        end        
        bExc.should ==  true  
        
        hmac.reset_key
        bExc = false
        begin
            hmac.update("foo")
        rescue Exception => e
            bExc = e.is_a?(RuntimeError)
        end        
        bExc.should ==  true  
        
    end
    
end    