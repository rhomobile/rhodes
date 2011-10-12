#--
#
# Author:: Nathaniel Talbott.
# Copyright::
#   * Copyright (c) 2000-2003 Nathaniel Talbott. All rights reserved.
#   * Copyright (c) 2008-2011 Kouhei Sutou <tt><kou@clear-code.com></tt>
# License:: Ruby license.

require 'test/unit/attribute'
require 'test/unit/fixture'
require 'test/unit/exceptionhandler'
require 'test/unit/assertions'
require 'test/unit/failure'
require 'test/unit/error'
require 'test/unit/pending'
require 'test/unit/omission'
require 'test/unit/notification'
require 'test/unit/priority'
require 'test/unit/data'
require 'test/unit/testsuite'
require 'test/unit/testsuitecreator'
require 'test/unit/assertionfailederror'
require 'test/unit/util/backtracefilter'
require 'test/unit/util/output'
require 'test/unit/util/method-owner-finder'

module Test
  module Unit

    # Ties everything together. If you subclass and add your own
    # test methods, it takes care of making them into tests and
    # wrapping those tests into a suite. It also does the
    # nitty-gritty of actually running an individual test and
    # collecting its results into a Test::Unit::TestResult object.
    #
    # You can run two hooks before/after a TestCase run.
    #
    # Example:
    #   class TestMyClass < Test::Unit::TestCase
    #     class << self
    #       def startup
    #         ...
    #       end
    #
    #       def shutdown
    #         ...
    #       end
    #     end
    #
    #     def setup
    #       ...
    #     end
    #
    #     def cleanup
    #       ...
    #     end
    #
    #     def teardown
    #       ...
    #     end
    #
    #     def test_my_method1
    #       ...
    #     end
    #
    #     def test_my_method2
    #       ...
    #     end
    #   end
    #
    # Here is a call order:
    # * startup
    # * setup
    # * test_my_method1
    # * cleanup
    # * teardown
    # * setup
    # * test_my_method2
    # * cleanup
    # * teardown
    # * shutdown
    class TestCase
      include Attribute
      include Fixture
      include ExceptionHandler
      include ErrorHandler
      include FailureHandler
      include TestCasePendingSupport
      include TestCaseOmissionSupport
      include TestCaseNotificationSupport
      include Priority
      include Data
      include Assertions
      include Util::BacktraceFilter
      include Util::Output

      STARTED = name + "::STARTED" # :nodoc:
      FINISHED = name + "::FINISHED" # :nodoc:
      STARTED_OBJECT = name + "::STARTED::OBJECT" # :nodoc:
      FINISHED_OBJECT = name + "::FINISHED::OBJECT" # :nodoc:

      DESCENDANTS = [] # :nodoc:
      AVAILABLE_ORDERS = [:alphabetic, :random, :defined] # :nodoc:

      class << self
        def inherited(sub_class) # :nodoc:
          DESCENDANTS << sub_class
        end

        @@added_methods = {}
        def method_added(name) # :nodoc:
          super
          _added_methods = added_methods
          stringified_name = name.to_s
          if _added_methods.include?(stringified_name)
            attribute(:redefined, {:backtrace => caller}, {}, stringified_name)
          end
          _added_methods << stringified_name
        end

        def added_methods # :nodoc:
          @@added_methods[self] ||= []
        end

        # Rolls up all of the test* methods in the fixture into
        # one suite, creating a new instance of the fixture for
        # each method.
        def suite
          suite_creator = TestSuiteCreator.new(self)
          suite_creator.create
        end

        # Called before every test case runs. Can be used
        # to set up fixture information used in test case
        # scope.
        #
        # Here is an example test case:
        #   class TestMyClass < Test::Unit::TestCase
        #     class << self
        #       def startup
        #         ...
        #       end
        #     end
        #
        #     def setup
        #       ...
        #     end
        #
        #     def test_my_class1
        #       ...
        #     end
        #
        #     def test_my_class2
        #       ...
        #     end
        #   end
        #
        # Here is a call order:
        # * startup
        # * setup
        # * test_my_class1 (or test_my_class2)
        # * setup
        # * test_my_class2 (or test_my_class1)
        #
        # Note that you should not assume test order. Tests
        # should be worked in any order.
        def startup
        end

        # Called after every test case runs. Can be used to tear
        # down fixture information used in test case scope.
        #
        # Here is an example test case:
        #   class TestMyClass < Test::Unit::TestCase
        #     class << self
        #       def shutdown
        #         ...
        #       end
        #     end
        #
        #     def teardown
        #       ...
        #     end
        #
        #     def test_my_class1
        #       ...
        #     end
        #
        #     def test_my_class2
        #       ...
        #     end
        #   end
        #
        # Here is a call order:
        # * test_my_class1 (or test_my_class2)
        # * teardown
        # * test_my_class2 (or test_my_class1)
        # * teardown
        # * shutdown
        #
        # Note that you should not assume test order. Tests
        # should be worked in any order.
        def shutdown
        end

        @@test_orders = {}

        # Returns the current test order. This returns
        # +:alphabetic+ by default.
        def test_order
          @@test_orders[self] || AVAILABLE_ORDERS.first
        end

        # Sets the current test order.
        #
        # Here are the available _order_:
        # [:alphabetic]
        #   Default. Tests are sorted in alphabetic order.
        # [:random]
        #   Tests are sorted in random order.
        # [:defined]
        #   Tests are sorted in defined order.
        def test_order=(order)
          @@test_orders[self] = order
        end

        # Defines a test in declarative syntax or marks
        # following method as a test method.
        #
        # In declarative syntax usage, the following two
        # test definitions are the almost same:
        #
        #   description "register user"
        #   def test_register_user
        #     ...
        #   end
        #
        #   test "register user" do
        #     ...
        #   end
        #
        # In test method mark usage, the "my_test_method" is
        # treated as a test method:
        #
        #   test
        #   def my_test_method
        #     assert_equal("call me", ...)
        #   end
        def test(*test_description_or_targets, &block)
          if block_given?
            test_description = test_description_or_targets.first
            if test_description.nil?
              raise ArgumentError, "test description is missing"
            end
            n_arguments = test_description_or_targets.size
            if n_arguments > 1
              message = "wrong number of arguments (#{n_arguments} for 1)"
              raise ArgumentError, message
            end
            method_name = "test: #{test_description}"
            define_method(method_name, &block)
            description(test_description, method_name)
            attribute(:test, true, {}, method_name)
          else
            targets = test_description_or_targets
            attribute(:test, true, {}, *targets)
          end
        end

        # Describes a test.
        #
        # The following example associates "register a
        # normal user" description with "test_register"
        # test.
        #
        #   description "register a normal user"
        #   def test_register
        #     ...
        #   end
        def description(value, target=nil)
          attribute(:description, value, {}, target || [])
        end
      end

      attr_reader :method_name

      # Creates a new instance of the fixture for running the
      # test represented by test_method_name.
      def initialize(test_method_name)
        @method_name = test_method_name
        @internal_data = InternalData.new
      end

      # Assigns test data to the test. It is used in internal.
      def assign_test_data(label, data) # :nodoc:
        @internal_data.assign_test_data(label, data)
      end

      # Returns the test is valid test. It is used in internal.
      def valid? # :nodoc:
        return false unless respond_to?(@method_name)
        test_method = method(@method_name)
        if @internal_data.have_test_data?
          return false unless test_method.arity == 1
        else
          return false unless test_method.arity <= 0
        end
        owner = Util::MethodOwnerFinder.find(self, @method_name)
        if owner.class != Module and self.class != owner
          return false
        end
        true
      end

      # Runs the individual test method represented by this
      # instance of the fixture, collecting statistics, failures
      # and errors in result.
      def run(result)
        begin
          @_result = result
          @internal_data.test_started
          yield(STARTED, name)
          yield(STARTED_OBJECT, self)
          begin
            run_setup
            run_test
            run_cleanup
            add_pass
          rescue Exception
            @internal_data.interrupted
            raise unless handle_exception($!)
          ensure
            begin
              run_teardown
            rescue Exception
              raise unless handle_exception($!)
            end
          end
          @internal_data.test_finished
          result.add_run
          yield(FINISHED, name)
          yield(FINISHED_OBJECT, self)
        ensure
          # @_result = nil # For test-spec's after_all :<
        end
      end

      # Called before every test method runs. Can be used
      # to set up fixture information.
      #
      # You can add additional setup tasks by the following
      # code:
      #   class TestMyClass < Test::Unit::TestCase
      #     def setup
      #       ...
      #     end
      #
      #     setup
      #     def my_setup1
      #       ...
      #     end
      #
      #     setup
      #     def my_setup2
      #       ...
      #     end
      #
      #     def test_my_class
      #       ...
      #     end
      #   end
      #
      # Here is a call order:
      # * setup
      # * my_setup1
      # * my_setup2
      # * test_my_class
      def setup
      end

      # Called after every test method runs but the test
      # method isn't marked as 'passed'. Can be used to
      # clean up and/or verify tested condition.
      # e.g. Can be used to verify mock.
      #
      # You can add additional cleanup tasks by the following
      # code:
      #   class TestMyClass < Test::Unit::TestCase
      #     def cleanup
      #       ...
      #     end
      #
      #     cleanup
      #     def my_cleanup1
      #       ...
      #     end
      #
      #     cleanup
      #     def my_cleanup2
      #       ...
      #     end
      #
      #     def test_my_class
      #       ...
      #     end
      #   end
      #
      # Here is a call order:
      # * test_my_class
      # * my_cleanup2
      # * my_cleanup1
      # * cleanup
      def cleanup
      end

      # Called after every test method runs. Can be used to tear
      # down fixture information.
      #
      # You can add additional teardown tasks by the following
      # code:
      #   class TestMyClass < Test::Unit::TestCase
      #     def teardown
      #       ...
      #     end
      #
      #     teardown
      #     def my_teardown1
      #       ...
      #     end
      #
      #     teardown
      #     def my_teardown2
      #       ...
      #     end
      #
      #     def test_my_class
      #       ...
      #     end
      #   end
      #
      # Here is a call order:
      # * test_my_class
      # * my_teardown2
      # * my_teardown1
      # * teardown
      def teardown
      end

      def default_test
        flunk("No tests were specified")
      end

      def size
        1
      end

      # Returns a label of test data for the test. If the
      # test isn't associated with any test data, it returns
      # +nil+.
      def data_label
        @internal_data.test_data_label
      end

      # Returns a human-readable name for the specific test that
      # this instance of TestCase represents.
      def name
        if @internal_data.have_test_data?
          "#{@method_name}[#{data_label}](#{self.class.name})"
        else
          "#{@method_name}(#{self.class.name})"
        end
      end

      # Returns a description for the test. A description
      # will be associated by Test::Unit::TestCase.test or
      # Test::Unit::TestCase.description.
      #
      # Returns a name for the test for no description test.
      def description
        self[:description] || name
      end

      # Overridden to return #name.
      def to_s
        name
      end

      # It's handy to be able to compare TestCase instances.
      def ==(other)
        return false unless other.kind_of?(self.class)
        return false unless @method_name == other.method_name
        self.class == other.class
      end

      # Returns a Time at the test was started.
      def start_time
        @internal_data.start_time
      end

      # Returns elapsed time for the test was ran.
      def elapsed_time
        @internal_data.elapsed_time
      end

      # Returns whether the test is interrupted.
      def interrupted?
        @internal_data.interrupted?
      end

      # Returns whether this individual test passed or
      # not. Primarily for use in teardown so that artifacts
      # can be left behind if the test fails.
      def passed?
        @internal_data.passed?
      end

      private
      def current_result
        @_result
      end

      def run_test
        redefined_info = self[:redefined]
        if redefined_info
          notify("#{self.class}\##{@method_name} was redefined",
                 :backtrace => redefined_info[:backtrace])
        end
        if @internal_data.have_test_data?
          __send__(@method_name, @internal_data.test_data)
        else
          __send__(@method_name)
        end
      end

      def handle_exception(exception)
        self.class.exception_handlers.each do |handler|
          return true if send(handler, exception)
        end
        false
      end

      def problem_occurred
        @internal_data.problem_occurred
      end

      def add_assertion
        current_result.add_assertion
      end

      def add_pass
        current_result.add_pass
      end

      class InternalData
        attr_reader :start_time, :elapsed_time
        attr_reader :test_data_label, :test_data
        def initialize
          @start_time = nil
          @elapsed_time = nil
          @passed = true
          @interrupted = false
          @test_data_label = nil
          @test_data = nil
        end

        def passed?
          @passed
        end

        def interrupted?
          @interrupted
        end

        def assign_test_data(label, data)
          @test_data_label = label
          @test_data = data
        end

        def have_test_data?
          not @test_data_label.nil?
        end

        def test_started
          @start_time = Time.now
        end

        def test_finished
          @elapsed_time = Time.now - @start_time
        end

        def problem_occurred
          @passed = false
        end

        def interrupted
          @interrupted = true
        end
      end
    end
  end
end
