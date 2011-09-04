require File.expand_path('../../../spec_helper', __FILE__)

describe "Float#round" do
#  it "returns the nearest Integer" do
#    5.5.round.should == 6
#    0.4.round.should == 0
#    -2.8.round.should == -3
#    0.0.round.should == 0
#    0.49999999999999994.round.should == 0 # see http://jira.codehaus.org/browse/JRUBY-5048
#  end

  ruby_version_is "1.9" do
#    it "rounds self to an optionally given precision" do
#      5.5.round(0).should == 6
#      1.2345678.round(2).should == 1.23
#      123456.78.round(-2).should == 123500 # rounded up
#      -123456.78.round(-2).should == -123500
#      12.345678.round(3.999).should == 12.346
#      0.8346268.round(-2.0**30).should == 0
#      0.42.round(2.0**30).should == 0.42
#    end

    it "raises a TypeError when its argument can not be converted to an Integer" do
      lambda { 1.0.round("4") }.should raise_error(TypeError)
      lambda { 1.0.round(nil) }.should raise_error(TypeError)
    end

    ruby_bug "redmine:5227",  "1.9.2" do
      it "works for corner cases" do
        42.0.round(308).should == 42.0
        1.0e307.round(2).should == 1.0e307
      end
    end
  end

end
