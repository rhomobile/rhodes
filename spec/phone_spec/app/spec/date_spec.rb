require 'date'
require 'time'

describe "DateTime" do

  it "should parse datetime" do
    str = "2010-07-08T12:14:20Z"
    
    date = Time.parse(str)

    date.year.should == 2010
    date.hour.should == 12
    
    str2 = date.strftime("%m/%d/%Y")
    str2.should == "07/08/2010"
  end
end
