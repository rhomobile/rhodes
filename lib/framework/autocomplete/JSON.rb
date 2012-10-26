 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
module JSON
  VERSION = "1.1.4"

  VERSION_ARRAY = []

  VERSION_MAJOR = 1

  VERSION_MINOR = 1

  VERSION_BUILD = 4

  VARIANT_BINARY = false

  NaN = 1

  Infinity = Infinity

  MinusInfinity = -Infinity

  class JSONError
  end

  class ParserError
  end

  class NestingError
  end

  class GeneratorError
  end

  class UnparserError
  end

  class CircularDatastructure
  end

  class MissingUnicodeSupport
  end

  MAP = {}

  module Pure
  end

  class State
    def self.from_state(opts)
    end    
    def indent
    end    
    def indent=(req)
    end    
    def space
    end    
    def space=(req)
    end    
    def space_before
    end    
    def space_before=(req)
    end    
    def object_nl
    end    
    def object_nl=(req)
    end    
    def array_nl
    end    
    def array_nl=(req)
    end    
    def max_nesting
    end    
    def max_nesting=(req)
    end    
    def check_max_nesting(depth)
    end    
    def check_circular?
    end    
    def allow_nan?
    end    
    def seen?(object)
    end    
    def remember(object)
    end    
    def forget(object)
    end    
    def configure(opts)
    end    
    def to_h
    end    
  end

  JSON_LOADED = true

  def self.[](object,opts)
  end  
  def self.parser
  end  
  def self.parser=(parser)
  end  
  def self.deep_const_get(path)
  end  
  def self.generator=(generator)
  end  
  def self.generator
  end  
  def self.state
  end  
  def self.state=(req)
  end  
  def self.create_id
  end  
  def self.create_id=(req)
  end  
  def self.generate(obj,state)
  end  
  def self.fast_unparse(obj)
  end  
  def self.pretty_unparse(obj,opts)
  end  
  def self.recurse_proc(result,proc)
  end  
  def self.restore(source,proc)
  end  
  def self.utf8_to_json(string)
  end  
  def self.swap!(string)
  end  
  def fast_generate(obj)
  end  
  def pretty_generate(obj,opts)
  end  
  def load(source,proc)
  end  
  def dump(obj,anIO,limit)
  end  
end

