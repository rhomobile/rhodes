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

module Enumerable
    def each_with_index 
        i = 0;
        each {|x| yield x, i; i = i + 1}
    end
end
