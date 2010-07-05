require 'spec_runner'

class RhoSpecModule
    attr_accessor :before, :after, :tests
    
    def initialize(msg, spec_body)
        @before = {}
        @after = {}
        @tests = {}
        
        @env = Object.new
        
        @spec_message = msg
        @spec_body = spec_body
    end
    
    def run_spec
        puts "TEST: #{@spec_message}"

        @env.instance_eval(&@spec_body) if @spec_body
    end

    def run_test(test_body)
        @env.instance_eval(&test_body) if test_body
    end
end

class RhoSpec
    attr_reader :exc_count, :count, :errorMessages, :code
    @@spec_modules = []
    @@spec_index = 0
    
    def self.current
        @@spec_modules[@@spec_index]
    end
    
    def start
        @exc_count = 0
        @count = 0
        @errorMessages = ""
        $is_network_available = System.get_property('has_network')
        
        run_all_specs()
        
        @code = @exc_count > 0 ? 1 : 0
        
        puts "***Total:  " + @count.to_s
        puts "***Passed: " + (@count - @exc_count).to_s
        puts "***Failed: " + @exc_count.to_s
    end

    def self.describe(mod, msg=nil, options=nil, &block)
        @@spec_modules << RhoSpecModule.new(mod, block)
    end
    
    def run_test(spec_name,test_name, &block)
        begin
            @count += 1
            puts "  - #{test_name}"
            yield if block_given?
        rescue Exception => e
            @exc_count += 1
            ntrace_index = RhoSpec.current.tests.size > 0 ? 2 : 1
            @errorMessages += "<br/>FAIL: '#{spec_name}:#{test_name}' failed: Error: #{e}\n" + 
                "#{e.backtrace[ntrace_index]}" if e.backtrace && e.backtrace.length > 0
            puts "FAIL: '#{spec_name}:#{test_name}' failed: Error: #{e}\n" + 
                "#{e.backtrace[ntrace_index]}" if e.backtrace && e.backtrace.length > 0
            #e.backtrace.each do |item|
            #    puts item
            #end
        end
    end
            
    def run_spec(spec_name)
        begin
            @@spec_modules = []
            @@spec_index = 0
            require 'Spec/' + spec_name.downcase()+'_spec'
            
            if @@spec_modules.size > 0
                
                @@spec_modules.each do |spec_module|
                    spec_module.run_spec
                    spec_module.run_test(spec_module.before[:all])
                    
                    spec_module.tests.each do |test_name, body|
                        spec_module.run_test(spec_module.before[:each])
                    
                        run_test(spec_name, test_name){spec_module.run_test(body)}
                        
                        spec_module.run_test(spec_module.after[:each])
                        
                    end

                    spec_module.run_test(spec_module.after[:all])
                    
                    @@spec_index += 1
                end
            else
                puts "TEST: #{spec_name}"
            
                testClass = Object.const_get(spec_name+'Spec')
                testObj = testClass.new
                
                testClass.instance_methods(false).each do |test_name|
                    next unless test_name.to_s().end_with?('_test')
                    run_test(spec_name, test_name){testObj.send test_name}
                end
            
                testObj.clear if testObj.respond_to?( :clear )
            end    
        rescue Exception => e
            @exc_count += 1
            puts "Test '#{spec_name}' failed: Error: #{e}"
            e.backtrace.each do |item|
                puts item
            end
        end
    end
    
end

class Object
  
    def before(at=:each, &block)
        RhoSpec.current.before[ at ] = block
    end

    def after(at=:each, &block)
        RhoSpec.current.after[ at ] = block
    end

    def describe(mod, msg=nil, options=nil, &block)
        RhoSpec.describe mod, msg, &block
    end

    def it(msg, &block)
        RhoSpec.current.tests[ msg ] = block
    end

    def it_should_behave_like(desc)
        raise "it_should_behave_like not implemented"
    end

    alias_method :context, :describe
    alias_method :specify, :it
  
end

class PositiveOperatorMatcher
  def initialize(actual)
    @actual = actual
  end

  def ==(expected)
    unless @actual == expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to equal #{expected.pretty_inspect}")
    end
  end

  def <(expected)
    unless @actual < expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to be less than #{expected.pretty_inspect}")
    end
  end

  def <=(expected)
    unless @actual <= expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to be less than or equal to #{expected.pretty_inspect}")
    end
  end

  def >(expected)
    unless @actual > expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to be greater than #{expected.pretty_inspect}")
    end
  end

  def >=(expected)
    unless @actual >= expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to be greater than or equal to #{expected.pretty_inspect}")
    end
  end

  def =~(expected)
    unless @actual =~ expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "to match #{expected.pretty_inspect}")
    end
  end
end

class Expectation
    def self.fail_with(msg, msg1)
        raise msg+ " " + msg1        
    end
end

module Kernel
  # returns a pretty printed object as a string.
  def pretty_inspect
    inspect
  end
end

class NegativeOperatorMatcher
  def initialize(actual)
    @actual = actual
  end

  def ==(expected)
    if @actual == expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to equal #{expected.pretty_inspect}")
    end
  end

  def <(expected)
    if @actual < expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to be less than #{expected.pretty_inspect}")
    end
  end

  def <=(expected)
    if @actual <= expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to be less than or equal to #{expected.pretty_inspect}")
    end
  end

  def >(expected)
    if @actual > expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to be greater than #{expected.pretty_inspect}")
    end
  end

  def >=(expected)
    if @actual >= expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to be greater than or equal to #{expected.pretty_inspect}")
    end
  end

  def =~(expected)
    if @actual =~ expected
      Expectation.fail_with("Expected #{@actual.pretty_inspect}",
                            "not to match #{expected.pretty_inspect}")
    end
  end
end

class Object
  def should(matcher=nil)

    if matcher  
        raise "Expected '#{self.inspect.to_s}' equal to '#{matcher.inspect.to_s}'" if self != matcher  
    else
        PositiveOperatorMatcher.new(self)
        #raise "Expected '#{matcher.inspect.to_s}' equal to nil" if self != nil
    end
    
  end

  def should_not(matcher=nil)
    if matcher  
        raise "Expected '#{self.inspect.to_s}' not equal to '#{matcher.inspect.to_s}'" if self == matcher  
    else
        #raise "Expected '#{self.inspect.to_s}' not equal to nil" if self == nil
        NegativeOperatorMatcher.new(self)
    end
  end
end

def Test_equal(p1,p2)
    raise "Expected '#{p1.inspect.to_s}' equal to '#{p2.inspect.to_s}'" if p1 != p2
end

def Test_not_equal(p1,p2)
    raise "Expected '#{p1.inspect.to_s}' not equal to '#{p2.inspect.to_s}'" if p1 == p2
end

