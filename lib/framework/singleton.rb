# The Singleton module implements the Singleton pattern.
#
# Usage:
#    class Klass
#       include Singleton
#       # ...
#    end
#
# *  this ensures that only one instance of Klass lets call it
#    ``the instance'' can be created.
#
#      a,b  = Klass.instance, Klass.instance
#      a == b    # => true
#      Klass.new #  NoMethodError - new is private ...
#
# *  ``The instance'' is created at instantiation time, in other
#    words the first call of Klass.instance(), thus
#
#      class OtherKlass
#        include Singleton
#        # ...
#      end
#      ObjectSpace.each_object(OtherKlass){} # => 0.
#
# *  This behavior is preserved under inheritance and cloning.
#
#
#
# This is achieved by marking
# *  Klass.new and Klass.allocate - as private
#
# Providing (or modifying) the class methods
# *  Klass.inherited(sub_klass) and Klass.clone()  -
#    to ensure that the Singleton pattern is properly
#    inherited and cloned.
#
# *  Klass.instance()  -  returning ``the instance''. After a
#    successful self modifying (normally the first) call the
#    method body is a simple:
#
#       def Klass.instance()
#         return @singleton__instance__
#       end
#
# *  Klass._load(str)  -  calling Klass.instance()
#
# *  Klass._instantiate?()  -  returning ``the instance'' or
#    nil. This hook method puts a second (or nth) thread calling
#    Klass.instance() on a waiting loop. The return value
#    signifies the successful completion or premature termination
#    of the first, or more generally, current "instantiation thread".
#
#
# The instance method of Singleton are
# * clone and dup - raising TypeErrors to prevent cloning or duping
#
# *  _dump(depth) - returning the empty string.  Marshalling strips
#    by default all state information, e.g. instance variables and
#    taint state, from ``the instance''.  Providing custom _load(str)
#    and _dump(depth) hooks allows the (partially) resurrections of
#    a previous state of ``the instance''.

#require 'thread'

module Singleton
  #  disable build-in copying methods
  def clone
    raise TypeError, "can't clone instance of singleton #{self.class}"
  end
  def dup
    raise TypeError, "can't dup instance of singleton #{self.class}"
  end

  #  default marshalling strategy
  def _dump(depth = -1)
    ''
  end

  module SingletonClassMethods
    # properly clone the Singleton pattern - did you know
    # that duping doesn't copy class methods?
    def clone
      Singleton.__init__(super)
    end

    def _load(str)
      instance
    end

    private

    #  ensure that the Singleton pattern is properly inherited
    def inherited(sub_klass)
      super
      Singleton.__init__(sub_klass)
    end
  end

  class << Singleton
    def __init__(klass)
      klass.instance_eval {
        @singleton__instance__ = nil
        @singleton__mutex__ = Mutex.new
      }
      def klass.instance
        return @singleton__instance__ if @singleton__instance__
        @singleton__mutex__.synchronize {
          return @singleton__instance__ if @singleton__instance__
          @singleton__instance__ = new()
        }
        @singleton__instance__
      end
      klass
    end

    private

    #  extending an object with Singleton is a bad idea
    undef_method :extend_object

    def append_features(mod)
      #  help out people counting on transitive mixins
      unless mod.instance_of?(Class)
        raise TypeError, "Inclusion of the OO-Singleton module in module #{mod}"
      end
      super
    end

    def included(klass)
      super
      klass.private_class_method  :new, :allocate
      klass.extend SingletonClassMethods
      Singleton.__init__(klass)
    end
  end

end
