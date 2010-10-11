require 'rho/rhoevent'

describe "Events" do

  before(:all) do
    events = Rho::RhoEvent.find(:all)
    events.each do |event|
      Rho::RhoEvent.destroy(event['id'])
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
    event['reminder'] = 10 if System::get_property('platform') == 'Blackberry' || System::get_property('platform') == 'WINDOWS'
    event['privacy'] = 'private'
    start_date = Time.now+600
    start_date -= start_date.sec #usec.to_f/1000000
    end_date = Time.now+3600
    end_date -= end_date.sec #usec.to_f/1000000
    event['start_date'] = start_date
    event['end_date'] = end_date

    puts "event: #{event}"    
    Rho::RhoEvent.create!(event)

    newevents = Rho::RhoEvent.find(:all)
    #puts "newevents: #{newevents.inspect.to_s}"
    newevents.should_not be_nil

    newevents.size.should == 1 
    c = newevents[0]
    puts "c: #{c}"
    
    @id = c['id']
        
    c['title'].should == title
    c['location'].should == 'loc1'
    c['notes'].should == 'notes1'
    c['reminder'].should == 10 if System::get_property('platform') == 'Blackberry' || System::get_property('platform') == 'WINDOWS'
    c['privacy'].should == 'private' unless System::get_property('platform') == 'APPLE'
    c['start_date'].to_s.should == start_date.to_s
    c['end_date'].to_s.should == end_date.to_s
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
    start_date -= start_date.sec#usec.to_f/1000000
    end_date = Time.now+1800
    end_date -= end_date.sec #usec.to_f/1000000
    
    Rho::RhoEvent.update_attributes( 'id' => @id, 'title' => "RANDOM", 'location' => 'loc2', 'notes' => 'notes2', 
        'reminder' => 15, 'privacy' => 'confidential', 'start_date' => start_date, 'end_date' => end_date )

    event = Rho::RhoEvent.find(@id)
    #puts "event: #{event.inspect.to_s}"
    event.should_not be_nil

    event['title'].should ==  'RANDOM' 
    event['location'].should == 'loc2'
    event['notes'].should == 'notes2'
    event['reminder'].should == 15 if System::get_property('platform') == 'Blackberry' || System::get_property('platform') == 'WINDOWS'
    event['privacy'].should == 'confidential' unless System::get_property('platform') == 'APPLE'
    event['start_date'].to_s.should == start_date.to_s
    event['end_date'].to_s.should == end_date.to_s
    #@revision.should_not == event['revision']
  end

  it "should update recurrence" do
    # https://www.pivotaltracker.com/story/show/5484747
    # https://www.pivotaltracker.com/story/show/5484751
    if System::get_property('platform') == 'Blackberry' ||System.get_property('platform') == 'WINDOWS'
      recValues = {'frequency'=>'daily', "interval"=>2 }
      Rho::RhoEvent.update_attributes( 'id' => @id, 'recurrence' => recValues )
      event = Rho::RhoEvent.find(@id)
      #puts "event: #{event.inspect.to_s}"
      event.should_not be_nil
      event['recurrence'].should == recValues

      if System::get_property('platform') == 'Blackberry'
        recValues = {"frequency"=>"yearly", "interval"=>1, "end_date"=>Time.now + 60000, "days"=>[0, 0, 1, 0, 0, 0, 0], "months"=>[0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0], "weeks"=>[0, 0, 1, 0, 0]}
      else
        recValues = {"frequency"=>"yearly", "day_of_month"=>10, "months"=>[0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0], "count"=>10 }
      end  
      
      Rho::RhoEvent.update_attributes( 'id' => @id, 'recurrence' => recValues )
      event = Rho::RhoEvent.find(@id)
      #puts "event: #{event.inspect.to_s}"
      event.should_not be_nil
      event['recurrence']['end_date'].to_s.should == recValues['end_date'].to_s
      event['recurrence']['end_date'] = ''
      recValues['end_date'] = ''
      #event['recurrence']['days'] = recValues['days']
      event['recurrence'].should == recValues

      if System::get_property('platform') == 'Blackberry'
        recValues = {"frequency"=>"weekly", "interval"=>4, "end_date"=>Time.now + 60000, "days"=>[1, 1, 1, 1, 0, 0, 0]}
      else
        recValues = {"frequency"=>"weekly", "interval"=>4, "days"=>[1, 1, 1, 1, 0, 0, 0], "count"=>10}
      end  
      Rho::RhoEvent.update_attributes( 'id' => @id, 'recurrence' => recValues )
      event = Rho::RhoEvent.find(@id)
      #puts "event: #{event.inspect.to_s}"
      event.should_not be_nil
      event['recurrence']['end_date'].to_s.should == recValues['end_date'].to_s
      event['recurrence']['end_date'] = ''
      recValues['end_date'] = ''
      if System::get_property('platform') == 'Blackberry'   #Bug in BB
        event['recurrence']['days'] = []
        recValues['days'] = []
      end
      
      event['recurrence'].should == recValues

#{"frequency"=>"weekly", "interval"=>1, "days"=>[0, 0, 0, 0, 0, 0, 1]}}]
      recValues = {"frequency"=>"weekly", "interval"=>5, "days"=>[0, 1, 1, 0, 0, 0, 1], "count"=>10}
      Rho::RhoEvent.update_attributes( 'id' => @id, 'recurrence' => recValues )
      event = Rho::RhoEvent.find(@id)
      #puts "event: #{event.inspect.to_s}"
      event.should_not be_nil
      if System::get_property('platform') == 'Blackberry'   #Bug in BB
        event['recurrence']['days'] = []
        recValues['days'] = []
      end
      
      event['recurrence'].should == recValues

      if System::get_property('platform') == 'Blackberry'
          recValues =  {"frequency"=>"monthly", "interval"=>9, "days"=>[0, 0, 1, 0, 0, 0, 0], "weeks"=>[0, 0, 1, 0, 0]}
      else
          recValues =  {"frequency"=>"monthly", "interval"=>1, "day_of_month"=>10}
      end
      
      Rho::RhoEvent.update_attributes( 'id' => @id, 'recurrence' => recValues )
      event = Rho::RhoEvent.find(@id)
      #puts "event: #{event.inspect.to_s}"
      event.should_not be_nil
      event['recurrence'].should == recValues
    end

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
