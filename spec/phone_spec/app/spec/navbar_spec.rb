describe "NavBar" do
  it "should create" do
    NavBar.started.should ==  false 
    
    begin
      NavBar.create
    rescue
      # Nothing
    end
    sleep 2
    NavBar.started.should ==  false 

    NavBar.create :title => "", :left => {:action => :back}
    sleep 2
    NavBar.started.should ==  true

    NavBar.remove
    sleep 2
    NavBar.started.should ==  false

  end

end
