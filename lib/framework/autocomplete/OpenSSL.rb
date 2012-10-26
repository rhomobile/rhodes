 
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
    def self.rand(rest)
    end    
    def self.pseudo_rand(rest)
    end    
    def self.rand_range(req)
    end    
    def self.pseudo_rand_range(req)
    end    
    def self.generate_prime(rest)
    end    
    def copy(req)
    end    
    def num_bytes
    end    
    def num_bits
    end    
    def +(req)
    end    
    def -(req)
    end    
    def *(req)
    end    
    def sqr
    end    
    def /(req)
    end    
    def %(req)
    end    
    def mod_add(req,req1)
    end    
    def mod_sub(req,req1)
    end    
    def mod_mul(req,req1)
    end    
    def mod_sqr(req)
    end    
    def **(req)
    end    
    def mod_exp(req,req1)
    end    
    def gcd(req)
    end    
    def cmp(req)
    end    
    def <=>(req)
    end    
    def ucmp(req)
    end    
    def eql?(req)
    end    
    def ==(req)
    end    
    def ===(req)
    end    
    def zero?
    end    
    def one?
    end    
    def odd?
    end    
    def prime?(rest)
    end    
    def set_bit!(req)
    end    
    def clear_bit!(req)
    end    
    def bit_set?(req)
    end    
    def mask_bits!(req)
    end    
    def <<(req)
    end    
    def >>(req)
    end    
    def lshift!(req)
    end    
    def rshift!(req)
    end    
    def to_s(rest)
    end    
    def to_i
    end    
    def to_int
    end    
    def to_bn
    end    
    def coerce(req)
    end    
    def mod_inverse(req)
    end    
    def prime_fasttest?(rest)
    end    
  end

  class Cipher
    def self.ciphers
    end    
    def reset
    end    
    def encrypt(rest)
    end    
    def decrypt(rest)
    end    
    def pkcs5_keyivgen(rest)
    end    
    def update(rest)
    end    
    def final
    end    
    def name
    end    
    def key=(req)
    end    
    def key_len=(req)
    end    
    def key_len
    end    
    def iv=(req)
    end    
    def iv_len
    end    
    def block_size
    end    
    def padding=(req)
    end    
    def random_key
    end    
    def random_iv
    end    
  end

  class ConfigError
  end

  class Digest
    def self.digest(name,data)
    end    
    def reset
    end    
    def update(req)
    end    
    def <<(req)
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
    def self.digest(req,req1,req2)
    end    
    def self.hexdigest(req,req1,req2)
    end    
    def reset
    end    
    def update(req)
    end    
    def <<(req)
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
    def self.create(rest)
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
    def self.read_smime(req)
    end    
    def self.write_smime(rest)
    end    
    def self.sign(rest)
    end    
    def self.encrypt(rest)
    end    
    def data
    end    
    def error_string
    end    
    def error_string=(req)
    end    
    def type=(req)
    end    
    def type
    end    
    def detached=(req)
    end    
    def detached
    end    
    def detached?
    end    
    def cipher=(req)
    end    
    def add_signer(req)
    end    
    def signers
    end    
    def add_recipient(req)
    end    
    def recipients
    end    
    def add_certificate(req)
    end    
    def certificates=(req)
    end    
    def certificates
    end    
    def add_crl(req)
    end    
    def crls=(req)
    end    
    def crls
    end    
    def add_data(req)
    end    
    def data=(req)
    end    
    def verify(rest)
    end    
    def decrypt(rest)
    end    
    def to_pem
    end    
    def to_s
    end    
    def to_der
    end    
  end

  module PKCS5
    def self.pbkdf2_hmac(req,req1,req2,req3,req4)
    end    
    def self.pbkdf2_hmac_sha1(req,req1,req2,req3)
    end    
  end

  module PKey
  end

  module SSL
    def self.verify_certificate_identity(cert,hostname)
    end    
  end

  module X509
  end

  module OCSP
  end

  class Engine
    def self.load(rest)
    end    
    def self.cleanup
    end    
    def self.engines
    end    
    def self.by_id(req)
    end    
    def id
    end    
    def name
    end    
    def finish
    end    
    def cipher(req)
    end    
    def digest(req)
    end    
    def load_private_key(rest)
    end    
    def load_public_key(rest)
    end    
    def set_default(req)
    end    
    def ctrl_cmd(rest)
    end    
    def cmds
    end    
    def inspect
    end    
  end

  module ASN1
    def self.traverse(req)
    end    
    def self.decode(req)
    end    
    def self.decode_all(req)
    end    
    def self.Boolean(rest)
    end    
    def self.Integer(rest)
    end    
    def self.Enumerated(rest)
    end    
    def self.BitString(rest)
    end    
    def self.OctetString(rest)
    end    
    def self.UTF8String(rest)
    end    
    def self.NumericString(rest)
    end    
    def self.PrintableString(rest)
    end    
    def self.T61String(rest)
    end    
    def self.VideotexString(rest)
    end    
    def self.IA5String(rest)
    end    
    def self.GraphicString(rest)
    end    
    def self.ISO64String(rest)
    end    
    def self.GeneralString(rest)
    end    
    def self.UniversalString(rest)
    end    
    def self.BMPString(rest)
    end    
    def self.Null(rest)
    end    
    def self.ObjectId(rest)
    end    
    def self.UTCTime(rest)
    end    
    def self.GeneralizedTime(rest)
    end    
    def self.Sequence(rest)
    end    
    def self.Set(rest)
    end    
  end

  module Buffering
    def sync
    end    
    def sync=(req)
    end    
    def read(size,buf)
    end    
    def readpartial(maxlen,buf)
    end    
    def read_nonblock(maxlen,buf)
    end    
    def gets(eol,limit)
    end    
    def each(eol)
    end    
    def each_line(eol)
    end    
    def readlines(eol)
    end    
    def readline(eol)
    end    
    def getc
    end    
    def each_byte
    end    
    def readchar
    end    
    def ungetc(c)
    end    
    def eof?
    end    
    def eof
    end    
    def write(s)
    end    
    def write_nonblock(s)
    end    
    def <<(s)
    end    
    def puts(args)
    end    
    def print(args)
    end    
    def printf(s,args)
    end    
    def flush
    end    
    def close
    end    
  end

  def self.debug
  end  
  def self.debug=(req)
  end  
  def self.errors
  end  
end

