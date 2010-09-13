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

describe "Contacts" do

  it "should create" do
    first_name = 'Random'
    last_name = 'Newbie'
    mobile_number = '+1222333444'

    contacts = Rho::RhoContact.find(:all)
    #puts "contacts: #{contacts.inspect.to_s}"
    contacts.should_not be_nil

    contact = {}
    contact['first_name'] = first_name
    contact['last_name'] = last_name
    contact['mobile_number'] = mobile_number
    Rho::RhoContact.create!(contact)

    newcontacts = Rho::RhoContact.find(:all)
    #puts "newcontacts: #{newcontacts.inspect.to_s}"
    newcontacts.should_not be_nil

    diff = newcontacts.diff(contacts)
    diff.size.should == 1 
    diff.keys.size.should ==  1 
    c = diff[diff.keys.first]
    c['first_name'].should ==  first_name 
    c['last_name'].should ==  last_name 
    c['mobile_number'].should == mobile_number 

    @id = c['id']
    #puts "id: #{@id}"
  end

  it "should update" do
    #puts "id: #{@id}"
    Rho::RhoContact.update_attributes 'id' => @id, 'first_name' => "RANDOM", 'last_name' => "NEWBIE"

    contact = Rho::RhoContact.find(@id)
    #puts "contacts: #{contact.inspect.to_s}"
    contact.should_not be_nil

    unless System.get_property('platform') == 'Blackberry'
      #https://www.pivotaltracker.com/story/show/3983643
      contact['first_name'].should ==  'RANDOM' 
      contact['last_name'].should ==  'NEWBIE' 
    end
    
  end

  it "should remove" do
    contacts = Rho::RhoContact.find(:all)
    puts "contacts: #{contacts.inspect.to_s}"
    contacts.should_not be_nil
    contacts.size.should >= 1 

    size = contacts.size

    Rho::RhoContact.destroy(@id)

    contacts = Rho::RhoContact.find(:all)
    puts "new contacts: #{contacts.inspect.to_s}"
    contacts.should_not be_nil

    (size - contacts.size).should == 1 
  end

end
