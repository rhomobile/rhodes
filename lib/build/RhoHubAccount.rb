require 'openssl'
require 'base64'
require 'digest/sha2'

class CRC32
  @@crc_t = nil

  class << self
    def generate_table()
      divisor = [0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 16, 22, 23, 26, 32].inject(0) {|sum, exponent| sum + (1 << (32 - exponent))}
      @@crc_t  = Array.new(256) do |octet|
        remainder = octet
        (0..7).each do |i|
          if !remainder[i].zero?
            remainder ^= (divisor << i)
          end
        end
        remainder >> 8
      end
    end

    def calc_i(string, crc = 0)
      if @@crc_t.nil?
        generate_table()
      end

      crc ^= 0xffff_ffff
      string.each_byte do |octet|
        remainder_1 = crc >> 8
        remainder_2 = @@crc_t[(crc & 0xff) ^ octet]
        crc = remainder_1 ^ remainder_2
      end
      crc ^ 0xffff_ffff
    end

    def calc_a(string)
      [calc_i(string)].pack('l<')
    end
  end
end

class HashBase64
  class << self
    def safe_encode(hash)
      Hash[hash.map {|k,v| [k, Base64.encode64(v).gsub("\n", '')]}]
    end

    def safe_decode(hash)
      result = {}
      begin
        result = Hash[hash.map {|k,v| [k, Base64.decode64(v)]}]
      rescue Exception => e
        result = {}
      end

      result
    end
  end
end

class RhoHubAccount
  @@token_preamble_len = 16
  @@token_size = 50
  @@min_update_interval = 60*60*24
  @@token_file_name = 'token'
  @@salt_file_name = 'salt'

  attr_reader :token_preamble_len, :token_size, :salt

  class << self
    def is_valid_token?(token)
      if !token.nil? && !token.empty?
        res = /([0-9a-f]{#{@@token_size}})/.match(token)
        return !res.nil? && !res[1].nil?
      end

      false
    end

    def generate_preamble(len)
      range = ((48..57).to_a+(65..90).to_a+(97..122).to_a)
      ([nil]*len).map { range.sample.chr }.join
    end

    def bin_to_hex(s)
      s.unpack('H*').first
    end

    def hex_to_bin(s)
      s.scan(/../).map { |x| x.hex }.pack('c*')
    end

    def encode_val(encodeable, salt, iv = nil)
      cipher = OpenSSL::Cipher::Cipher.new("aes-256-cbc")
      cipher.encrypt
      cipher.key = Digest::SHA2.hexdigest(salt)

      if iv.nil?
        iv = cipher.random_iv
      end
      cipher.iv = iv

      #random preamble
      encrypted = cipher.update(encodeable)
      encrypted << cipher.final

      return encrypted, iv
    end

    def decode_val(decodeable, salt, iv)
      cipher = OpenSSL::Cipher::Cipher.new("aes-256-cbc")
      cipher.decrypt

      cipher.key = Digest::SHA2.hexdigest(salt)
      cipher.iv = iv

      # and decrypt it
      decrypted = cipher.update(decodeable)
      decrypted << cipher.final

      decrypted
    end

    def encode_token_old(token, salt, token_preamble_len, iv = nil, subscription = nil)
      #random preamble
      token_code = generate_preamble(token_preamble_len)
      token_code << token
      token_code << Digest::SHA2.hexdigest(token)

      encrypted, iv = encode_val(token_code, salt, iv)

      curr_time = [Time.now.to_i].pack('l<')
      time_code = curr_time + CRC32.calc_a(curr_time)

      time, iv = encode_val(time_code, salt, iv)

      if !(subscription.nil? || subscription.empty?)
        sub_code_crc = subscription + CRC32.calc_a(subscription)
        sub_code, iv = encode_val(sub_code_crc, salt, iv)
      else
        sub_code = ""
      end

      message = HashBase64.safe_encode({:iv => iv, :token => encrypted, :lt => time, :ft => sub_code})

      JSON.generate(message)
    end

    def check_field(field, salt, iv)
      decode = decode_val(field, salt, iv)
      len = CRC32.calc_a("token").length
      code = decode.slice(0, decode.length - len)
      code_hash = decode.slice(decode.length - len, len)

      return code, code_hash == CRC32.calc_a(code)
    end

    def decode_validate_token_old(token_hash, salt)
      code = 0
      result = {}

      result[:token] = nil
      result[:lt] = 0
      result[:ft] = nil

      if !token_hash.nil?
        message = HashBase64.safe_decode(token_hash)

        if !(message["iv"].nil? || message["token"].nil?)
          result[:iv] = message["iv"]

          decrypted = decode_val(message["token"], salt, message["iv"])

          tokenlen = decrypted.length - @@token_preamble_len - Digest::SHA2.hexdigest("token").length
          tok = decrypted.slice(@@token_preamble_len, tokenlen)

          if is_valid_token?(tok)
            token_hash = decrypted.slice(tokenlen + @@token_preamble_len,decrypted.length)

            if token_hash != Digest::SHA2.hexdigest(tok)
              return nil
            else
              result[:token] = tok
            end
          else
            return nil
          end

          if !(message["lt"].nil?)
            code, is_correct = check_field(message["lt"], salt, message["iv"])

            if is_correct
              result[:lt] = code.unpack('l<').first
            end
          end

          if !(message["ft"].nil? || message["ft"].empty?)
            code, is_correct = check_field(message["ft"], salt, message["iv"])

            if is_correct
              result[:ft] = code
            end
          end
        else
          return nil
        end
      else
        return nil
      end

      result
    end

    def env_safe_decode64(env_val_name)
      res = nil
      val = ENV[env_val_name]
      if !val.nil?
        begin
          res = JSON.parse(Base64.decode64(val))
        rescue
          res = nil
        end
      end

      res
    end

    def remove_account_files(token_folder)
      token_file = File.join(token_folder, @@token_file_name)
      salt_file = File.join(token_folder, @@salt_file_name)

      files = [token_file, salt_file]

      begin
        files.each do |f_name|
          if File.exist?(f_name)
            File.delete(f_name)
          end
        end
      rescue => e
        puts "could not delete token files"
        raise
      end
    end
  end

  attr_accessor :salt, :iv
  attr_reader :changed

  def initialize()
    self.clear()
  end

  def clear()
    @iv = nil
    @salt = SecureRandom.urlsafe_base64(32)
    @info = {
      :subscription => nil,
      :token => nil,
      :time => 0,
      :server => nil
    }
    @changed = true
  end

  def server()
    @info[:server].nil? ? "" : @info[:server]
  end

  def server=(value)
    if value != @info[:server]
      @changed = true
    end

    @info[:server] = value
  end

  def token()
    @info[:token]
  end

  def token=(value)
    new_val = self.class.is_valid_token?(value) ? value : nil
    # reset time because token was changed
    if new_val != @info[:token]
      @changed = true
      @info[:time] = 0
    end

    @info[:token] = new_val
  end

  def is_valid_token?()
    !(@info[:token].nil? || @info[:token].empty?)
  end

  def subscription()
    @info[:subscription]
  end

  def subscription=(value)
    @changed = @changed || value != @info[:subscription]
    @info[:subscription] = value
  end

  def is_valid_subscription?()
    remaining_subscription_time() > 0
  end

  def remaining_subscription_time()
    diff = 0

    if !(@info[:subscription].nil? || @info[:subscription].empty?)
      begin
        diff = [0,(JSON.parse(@info[:subscription])["tokenValidUntil"]).to_i - Time.now.to_i].max
      rescue Exception => e
        diff = 0
      end
    end

    diff
  end

  def to_boolean(s)
    if s.kind_of?(String)
      !!(s =~ /^(true|t|yes|y|1)$/i)
    elsif s.kind_of?(TrueClass)
      true
    else
      false
    end
  end

  def subsciption_plan()
    text = "free" 

    begin
      resp = JSON.parse(subscription)
      unsigned = subscription.gsub(/"signature":"[^"]*"/, '"signature":""')
      hash = Digest::SHA1.hexdigest(unsigned)

      if (resp["signature"] == Digest::SHA1.hexdigest(unsigned))
        if !(resp["features"].nil? && resp["features"]["plan"].nil?)
          text = resp["features"]["plan"]
        end
      else
        text = "invalid"
      end
    rescue Exception => e
    end

    text
  end

  def subscription_level()
    level = -1

    if is_valid_subscription?()
      case subsciption_plan()
      when "premium"
        level = 2
      when "gold"
        level = 2
      when "enterprise"
        level = 1
      when "silver"
        level = 1
      when "free"
        level = 0
      else
        level = -1
      end
    end

    level
  end

  def time()
    @info[:time]
  end

  def time=(value)
    @changed = @changed || value != @info[:time]
    @info[:time] = value
  end

  def is_outdated()
    (@info[:time] - Time.now.to_i > @@min_update_interval) || @changed
  end

  def remaining_time()
    [0,@info[:time] + @@min_update_interval - Time.now.to_i].max
  end

  def read_token_from_env()
    packed = self.class.env_safe_decode64('RHOMOBILE_USER_INFO')
    if !packed.nil?
      self.token = packed["token"]
      self.time = Time.now.to_i
      self.subscription = packed["subscription"]
      @changed = false
    end

    !packed.nil? && self.is_valid_token?()
  end

  def encode_token_for_env()
    HashBase64.safe_encode(
      JSON.generate(
        {
          "token" => self.token,
          "subscription" => self.subscription
        }
      )
    )
  end

  def read_token_from_files(token_folder)
    result = false

    token_file = File.join(token_folder, @@token_file_name)
    salt_file = File.join(token_folder, @@salt_file_name)

    if File.exist?(token_file)
      begin
        data = JSON.parse(File.read(token_file))

        if !data["lt"].nil? || data["salt"].nil?
          if File.exist?(salt_file)
            salt = File.read(salt_file)

            result = self.class.decode_validate_token_old(data, salt)

            # changed will be automatically updated
            if !result.nil?
              @iv = result[:iv]
              @salt = salt
              self.token = result[:token]
              self.time = result[:lt]
              self.subscription = result[:ft]
            end

            result = true
          end
        else

          message = HashBase64.safe_decode(data)

          if !(message["iv"].nil? || message["data"].nil? || message["salt"].nil?)
            decrypted = self.class.decode_val(message["data"], message["salt"], message["iv"])

            data_len = decrypted.length - @@token_preamble_len - Digest::SHA2.hexdigest("data").length
            data = decrypted.slice(@@token_preamble_len, data_len)

            data_hash = decrypted.slice(data_len + @@token_preamble_len,decrypted.length)

            if data_hash == Digest::SHA2.hexdigest(data)
              packed = JSON.parse(data)
              packed.each do |key, value|
                case key
                when "token"
                  self.token = value
                when "time"
                  self.time = value
                when "subscription"
                  self.subscription = value
                when "server"
                  self.server = value
                else
                  raise "Unknown key #{key} in config"
                end
              end

              @changed = false

              result = true
            end
          end
        end
      rescue Exception => e
        puts "exception! #{e.inspect}"
        self.clear
      end
    end

    result
  end

  def save_token(token_folder)
    result = false

    self.time = Time.now.to_i

    payload = JSON.generate(@info)
    data = self.class.generate_preamble(@@token_preamble_len) + payload + Digest::SHA2.hexdigest(payload)

    encrypted, @iv = self.class.encode_val(data, @salt)

    message = {
      :iv => @iv,
      :salt => @salt,
      :data => encrypted
    }

    packed = JSON.generate(HashBase64.safe_encode(message))

    File.open(File.join(token_folder, @@token_file_name),"w") { |f| f.write(packed) }

    @changed = false
  end
end
