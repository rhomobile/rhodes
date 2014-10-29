require 'mspec/runner/formatters/dotted'
require 'mspec/runner/formatters/junit'

# Rhodes test specific formatters

# Test results formatter that output spec statistics in Jasmine style
# Used for command-line spec runners

class JasmineLikeFormatter < DottedFormatter
  def after(state)
    # do not put anything outside
  end

  def finish
    Rho::Log.info("***Total: #{@tally.counter.examples}","APP")
    Rho::Log.info("***Expectations: #{@tally.counter.expectations}","APP")
    failed = @tally.counter.failures + @tally.counter.errors
    passed = @tally.counter.examples - failed
    Rho::Log.info("***Passed: #{passed}","APP")
    Rho::Log.info("***Failed: #{failed}","APP")
  end
end

# Junit formatter with cache, stores all the output inside StringIO
# After test is completed put results in log with small pieces
# and save xml to file

class JUnitRhoLogFormatter < JUnitFormatter
  def initialize(file_name=nil)
    super
    @fname = file_name
    @finish = StringIO.new()
  end

  def finish
    super

    @finish.rewind()

    Rho::Log.info(@fname.nil? ? 'spec' : @fname,"JUNITNAME")
    max_buffer_size = 2 * 1024
    buffer = []
    buffer_size = 0
    @finish.each_line do |line|
      pure_line = line.rstrip
      
      if (buffer_size + pure_line.length > max_buffer_size) && !buffer.empty?
        Rho::Log.info(buffer.join("~~"),'JUNITBLOB')
        buffer = []
        buffer_size = 0        
      end

      buffer << pure_line
      buffer_size += pure_line.length + 2
    end

    Rho::Log.info(buffer.join("~~"),'JUNITBLOB') unless buffer.empty?

    @finish.rewind()

    if !@fname.nil?
      File.open(@fname, "w") { |io| io.write(@finish.string) }
    end
  end
end
