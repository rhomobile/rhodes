#
# Copyright 2005-2007 Xue Yong Zhi, Jie Li, Ye Zheng, Yu Zhang
# Distributed under the BSD License
#

#
#Some code are borrowed from rubinius, they are indentified by 'from rubinius'
#notice before the method. rubinius's copyright is here:
#http://code.fallingsnow.net/svn/rubinius/trunk/LICENSE
#


#Some built-in functions can be implemented in pure ruby, so they are implemented here.
#

$: = ["./lib/ruby/site_ruby/1.8", 
        "./lib/ruby/1.8",
        "./lib/ruby/1.8/xruby",
        "."]
$" = []
$* = ARGV
$SAFE = 0

TRUE = true
FALSE = false
NIL = nil

RUBY_VERSION = "1.8.5"
VERSION = "1.8.5"

TOPLEVEL_BINDING = self

#Float::EPSILON = 2.2204460492503131e-16

module Kernel   
    def nil?
        false
    end
    
    def fork
        raise NotImplementedError, "the fork() function is unimplemented on this machine"
    end

    def =~ x
        false
    end

    def singleton_method_added symbol
    end

    def singleton_method_removed symbol
    end

    def singleton_method_undefined symbol
    end

    #private
    def require(file_name)
        return false if ($".include?(file_name) || $".include?(file_name + ".rb"))
        
        load(file_name);
    end

    #private
    def load(file_name)
        if __load_with_reflection__(file_name)
            return true
        end
        
        $:.each do |path|
            return true if load_rb_file(path, file_name)
        end
        
        raise LoadError, "no such file to load -- " + file_name
    end

    private
    def load_rb_file(path, file_name)
        fullname = path + '/' + file_name
        if fullname[-3..-1] != ".rb"
            fullname << ".rb"
        end
        
        if !::File.file?(fullname)
            return false
        end

        content = ::IO.read(fullname)
        $".push(file_name) unless $".include?(file_name)
        eval(content, nil, fullname)
        return true
    end

    private
    def method_added symbol
    end
end

class Object
    def to_a
        [self]
    end

    alias type :class 
    
    private
    def initialize
    end
end

class Array
	
    alias reject! delete_if
  
    def reject
      a = []
      each {|x|
        if !yield x
          a << x
        end
      }
      a
    end
	
    def to_a
        self
    end

    def join(sepString="")
        return to_s if sepString.nil? || sepString == ""

        result = ""
        (length - 1).times do |index|
            result += (self[index].to_s) + sepString
        end
        result += self[length - 1].to_s if length != 0
        result
    end

    #alias join to_s
    alias to_ary to_a
    alias size length

    def inspect
        str = "["
        is_first = true
        self.each() { |x|
            if (!is_first)
                str << ", "
            end
            is_first = false
            str << x.inspect
        }
        str << "]"
    end
    
    #from rubinius
    def fetch(pos, *rest)
    	if rest.length > 1
      		raise ArgumentError, "wrong number of arguments (#{1 + rest.length} for 2)"
    	end
    	if !rest.empty? && block_given?
      		#warn about block superceding default arg
    	end
    	index = pos
    	if index < 0
      		index += self.length
    	end
    	if index < 0 || self.length <= index
      		return yield(pos) if block_given?
      		if rest.length == 0
        		raise IndexError, "index #{index} out of array"
      		end
      		return rest[0]
    	end
    	self.at(index)
  end
  
  #from rubinius
  def fill(*args)
    start_index = 0
    end_index = self.length - 1
    len = item = nil
    arg_length = args.length
    arg_index_offset = 0
    max_args = 2
    if !block_given?
      item = args[0]
      arg_length -= 1
      arg_index_offset = 1
      max_args = 3
    end

    arg1 = args[arg_index_offset] || 0
    arg2 = args[1 + arg_index_offset]

    case arg_length
    when 0
      start_index = 0
      end_index = self.length - 1
    when 1
      if arg1.nil? || arg1.kind_of?(Integer)
        start_index = arg1 || 0
        end_index = self.length - 1
      elsif arg1.kind_of?(Range)
        start_index = arg1.first
        end_index = arg1.last
        end_index -= 1 if arg1.exclude_end?
        if end_index < 0
          end_index += self.length
        end
      else
        raise TypeError, "can't convert #{arg1.class} into Integer"
      end
    when 2
      start_index = arg1
      len = arg2
    else
      raise ArgumentError, "wrong number of arguments (#{args.length} for #{max_args})"
    end
    if start_index < 0
      start_index += self.length
      start_index = 0 if start_index < 0
    end
    if len
      end_index = start_index + len - 1
    end
    
    (start_index..end_index).each do |i|
      self[i] = block_given? ? yield(i) : item
    end
    self
  end
  
  #from rubinius
  def transpose
    out = []
    a1 = shift
    a1.zip(*self) { |x| out << x }
    out
  end
  
  #from rubinius
  def values_at (*args)
    out = []
    args.each { |x|
      if Range === x
        # doesn't work: x.each { |i| out << self[i] }: one nil too many
        # doesn't work: out.concat self[x]: [1, 2, 3, 4, 5].values_at(0..2, 1...3, 4..6).should == [1, 2, 3, 2, 3, 5, nil]
        lst = x.last
        if lst < 0
          lst += @total
        end
        if lst > size
          lst = size            # sick to do this BEFORE exclude_end?
        end
        lst -= 1 if x.exclude_end?
        idx = x.first  
        idx.upto(lst) { |i|
          out << self[i]
        }
      else
        out << self[x]
      end
    }
    out
  end
  
  alias map! collect!
  
end

class Hash
    def each
        ks = keys
        ks.each {|k| yield([k, self[k]])}
    end
    
    def each_key
        ks = keys
        ks.each {|k| yield(k)}
    end
      
    def each_value
        vs = values
        vs.each {|k| yield(k)}
    end
    
    def empty?
        length == 0
    end
    
    alias each_pair each
    
    def inspect
      r = '{'
      is_first = true
      each_pair do |k, v|
        if !is_first
          r << ", "
        end
        is_first = false
        r << k.inspect
        r << '=>'
        r << v.inspect
      end
      r << '}'
    end

    def invert
      h = {}
      each {|k, v| h[v] = k}
      h
    end
    
    def update other
        other.each {|k, v| self[k] = v}
        self
    end
    
    alias merge! update
    
    def merge other
        clone.merge!(other)
    end
    
    def index value
        each {|k, v| return k if value == v }
        return nil
    end
end

class Symbol
    alias to_s id2name
end

class << self
    def to_s
        return "main"
    end

    def public
        Object.public
    end

    def private
        Object.private
    end

    def protected
        Object.protected
    end
end

class Fixnum
    def is_alpha_numeric
        return (self>=?0 && self<=?9) || (self>=?a && self<=?z) || (self>=?A && self<=?Z)
    end
end

class String
    include Comparable

    def to_s
        return self
    end

    def sum(n=16)
        sum = 0
        each_byte {|x| sum += x}
        if n != 0
            return sum & ((1 << n) - 1)
        else
            return sum
        end
    end

    #    def =~ x
    #        return x.match(self)
    #    end

    def index(x, *start)
        begin_index = 0
        if start.size > 0
            begin_index = start[0]
            begin_index = self.length + begin_index if begin_index < 0
        end
        if String === x
            begin_index.upto(self.length) do |i|
                if self[i, x.length] == x
                    return i
                end
            end
        elsif Fixnum === x
            begin_index.upto(self.length) do |i|
                if self[i] == x
                    return i
                end
            end
        else #regex
            x = (x =~ (self[begin_index, self.length-begin_index]))
            if x
                return x + begin_index
            end
            return nil
        end
    end

    def succ!
        if self == ""
            return ""
        end

        result = self
        index = self.length - 1

        if (last_index = __last_alpha_numeric__) == -1
            result[index] = (result[index] + 1).chr
            return result
        end
        carry = false
        carry_type = nil

        return __add_nocarry__(result, last_index)
    end 

    def succ
        result = self.dup  #may be dup?
        return result.succ!
    end

    def upto(end_str)
        start_str = self
        while start_str <= end_str
            puts "in:" + start_str
            yield start_str
            if start_str == end_str
                break
            end
            start_str = start_str.succ
        end
    end

    #add a carry to postion index, with carry_type
    def __add__(str, index, carry, carry_type)
        result = str
        char = str[index]
        if carry
            if index == -1 || str[0, str.length-1].__last_alpha_numeric__(index) == -1 #no char beforehand
                if carry_type == "CARRY_NUM"
                    return result.insert(index+1, "1")
                elsif carry_type == "CARRY_CHAR"
                    return result.insert(index+1, "a")
                end
            else
                return __add_nocarry__(str, index)
            end
        else #normal situation
            return __add_nocarry__(str, index)
        end


    end

    def __add_nocarry__(str, index)
        char = str[index]
        result = str
        if char == ?9 || char == ?z
            carry = true
            if char == ?9
                result[index] = ?0.chr
                carry_type = "CARRY_NUM"
            else
                result[index] = ?a.chr
                carry_type = "CARRY_CHAR"
            end

            return __add__(result, index-1, carry, carry_type)
        else #normal situation without carry
            result[index] = (result[index] + 1).chr
            return result
        end
    end

    def __last_alpha_numeric__(*start)
        index = nil
        if start.size > 0
            index = start[0]
            index = self.length + index if index < 0
        else
            index = self.length - 1
        end

        index.downto(0) do |i|
            if self[i].is_alpha_numeric
                return i
            end
        end
        return -1
    end

    def empty?
        length == 0
    end

    def inspect
        '"' + to_s + '"'
    end
    
    #from rubinius
    def insert(idx, str)
        if idx < 0
            idx += length + 1
        end
        raise IndexError, "index #{idx} out of string" if idx < 0 || idx > length
        if idx < length
            self[idx, 0] = str
        else
            self << str
        end
        self
    end

	#from rubinius
    # justify left = -1, center = 0, right = 1
    def justify_string(width, str, justify)
        return self if width <= length
        pad = width - length
        out = str.to_str * (pad / str.length)
        out << str[0, pad - out.length] if out.length < pad
        # Left justification
        return self << out if justify == -1
        # Right justification
        return out << self if justify == 1
        # and finially center
        split = (width / 2) - (length / 2)
        return out.insert(split-((width-length)%2), self)
    end

	#from rubinius
    def rjust(width, str=" ")
        justify_string(width, str, 1)
    end
	
	#from rubinius
    def ljust(width, str=" ")
        justify_string(width, str, -1)
    end

	#from rubinius
    def center(width, str=" ")
        justify_string(width, str, 0)
    end

    alias to_str to_s
    alias size length
end

module Comparable
    def >=(value)
        compare = (self <=> value)
        return compare != -1 and compare != nil
    end

    def ==(value)
        compare = (self <=> value)
        return compare == 0
    end

    def <=(value)
        compare = (self <=> value)
        return compare != 1 and compare != nil
    end

    def >(value)
        compare = (self <=> value)
        return compare == 1
    end

    def <(value)
        compare = (self <=> value)
        return compare == -1
    end

    def between?(a, b)
        self >= a && self <= b
    end
end

class Numeric
    include Comparable

    def floor
        self.to_f.floor
    end

    def abs
        return -self if (self <=> 0) == -1
        self
    end
    
    def div value
        (self/value).floor
    end

    def divmod(value)
        [(self/value).floor, self % value]
    end

    def integer?
        false
    end

    alias eql? :==

    def modulo(value)
        self % value
    end

    def nonzero?
        return nil if self == 0
        self
    end

    def zero?
        return true if self == 0
        false
    end

    def remainder(value)
        self_sign = (self < 0)
        value_sign = (value < 0)
        return self % value if self_sign == value_sign
        self % (-value)
    end

end

class Integer < Numeric

    def to_i
        return self
    end

    alias to_int to_i

    #Returns the Integer equal to int + 1
    def next
        self + 1
    end

    #Synonym for Integer#next
    def succ
        self + 1
    end

    #Always returns true
    def integer?
        true
    end

    def upto(to)
        a = self
        while a <= to
            yield a
            a += 1
        end
    end

    def downto(to)
        a = self
        while a >= to
            yield a
            a -= 1
        end
    end

    def size
        4
    end

    def integer?
        true
    end
end

class Fixnum < Integer
    def to_i
        self
    end

    alias inspect to_s
end

class Bignum < Integer
    #undef new
end

class NilClass

    #Returns false
    def &(anObject)
        false
    end

    #Returns false if anObject is nil or false, true otherwise
    def ^(anObject)
        anObject ? true : false
    end

    #Returns false if anObject is nil or false, true otherwise
    def |(anObject)
        anObject ? true : false
    end

    #Always returns true
    def nil?
        true
    end

    #Always returns an empty array
    def to_a
        []
    end

    #Always returns zero
    def to_i
        0
    end

    def to_f
        0.0
    end

    #Always returns the empty string
    def to_s
        ""
    end

    def inspect
        "nil"
    end
end

class TrueClass
    #Returns false if anObject is nil or false, true otherwise
    def &(anObject)
        anObject ? true : false
    end

    #Returns true if anObject is nil or false, false otherwise
    def ^(anObject)
        anObject ? false : true
    end

    #Returns true
    def |(anObject)
        true
    end

    def to_s
        "true"
    end

    def inspect
        "true"
    end
end

class FalseClass

    #Returns false
    def &(anObject)
        false
    end

    #If anObject is nil or false, returns false; otherwise, returns true
    def ^(anObject)
        anObject ? true : false
    end

    #Returns false if anObject is nil or false; true otherwise
    def |(anObject)
        anObject ? true : false
    end

    def to_s
        "false"
    end

    def inspect
        "false"
    end
end

class Range
    include Enumerable

    def ==(value)
        if value.first == first and value.last == last and value.exclude_end? == exclude_end?
            true
        else
            false
        end
    end

    def ===(value)
        each do |item|
            return true if value == item
        end
        false
    end

    def to_s
        return self.begin.to_s + "..." + self.end.to_s if exclude_end?
        return self.begin.to_s + ".." + self.end.to_s
    end

    alias inspect :to_s

    alias first :begin
    alias last :end

    def step(n=1)
        if n == 1 then
            each{|i|yield(i)}
        else
            counter = 0
		        each do |i|
            yield(i) if counter%n == 0
			      counter = counter + 1
		    end
	   end
    end
end

class File < IO
    RDWR = 2
    CREAT = 256
    EXCL = 1024
  
    SEPARATOR = separator

    def self.join(*strings)
        strings.join(separator)
    end

    def self.split(filename)
        [dirname(filename), basename(filename)]
    end
    
    module Constants
    end
end

class Time
    include Comparable
end

class Dir
    def Dir.chroot
        raise NotImplementedError, "the chroot() function is unimplemented on this machine"
    end
end

module Process
    def Process.getrlimit x
        raise NotImplementedError, "the getrlimit() function is unimplemented on this machine"
    end
    
    def Process.setrlimit *x
        raise NotImplementedError, "the setrlimit() function is unimplemented on this machine"
    end
    
    def Process.fork
        raise NotImplementedError, "the fork() function is unimplemented on this machine"
    end
    
end

class ThreadError < StandardError
end

class Proc
    def to_proc
        self
    end
    
    def inspect
        to_s  
    end      
end

class File
    SEPARATOR = '/'

    def File.join *aString
        s = ""
        first = true
        aString.each {|x|
            if !first
                s += File::SEPARATOR
            end
            s+= x
            first = false
        }
        s
    end
end

module Enumerable
    def find_all
        a = []
        each {|x|
            if yield x
                a.push(x)
            end
        }
        return a
    end

    alias :select :find_all

    def sort_by &block
        array_of_tuples = []
        each {|x| array_of_tuples.push([x, yield(x)])}
        array_of_tuples = array_of_tuples.sort {|x, y| x[1] <=> y[1]}
        return array_of_tuples.collect {|x| x[0]}
    end

    def detect(ifnone = nil)
        each { |obj| return obj if yield(obj) }
        ifnone.call if ifnone
    end
    
    alias find :detect

    def each_with_index 
        i = 0;
        each {|x| yield x, i; i = i + 1}
    end
    
    def to_a
        arr = []
        each{|obj| arr <<obj}
        return arr
    end
    
    alias entries :to_a

    def sort(&proc)
        #proc = lambda{ |a,b| a<=>b } unless block_given?
        arr = to_a
        arr.sort{|x,y| 
            if block_given?  then
                proc.call(x,y)
            else
            x<=>y
            end
        }
    end
    
    def member?(other)
        each{|obj| return true if obj == other }
        return false
    end
    
    alias include? :member?
    
    def inject(*args)
        if args.size == 0 then
            vals = to_a
            memo = vals[0]
            vals[1..vals.size-1].each {|obj| memo = yield(memo, obj)}
            return memo
        elsif args.size == 1 then
            memo = args[0]
            each {|obj| memo = yield(memo, obj)}
            return memo
        else
            nil
        end
    end
    
    def all?(&proc)
        proc = lambda { |obj| obj } unless block_given?
        each { |obj| return false unless proc.call(obj) }
        true
    end
    
    def any?(&proc)
        proc = lambda { |obj| obj } unless block_given?
        each { |obj| return true if proc.call(obj) }
        false
    end

    def collect
        arr = []
        each{|obj| arr << yield(obj)}
        return arr
    end   

    alias map :collect

    def max(&proc)
        proc = lambda { |a, b| a <=> b } unless block_given?
        max = nil
        each {|obj| max = obj if max.nil? || proc.call(max, obj) < 0}
        max
    end

    def min(&proc)
        proc = lambda { |a, b| a <=> b } unless block_given?
        min = nil
        each {|obj| min = obj if min.nil? || proc.call(min, obj) > 0}
        min
    end

    def partition
        left = []
        right = []
        each { |obj| yield(obj) ? left.push(obj) : right.push(obj) }
        return [left, right]
    end

    def zip(*args)
        result = []
        args = args.map{|arg| arg.to_a}
        each_with_index{|obj,i|
           arr = [obj]
           args.each{|x| arr << x[i]}
           result << arr

           yield(result.last) if block_given?
        }
        result unless block_given?
    end 
    
    def grep(pattern, &proc)
        result = []
        each do |obj|
            if pattern === obj
            if block_given? then
                result << proc.call(obj)
            else
                result << obj
            end
            end
            end
        result
  end
end

class Regexp
    EXTENDED = 2
    
    def ==(other)
        return false unless Regexp === other
        return inspect == other.inspect
    end
    
    def inspect
        '/' + source + '/'
    end
end

class IO
    def <<(v)
        write v
        return self
    end
    
    def rewind
        seek(0)
        return 0
    end    
    
    def getc
        return read(1)
    end

end

class Float
    def to_f
        self
    end
end

class <<ENV
    def to_s
        return "ENV"
    end
    
    #As we can not modify environment variable in java, 
    #we just store it in @fake_ev
    def delete x
        return if @fake_ev == nil
        @fake_ev.delete x.upcase
    end

    def [](x)
        raise TypeError.new("[] can't convert into String") if !(x.instance_of?(String))
        
        return __get_os_ev__(x) if @fake_ev == nil
        
        v = @fake_ev[x.upcase]
        if v == nil
            return __get_os_ev__(x)
        else
            return v
        end    
    end
    
    def []=(x, y)
        raise TypeError.new("[]= can't convert into String") if !(x.instance_of?(String))
        raise TypeError.new("[]= can't convert value into String") if !(y.instance_of?(String))
        @fake_ev = {} if @fake_ev == nil
        @fake_ev[x.upcase]=y
    end
    
    def index x
        v = @fake_ev.index(x)
        if v != nil
            return v
        else
            return __os_ev_index__(x)
        end
    end
    
    def has_value? x
        return (self.index(x) != nil)
    end
end

class SignalException < Exception
end

class Interrupt < SignalException
end

ENV["RUBY"] = 'java -jar xruby-0.3.3.jar'
