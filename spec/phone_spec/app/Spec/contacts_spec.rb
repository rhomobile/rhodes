require 'rho/rhocontact'

class ContactsTest
  def initialize
  end

  def create_test
    contacts = Rho::RhoContact.find(:all)

    contact = {}
    contact['first_name'] = 'Random'
    contact['last_name'] = 'Newbie'
    contact['mobile_number'] = '+1222333444'
    Rho::RhoContact.create!(contact)

    newcontacts = Rho::RhoContact.find(:all)

    puts "contacts: #{contacts.inspect.to_s}"
    puts "newcontacts: #{contacts.inspect.to_s}"
    #Test_equal( parsed.is_a?(Array), true )
  end
  
end    
