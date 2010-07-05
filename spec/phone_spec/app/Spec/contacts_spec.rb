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

class ContactsSpec
  def initialize
  end

  def create_test
    first_name = 'Random'
    last_name = 'Newbie'
    mobile_number = '+1222333444'

    contacts = Rho::RhoContact.find(:all)
    Test_not_equal( contacts, nil )

    contact = {}
    contact['first_name'] = first_name
    contact['last_name'] = last_name
    contact['mobile_number'] = mobile_number
    Rho::RhoContact.create!(contact)

    newcontacts = Rho::RhoContact.find(:all)
    Test_not_equal( newcontacts, nil )

    diff = newcontacts.diff(contacts)
    Test_equal( diff.size, 1 )
    Test_equal( diff.keys.size, 1 )
    c = diff[diff.keys.first]
    Test_equal( c['first_name'], first_name )
    Test_equal( c['last_name'], last_name )
    Test_equal( c['mobile_number'], mobile_number )

    @id = c['id']
  end

  def update_test
    Rho::RhoContact.update_attributes 'id' => @id, 'first_name' => "RANDOM", 'last_name' => "NEWBIE"

    contact = Rho::RhoContact.find(@id)
    Test_not_equal( contact, nil )

    unless System.get_property('platform') == 'Blackberry'
    #https://www.pivotaltracker.com/story/show/3983643
        Test_equal( contact['first_name'], 'RANDOM' )
        Test_equal( contact['last_name'], 'NEWBIE' )
    end
    
  end

  def remove_test
    contacts = Rho::RhoContact.find(:all)
    Test_not_equal( contacts, nil )
    Test_equal( contacts.size >= 1, true )

    size = contacts.size

    Rho::RhoContact.destroy(@id)

    contacts = Rho::RhoContact.find(:all)
    Test_not_equal( contacts, nil )

    Test_equal( size - contacts.size, 1 )
  end

end
