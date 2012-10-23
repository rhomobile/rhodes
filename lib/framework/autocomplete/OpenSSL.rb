 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
module OpenSSL
  VERSION = "1.1.0"

  OPENSSL_VERSION = "OpenSSL 1.0.1c 10 May 2012"

  OPENSSL_VERSION_NUMBER = 268439615

  class OpenSSLError
  end

  class BNError
  end

  class BN
    def self.rand
    end    
    def self.pseudo_rand
    end    
    def self.rand_range
    end    
    def self.pseudo_rand_range
    end    
    def self.generate_prime
    end    
    def copy
    end    
    def num_bytes
    end    
    def num_bits
    end    
    def +
    end    
    def -
    end    
    def *
    end    
    def sqr
    end    
    def /
    end    
    def %
    end    
    def mod_add
    end    
    def mod_sub
    end    
    def mod_mul
    end    
    def mod_sqr
    end    
    def **
    end    
    def mod_exp
    end    
    def gcd
    end    
    def cmp
    end    
    def <=>
    end    
    def ucmp
    end    
    def eql?
    end    
    def ==
    end    
    def ===
    end    
    def zero?
    end    
    def one?
    end    
    def odd?
    end    
    def prime?
    end    
    def set_bit!
    end    
    def clear_bit!
    end    
    def bit_set?
    end    
    def mask_bits!
    end    
    def <<
    end    
    def >>
    end    
    def lshift!
    end    
    def rshift!
    end    
    def to_s
    end    
    def to_i
    end    
    def to_int
    end    
    def to_bn
    end    
    def coerce
    end    
    def mod_inverse
    end    
    def prime_fasttest?
    end    
  end

  class Cipher
    def self.ciphers
    end    
    def reset
    end    
    def encrypt
    end    
    def decrypt
    end    
    def pkcs5_keyivgen
    end    
    def update
    end    
    def final
    end    
    def name
    end    
    def key=
    end    
    def key_len=
    end    
    def key_len
    end    
    def iv=
    end    
    def iv_len
    end    
    def block_size
    end    
    def padding=
    end    
    def random_key
    end    
    def random_iv
    end    
  end

  class ConfigError
  end

  class Digest
    def self.digest
    end    
    def reset
    end    
    def update
    end    
    def <<
    end    
    def digest_length
    end    
    def block_length
    end    
    def name
    end    
  end

  class HMACError
  end

  class HMAC
    def self.digest
    end    
    def self.hexdigest
    end    
    def reset
    end    
    def update
    end    
    def <<
    end    
    def digest
    end    
    def hexdigest
    end    
    def inspect
    end    
    def to_s
    end    
  end

  module Netscape
  end

  class PKCS12
    def self.create
    end    
    def key
    end    
    def certificate
    end    
    def ca_certs
    end    
    def to_der
    end    
  end

  class PKCS7
    def self.read_smime
    end    
    def self.write_smime
    end    
    def self.sign
    end    
    def self.encrypt
    end    
    def data
    end    
    def error_string
    end    
    def error_string=
    end    
    def type=
    end    
    def type
    end    
    def detached=
    end    
    def detached
    end    
    def detached?
    end    
    def cipher=
    end    
    def add_signer
    end    
    def signers
    end    
    def add_recipient
    end    
    def recipients
    end    
    def add_certificate
    end    
    def certificates=
    end    
    def certificates
    end    
    def add_crl
    end    
    def crls=
    end    
    def crls
    end    
    def add_data
    end    
    def data=
    end    
    def verify
    end    
    def decrypt
    end    
    def to_pem
    end    
    def to_s
    end    
    def to_der
    end    
  end

  module PKCS5
    def self.pbkdf2_hmac
    end    
    def self.pbkdf2_hmac_sha1
    end    
  end

  module PKey
  end

  module SSL
    def self.verify_certificate_identity
    end    
  end

  module X509
  end

  module OCSP
  end

  class Engine
    def self.load
    end    
    def self.cleanup
    end    
    def self.engines
    end    
    def self.by_id
    end    
    def id
    end    
    def name
    end    
    def finish
    end    
    def cipher
    end    
    def digest
    end    
    def load_private_key
    end    
    def load_public_key
    end    
    def set_default
    end    
    def ctrl_cmd
    end    
    def cmds
    end    
    def inspect
    end    
  end

  module ASN1
    def self.traverse
    end    
    def self.decode
    end    
    def self.decode_all
    end    
    def self.Boolean
    end    
    def self.Integer
    end    
    def self.Enumerated
    end    
    def self.BitString
    end    
    def self.OctetString
    end    
    def self.UTF8String
    end    
    def self.NumericString
    end    
    def self.PrintableString
    end    
    def self.T61String
    end    
    def self.VideotexString
    end    
    def self.IA5String
    end    
    def self.GraphicString
    end    
    def self.ISO64String
    end    
    def self.GeneralString
    end    
    def self.UniversalString
    end    
    def self.BMPString
    end    
    def self.Null
    end    
    def self.ObjectId
    end    
    def self.UTCTime
    end    
    def self.GeneralizedTime
    end    
    def self.Sequence
    end    
    def self.Set
    end    
  end

  module Buffering
    def sync
    end    
    def sync=
    end    
    def read
    end    
    def readpartial
    end    
    def read_nonblock
    end    
    def gets
    end    
    def each
    end    
    def each_line
    end    
    def readlines
    end    
    def readline
    end    
    def getc
    end    
    def each_byte
    end    
    def readchar
    end    
    def ungetc
    end    
    def eof?
    end    
    def eof
    end    
    def write
    end    
    def write_nonblock
    end    
    def <<
    end    
    def puts
    end    
    def print
    end    
    def printf
    end    
    def flush
    end    
    def close
    end    
  end

  def self.debug
  end  
  def self.debug=
  end  
  def self.errors
  end  
end

