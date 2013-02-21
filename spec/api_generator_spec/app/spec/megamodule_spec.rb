describe "megamodule" do
 
  it "should default" do

     defItem = Rho::Examples::Megamodule.default
     puts 'defItem.class = '+defItem.class.to_s
     
     defItem.should_not be_nil
     defItem.class.to_s.should == "Rho::Examples::Megamodule"
     puts 'defItem.getProperty("ID").class = '+defItem.getProperty("ID").class.to_s
     puts 'defItem.getProperty("ID").to_s = '+defItem.getProperty("ID").to_s
     defItem.getProperty("ID").should == 'MM1'

  end

  it "should enumerate" do

     items = Rho::Examples::Megamodule.enumerate
     items.should_not be_nil
     items.class.to_s.should == "Array"
     items.size.should == 3
     items[0].should_not be_nil
     items[0].class.to_s.should == "Rho::Examples::Megamodule"
     items[0].getProperty("ID").should == "MM1"
     items[1].getProperty("ID").should == "MM2"
     items[2].getProperty("ID").should == "MM3"

  end

  it "should static object accessors" do

     count = Rho::Examples::Megamodule.getObjectsCount

     count.should_not be_nil
     count.class.to_s.should == "Fixnum"
     count.should == 3

     secItem = Rho::Examples::Megamodule.getObjectByIndex 1
     secItem.should_not be_nil
     secItem.class.to_s.should == "Rho::Examples::Megamodule"
     secItem.getProperty("ID").should == "MM2"

  end

  it "should string property" do
     defItem = Rho::Examples::Megamodule.default

     defItem.setProperty("StringProperty", "EDF")

     defItem.getProperty("StringProperty").should == "EDF"

     #use accessor
     value = defItem.StringProperty
     value.should == "EDF"

  end

  it "should int property" do
     defItem = Rho::Examples::Megamodule.default

     defItem.IntegerProperty = 3
     defItem.IntegerProperty.should == 3

     Rho::Examples::Megamodule.IntegerProperty = 6
     Rho::Examples::Megamodule.IntegerProperty.should == 6
     defItem.IntegerProperty.should == 6

  end


  it "should default resetting"  do
     defItem = Rho::Examples::Megamodule.default
     secItem = Rho::Examples::Megamodule.getObjectByIndex 1

     defItem.getProperty("ID").should == 'MM1'

     Rho::Examples::Megamodule.default = secItem
     defItem = Rho::Examples::Megamodule.default

     defItem.getProperty("ID").should == 'MM2'

     Rho::Examples::Megamodule.setProperty("StringProperty", "ABC")
     secItem = Rho::Examples::Megamodule.getObjectByIndex 1
     value = secItem.getProperty("StringProperty")
     value.should == "ABC"

  end

  it "should types check"  do

      Rho::Examples::Megamodule.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == true
      defItem = Rho::Examples::Megamodule.default
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == true

      defItem.typesTest("ABE", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABCD", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("AB", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", false, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 5551230, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 55512, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 556123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.141, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.15, [1,2,3], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3,4], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,4], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2], {:p1 => "abc", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qw5"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe", :p3 => "rty"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abe", :p2 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p3 => "qwe"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwa"}).should == false
      defItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc"}).should == false
      defItem.typesTest("ABCD", false, 553123, 3.24, [1,5,3], {:p2 => "abce", :p55 => "qwef"}).should == false

  end

  it "should static method aliases" do

     Rho::Examples::Megamodule.get_objects_count.should == 3
     Rho::Examples::Megamodule.IntegerProperty = 33
     Rho::Examples::Megamodule.integer_property.should == 33

  end

  it "should instance method aliases" do
     defItem = Rho::Examples::Megamodule.default

     defItem.IntegerProperty = 33
     defItem.integer_property.should == 33


  end

  it "should constants" do
     Rho::Examples::Megamodule::CONST1_STRING.should == "value1"
     Rho::Examples::Megamodule::CONST2_INTEGER.should == 123
     Rho::Examples::Megamodule::CONST_STRING_1.should == "const1"
     Rho::Examples::Megamodule::CONST_STRING_2.should == "const2"
  end


end
