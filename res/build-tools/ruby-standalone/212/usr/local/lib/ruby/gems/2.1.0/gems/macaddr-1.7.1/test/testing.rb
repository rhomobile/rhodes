require 'test/unit'

testdir = File.expand_path(File.dirname(__FILE__))
rootdir = File.dirname(testdir)
libdir = File.join(rootdir, 'lib')

STDOUT.sync = true

$:.unshift(testdir) unless $:.include?(testdir)
$:.unshift(libdir) unless $:.include?(libdir)
$:.unshift(rootdir) unless $:.include?(rootdir)

def Testing(*args, &block)
  Class.new(Test::Unit::TestCase) do

  ## class methods
  #
    class << self
      def context(*args, &block)
        @context = args unless args.empty?
        block.call(*@context) if block
        @context
      end

      def slug_for(*args)
        string = [@context, args].flatten.compact.join('-')
        words = string.to_s.scan(%r/\w+/)
        words.map!{|word| word.gsub %r/[^0-9a-zA-Z_-]/, ''}
        words.delete_if{|word| word.nil? or word.strip.empty?}
        words.join('-').downcase
      end

      def name() const_get(:Name) end

      def testno()
        '%05d' % (@testno ||= 0)
      ensure
        @testno += 1
      end

      def testing(*args, &block)
        method = ["test", testno, slug_for(*args)].delete_if{|part| part.empty?}.join('_')
        define_method("test_#{ testno }_#{ slug_for(*args) }", &block)
      end

      def setup(&block)
        define_method(:setup, &block) if block
      end

      def teardown(&block)
        define_method(:teardown, &block) if block
      end

      def prepare(&block)
        @prepare ||= []
        @prepare.push(block) if block
        @prepare
      end

      def cleanup(&block)
        @cleanup ||= []
        @cleanup.push(block) if block
        @cleanup
      end
    end

  ## configure the subclass!
  #
    const_set(:Testno, '0')
    slug = slug_for(*args).gsub(%r/-/,'_')
    name = ['TESTING', '%03d' % const_get(:Testno), slug].delete_if{|part| part.empty?}.join('_')
    name = name.upcase!
    const_set(:Name, name)
    const_set(:Missing, Object.new.freeze)

  ## instance methods
  #
    alias_method('__assert__', 'assert')

    def assert(*args, &block)
      if args.size == 1 and args.first.is_a?(Hash)
        options = args.first
        expected = getopt(:expected, options){ missing }
        actual = getopt(:actual, options){ missing }
        if expected == missing and actual == missing
          actual, expected, *ignored = options.to_a.flatten
        end
        expected = expected.call() if expected.respond_to?(:call)
        actual = actual.call() if actual.respond_to?(:call)
        assert_equal(expected, actual)
      end

      if block
        label = "assert(#{ args.join(' ') })"
        result = nil
        assert_nothing_raised{ result = block.call }
        __assert__(result, label)
        result
      else
        result = args.shift
        label = "assert(#{ args.join(' ') })"
        __assert__(result, label)
        result
      end
    end

    def missing
      self.class.const_get(:Missing)
    end

    def getopt(opt, hash, options = nil, &block)
      [opt.to_s, opt.to_s.to_sym].each do |key|
        return hash[key] if hash.has_key?(key)
      end
      default =
        if block
          block.call
        else
          options.is_a?(Hash) ? options[:default] : nil
        end
      return default
    end

    def subclass_of exception
      class << exception
        def ==(other) super or self > other end
      end
      exception
    end

  ##
  #
    module_eval(&block)

    self.setup()
    self.prepare.each{|b| b.call()}

    at_exit{ 
      self.teardown()
      self.cleanup.each{|b| b.call()}
    }


    self
  end
end
