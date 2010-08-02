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
        nExt = file_name.index('.rb')
        if nExt 
            file_name1 = file_name[0,nExt]
            #puts "require test: #{file_name1}"
            return false if ($".include?(file_name1))
        end
        
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
    
    public
    def fork
        raise NotImplementedError, "the fork() function is unimplemented on this machine"
    end
    
end

class Object
#    def to_a
#        [self]
#    end
	
    alias type :class 
    
    private
    def initialize
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
        each {|x| puts "x: #{x}"; array_of_tuples.push([x, yield(x)])}
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

    def group_by(&block)
        res = {}
        self.each do |obj|
            group = yield(obj)
            values = res[group]
            if values.nil?
                values = [obj]
                res[group] = values
            else
                values << obj
            end
        end
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
	
#    def to_a
#        self
#    end
	
    def join(sepString="")
        #return to_s if sepString.nil? || sepString == ""
		
        result = ""
        return result if length==0
        
        sepString = "" if sepString.nil?
        
        (length - 1).times do |index|
            result += (self[index].to_s) + sepString
        end
        result += self[length - 1].to_s if length != 0
        result
    end

    alias map! collect!
    alias size length
    alias to_ary to_a
  
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
    
    def count(*args, &block)
        
        if !args || args.length() == 0
            n = 0
            if !block_given?
                return self.length()
            else
                self.each do |item|                
                    n += 1 if yield(item)
                end
            end
            
            return n
        else
            n = 0
            self.each do |item|                
                n += 1 if item == args[0]
            end
            
            return n
        end    
    end
    
end

class File
    SEPARATOR = '/'

    def File.join *aString
        s = ""
        first = true
        aString.each {|x|
            if !first and !s.end_with?(File::SEPARATOR)
                s += File::SEPARATOR
            end
            s+= x
            first = false
        }
        s
    end
end

class IO
    def each
        while !eof
          yield gets
        end
        close
    end
    
    alias each_line each
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
    
    def to_a
        res = []
        each_pair do |k, v|
          item = []
          item << k
          item << v
          res << item
        end
        res
    end

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
    
    def replace(other)
        clear
        update other
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
	
    def empty?
        length == 0
    end

    def inspect
        '"' + to_s + '"'
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

    def slice!(arg1)
        res = slice(arg1)
        self[arg1]= "" if res
        res
    end
    
    def slice!(arg1,arg2)
        res = slice(arg1,arg2)
        self[arg1,arg2]= "" if res
        res
    end
    	
end
