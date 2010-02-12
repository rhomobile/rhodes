describe "WebView" do
  it "should evaulate simple javascript" do
    WebView.execute_js("1+1").should == "2"
  end
end
