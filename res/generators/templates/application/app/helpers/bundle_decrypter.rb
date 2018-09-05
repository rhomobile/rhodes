require 'openssl'
require 'base64'
require 'stringio'

module BundleDecrypter

  class << self

      def decrypt(data, key_64)
          alg = "AES-256-CBC"

          key = Base64.decode64(key_64)

          istream = StringIO.new(data)

          iv = istream.read(16)
          digest = istream.read(20)
          cipher = istream.read()

          de_aes = OpenSSL::Cipher::Cipher.new(alg)
          de_aes.decrypt
          de_aes.key = key
          de_aes.iv = iv

          decrypted_text = de_aes.update(cipher)
          decrypted_text << de_aes.final

          return decrypted_text
      end

      # you should add ".encrypted" to the filepath !
      def decrypt_file(path, key_64)
          ec = File.binread(path)
          dc = decrypt(ec, key_64)
          return dc
      end
  end
end