class XrubySpec
  def initialize
  end

  #https://www.pivotaltracker.com/story/show/3979550  
  def compilebug3979550_test
    #[:action => :back] - Does not compile
    [{:action => :back}]
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
