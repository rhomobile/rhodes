describe "NativeBar" do

if !Rho::System.isRhoSimulator && System.get_property('platform') != 'WINDOWS' && System.get_property('platform') != 'WINDOWS_DESKTOP'
  it "should create" do
    NativeBar.create Rho::RhoApplication::NOBAR_TYPE, []
    sleep 20
    NativeBar.started.should ==  false

    NativeBar.create Rho::RhoApplication::TOOLBAR_TYPE, [{:action => :back}]
    sleep 20
    NativeBar.started.should ==  true

    NativeBar.remove
    sleep 20
    NativeBar.started.should ==  false

  end
else

  it "should create" do
    Rho::NativeToolbar.remove
    sleep 20
    Rho::NativeToolbar.isCreated.should ==  false

    Rho::NativeToolbar.create [{:action => :back}]
    sleep 20
    Rho::NativeToolbar.isCreated.should ==  true

    Rho::NativeToolbar.remove
    sleep 20
    Rho::NativeToolbar.isCreated.should == false

  end

end

end
