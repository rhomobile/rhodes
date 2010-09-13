describe "Crypt" do
 
  it "should crypt" do

    require 'crypt/rijndael' 
    rijndael = Crypt::Rijndael.new("A key 16, 24, or 32 bytes length")
    plainString = "Bla bla bla !!!"
    encryptedString = rijndael.encrypt_string(plainString)
    decryptedString = rijndael.decrypt_string(encryptedString)

    plainString.should == decryptedString
  end

end
