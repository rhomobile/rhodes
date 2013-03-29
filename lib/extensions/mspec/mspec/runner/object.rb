class Object
  def before(at=:each, &block)
    MSpec.current.before at, &block
  end

  def after(at=:each, &block)
    MSpec.current.after at, &block
  end

  def describe(mod, msg=nil, options=nil, &block)
    MSpec.describe mod, msg, &block
  end

  def it(msg, &block)
    MSpec.current.it msg, &block
  end

  def it_should_behave_like(desc)
    MSpec.current.it_should_behave_like desc
  end

  # For ReadRuby compatiability
  def doc(*a)
  end
    

    
  def call_block(&block)
      block.call
      return nil
  rescue Exception => e
      return e
  end
    
    
  NO_BLOCK_PASSED = "you must pass a block to the eventually method"
    
  def eventually options = {}, &block
    raise NO_BLOCK_PASSED if block.nil?
    timeout = options[:timeout] || 5
    delay = options[:delay] || 0.25
    last_error = nil
    begin_time = Time.now
      
    while (Time.now - begin_time) < timeout
        e = call_block(&block)
        if e then
            last_error = e
            sleep delay
        else
            return
        end        
    end

    raise last_error
  end

  alias_method :context, :describe
  alias_method :specify, :it
end
