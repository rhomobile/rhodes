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
    
    #remove contact if it was created already
    Rho::RhoContact.select('first_name' => first_name, 'last_name' => last_name, 'mobile_number' => mobile_number ).each {|x| Rho::RhoContact.destroy(x[1]['id']) } 

    contacts = Rho::RhoContact.find(:all, :select => ['first_name', 'last_name', 'mobile_number'])
    #puts "contacts: #{contacts.inspect.to_s}"
    contacts.should_not be_nil

    contact = {}
    contact['first_name'] = first_name
    contact['last_name'] = last_name
    contact['mobile_number'] = mobile_number
    Rho::RhoContact.create!(contact)

    @newcontacts = Rho::RhoContact.find(:all, :select => ['first_name', 'last_name', 'mobile_number'])
    #puts "newcontacts: #{newcontacts.inspect.to_s}"
    @newcontacts.should_not be_nil

    diff = @newcontacts.diff(contacts)
    diff.size.should == 1 
    diff.keys.size.should ==  1 
    c = diff[diff.keys.first]

    puts c.inspect

    c['first_name'].should ==  first_name 
    c['last_name'].should ==  last_name 
    c['mobile_number'].should == mobile_number 

    @id = c['id']
    puts "Contact created, id: #{@id}."
  end

if System.get_property('platform') != 'Blackberry' || System.get_property('os_version')[0].to_i() < 5
  it "should create many" do
    @last_name_bulk = 'TestUserLastName'
    names = ['Abigail', 'Addison', 'Aiden', 'Alexander', 'Alexis', 'Allison', 'Alyssa', 'Amelia', 'Andrew', 'Anna', 
            'Anthony', 'Aubrey', 'Audrey', 'Ava', 'Avery', 'Benjamin', 'Brayden', 'Brooklyn', 'Caleb', 'Carter', 
            'Charlotte', 'Chloe', 'Christian', 'Christopher', 'Claire', 'Connor', 'Daniel', 'David', 'Dylan', 'Eli', 
            'Elijah', 'Elizabeth', 'Ella', 'Emily', 'Emma', 'Ethan', 'Evan', 'Evelyn', 'Gabriel', 'Gavin', 
            'Grace', 'Hailey', 'Hannah', 'Harper', 'Henry', 'Isaac', 'Isabella', 'Jack', 'Jackson', 'Jacob', 
            'James', 'Jayden', 'John', 'Jonathan', 'Joseph', 'Joshua', 'Kaylee', 'Landon', 'Layla', 'Leah', 
            'Liam', 'Lillian', 'Lily', 'Logan', 'Lucas', 'Lucy', 'Luke', 'Madison', 'Mason', 'Matthew', 
            'Max', 'Mia', 'Michael', 'Natalie', 'Nathan', 'Nevaeh', 'Noah', 'Olivia', 'Owen', 'Riley', 
            'Ryan', 'Samantha', 'Samuel', 'Sarah', 'Sofia', 'Sophie', 'Tyler', 'Victoria', 'William', 'Wyatt', 
            'Zoe', 'Zoey'] 
    mobile_number_int = 1222333400
    @n_contacts = 16
    
    @n_contacts.times do |i|
      contact_start = Time.now
      contact = {}
      contact['first_name'] = names.sample
      contact['last_name'] =  @last_name_bulk
      contact['email_address'] = 'usr' + i.to_s + '@somerandomname.com'
      tag = ['mobile_number', 'home_number', "business_number"].sample
      contact[tag] = "+" + (mobile_number_int + i).to_s
      Rho::RhoContact.create!(contact)
      puts 'Create contact time' + (Time.now - contact_start).to_s
    end
    
    records = Rho::RhoContact.select('last_name' => @last_name_bulk)
    records.size.should == @n_contacts
    
    @bulk_create = Rho::RhoContact.find(:all, :select => ['first_name', 'last_name', 'mobile_number'])
    #puts "bulk_create: #{@bulk_create.inspect.to_s}"
    
    diff = @bulk_create.diff(@newcontacts)
    diff.size.should == @n_contacts 
    diff.keys.size.should ==  @n_contacts 
  end
    
  it "should access many" do
    total = Rho::RhoContact.find(:count)
    per_page = 10
    usercount = 0
    pages = (total.to_f / per_page).ceil
    puts "Contacts total: #{total}, per page: #{per_page}, pages #{pages}"
       
    start = Time.now
    pages.times do |i|
      page_start = Time.now
      all_contacts = Rho::RhoContact.find(:all, :per_page => per_page, :offset => per_page * i, :select => ['id', 'display_name', 'first_name', 'last_name', 'mobile_number', 'home_number', 'business_number', 'email_address']).values
       
      all_contacts.each do |contact|
        if contact['last_name'] == @last_name_bulk
          puts contact.inspect.to_s
          
          nomobile = contact['mobile_number'].nil? && contact['home_number'].nil? && contact['business_number'].nil?
          nomobile.should == false
          
          contact['email_address'].should_not be_nil
          
          usercount = usercount + 1
        end
      end
      puts 'Page access time ' + (Time.now - page_start).to_s
    end
    puts 'All pages access time ' + (Time.now - start).to_s
    
    usercount.should == @n_contacts 
  end
  
  it "should remove many" do
    test_user_hash = Rho::RhoContact.select('last_name' => @last_name_bulk)
    test_user_hash.should_not be_nil
    
    test_user_hash.each do |x| 
      contact_start = Time.now 
      Rho::RhoContact.destroy(x[1]['id'])
      puts 'Remove contact time ' + (Time.now - contact_start).to_s
    end    
    
    remaining = Rho::RhoContact.select('last_name' => @last_name_bulk)
    remaining.size.should == 0 unless remaining.nil?

    removecontacts = Rho::RhoContact.find(:all, :select => ['first_name', 'last_name', 'mobile_number'])
    #puts "removecontacts: #{removecontacts.inspect.to_s}"
    
    diff = removecontacts.diff(@newcontacts)
    diff.size.should == 0 
    diff.keys.size.should ==  0 
  end
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

if System.get_property('platform') != 'Blackberry' || System.get_property('os_version')[0].to_i() < 5
  it "should remove" do
    contacts = Rho::RhoContact.find(:all)
    puts "contacts: #{contacts.inspect}"
    contacts.should_not be_nil
    contacts.size.should >= 1 

    size = contacts.size

    puts "Removing previosly created contact, id: #{@id}."
    Rho::RhoContact.destroy(@id)

    contacts = Rho::RhoContact.find(:all)
    puts "new contacts: #{contacts.inspect.to_s}"
    contacts.should_not be_nil

    (size - contacts.size).should == 1 
  end
end

end
