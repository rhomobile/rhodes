require 'association_key_holder_test'

class EncUser < ActiveRecord::Base
  set_table_name "users"
  encrypt :name, :base64=>true
end

class RawUser < ActiveRecord::Base
  set_table_name "users"
end

class EncryptTest < AssociationKeyHolderTest
  def setup
    @key_holder=EncUser.create 
  end
  
  def test_should_be_base64
    assert EncUser.ezcrypto_base64?
  end
  
  def test_should_retain_session_key_on_reload
    key=EzCrypto::Key.generate
    key_holder.set_session_key key
    assert_not_nil key_holder.session_key
    assert_equal key.raw,key_holder.session_key.raw
    
    key_holder.name="bob"
    key_holder.save
    
    assert_equal "bob",key_holder.name
    
    found=EncUser.find key_holder.id
    assert_equal key_holder.name,found.name
    
    raw=RawUser.find key_holder.id
    assert_not_nil raw.name
    assert_not_equal key_holder.name,raw.name
    
  end
  
end
