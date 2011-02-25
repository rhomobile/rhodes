describe "NativeBar" do

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

end
