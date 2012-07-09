$:.unshift(File.dirname(__FILE__) + "/../lib/")

require 'test/unit'
require 'fileutils'
require 'ezcrypto'
require 'base64'

class EzCryptoTest < Test::Unit::TestCase

  def setup
  end

  def test_generate_alg_key
    assert_generate_alg_key "aes-128-cbc",16
    assert_generate_alg_key "aes-192-cbc",24
    assert_generate_alg_key "aes-256-cbc",32    
    assert_generate_alg_key "rc2-40-cbc",5 
    assert_generate_alg_key "rc2-64-cbc",8
    assert_generate_alg_key "rc4-64" ,8
    assert_generate_alg_key "blowfish" ,16
    assert_generate_alg_key "des" ,8
  end
  
  def test_with_password
      assert_with_password "","secret","aes-128-cbc",16
      assert_with_password "test","secret","aes-128-cbc",16
      assert_with_password "password","secret","aes-128-cbc",16
      assert_with_password "a�sldfad8q5�34j2�l4j24l6j2456","secret","aes-128-cbc",16
      
      assert_with_password "","secret","aes-192-cbc",24
      assert_with_password "test","secret","aes-192-cbc",24
      assert_with_password "password","secret","aes-192-cbc",24
      assert_with_password "a�sldfad8q5�34j2�l4j24l6j2456","secret","aes-192-cbc",24

      assert_with_password "","secret","aes-256-cbc",32
      assert_with_password "test","secret","aes-256-cbc",32
      assert_with_password "password","secret","aes-256-cbc",32
      assert_with_password "a�sldfad8q5�34j2�l4j24l6j2456","secret","aes-256-cbc",32

  end

  def test_encoded
    0.upto 32 do |size|
      assert_encoded_keys size
    end
  end
  
  def test_encrypt    
    0.upto(CLEAR_TEXT.size-1) do |size|
      assert_encrypt CLEAR_TEXT[0..size]
    end  
  end
  
  def test_decrypt    
    0.upto(CLEAR_TEXT.size) do |size|
      assert_decrypt CLEAR_TEXT[0..size]
    end  
  end

  def test_decrypt64
    0.upto(CLEAR_TEXT.size) do |size|
      assert_decrypt64 CLEAR_TEXT[0..size]
    end  
  end
    
  def test_keyfile_store_load

    algo, size = "aes-256-cbc", 32   
    keyfile = 'ezcrypto-test.key'
    
    FileUtils.rm [keyfile], :force => true
    key = EzCrypto::Key.generate :algorithm => algo    
    assert_file_not_exists keyfile    
    key.store keyfile    
    assert_file_exists keyfile    
    assert_file_permissions keyfile, 0100400    
    
    key2 = EzCrypto::Key.load(keyfile)    
    assert_equal key.raw, key2.raw
    
    FileUtils.rm [keyfile], :force => true    
  end
  
  def test_filestuff_with_defaults      

    clearfile = 'lorem_ipsum.txt'
    keyfile = 'lorem_ipsum.key'
    algo, size = "aes-256-cbc", 32
    
    File.open(clearfile, 'w') { |f| f.write(CLEAR_TEXT) }
    assert_file_contains clearfile, CLEAR_TEXT    

    key = EzCrypto::Key.generate :algorithm => algo    

    # default behaviour: remove clearfile, append '.ez' suffix  
    cryptfile = key.encrypt_file(clearfile)    
    assert_equal cryptfile, clearfile + ".ez"
    assert_file_not_exists clearfile
    assert_file_exists cryptfile 
    assert_file_contains cryptfile, key.encrypt(CLEAR_TEXT)    

    # default behaviour: unlink cryptfile and remove suffix from filename
    clearfile = key.decrypt_file cryptfile
    assert_file_exists clearfile
    assert_file_not_exists cryptfile
    assert_file_contains clearfile, CLEAR_TEXT    
    FileUtils.rm [keyfile, clearfile, cryptfile], :force => true
  end
  
  def test_filestuff_with_options
  
    clearfile = 'lorem_ipsum.txt'
    keyfile = 'lorem_ipsum.key'
    algo, size = "aes-256-cbc", 32    

    File.open(clearfile, 'w') { |f| f.write(CLEAR_TEXT) }
    assert_file_contains clearfile, CLEAR_TEXT
    
    key = EzCrypto::Key.generate :algorithm => algo      
    
    # with options: keep the original file, auto-create a 
    # new filename with a user-defined suffix        
    cryptfile = key.encrypt_file(clearfile, nil, :autoclean => false, :suffix => '.Encrypted')
    assert_equal cryptfile, clearfile + ".Encrypted", 'suffix was added'
    assert_file_exists clearfile
    assert_file_exists cryptfile    
    assert_file_permissions cryptfile, 0100600
    
    assert_raises(Errno::EEXIST, "the original file would not be overwritten") { 
      key.decrypt_file(cryptfile, nil, :autoclean => false, :suffix => '.Encrypted')
    } 
    FileUtils.rm [clearfile], :force => true    

    clearfile = key.decrypt_file(cryptfile, nil, :autoclean => false, :suffix => '.Encrypted')
    assert_equal cryptfile, clearfile + ".Encrypted", 'suffix was removed'    
    assert_file_exists clearfile
    assert_file_exists cryptfile
    assert_file_permissions cryptfile, 0100600    
    assert_file_contains clearfile, CLEAR_TEXT    

    FileUtils.rm [keyfile, clearfile, cryptfile], :force => true        
  end
    
  def assert_file_permissions(filename, mode, msg='')
    fmode = File.stat(filename).mode
    assert_equal fmode, mode, msg 
  end
  
  def assert_file_exists(filename)
    assert File.exists?(filename)
  end

  def assert_file_not_exists(filename)
    assert !File.exists?(filename)
  end
  
  def assert_file_contains(filename, expected)
    assert_file_exists(filename)
    content = File.open(filename,'r').read
    assert_equal expected, content
  end
  
  def assert_key_size(size,key)
    assert_equal size,key.raw.size      
  end
  
  def assert_generate_alg_key(algorithm,size)
    key=EzCrypto::Key.generate :algorithm=>algorithm
    assert_key_size size,key 
  end
 
  def assert_with_password(password,salt,algorithm,size)
    key=EzCrypto::Key.with_password password,salt,:algorithm=>algorithm
    assert_key_size size,key
    assert_equal key.raw,EzCrypto::Key.with_password( password,salt,:algorithm=>algorithm).raw
  end
  
  def assert_encoded_keys(size)
    key=EzCrypto::Key.generate size
    key2=EzCrypto::Key.decode(key.encode)
    assert_equal key.raw, key2.raw    
  end
  
  def assert_encrypt(clear)
    ALGORITHMS.each do |alg|
      key=EzCrypto::Key.generate :algorithm=>alg
      encrypted=key.encrypt clear
      assert_not_nil encrypted    
    end
  end
  
  def assert_decrypt(clear)
    ALGORITHMS.each do |alg|
      key=EzCrypto::Key.generate :algorithm=>alg
      encrypted=key.encrypt clear
      assert_not_nil encrypted
      assert_equal clear,key.decrypt(encrypted)
    end
  end
  
  def assert_decrypt64(clear)
    key=EzCrypto::Key.generate
    encrypted=key.encrypt64 clear
    assert_not_nil encrypted
    assert_equal clear,key.decrypt64(encrypted)
  end
  
  ALGORITHMS=["aes128","bf","blowfish","des","des3","rc4","rc2"]
  CLEAR_TEXT="Lorem ipsum dolor sit amet, suspendisse id interdum mus leo id. Sapien tempus consequat nullam, platea vitae sociis sed elementum et fermentum, vel praesent eget. Sed blandit augue, molestie mus sed habitant, semper voluptatibus neque, nullam a augue. Aptent imperdiet curabitur, quam quis laoreet. Dolor magna. Quis vestibulum amet eu arcu fringilla nibh, mi urna sunt dictumst nulla, elit quisque purus eros, sem hendrerit. Vulputate tortor rhoncus ac nonummy tortor nulla. Nunc id nunc luctus ligula."
end


