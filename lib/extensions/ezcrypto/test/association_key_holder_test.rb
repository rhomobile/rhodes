require 'key_holder_test'

class User < ActiveRecord::Base
  keyholder
end

class AssociationKeyHolderTest < KeyHolderTest
  def setup
    @key_holder=User.create
  end
  
  def test_should_retain_session_key_on_reload
    key=EzCrypto::Key.generate
    key_holder.set_session_key key
    assert_not_nil key_holder.session_key
    assert_equal key.raw,key_holder.session_key.raw
    
    reloaded=User.find key_holder.id
    assert_not_nil reloaded.session_key
    assert_equal key.raw,reloaded.session_key.raw    
  end
  
  def test_should_retain_session_key_on_reload_for_record_with_key_set_before_create
    key=EzCrypto::Key.generate
    key_holder=User.new
    assert_nil key_holder.session_key
    
    key_holder.set_session_key key
    assert_not_nil key_holder.session_key
    
    assert_equal key.raw,key_holder.session_key.raw
    key_holder.save
    
    reloaded=User.find key_holder.id
    assert_not_nil reloaded.session_key
    assert_equal key.raw,reloaded.session_key.raw    
  end
end
