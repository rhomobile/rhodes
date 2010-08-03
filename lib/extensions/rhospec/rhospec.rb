class MSpecScript
  # Returns the config object. Maintained at the class
  # level to easily enable simple config files. See the
  # class method +set+.
  def self.config
    @config ||= {
    }
  end
  
  def config
    MSpecScript.config
  end
  
end

class RhoSpecModule
    attr_accessor :before, :after, :tests, :spec_name
    
    def initialize(spec_name, spec_body)
        @before = {}
        @after = {}
        @tests = {}
        
        @env = Object.new
        
        @spec_name = spec_name
        @spec_body = spec_body
    end
    
    def run_spec
        setting_str = $spec_settings.length()>0  ? ";#{$spec_settings}" : ""
        puts "TEST: #{@spec_name}" + setting_str

        @env.instance_eval(&@spec_body) if @spec_body
    end

    def run_test(test_body)
        @env.instance_eval(&test_body) if test_body
    end
end

class MSpec
    VERSION = "RhoSpec 1.0"
    
    attr_reader :exc_count, :count, :errorMessages, :code, :is_network_available
    @@spec_modules = []
    @@spec_index = 0
    @@spec_files = []
    @@instance = nil
    
    def self.exit_code
        @@instance.code   
    end

    def self.exc_count
        @@instance.exc_count   
    end

    def self.count
        @@instance.count   
    end

    def self.errorMessages
        @@instance.errorMessages   
    end
    
    def self.is_network_available
        @@instance.is_network_available
    end
    
    def self.current
        @@spec_modules[@@spec_index]
    end

    def self.register_files(spec_files)
        @@spec_files = spec_files
    end
    
    def self.process
        @@instance = MSpec.new        
        @@instance.start        
    end
    
    def start
        @exc_count = 0
        @count = 0
        @errorMessages = ""
        @is_network_available = System.get_property('has_network')
        $is_network_available = @is_network_available
        
        @@spec_files.each do |spec_file|
            if spec_file.is_a?(Array)
                run_spec(spec_file[0], spec_file[1])
            else
                run_spec(spec_file, [{}])
            end    
        end
        
        @code = @exc_count > 0 ? 1 : 0
        
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
            ntrace_index = 2 #MSpec.current && MSpec.current.tests.size() > 0 ? 2 : 1
            @errorMessages += "<br/>FAIL: '#{spec_name}:#{test_name}; #{$spec_settings}' failed: Error: #{e}\n" + 
                "#{e.backtrace[ntrace_index]}" if e.backtrace && e.backtrace.length > 0
            puts "FAIL: '#{spec_name}:#{test_name}; #{$spec_settings}' failed: Error: #{e}\n" + 
                "#{e.backtrace[ntrace_index]}" if e.backtrace && e.backtrace.length > 0
            e.backtrace.each do |item|
                puts item
            end
        end
    end

    def run_specs
        @@spec_modules.each do |spec_module|
            spec_module.run_spec
            spec_module.run_test(spec_module.before[:all])
            
            spec_module.tests.each do |test_name, body|
                spec_module.run_test(spec_module.before[:each])
            
                @@spec_name  = spec_module.spec_name
                run_test(@@spec_name, test_name){spec_module.run_test(body)}
                
                spec_module.run_test(spec_module.after[:each])
                
            end

            spec_module.run_test(spec_module.after[:all])
            
            @@spec_index += 1
        end
    end
            
    def run_spec(spec_file, spec_settings)
        @@spec_name = ""
        begin
            @@spec_modules = []
            require spec_file

            spec_settings.each do |settings|
                $spec_settings = settings
                @@spec_index = 0
                run_specs 
            end    
        rescue Exception => e
            @exc_count += 1
            puts "Test '#{@@spec_name}; #{$spec_settings}' FAIL: Error: #{e}"
            e.backtrace.each do |item|
                puts item
            end
        end
    end
    
end

class Object
  
    def before(at=:each, &block)
        MSpec.current.before[ at ] = block
    end

    def after(at=:each, &block)
        MSpec.current.after[ at ] = block
    end

    def describe(mod, msg=nil, options=nil, &block)
        MSpec.describe mod, msg, &block
    end

    def it(msg, &block)
        MSpec.current.tests[ msg ] = block
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

class BeNilMatcher
  def matches?(actual)
    @actual = actual
    @actual.nil?
  end

  def failure_message
    ["Expected #{@actual.inspect}", "to be nil"]
  end
  
  def negative_failure_message
    ["Expected #{@actual.inspect}", "not to be nil"]
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
      unless matcher.matches?(self)
        Expectation.fail_with(*matcher.failure_message)
      end
    else
        PositiveOperatorMatcher.new(self)
        #raise "Expected '#{matcher.inspect.to_s}' equal to nil" if self != nil
    end
    
  end

  def should_not(matcher=nil)
    if matcher
      if matcher.matches?(self)
        Expectation.fail_with(*matcher.negative_failure_message)
      end
    else
        #raise "Expected '#{self.inspect.to_s}' not equal to nil" if self == nil
        NegativeOperatorMatcher.new(self)
    end
  end
  
  def be_nil
    BeNilMatcher.new
  end
  
end
