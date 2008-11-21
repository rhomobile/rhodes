$" = []
$* = ARGV
$SAFE = 0

TRUE = true
FALSE = false
NIL = nil

RUBY_VERSION = "1.8.5"
VERSION = "1.8.5"

TOPLEVEL_BINDING = self

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
        
#        $:.each do |path|
#            return true if load_rb_file(path, file_name)
#        end
        
        raise LoadError, "no such file to load -- " + file_name
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

module Enumerable
    def each_with_index 
        i = 0;
        each {|x| yield x, i; i = i + 1}
    end
	
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
	
    def collect
        arr = []
        each{|obj| arr << yield(obj)}
        return arr
    end   
    
    alias map :collect
    
end

class Array
    alias size length
	
    def join(sepString="")
        return to_s if sepString.nil? || sepString == ""
		
        result = ""
        (length - 1).times do |index|
            result += (self[index].to_s) + sepString
        end
        result += self[length - 1].to_s if length != 0
        result
    end

    alias map! collect!
  
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
    alias to_s inspect
    
end

class Time
    include Comparable
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
    
    alias to_s inspect
    
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
    alias to_str to_s		
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

class String
    include Comparable
	
    def to_s
        return self
    end
	
    def empty?
        length == 0
    end

    def inspect
        '"' + to_s + '"'
    end
	
    alias to_str to_s	
    alias size length	
end
