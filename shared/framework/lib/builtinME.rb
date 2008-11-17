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
	
    def update other
        other.each {|k, v| self[k] = v}
        self
    end
    
    alias merge! update
    
    def merge other
        clone.merge!(other)
    end
	
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
