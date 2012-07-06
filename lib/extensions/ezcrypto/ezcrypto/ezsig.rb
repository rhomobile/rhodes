require 'ezcrypto'
require 'net/http'
=begin rdoc

These modules provides a simple ruby like way to create and verify digital signatures.

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
module EzCrypto

=begin rdoc
  The signer is used for signing stuff. It encapsulates the functionality of a private key.
=end
  class Signer
=begin rdoc
  Initialize a Signer with a OpenSSL Private Key. You generally should not call new directly. 
  Unless you are interfacing with your own underlying OpenSSL code.
=end
    def initialize(priv,options = {})
      @priv=priv
    end
  
=begin rdoc
  Generate a new keypair. Defaults to 2048 bit RSA.
=end
    def self.generate(strength=2048,type=:rsa)
      key_class=case type
      when :dsa
        OpenSSL::PKey::DSA
      else
        OpenSSL::PKey::RSA
      end
      EzCrypto::Signer.new(key_class.generate(strength))
    end
    
=begin rdoc
  Decode a PEM encoded Private Key  and return a signer. Takes an optional password
=end  
    def self.decode(encoded,password=nil)
      begin
        EzCrypto::Signer.new(OpenSSL::PKey::RSA.new( encoded,password))
      rescue
        EzCrypto::Signer.new(OpenSSL::PKey::DSA.new( encoded,password))
      end
    end
  
=begin rdoc
  Decode a PEM encoded Private Key file and return a signer. Takes an optional password
=end  
    def self.from_file(filename,password=nil)
      file = File.read( filename )
      decode(file,password)
    end

=begin rdoc
  Returns the OpenSSL Public Key object. You normally do not need to use this.
=end
    def public_key
      @priv.public_key
    end

=begin rdoc
  Returns the corresponding Verifier object.
=end    
    def verifier
      Verifier.new(public_key)
    end
    
=begin rdoc
  Returns the OpenSSL Private Key object. You normally do not need to use this.
=end  
    def private_key
      @priv
    end

=begin rdoc
  signs data using the private key and the corresponding digest function. SHA1 for RSA and DSS1 for DSA.
  99% of signing use these parameters. 
  Email a request or send me a patch if you have other requirements.
=end  
    def sign(data)
      if rsa?
        @priv.sign(OpenSSL::Digest::SHA1.new,data)
      elsif dsa?
        @priv.sign(OpenSSL::Digest::DSS1.new,data)
      end
    end
    
=begin rdoc
  Returns true if it is a RSA private key
=end
    def rsa?
      @priv.is_a? OpenSSL::PKey::RSA
    end
    
=begin rdoc
  Returns true if it is a DSA private key
=end    
    def dsa?
      @priv.is_a? OpenSSL::PKey::DSA
    end

  end

=begin rdoc
  The Verifier is used for verifying signatures. If you use the decode or 
  from_file methods you can use either raw PEM encoded public keys or certificate.
=end
  class Verifier
=begin rdoc
  Initializes a Verifier using a OpenSSL public key object.
=end
    def initialize(pub)
      @pub=pub
    end

=begin rdoc
  Decodes a PEM encoded Certificate or Public Key and returns a Verifier object.
=end  
    def self.decode(encoded)
      case encoded
      when /-----BEGIN CERTIFICATE-----/
        EzCrypto::Certificate.new(OpenSSL::X509::Certificate.new( encoded))
      else
        begin
          EzCrypto::Verifier.new(OpenSSL::PKey::RSA.new( encoded))
        rescue
          EzCrypto::Verifier.new(OpenSSL::PKey::DSA.new( encoded))
        end
      end
    end
  
=begin rdoc
  Decodes a PEM encoded Certificate or Public Key from a file and returns a Verifier object.
=end  
    def self.from_file(filename)
      file = File.read( filename )
      decode(file)
    end

=begin rdoc
  Load a certificate or public key from PKYP based on it's hex digest
=end  
    def self.from_pkyp(digest)
      digest=digest.strip.downcase
      if digest=~/[0123456789abcdef]{40}/
#        Net::HTTP.start("localhost", 9000) do |query|
        Net::HTTP.start("pkyp.org", 80) do |query|
          response=query.get "/#{digest}.pem"
          if response.code=="200"
            decode(response.body)
          else
            raise "Error occured (#{response.code}): #{response.body}"      
          end
        end
      else
        raise "Invalid digest"
      end
    end

=begin rdoc
  Decodes all certificates or public keys in a file and returns an array.
=end
    def self.load_all_from_file(filename)
      file = File.read( filename )
      certs=[]
      count=0
      file.split( %q{-----BEGIN}).each do |pem|
        if pem and pem!=""
            pem="-----BEGIN#{pem}\n"
              cert=decode(pem)
              if cert.is_a? EzCrypto::Verifier
                certs<<cert
              end
        end
      end
      certs
    end

=begin rdoc
  Is the Verifier a Certificate or not.
=end  
    def cert?
      false
    end

=begin rdoc
  Returns the OpenSSL public key object. You would normally not need to use this.
=end        
    def public_key
      @pub
    end

=begin rdoc
  Returns the SHA1 hexdigest of the DER encoded public key. This can be used as a unique key identifier.
=end
    def digest
      Digest::SHA1.hexdigest(@pub.to_der)
    end
=begin rdoc
  Is this a RSA key?
=end
    def rsa?
      @pub.is_a? OpenSSL::PKey::RSA
    end
=begin rdoc
  Is this a DSA key?
=end    
    def dsa?
      @pub.is_a? OpenSSL::PKey::DSA
    end


=begin rdoc
  Returns true if the public key signed the given data.
=end  
    def verify(sig,data)
      if rsa?
        @pub.verify( OpenSSL::Digest::SHA1.new, sig, data )
      elsif dsa?
        @pub.verify( OpenSSL::Digest::DSS1.new, sig, data )
      else
        false
      end
    end

=begin rdoc
  Register the public key or certificate at PKYP
=end
    def register_with_pkyp
      send_to_pkyp(@pub.to_s)
    end
    
    protected
    
    def send_to_pkyp(pem)
#      Net::HTTP.start("localhost", 9000) do |query|
      Net::HTTP.start("pkyp.org", 80) do |query|
        output=URI.escape(pem).gsub("+","%2b")
        response=query.post "/register","body="+output
        if response.code=="302"
          response["Location"]=~/([0123456789abcdef]{40}$)/
          $1
        else
          raise "Error occured (#{response.code}): #{response.body}"      
        end
      end
    end

  end

=begin rdoc
  Certificate provides functionality to make it easy to extract information from a Certificate. 
  This also provides all the same functionality as a Verifier.
=end  
  class Certificate < Verifier

=begin rdoc
  Intialize with a OpenSSL cert object.
=end
    def initialize(cert)
      super(cert.public_key)
      @cert=cert
    end

=begin rdoc
  Returns true
=end    
    def cert?
      true
    end

=begin rdoc
  Register the certificate at PKYP
=end
    def register_with_pkyp
      send_to_pkyp(@cert.to_s)
    end

=begin rdoc
  Returns the SHA1 hex digest of a the DER encoded certificate. This is useful as a unique identifier.
=end    
    def cert_digest
      Digest::SHA1.hexdigest(@cert.to_der)
    end

=begin rdoc
  Returns a Name object containt the subject of the certificate. The subject in X509 speak is the details of the certificate owner.
=end    
    def subject
      @subject=EzCrypto::Name.new(@cert.subject) unless @subject
      @subject
    end

=begin rdoc
  Returns a Name object containt the issuer of the certificate. 
=end        
    def issuer
      @issuer=EzCrypto::Name.new(@cert.issuer) unless @issuer
      @issuer
    end
    
=begin rdoc
  Returns the issuers serial number for this certificate
=end
    def serial
      @cert.serial
    end

=begin rdoc
  Returns the OpenSSL Certificate object
=end    
    def cert
      @cert
    end

=begin rdoc
  Returns the certificates valid not before date.
=end    
    def not_before
      @cert.not_before
    end
    
=begin rdoc
  Returns the certificates valid not after date.
=end
    def not_after
      @cert.not_after
    end

=begin rdoc
  Is this certificate valid at this point in time. Note this only checks if it is valid with respect to time.
  It is important to realize that it does not check with any CRL or OCSP services to see if the certificate was 
  revoked.
=end    
    def valid?(time=Time.now.utc)
      time>not_before && time<self.not_after
    end

=begin rdoc
  Returns the hash of extensions available in the certificate. These are not always present.
=end    
    def extensions
      unless @extensions
        @extensions={}
        cert.extensions.each {|e| @extensions[e.oid]=e.value} if cert.extensions
      end
      @extensions
    end

=begin rdoc
  Any methods defined in Name can be used here. This means you can do cert.email rather than cert.subject.email.
=end    
    def method_missing(method)
      subject.send method
    end
    
  end

=begin rdoc
  A handy ruby wrapper around OpenSSL's Name object. This was created to make it really easy to extract information out of the certificate.
=end
  class Name
=begin rdoc
  Initializes the Name object with the underlying OpenSSL Name object. You generally do not need to use this. 
  Rather use the Certificates subject or issuer methods. 
=end
    def initialize(name)
      @name=name
      @attributes={}
      name.to_s.split(/\//).each do |field| 
        key, val = field.split(/=/,2)
        if key
          @attributes[key.to_sym]=val
        end
      end  
    end

=begin rdoc
  Returns the full name object in classic horrible X500 format.
=end
    def to_s
      @name.to_s
    end

=begin rdoc
  Returns the email if present in the name
=end    
    def email
      self[:emailAddress]
    end
=begin rdoc
  The 2 letter country code of the name
=end
    def country
      self[:C]
    end
    alias_method :c,:country
=begin rdoc
  The state or province code
=end
    def state
      self[:ST]
    end
    alias_method :st,:state
    alias_method :province,:state

=begin rdoc
  The locality
=end    
    def locality
      self[:L]
    end
    alias_method :l,:locality

=begin rdoc
  The Organizational Unit
=end    
    def organizational_unit
      self[:OU]
    end
    alias_method :ou,:organizational_unit
    alias_method :organisational_unit,:organizational_unit

=begin rdoc
  The Organization
=end    
    def organization
      self[:O]
    end
    alias_method :o,:organization
    alias_method :organisation,:organization

=begin rdoc
  The common name. For SSL this means the domain name. For personal certificates it is the name.
=end    
    def common_name
      self[:CN]
    end
    alias_method :name,:common_name
    alias_method :cn,:common_name

=begin rdoc
  Lookup fields in the certificate.
=end    
    def [](attr_key)
      @attributes[attr_key.to_sym]
    end
    
    def method_missing(method)
      self[method]
    end
    
  end

=begin rdoc
  Wraps around the OpenSSL trust store. This allows you to decide which certificates you trust.
  
  You can either point it at a path which contains a OpenSSL trust store (see OpenSSL for more) or build it up manually.
  
  For a certificate to verify you need the issuer and the issuers issuers certs added to the Trust store.
  
  NOTE: Currently this does not support CRL's or OCSP. We may add support for this later.
=end  
  class TrustStore
    
=begin rdoc
  Create a trust store of normally trusted root certificates as found in a browser. Extracted from Safari.
=end
    def self.default_trusted
      load_from_file(File.dirname(__FILE__) + "/trusted.pem")
    end
=begin rdoc
  Create a trust store from a list of certificates in a pem file.
  These certificates should just be listed one after each other.
=end    
    def self.load_from_file(file)
      store=TrustStore.new
      EzCrypto::Verifier.load_all_from_file(file).each do |cert|
        store.add cert
      end
      store
    end
=begin rdoc
  Create trust store with an optional list of paths of openssl trust stores.
=end
    def initialize(*paths)
      @store=OpenSSL::X509::Store.new
#      @store.set_default_path paths.shift if paths.length>0
      paths.each {|path| @store.add_path path}
    end

=begin rdoc
  Add either a EzCrypto::Certificate or a OpenSSL::X509::Cert object to the TrustStore. This should be a trusted certificate such as a CA's issuer certificate.
=end
    def add(obj)
      if obj.kind_of?(EzCrypto::Certificate)
        @store.add_cert obj.cert
      elsif obj.kind_of?(OpenSSL::X509::Certificate)
        @store.add_cert obj
      else 
        raise "unsupported object type"
      end
    end
=begin rdoc
  Returns true if either the EzCrypto::Certificate or OpenSSL::X509::Cert object is verified using issuer certificates in the trust store.
=end
    def verify(cert)
      if cert.kind_of?(EzCrypto::Certificate)
        @store.verify cert.cert
      elsif cert.kind_of?(OpenSSL::X509::Certificate)
        @store.verify cert
      else 
        false
      end
    end
  end
end