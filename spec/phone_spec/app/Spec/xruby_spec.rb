describe "Xruby" do

  #https://www.pivotaltracker.com/story/show/3979550  
  it "should compilebug3979550" do
    #[:action => :back] - Does not compile
    [{:action => :back}]
  end
  
  #https://www.pivotaltracker.com/story/show/4365686
  def test_method(p)
    p == 1 || p == 2
  end
  
  it "if with function parameters" do
    test = false
    #if test_method 1 and test_method 2
    if test_method( 1 ) and test_method 2
        test = true
    end
    
    test.should == true
  end
  
=begin
  def elsif_test
    cond = 'false'
    res = false
	if cond == 'true'
		res = false
	elsif
	    res = true
	end
	
	Test_equal(res,true)
	
    cond = 'true'
    res = false
	if cond == 'true'
		res = true
	elsif
	    res = false
	end
	
	Test_equal(res,true)
	
  end
=end 
end
