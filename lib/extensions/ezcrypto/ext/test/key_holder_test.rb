$:.unshift(File.dirname(__FILE__) + "/../lib/")
require File.join(File.dirname(__FILE__), 'test_helper')
require 'test/unit'
require 'active_crypto'
class KeyHolderSubject
  include ActiveCrypto::KeyHolder
end

class KeyHolderTest < Test::Unit::TestCase
  
  attr :key_holder
  
  def setup
    @key_holder=KeyHolderSubject.new
  end
  
  def test_session_key_should_not_be_set_on_new
    assert_nil key_holder.session_key
  end
  
  def test_set_session_key_should_set_session_key
    key=EzCrypto::Key.generate
    key_holder.set_session_key key
    assert_not_nil key_holder.session_key
    assert_equal key.raw,key_holder.session_key.raw
  end
  
  def test_set_encoded_key_should_set_session_key
    key=EzCrypto::Key.generate
    key_holder.set_encoded_key key.encode
    assert_not_nil key_holder.session_key
    assert_equal key.raw,key_holder.session_key.raw
  end
  
  def test_enter_password_should_set_session_key
    key=EzCrypto::Key.with_password "test","onetwothree"
    key_holder.enter_password "test"
    assert_not_nil key_holder.session_key
    assert_equal key.raw,key_holder.session_key.raw
  end
  
    
end

