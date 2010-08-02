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

  #https://www.pivotaltracker.com/story/show/4528270
  it "unary priority" do
    s = "123"
    #res = -s.to_i - it does not work
    res = -(s.to_i)
    res.should == -123
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
	
	res.should == true
	
    cond = 'true'
    res = false
	if cond == 'true'
		res = true
	elsif
	    res = false
	end
	
	res.should == true
	
  end
=end 
end
