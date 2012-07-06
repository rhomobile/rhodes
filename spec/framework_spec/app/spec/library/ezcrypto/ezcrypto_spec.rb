require 'ezcrypto'

describe "EZCrypto" do
    it "should generate key" do
        algo, size = "aes-256-cbc", 32   
        key = EzCrypto::Key.generate :algorithm => algo    
        
        puts "key: #{key}"
        
        key.should_not be_nil
    end
end

