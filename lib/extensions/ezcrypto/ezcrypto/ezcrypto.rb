require 'openssl'
require 'digest/sha2'
require 'digest/sha1'
require 'base64'

module EzCrypto #:nodoc:


=begin rdoc
The Key is the only class you need to understand for simple use.

=== Algorithms

The crypto algorithms default to aes-128-cbc however on any of the class methods you can change it to one of the standard openssl cipher names using the
optional <tt>:algorithm=>alg name</tt> parameter.

Eg. 
    Key.new @raw, :algorithm=>"des"
    Key.generate :algorithm=>"blowfish"
    Key.with_password @pwd,@salt,:algorithm=>"aes256"    


== License

ActiveCrypto and EzCrypto are released under the MIT license.


== Support

To contact the author, send mail to pelleb@gmail.com

Also see my blogs at:
http://stakeventures.com and
http://neubia.com

This project was based on code used in my project StakeItOut, where you can securely share web services with your partners.
https://stakeitout.com

(C) 2005 Pelle Braendgaard

=end

  class Key

    attr_reader :raw, :algorithm

    @@block_size = 512

=begin rdoc
Set the block-size for IO-operations (default: 512 bytes)
=end
    def self.block_size=(size)
      @@block_size = size
    end

=begin rdoc
Return the block-size for IO-operations.
=end
    def self.block_size
      @@block_size
    end


=begin rdoc
Initialize the key with raw unencoded binary key data. This needs to be at least
16 bytes long for the default aes-128 algorithm.
=end
    def initialize(raw,options = {})
      @raw=raw
      @algorithm=options[:algorithm]||"aes-128-cbc"
    end

=begin rdoc
Generate random key.
=end
    def self.generate(options = {})
      Key.new(EzCrypto::Digester.generate_key(calculate_key_size(options[:algorithm])),options)
    end

=begin rdoc
Create key generated from the given password and salt
=end
    def self.with_password(password,salt,options = {})
      Key.new(EzCrypto::Digester.get_key(password,salt,calculate_key_size(options[:algorithm])),options)
    end

=begin rdoc
Initialize the key with Base64 encoded key data.
=end
    def self.decode(encoded,options = {})
      Key.new(Base64.decode64(encoded),options)
    end

=begin rdoc
Encrypts the data with the given password and a salt. Short hand for:

  key=Key.with_password(password,salt,options)
  key.encrypt(data)

=end
    def self.encrypt_with_password(password,salt,data,options = {})
      key=Key.with_password(password,salt,options)
      key.encrypt(data)
    end

=begin rdoc
Decrypts the data with the given password and a salt. Short hand for:

  key=Key.with_password(password,salt,options)
  key.decrypt(data)


=end
    def self.decrypt_with_password(password,salt,data,options = {})
      key=Key.with_password(password,salt,options)
      key.decrypt(data)
    end

=begin rdoc
Given an algorithm this calculates the keysize. This is used by both
the generate and with_password methods. This is not yet 100% complete.
=end
    def self.calculate_key_size(algorithm)
      if !algorithm.nil?
        algorithm=~/^([[:alnum:]]+)(-(\d+))?/
        if $3
          size=($3.to_i)/8
        else
          case $1
            when "bf"
              size = 16
            when "blowfish"
              size = 16
            when "des"
              size = 8
            when "des3"
              size = 24
            when "aes128"
              size = 16
            when "aes192"
              size = 24
            when "aes256"
              size = 32
            when "rc2"
              size = 16
            when "rc4"
              size = 16
            else
              size = 16
            end
        end
      end
      if size.nil?
        size = 16
      end

      size
    end

=begin rdoc
returns the Base64 encoded key.
=end
    def encode
      Base64.encode64(@raw).chop
    end

=begin rdoc
returns the Base64 encoded key. Synonym for encode.
=end
    def to_s
      encode
    end

=begin rdoc
Encrypts the data and returns it in encrypted binary form.
=end
    def encrypt(data)
      if data==nil || data==""
        nil
      else
        encrypter("")
        @cipher.encrypt(data)
      end
    end

=begin rdoc
Encrypts the data and returns it in encrypted Base64 encoded form.
=end
    def encrypt64(data)
      Base64.encode64(encrypt(data))
    end

=begin rdoc
Decrypts the data passed to it in binary format.
=end
    def decrypt(data)
      if data==nil || data==""
        nil
      else
        decrypter("")
        @cipher.gulp(data)
      end
#    rescue
#      puts @algorithm
#      puts self.encode
#      puts data.size
#      throw $!
    end

=begin rdoc
Decrypts a Base64 formatted string
=end
    def decrypt64(data)
      decrypt(Base64.decode64(data))
    end
    
=begin rdoc
Allows keys to be marshalled
=end
    def marshal_dump
       "#{self.algorithm}$$$#{self.encode}"
    end
    
=begin rdoc
Allows keys to be unmarshalled
=end
    def marshal_load(s) 
       a, r = s.split '$$$'
       @algorithm = a
       @raw = Base64.decode64(r)
    end

=begin rdoc
Create a file with minimal permissions, and yield
it to a block. By default only the owner may read it.
=end
    def safe_create(file, mod=0400, mask=0066)
      begin
        old_umask = File.umask
        File.umask(mask)
        File.open(file, File::CREAT | File::EXCL | File::WRONLY) do |f|
          yield(f) if block_given?
        end
      ensure
        File.umask(old_umask)
      end
      File.chmod(mod, file) if File.exists?(file)
      File.size(file)
    end


=begin rdoc
Overwrite a file with zero values before removing it's filesystem inode.
This is not very safe :-)
=end
    def safe_delete(file)
      begin
        to_clear = File.size(file)
        zeroes   = Array.new(Key.block_size, "\0").join
        File.chmod(0600, file)
        File.open(file, File::WRONLY) do |f|
          f.rewind
          while to_clear > 0
            f.write(zeroes)
            to_clear -= Key.block_size
          end
          f.flush
        end
      ensure
        File.delete(file) if File.exists?(file)
      end
      return !File.exists?(file)
    end


=begin rdoc
Open a file readonly and yield it to a block.
=end
    def safe_read(file)
      File.open(file, File::RDONLY) do |i|
        yield(i) if block_given?
      end
    end

    private :safe_create, :safe_read, :safe_delete

=begin rdoc
Load a key from a yaml_file generated via Key#store.
=end
    def self.load(filename)
      require 'yaml'
      hash = YAML::load_file(filename)
      req = proc { |k| hash[k] or raise "Missing element #{k} in #{filename}" }
      key = self.new Base64.decode64(req.call(:key)) , :algorithm => req.call(:algorithm)
      return key
    end


=begin rdoc
Save the key data into a file, try to do this in a secure manner.
NOTE: YAML::store & friends are not used to encance control over
the generated file format.
=end
    def store(filename)
      safe_create(filename) do |f|
        selfenc = self.encode
        f.puts "---"
        f.puts ":EZCRYPTO KEY FILE: KEEP THIS SECURE !"
        f.puts ":created: #{Time.now}"
        f.puts ":algorithm: #{@algorithm}"
        f.puts ":key: #{selfenc}"
      end
    end


=begin rdoc
Get a Encrypter object. You have to call #final on it by yourself!
=end
    def encrypter(target='')
      @cipher = EzCrypto::Encrypter.new(self,target,@algorithm)
    end


=begin rdoc
Get a Decrypter object. You have to call #final on it by yourself!
=end
    def decrypter(target='')
      @cipher = EzCrypto::Decrypter.new(self,target,@algorithm)
    end

=begin rdoc
Create a Decrypter object and yield it to a block.
You must *not* call #final by yourself, the method
does this.
=end
    def on_decrypter(target='', &block)
      decrypter(target)
      on_cipher(&block)
    end

=begin rdoc
Create an Encrypter object and yield it to a block.
You must *not* call #final by yourself, the method
does this.
=end
    def on_encrypter(target='', &block)
      encrypter(target)
      on_cipher(&block)
    end


=begin rdoc
Helper method, yields the current cipher to a block.
=end
    def on_cipher(&block)
      begin
        block.call @cipher
      ensure
        @cipher.final
      end
    end

    private :on_cipher


=begin rdoc
Encrypt a file 'inplace' and add a suffix
see #cipher_file.
IMPORTANT: The inputfile will be deleted by default.
=end
    def encrypt_file(src, tgt=nil, options = {} )
      options = { :suffix => '.ez', :autoclean => 'true' }.update(options)
      tgt = "#{src}#{options[:suffix]}" unless tgt
      cipher_file :on_encrypter, src, tgt, options[:autoclean]
    end


=begin rdoc
Decrypt a file 'inplace' and remove a suffix
see #cipher_file
IMPORTANT: The inputfile will be deleted by default.
=end
    def decrypt_file(src, tgt=nil, options = {} )
      options = { :suffix => '.ez', :autoclean => 'true' }.update(options)
      unless tgt
        tgt = src
        tgt = tgt.gsub(/#{options[:suffix]}$/, '')
      end
      cipher_file :on_decrypter, src, tgt, options[:autoclean]
    end


=begin rdoc
uses either  #on_decrypter or #on_encrypter to transform a given sourcefile
to a targetfile. the sourcefile is deleted after sucessful transformation
the delete_source is 'true'.
=end
    def cipher_file(method, sourcefile, targetfile, delete_source)
      raise(ArgumentError, "source == target #{sourcefile}") if sourcefile == targetfile
      safe_create(targetfile,0600) do |o|
        self.send(method, o) do |c|
          safe_read(sourcefile) do |i|
            loop do
              buffer = i.read(Key.block_size) or break
              c << buffer
            end
          end
        end
      end
      safe_delete(sourcefile) if delete_source && File.exists?(targetfile)
      return targetfile
    end
    
    private :cipher_file
    
  end
  
=begin rdoc
Abstract Wrapper around OpenSSL's Cipher object. Extended by Encrypter and Decrypter.
  
You probably should be using the Key class instead.

Warning! The interface may change.

=end
  class CipherWrapper #:nodoc:

=begin rdoc
  
=end
    def initialize(key,target,mode,algorithm)
      @cipher = OpenSSL::Cipher::Cipher.new(algorithm)  
      if mode
        @cipher.encrypt
      else
        @cipher.decrypt
      end
      @cipher.key=key.raw
      @cipher.padding=1
      @target=target
      @finished=false
    end
    
    
    def to_target(data)
      return data
    end
    
=begin rdoc
Process the givend data with the cipher.
=end
    def update(data)
      reset if @finished
      @target<< to_target(@cipher.update(data))
    end

=begin rdoc
  
=end
    def <<(data)
      update(data)
    end
    
=begin rdoc
Finishes up any last bits of data in the cipher and returns the final result.
=end
    def final
      @target<< to_target(@cipher.final)
      @finished=true
      @target
    end
    
=begin rdoc
Processes the entire data string using update and performs a final on it returning the data.
=end
    def gulp(data)
      update(data)
      final 
#    rescue
#      breakpoint
    end  

=begin rdoc
  
=end
    def reset(target="")
      @target=target
      @finished=false
    end
  end

=begin rdoc
Wrapper around OpenSSL Cipher for Encryption use.

You probably should be using Key instead.

Warning! The interface may change.

=end
  class Encrypter<EzCrypto::CipherWrapper #:nodoc:

=begin rdoc
  
=end
    def initialize(key,target="",algorithm="aes-128-cbc")
      super(key,target,true,algorithm)
    end
    
=begin rdoc
  
=end    
    def encrypt(data)    
      gulp(data)
    end
  end

=begin rdoc
Wrapper around OpenSSL Cipher for Decryption use.

You probably should be using Key instead.

Warning! The interface may change.
=end
  class Decrypter<EzCrypto::CipherWrapper #:nodoc:
=begin rdoc
  
=end
    def initialize(key,target="",algorithm="aes-128-cbc")
      super(key,target,false,algorithm)
    end
    
=begin rdoc
  
=end    
    def decrypt(data)
      gulp(data)
    end
  end

=begin rdoc

=end
  class Digester    
=begin rdoc
Various handy Digest methods. 

Warning! The interface may change.
=end
    def self.get_key(password,salt,size)
        digest(salt+password,size)
    end
  
=begin rdoc
  
=end
    def self.generate_key(size=16)
        key=OpenSSL::Random.random_bytes(size)
        digest(key,size)
    end
  
=begin rdoc
  
=end
    def self.generate_key64(size=32)
        key=OpenSSL::Random.random_bytes(size)
        digest64(key,size)
    end   
=begin rdoc

=end
    def self.generate_hexkey(size=40)
        key=OpenSSL::Random.random_bytes(size)
        hexdigest(key,size)
    end   
    
=begin rdoc
  
=end
    def self.digest(data,size=16)
      if size==0
        ""
      elsif size<=16
        Digest::SHA1.digest(data)[0..(size-1)]
      else
        Digest::SHA256.digest(data)[0..(size-1)]
      end
    end

=begin rdoc

=end
    def self.hexdigest(data,size=40)
      if size==0
        ""
      elsif size<=40
        Digest::SHA1.hexdigest(data)[0..(size-1)]
      else
        Digest::SHA256.hexdigest(data)[0..(size-1)]
      end
    end
    
=begin rdoc
  
=end
    def self.digest64(data,size=16)
      Base64.encode64(digest(data,size))
    end
	end

end


