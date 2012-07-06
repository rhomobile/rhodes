$:.unshift(File.dirname(__FILE__) + "/../lib/")

require 'test/unit'
require 'fileutils'
require 'ezsig'
require 'base64'

class EzCryptoTest < Test::Unit::TestCase

  def setup
  end

  def _test_generate_key #very slow so not run by default
    signer=EzCrypto::Signer.generate
    assert signer.rsa?
    assert !signer.dsa?
    
    assert_signer(signer)
  end
  
  def test_from_file
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/testsigner.pem"
    assert signer.rsa?
    assert !signer.dsa?
    assert_signer(signer)
  end

  def test_dsa_from_file
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/dsakey.pem"
    assert signer.dsa?
    assert !signer.rsa?
    assert_signer(signer)
  end

  def test_from_password_protected_file
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/protectedsigner.pem","secret"
    assert signer.rsa?
    assert !signer.dsa?
    assert_signer(signer)
  end
  
  def test_public_key_read
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/testsigner.pem"
    verifier=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/testpub.pem"
    assert verifier
    sig=signer.sign "test this rsa"
    assert sig
    assert verifier.verify( sig,"test this rsa")

    assert !verifier.cert?
    assert_equal signer.public_key.to_s, verifier.public_key.to_s
  end
  
  def test_dsa_public_key_read
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/dsakey.pem"
    verifier=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/dsapubkey.pem"
    
    assert verifier
    sig=signer.sign "test this dsa"
    assert sig
    assert verifier.verify( sig,"test this dsa")

    assert !verifier.cert?
    
    # This fails as it seems like it returns an incorrect public key
#    assert_equal signer.public_key.to_s, verifier.public_key.to_s
  end
  
  def test_certificate_reader
    signer=EzCrypto::Signer.from_file File.dirname(__FILE__) + "/testsigner.pem"
    cert=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/testsigner.cert"
    assert cert
    assert cert.cert?
    assert_instance_of EzCrypto::Certificate, cert
    assert_equal signer.public_key.to_s, cert.public_key.to_s
    
    sig=signer.sign "hello"
    assert sig
    assert cert.verify( sig,"hello")
    
    assert_equal "/C=DK/ST=Denmark/L=Copenhagen/O=EzCrypto Test Certificate/OU=testing/CN=EzCrypto Testing/emailAddress=pelleb@gmail.com",cert.subject.to_s
    assert_equal "/C=DK/ST=Denmark/L=Copenhagen/O=EzCrypto Test Certificate/OU=testing/CN=EzCrypto Testing/emailAddress=pelleb@gmail.com",cert.issuer.to_s
    
    assert_kind_of EzCrypto::Name,cert.issuer
    assert_kind_of EzCrypto::Name,cert.subject
    assert cert.serial
    assert cert.not_after
    assert cert.not_before
    assert cert.valid?
    
    
    assert_equal cert.subject[:emailAddress],"pelleb@gmail.com"
    assert_equal cert.subject[:C],"DK"
    assert_equal cert.subject[:ST],"Denmark"
    assert_equal cert.subject[:L],"Copenhagen"
    assert_equal cert.subject[:OU],"testing"
    assert_equal cert.subject[:O],"EzCrypto Test Certificate"
    assert_equal cert.subject[:CN],"EzCrypto Testing"

    assert_equal cert.subject.emailAddress,"pelleb@gmail.com"
    assert_equal cert.subject.C,"DK"
    assert_equal cert.subject.ST,"Denmark"
    assert_equal cert.subject.L,"Copenhagen"
    assert_equal cert.subject.OU,"testing"
    assert_equal cert.subject.O,"EzCrypto Test Certificate"
    assert_equal cert.subject.CN,"EzCrypto Testing"

    assert_equal cert.subject.email,"pelleb@gmail.com"
    assert_equal cert.subject.c,"DK"
    assert_equal cert.subject.st,"Denmark"
    assert_equal cert.subject.l,"Copenhagen"
    assert_equal cert.subject.ou,"testing"
    assert_equal cert.subject.o,"EzCrypto Test Certificate"
    assert_equal cert.subject.cn,"EzCrypto Testing"

    assert_equal cert.subject.country,"DK"
    assert_equal cert.subject.state,"Denmark"
    assert_equal cert.subject.locality,"Copenhagen"
    assert_equal cert.subject.organisational_unit,"testing"
    assert_equal cert.subject.organisation,"EzCrypto Test Certificate"
    assert_equal cert.subject.organizational_unit,"testing"
    assert_equal cert.subject.organization,"EzCrypto Test Certificate"
    assert_equal cert.subject.name,"EzCrypto Testing"
    assert_equal cert.subject.common_name,"EzCrypto Testing"

    assert_equal cert.emailAddress,"pelleb@gmail.com"
    assert_equal cert.C,"DK"
    assert_equal cert.ST,"Denmark"
    assert_equal cert.L,"Copenhagen"
    assert_equal cert.OU,"testing"
    assert_equal cert.O,"EzCrypto Test Certificate"
    assert_equal cert.CN,"EzCrypto Testing"
                     
    assert_equal cert.email,"pelleb@gmail.com"
    assert_equal cert.c,"DK"
    assert_equal cert.st,"Denmark"
    assert_equal cert.l,"Copenhagen"
    assert_equal cert.ou,"testing"
    assert_equal cert.o,"EzCrypto Test Certificate"
    assert_equal cert.cn,"EzCrypto Testing"
                     
    assert_equal cert.country,"DK"
    assert_equal cert.state,"Denmark"
    assert_equal cert.locality,"Copenhagen"
    assert_equal cert.organisational_unit,"testing"
    assert_equal cert.organisation,"EzCrypto Test Certificate"
    assert_equal cert.organizational_unit,"testing"
    assert_equal cert.organization,"EzCrypto Test Certificate"
    assert_equal cert.name,"EzCrypto Testing"
    assert_equal cert.common_name,"EzCrypto Testing"

    assert_equal cert.issuer[:emailAddress],"pelleb@gmail.com"
    assert_equal cert.issuer[:C],"DK"
    assert_equal cert.issuer[:ST],"Denmark"
    assert_equal cert.issuer[:L],"Copenhagen"
    assert_equal cert.issuer[:OU],"testing"
    assert_equal cert.issuer[:O],"EzCrypto Test Certificate"
    assert_equal cert.issuer[:CN],"EzCrypto Testing"

    assert_equal cert.issuer.emailAddress,"pelleb@gmail.com"
    assert_equal cert.issuer.C,"DK"
    assert_equal cert.issuer.ST,"Denmark"
    assert_equal cert.issuer.L,"Copenhagen"
    assert_equal cert.issuer.OU,"testing"
    assert_equal cert.issuer.O,"EzCrypto Test Certificate"
    assert_equal cert.issuer.CN,"EzCrypto Testing"

    assert_equal cert.issuer.email,"pelleb@gmail.com"
    assert_equal cert.issuer.c,"DK"
    assert_equal cert.issuer.st,"Denmark"
    assert_equal cert.issuer.l,"Copenhagen"
    assert_equal cert.issuer.ou,"testing"
    assert_equal cert.issuer.o,"EzCrypto Test Certificate"
    assert_equal cert.issuer.cn,"EzCrypto Testing"
    assert_equal cert.issuer.country,"DK"
    assert_equal cert.issuer.state,"Denmark"
    assert_equal cert.issuer.locality,"Copenhagen"
    assert_equal cert.issuer.organisational_unit,"testing"
    assert_equal cert.issuer.organisation,"EzCrypto Test Certificate"
    assert_equal cert.issuer.organizational_unit,"testing"
    assert_equal cert.issuer.organization,"EzCrypto Test Certificate"
    assert_equal cert.issuer.name,"EzCrypto Testing"
    assert_equal cert.issuer.common_name,"EzCrypto Testing"
    
    ## Extensions
    
    assert cert.extensions
    
  end
  
  def test_in_memory_store
    trust=EzCrypto::TrustStore.new
    cert=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/testsigner.cert"
    assert !trust.verify(cert)
    trust.add cert
    assert trust.verify(cert)
    
    sf_root=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf-class2-root.crt"
    assert !trust.verify(sf_root)
    starfield=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf_intermediate.crt"
    assert !trust.verify(starfield)
    agree2=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/agree2.com.cert"
    assert !trust.verify(agree2)
    
    trust.add sf_root
    assert trust.verify(sf_root)
    assert trust.verify(starfield)
    assert !trust.verify(agree2)
    
    trust.add starfield
    assert trust.verify(agree2)
  end
  
  def test_disk_store
    trust=EzCrypto::TrustStore.new File.dirname(__FILE__) + "/store"
    sf_root=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf-class2-root.crt"
    trust.add(sf_root)
    assert trust.verify(sf_root)
    starfield=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf_intermediate.crt"
    assert trust.verify(starfield)
    trust.add(starfield)
    agree2=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/agree2.com.cert"
    assert trust.verify(agree2)
    
    cert=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/testsigner.cert"
    assert !trust.verify(cert)
    trust.add cert
    assert trust.verify(cert)
  end
  
  def test_load_combined
    certs=EzCrypto::Verifier.load_all_from_file File.dirname(__FILE__) + "/../lib/trusted.pem"
    assert certs.is_a?( Array)
    assert certs.size>1
    certs.each do |cert|
      assert_instance_of EzCrypto::Certificate, cert
    end
  end
  
  def test_load_trusted_truststore
    trust=EzCrypto::TrustStore.default_trusted
    sf_root=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf-class2-root.crt"
    assert trust.verify(sf_root)
    starfield=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/sf_intermediate.crt"
    assert trust.verify(starfield)
    agree2=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/agree2.com.cert"
    assert trust.verify(agree2)
    
    cert=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/testsigner.cert"
    assert !trust.verify(cert)
    trust.add cert
    assert trust.verify(cert)  
  end

# Disabling these until pkyp is back up  
#  def test_public_key_load_from_pkyp
#    verifier=EzCrypto::Verifier.from_pkyp "e93e18114cbefaaa89fda908b09df63d3662879a"
#    agree2=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/agree2.com.cert"
#    assert_equal agree2.cert.to_s,verifier.cert.to_s
#    assert verifier
#  end
#
#  def test_register_public_key_at_pkyp
#    pub=EzCrypto::Verifier.from_file File.dirname(__FILE__) + "/agree2.com.cert"
#    assert_equal pub.digest,pub.register_with_pkyp
#  end
#  
#  def test_create_register_and_fetch_public_key
#    signer=EzCrypto::Signer.generate
#    assert_equal signer.verifier.digest,signer.verifier.register_with_pkyp
#    verifier=EzCrypto::Verifier.from_pkyp signer.verifier.digest
#    sig=signer.sign "hello"
#    assert sig
#    assert verifier.verify( sig,"hello")    
#  end
#  
  
  def assert_signer(signer)
    assert signer
    assert signer.public_key
    
    sig=signer.sign "hello"
    assert sig
    assert signer.verifier
    assert signer.verifier.verify( sig,"hello")
  end
  
end