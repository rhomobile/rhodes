class XrubyTest
  def initialize
  end

  def compilebug3979550_test
    #[:action => :back] - Does not compile
    [{:action => :back}]
  end
  
end
