$:.unshift(File.dirname(__FILE__) + "/../lib/")
require File.join(File.dirname(__FILE__), 'test_helper')
require 'test/unit'
require 'active_crypto'


class User < ActiveRecord::Base
	has_many :secrets
	has_many :groups
	keyholder
end

class Secret < ActiveRecord::Base
	encrypt :name,:email, :key=>:user, :base64=>true
	belongs_to :user
	has_many :children
end

class Child < ActiveRecord::Base
	encrypt :email, :key=>:secret, :base64=>true
	belongs_to :secret
end

class Asset<ActiveRecord::Base
  encrypt :title, :base64=>true
  has_many :caps,:dependent=>:destroy
  
  def self.create(title,email)
    asset=Asset.new
    asset.set_session_key(EzCrypto::Key.generate)
    asset.title=title
    if asset.save
      asset.share(email)
    else
      nil
    end
  end

  def share(email=nil)
    Cap.create_for_asset(self,email)
  end
  
end

class AssetRaw<ActiveRecord::Base
  set_table_name "assets"
end

class Cap < ActiveRecord::Base
  belongs_to :asset
  encrypt :shared_key, :base64=>true
  
  def self.find_by_key(cap_key)
    cap_key.chop
    hash=Digest::SHA1.hexdigest(cap_key)
    if (cap_key.length>=20) # Sanity check
      cap=self.find_by_key_hash(hash)
      if cap
        cap.set_encoded_key(cap_key)
        cap.asset.set_encoded_key(cap.shared_key)
        cap
      end
    else
      nil
    end
  end
  
  def self.create_for_asset(asset,email=nil)
    cap=Cap.new
    cap.email=email if email
    cap.asset=asset
    if cap.save
      cap.set_session_key(EzCrypto::Key.generate)
      cap_key=cap.session_key.encode
      cap.key_hash=Digest::SHA1.hexdigest(cap_key)
      cap.shared_key=asset.session_key.encode
      cap.save
      cap_key
    else
      nil
    end
  end
  
end

class Group < ActiveRecord::Base
	belongs_to :user
	has_many :group_secrets
	
	encrypt :name,:group_key, :key=>:user	, :base64=>true
end

class GroupSecret < ActiveRecord::Base
	belongs_to :group
	
	encrypt :title,:body, :key=>:group, :base64=>true
	
end

class ActiveCryptoTest < Test::Unit::TestCase
 
  def setup
  end
    
  def test_key_holder_in_record
    user=User.new
    user.name="bob"
    user.save
  	assert user.kind_of?(ActiveCrypto::KeyHolder)
  	assert user.kind_of?(ActiveRecord::Base)
  	assert user.kind_of?(User)
    assert_nil user.session_key
  	user.enter_password "shhcccc"
  	assert_not_nil user.session_key
  	assert_not_nil user.session_key.encrypt("test")    
  end
  
  def test_encrypted_child
    user=User.new
    user.save
    assert_nil user.session_key
  	user.enter_password "shhcccc"
  	assert_not_nil user.session_key
  	assert user.kind_of?(ActiveCrypto::KeyHolder)
  	assert user.kind_of?(ActiveRecord::Base)
  	assert user.kind_of?(User)
  	
  	jill=user.secrets.create
  	
  	assert_not_nil jill
  	assert jill.kind_of?(ActiveCrypto::AssociationKeyHolder)
  	assert jill.kind_of?(ActiveCrypto::KeyHolder)
  	assert jill.kind_of?(ActiveCrypto::Encrypted)
  	assert jill.kind_of?(ActiveRecord::Base)
  	assert jill.kind_of?(Secret)
  	
  	assert jill.respond_to?(:session_key)
  	
    assert_not_nil jill.user
    assert_not_nil jill.user.session_key
  	
  	
    assert_not_nil jill.session_key
    assert_equal user.session_key,jill.session_key

    jill.name="jill"
    jill.save


    assert_equal "jill",jill.name
    
    jill=user.secrets.first
    assert_not_nil jill.session_key
    assert_equal user.session_key,jill.session_key
    assert_equal "jill",jill.name
    
    child=jill.children.create
    child.email="pelle@neubia.com"
    child.save

    assert_not_nil child.secret
    assert_not_nil child.secret.session_key


    assert_not_nil child.session_key
    assert_equal user.session_key,child.session_key

    assert_equal "pelle@neubia.com",child.email

    child=jill.children.first

    assert_not_nil child.secret
    assert_not_nil child.secret.session_key


    assert_not_nil child.session_key
    assert_equal user.session_key,child.session_key

    assert_equal "pelle@neubia.com",child.email

  end

  def test_caps
    key=Asset.create("title","pelle@neubia.com")
    assert_not_nil key
    cap=Cap.find_by_key key
    assert_not_nil cap
    assert_not_nil cap.asset
    
    assert_equal "title",cap.asset.title
    assert_equal "title",cap.asset["title"]
    assert_equal "pelle@neubia.com",cap.email
    assert_equal "pelle@neubia.com",cap["email"]
    
    # Non decrypting version
    raw=AssetRaw.find cap.asset.id
    assert_not_equal "title",raw.title
    assert_not_equal "title",raw["title"]
    
    bob_key=cap.asset.share("bob@bob.com")
    bob_cap=Cap.find_by_key bob_key
    
    assert_not_equal key,bob_key
    assert_not_nil bob_cap
    assert_not_nil bob_cap.asset
    assert_equal "title",bob_cap.asset.title
    assert_equal "bob@bob.com",bob_cap.email
  end
end


