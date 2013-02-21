describe "simpleonlystatic" do
 
  it "should calc" do

     res = Rho::Examples::SimpleOnlyStaticModule.calcSumm(2,3)
     
     res.should == 5
     res.class.to_s.should == "Fixnum"

     res = Rho::Examples::SimpleOnlyStaticModule.calcSumm(-2,-3)

     res.should == -5
     res.class.to_s.should == "Fixnum"

  end

  it "should joinString" do

     res = Rho::Examples::SimpleOnlyStaticModule.joinStrings ['111', '222', '333']
     
     res.should == '111222333'
     res.class.to_s.should == "String"

     res = Rho::Examples::SimpleOnlyStaticModule.joinStrings ['1', '2', '3']
     
     res.should == '123'
     res.class.to_s.should == "String"

     res = Rho::Examples::SimpleOnlyStaticModule.joinStrings ['1', '', '3']
     
     res.should == '13'
     res.class.to_s.should == "String"

  end



end
