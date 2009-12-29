# cbc.rb  Richard Kernahan <kernighan_rich@rubyforge.org>

module Crypt
module CBC
  
  require 'crypt/stringio'
  require 'crypt/stringxor'
  
  ULONG = 0x100000000
  
  # When this module is mixed in with an encryption class, the class
  # must provide three methods: encrypt_block(block) and decrypt_block(block)
  # and block_size()
  
  
  def generate_initialization_vector(words)
    srand(Time.now.to_i)
    vector = ""
    words.times {
      vector << [rand(ULONG)].pack('N')
    }
    return(vector)
  end
  
  
  def encrypt_stream(plainStream, cryptStream)
    # Cypher-block-chain mode
    
    initVector = generate_initialization_vector(block_size() / 4)
    chain = encrypt_block(initVector)
    cryptStream.write(chain)

    while ((block = plainStream.read(block_size())) && (block.length == block_size()))
      block = block ^ chain 
      encrypted = encrypt_block(block)
      cryptStream.write(encrypted)
      chain = encrypted
    end
   
    # write the final block
    # At most block_size()-1 bytes can be part of the message. 
    # That means the final byte can be used to store the number of meaningful
    # bytes in the final block
    block = '' if block.nil?
    buffer = block.split('')
    remainingMessageBytes = buffer.length
    # we use 7-bit characters to avoid possible strange behavior on the Mac
    remainingMessageBytes.upto(block_size()-2) { buffer << rand(128).chr }
    buffer << remainingMessageBytes.chr
    block = buffer.join('')
    block = block ^ chain
    encrypted = encrypt_block(block)
    cryptStream.write(encrypted)
  end
  
  
  def decrypt_stream(cryptStream, plainStream)
    # Cypher-block-chain mode
    chain = cryptStream.read(block_size())

    while (block = cryptStream.read(block_size()))
      decrypted = decrypt_block(block)
      plainText = decrypted ^ chain
      plainStream.write(plainText) unless cryptStream.eof?
      chain = block
    end
    
    # write the final block, omitting the padding
    buffer = plainText.split('')
    remainingMessageBytes = buffer.last.unpack('C').first
    remainingMessageBytes.times { plainStream.write(buffer.shift) }
  end
  
  
  def carefully_open_file(filename, mode)
    begin
      aFile = File.new(filename, mode)
    rescue
      puts "Sorry. There was a problem opening the file <#{filename}>."
      aFile.close() unless aFile.nil?
      raise
    end
    return(aFile)
  end
  
  
  def encrypt_file(plainFilename, cryptFilename)
    plainFile = carefully_open_file(plainFilename, 'rb')
    cryptFile = carefully_open_file(cryptFilename, 'wb+')
    encrypt_stream(plainFile, cryptFile)
    plainFile.close unless plainFile.closed?
    cryptFile.close unless cryptFile.closed?
  end
  
  
  def decrypt_file(cryptFilename, plainFilename)
    cryptFile = carefully_open_file(cryptFilename, 'rb')
    plainFile = carefully_open_file(plainFilename, 'wb+')
    decrypt_stream(cryptFile, plainFile)
    cryptFile.close unless cryptFile.closed?
    plainFile.close unless plainFile.closed?
  end
  
  
  def encrypt_string(plainText)
    plainStream = StringIO.new(plainText)
    cryptStream = StringIO.new('')
    encrypt_stream(plainStream, cryptStream)
    cryptText = cryptStream.string
    return(cryptText)
  end
  
  
  def decrypt_string(cryptText)
    cryptStream = StringIO.new(cryptText)
    plainStream = StringIO.new('')
    decrypt_stream(cryptStream, plainStream)
    plainText = plainStream.string
    return(plainText)
  end
  
end
end