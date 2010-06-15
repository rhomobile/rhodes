require 'rho/rhocontroller'
require 'Spec/spec_runner'

def Test_equal(p1,p2)
    raise "Expected '#{p1}' equal to '#{p2}'" if p1 != p2
end

def Test_not_equal(p1,p2)
    raise "Expected '#{p1}' not equal to '#{p2}'" if p1 == p2
end

class SpecController < Rho::RhoController

  #GET /Spec
  def index
    @exc_count = 0
    @count = 0
    $is_network_available = System.get_property('has_network')
    
    run_all_tests()
    
    @code = @exc_count > 0 ? 1 : 0
    
    puts "***Total:  " + @count.to_s
    puts "***Passed: " + (@count - @exc_count).to_s
    puts "***Failed: " + @exc_count.to_s
  end
  
  def run_spec(name)
    begin
        puts "TEST: #{name}"
    
        require 'Spec/' + name+'_spec'
        testClass = Object.const_get(name+'Test')
        testObj = testClass.new
        
        testClass.instance_methods(false).each do |meth|
            begin
                next unless meth.to_s().end_with?('_test')
                @count += 1
                puts "  - #{meth}"
                testObj.send meth
            rescue Exception => e
                @exc_count += 1
                puts "  FAILED: '#{name}:#{meth}' failed: Error: #{e}"
                puts "  #{e.backtrace[1]}" if e.backtrace && e.backtrace.length > 0
                #e.backtrace.each do |item|
                #    puts item
                #end
            end
            
        end
        
        testObj.clear if testObj.respond_to?( :clear )
    rescue Exception => e
        @exc_count += 1
        puts "Test '#{name}' failed: Error: #{e}"
        e.backtrace.each do |item|
            puts item
        end
    end
  end
  
end