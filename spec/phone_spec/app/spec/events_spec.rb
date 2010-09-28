require 'rho/rhoevent'

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

describe "Events" do

  it "should create" do
    title = 'Random'

    events = Rho::RhoEvent.find(:all)
    #puts "events: #{events.inspect.to_s}"
    events.should_not be_nil

    event = {}
    event['title'] = title
    Rho::RhoEvent.create!(event)

    newevents = Rho::RhoEvent.find(:all)
    #puts "newevents: #{newevents.inspect.to_s}"
    newevents.should_not be_nil

    diff = newevents.diff(events)
    diff.size.should == 1 
    diff.keys.size.should ==  1 
    c = diff[diff.keys.first]
    c['title'].should == title

    @id = c['id']
    #puts "id: #{@id}"
  end

  it "should update" do
    #puts "id: #{@id}"
    Rho::RhoEvent.update_attributes 'id' => @id, 'title' => "RANDOM"

    event = Rho::RhoEvent.find(@id)
    #puts "event: #{event.inspect.to_s}"
    event.should_not be_nil

    event['title'].should ==  'RANDOM' 
  end

  it "should remove" do
    events = Rho::RhoEvent.find(:all)
    puts "events: #{events.inspect.to_s}"
    events.should_not be_nil
    events.size.should >= 1 

    size = events.size

    Rho::RhoEvent.destroy(@id)

    events = Rho::RhoEvent.find(:all)
    puts "new events: #{events.inspect.to_s}"
    events.should_not be_nil

    (size - events.size).should == 1 
  end

end
