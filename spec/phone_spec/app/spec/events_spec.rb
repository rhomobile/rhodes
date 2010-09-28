require 'rho/rhoevent'

describe "Events" do

  before(:all) do
    events = Rho::RhoEvent.find(:all)
    events.each do |key, val|
        Rho::RhoEvent.destroy(key)
    end    
  end

  it "should create" do
    title = 'Random'

    events = Rho::RhoEvent.find(:all)
    #puts "events: #{events.inspect.to_s}"
    events.should_not be_nil

    event = {}
    event['title'] = title
    event['location'] = 'loc1'
    event['notes'] = 'notes1'
    event['reminder'] = 60
    event['privacy'] = 'private'
    start_date = Time.now+600
    end_date = Time.now+3600
    event['start_date'] = start_date
    event['end_date'] = end_date

    puts "event: #{event}"    
    Rho::RhoEvent.create!(event)

    newevents = Rho::RhoEvent.find(:all)
    #puts "newevents: #{newevents.inspect.to_s}"
    newevents.should_not be_nil

    diff = newevents #.diff(events)
    diff.size.should == 1 
    diff.keys.size.should ==  1 
    c = diff[diff.keys.first]
    puts "c: #{c}"
    
    @id = c['id']
        
    c['title'].should == title
    c['location'].should == 'loc1'
    c['notes'].should == 'notes1'
    c['reminder'].should == 60
    c['privacy'].should == 'private'
    c['start_date'].should == start_date
    c['end_date'].should == end_date
    
    #@revision = c['revision']
    #c['revision'].should_not be_nil


    #puts "id: #{@id}"
  end

  it "should find by dates" do
    start = Time.now
    end_time = start + 3600

    events = Rho::RhoEvent.find(:all, :start_date => start, :end_date => end_time, :find_type => 'starting', 
        :include_repeating => true )
        
    events.should_not be_nil
    events.size.should == 1 
  end
    
  it "should update" do
    #puts "id: #{@id}"
    
    start_date = Time.now
    end_date = Time.now+1800
    
    Rho::RhoEvent.update_attributes( 'id' => @id, 'title' => "RANDOM", 'location' => 'loc2', 'notes' => 'notes2', 
        'reminder' => 100, 'privacy' => 'confidential', 'start_date' => start_date, 'end_date' => end_date )

    event = Rho::RhoEvent.find(@id)
    #puts "event: #{event.inspect.to_s}"
    event.should_not be_nil

    event['title'].should ==  'RANDOM' 
    event['location'].should == 'loc2'
    event['notes'].should == 'notes2'
    event['reminder'].should == 100
    event['privacy'].should == 'confidential'
    event['start_date'].should == start_date
    event['end_date'].should == end_date
    #@revision.should_not == event['revision']
  end

  it "should remove" do
    events = Rho::RhoEvent.find(:all)
    #puts "events: #{events.inspect.to_s}"
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
