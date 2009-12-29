# idea.rb  Richard Kernahan <kernighan_rich@rubyforge.org>

# IDEA (International Data Encryption Algorithm) by 
# Xuejia Lai and James Massey (1992).  Refer to license info at end.
# Ported by Richard Kernahan 2005

module Crypt
class IDEA
  
  require 'crypt/cbc'
  include Crypt::CBC

  require 'digest/md5'
  
  ULONG   = 0x100000000
  USHORT  = 0x10000
  
  ENCRYPT = 0
  DECRYPT = 1
  
  
  def block_size
    return(8)
  end
  
  
  def initialize(key128, mode)
    # IDEA is subject to attack unless the key is sufficiently random, so we
    # take an MD5 digest of a variable-length passphrase to ensure a solid key
    if (key128.class == String)  
      digest = Digest::MD5.new(key128).digest
      key128 = digest.unpack('n'*8)
    end
    raise "Key must be 128 bits (8 words)" unless (key128.class == Array) && (key128.length == 8)
    raise "Mode must be IDEA::ENCRYPT or IDEA::DECRYPT" unless ((mode == ENCRYPT) | (mode == DECRYPT))
    if (mode == ENCRYPT)
      @subkeys = generate_encryption_subkeys(key128)
    else (mode == DECRYPT)
      @subkeys = generate_decryption_subkeys(key128)
    end
  end
  
  
  def mul(a, b)
    modulus = 0x10001
    return((1 - b) % USHORT) if (a == 0)
    return((1 - a) % USHORT) if (b == 0)
    return((a * b) % modulus)
  end
  
  
  def mulInv(x)
    modulus = 0x10001
    x = x.to_i % USHORT
    return(x) if (x <= 1)
    t1 = USHORT / x
    y  = modulus % x
    if (y == 1)
      inv = (1 - t1) & 0xFFFF
      return(inv)
    end
    t0 = 1
    while (y != 1)
      q = x / y
      x = x % y
      t0 = t0 + (q * t1)
      return(t0) if (x == 1)
      q = y / x
      y = y % x
      t1 = t1 + (q * t0)
    end
    inv = (1 - t1) & 0xFFFF
    return(inv)
  end
  
  
  def generate_encryption_subkeys(key)
    encrypt_keys = []
    encrypt_keys[0..7] = key.dup
    8.upto(51) { |i|
      a = ((i + 1) % 8 > 0) ? (i-7)  : (i-15)
      b = ((i + 2) % 8 < 2) ? (i-14) : (i-6)
      encrypt_keys[i] = ((encrypt_keys[a] << 9) | (encrypt_keys[b] >> 7)) % USHORT
    }
    return(encrypt_keys)
  end
  
  
  def generate_decryption_subkeys(key)
    encrypt_keys = generate_encryption_subkeys(key)
    decrypt_keys = []
    decrypt_keys[48] = mulInv(encrypt_keys.shift)
    decrypt_keys[49] = (-encrypt_keys.shift) % USHORT
    decrypt_keys[50] = (-encrypt_keys.shift) % USHORT
    decrypt_keys[51] = mulInv(encrypt_keys.shift)
    42.step(0, -6) { |i|
      decrypt_keys[i+4] = encrypt_keys.shift % USHORT
      decrypt_keys[i+5] = encrypt_keys.shift % USHORT
      decrypt_keys[i]   = mulInv(encrypt_keys.shift)
      if (i ==0)
        decrypt_keys[1] = (-encrypt_keys.shift) % USHORT
        decrypt_keys[2] = (-encrypt_keys.shift) % USHORT
      else
        decrypt_keys[i+2] = (-encrypt_keys.shift) % USHORT
        decrypt_keys[i+1] = (-encrypt_keys.shift) % USHORT
      end
      decrypt_keys[i+3] = mulInv(encrypt_keys.shift)
    }
    return(decrypt_keys)
  end
  
  
  def crypt_pair(l, r)
    word = [l, r].pack('NN').unpack('nnnn')
    k = @subkeys[0..51]
    8.downto(1) { |i|
      word[0] = mul(word[0], k.shift)
      word[1] = (word[1] + k.shift) % USHORT
      word[2] = (word[2] + k.shift) % USHORT
      word[3] = mul(word[3], k.shift)
      t2 = word[0] ^ word[2]
      t2 = mul(t2, k.shift)
      t1 = (t2 + (word[1] ^ word[3])) % USHORT
      t1 = mul(t1, k.shift)
      t2 = (t1 + t2) % USHORT
      word[0] ^= t1
      word[3] ^= t2
      t2 ^= word[1]
      word[1] = word[2] ^ t1
      word[2] = t2
    }
    result = []
    result << mul(word[0], k.shift)
    result << (word[2] + k.shift) % USHORT
    result << (word[1] + k.shift) % USHORT
    result << mul(word[3], k.shift)
    twoLongs = result.pack('nnnn').unpack('NN')
    return(twoLongs)
  end
  
  def encrypt_block(block)
    xl, xr = block.unpack('NN')
    xl, xr = crypt_pair(xl, xr)
    encrypted = [xl, xr].pack('NN')
    return(encrypted)
  end
  
  
  def decrypt_block(block)
    xl, xr = block.unpack('NN')
    xl, xr = crypt_pair(xl, xr)
    decrypted = [xl, xr].pack('NN')
    return(decrypted)
  end


end
end

  # LICENSE INFORMATION
  #
  # This software product contains the IDEA algorithm as described and claimed in
  # US patent 5,214,703, EPO patent 0482154 (covering Austria, France, Germany,
  # Italy, the Netherlands, Spain, Sweden, Switzerland, and the UK), and Japanese
  # patent application 508119/1991, "Device for the conversion of a digital block
  # and use of same" (hereinafter referred to as "the algorithm").  Any use of
  # the algorithm for commercial purposes is thus subject to a license from Ascom
  # Systec Ltd. of CH-5506 Maegenwil (Switzerland), being the patentee and sole
  # owner of all rights, including the trademark IDEA.
  # 
  # Commercial purposes shall mean any revenue generating purpose including but
  # not limited to:
  # 
  # i) Using the algorithm for company internal purposes (subject to a site
  #    license).
  # 
  # ii) Incorporating the algorithm into any software and distributing such
  #     software and/or providing services relating thereto to others (subject to
  #     a product license).
  # 
  # iii) Using a product containing the algorithm not covered by an IDEA license
  #      (subject to an end user license).
  # 
  # All such end user license agreements are available exclusively from Ascom
  # Systec Ltd and may be requested via the WWW at http://www.ascom.ch/systec or
  # by email to idea@ascom.ch.
  # 
  # Use other than for commercial purposes is strictly limited to non-revenue
  # generating data transfer between private individuals.  The use by government
  # agencies, non-profit organizations, etc is considered as use for commercial
  # purposes but may be subject to special conditions.  Any misuse will be
  # prosecuted.

