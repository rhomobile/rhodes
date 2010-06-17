require 'rho/rhocontact'

class Hash
  def diff(rhs)
    diff = {}
    self.each do |k,v|
      next if rhs.has_key?(k) and rhs[k] == v
      diff[k] = v
    end
    rhs.each do |k,v|
      next if self.has_key?(k) and self[k] == v
      diff[k] = v
    end
    diff
  end
end

class ContactsTest
  def initialize
  end

  def create_test
    first_name = 'Random'
    last_name = 'Newbie'
    mobile_number = '+1222333444'

    contacts = Rho::RhoContact.find(:all)

    contact = {}
    contact['first_name'] = first_name
    contact['last_name'] = last_name
    contact['mobile_number'] = mobile_number
    Rho::RhoContact.create!(contact)

    newcontacts = Rho::RhoContact.find(:all)

    diff = newcontacts.diff(contacts)
    Test_equal( diff.size, 1 )
    Test_equal( diff.keys.size, 1 )
    c = diff[diff.keys.first]
    Test_equal( c['first_name'], first_name )
    Test_equal( c['last_name'], last_name )
    Test_equal( c['mobile_number'], mobile_number )
  end
  
end    
