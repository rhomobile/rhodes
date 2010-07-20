# == Pretty-printer for Ruby objects.
#
# = Which seems better?
#
# non-pretty-printed output by #p is:
#   #<PP:0x81fedf0 @genspace=#<Proc:0x81feda0>, @group_queue=#<PrettyPrint::GroupQueue:0x81fed3c @queue=[[#<PrettyPrint::Group:0x81fed78 @breakables=[], @depth=0, @break=false>], []]>, @buffer=[], @newline="\n", @group_stack=[#<PrettyPrint::Group:0x81fed78 @breakables=[], @depth=0, @break=false>], @buffer_width=0, @indent=0, @maxwidth=79, @output_width=2, @output=#<IO:0x8114ee4>>
#
# pretty-printed output by #pp is:
#   #<PP:0x81fedf0
#    @buffer=[],
#    @buffer_width=0,
#    @genspace=#<Proc:0x81feda0>,
#    @group_queue=
#     #<PrettyPrint::GroupQueue:0x81fed3c
#      @queue=
#       [[#<PrettyPrint::Group:0x81fed78 @break=false, @breakables=[], @depth=0>],
#        []]>,
#    @group_stack=
#     [#<PrettyPrint::Group:0x81fed78 @break=false, @breakables=[], @depth=0>],
#    @indent=0,
#    @maxwidth=79,
#    @newline="\n",
#    @output=#<IO:0x8114ee4>,
#    @output_width=2>
#
# I like the latter.  If you do too, this library is for you.
#
# = Usage
#
#   pp(obj)
#
# output +obj+ to +$>+ in pretty printed format.
#
# It returns +nil+.
#
# = Output Customization
# To define your customized pretty printing function for your classes,
# redefine a method #pretty_print(+pp+) in the class.
# It takes an argument +pp+ which is an instance of the class PP.
# The method should use PP#text, PP#breakable, PP#nest, PP#group and
# PP#pp to print the object.
#
# = Author
# Tanaka Akira <akr@m17n.org>

# This class implements a pretty printing algorithm. It finds line breaks and
# nice indentations for grouped structure.
#
# By default, the class assumes that primitive elements are strings and each
# byte in the strings have single column in width. But it can be used for
# other situations by giving suitable arguments for some methods:
# * newline object and space generation block for PrettyPrint.new
# * optional width argument for PrettyPrint#text
# * PrettyPrint#breakable
#
# There are several candidate uses:
# * text formatting using proportional fonts
# * multibyte characters which has columns different to number of bytes
# * non-string formatting
#
# == Bugs
# * Box based formatting?
# * Other (better) model/algorithm?
#
# == References
# Christian Lindig, Strictly Pretty, March 2000,
# http://www.st.cs.uni-sb.de/~lindig/papers/#pretty
#
# Philip Wadler, A prettier printer, March 1998,
# http://homepages.inf.ed.ac.uk/wadler/topics/language-design.html#prettier
#
# == Author
# Tanaka Akira <akr@m17n.org>
#
class PrettyPrint

  # This is a convenience method which is same as follows:
  #
  #   begin
  #     q = PrettyPrint.new(output, maxwidth, newline, &genspace)
  #     ...
  #     q.flush
  #     output
  #   end
  #
  def PrettyPrint.format(output='', maxwidth=79, newline="\n", genspace=lambda {|n| ' ' * n})
    q = PrettyPrint.new(output, maxwidth, newline, &genspace)
    yield q
    q.flush
    output
  end

  # This is similar to PrettyPrint::format but the result has no breaks.
  #
  # +maxwidth+, +newline+ and +genspace+ are ignored.
  #
  # The invocation of +breakable+ in the block doesn't break a line and is
  # treated as just an invocation of +text+.
  #
  def PrettyPrint.singleline_format(output='', maxwidth=nil, newline=nil, genspace=nil)
    q = SingleLine.new(output)
    yield q
    output
  end

  # Creates a buffer for pretty printing.
  #
  # +output+ is an output target. If it is not specified, '' is assumed. It
  # should have a << method which accepts the first argument +obj+ of
  # PrettyPrint#text, the first argument +sep+ of PrettyPrint#breakable, the
  # first argument +newline+ of PrettyPrint.new, and the result of a given
  # block for PrettyPrint.new.
  #
  # +maxwidth+ specifies maximum line length. If it is not specified, 79 is
  # assumed. However actual outputs may overflow +maxwidth+ if long
  # non-breakable texts are provided.
  #
  # +newline+ is used for line breaks. "\n" is used if it is not specified.
  #
  # The block is used to generate spaces. {|width| ' ' * width} is used if it
  # is not given.
  #
  def initialize(output='', maxwidth=79, newline="\n", &genspace)
    @output = output
    @maxwidth = maxwidth
    @newline = newline
    @genspace = genspace || lambda {|n| ' ' * n}

    @output_width = 0
    @buffer_width = 0
    @buffer = []

    root_group = Group.new(0)
    @group_stack = [root_group]
    @group_queue = GroupQueue.new(root_group)
    @indent = 0
  end
  attr_reader :output, :maxwidth, :newline, :genspace
  attr_reader :indent, :group_queue

  def current_group
    @group_stack.last
  end

  # first? is a predicate to test the call is a first call to first? with
  # current group.
  #
  # It is useful to format comma separated values as:
  #
  #   q.group(1, '[', ']') {
  #     xxx.each {|yyy|
  #       unless q.first?
  #         q.text ','
  #         q.breakable
  #       end
  #       ... pretty printing yyy ...
  #     }
  #   }
  #
  # first? is obsoleted in 1.8.2.
  #
  def first?
    warn "PrettyPrint#first? is obsoleted at 1.8.2."
    current_group.first?
  end

  def break_outmost_groups
    while @maxwidth < @output_width + @buffer_width
      return unless group = @group_queue.deq
      until group.breakables.empty?
        data = @buffer.shift
        @output_width = data.output(@output, @output_width)
        @buffer_width -= data.width
      end
      while !@buffer.empty? && Text === @buffer.first
        text = @buffer.shift
        @output_width = text.output(@output, @output_width)
        @buffer_width -= text.width
      end
    end
  end

  # This adds +obj+ as a text of +width+ columns in width.
  #
  # If +width+ is not specified, obj.length is used.
  #
  def text(obj, width=obj.length)
    if @buffer.empty?
      @output << obj
      @output_width += width
    else
      text = @buffer.last
      unless Text === text
        text = Text.new
        @buffer << text
      end
      text.add(obj, width)
      @buffer_width += width
      break_outmost_groups
    end
  end

  def fill_breakable(sep=' ', width=sep.length)
    group { breakable sep, width }
  end

  # This tells "you can break a line here if necessary", and a +width+\-column
  # text +sep+ is inserted if a line is not broken at the point.
  #
  # If +sep+ is not specified, " " is used.
  #
  # If +width+ is not specified, +sep.length+ is used. You will have to
  # specify this when +sep+ is a multibyte character, for example.
  #
  def breakable(sep=' ', width=sep.length)
    group = @group_stack.last
    if group.break?
      flush
      @output << @newline
      @output << @genspace.call(@indent)
      @output_width = @indent
      @buffer_width = 0
    else
      @buffer << Breakable.new(sep, width, self)
      @buffer_width += width
      break_outmost_groups
    end
  end

  # Groups line break hints added in the block. The line break hints are all
  # to be used or not.
  #
  # If +indent+ is specified, the method call is regarded as nested by
  # nest(indent) { ... }.
  #
  # If +open_obj+ is specified, <tt>text open_obj, open_width</tt> is called
  # before grouping. If +close_obj+ is specified, <tt>text close_obj,
  # close_width</tt> is called after grouping.
  #
  def group(indent=0, open_obj='', close_obj='', open_width=open_obj.length, close_width=close_obj.length)
    text open_obj, open_width
    group_sub {
      nest(indent) {
        yield
      }
    }
    text close_obj, close_width
  end

  def group_sub
    group = Group.new(@group_stack.last.depth + 1)
    @group_stack.push group
    @group_queue.enq group
    begin
      yield
    ensure
      @group_stack.pop
      if group.breakables.empty?
        @group_queue.delete group
      end
    end
  end

  # Increases left margin after newline with +indent+ for line breaks added in
  # the block.
  #
  def nest(indent)
    @indent += indent
    begin
      yield
    ensure
      @indent -= indent
    end
  end

  # outputs buffered data.
  #
  def flush
    @buffer.each {|data|
      @output_width = data.output(@output, @output_width)
    }
    @buffer.clear
    @buffer_width = 0
  end

  class Text
    def initialize
      @objs = []
      @width = 0
    end
    attr_reader :width

    def output(out, output_width)
      @objs.each {|obj| out << obj}
      output_width + @width
    end

    def add(obj, width)
      @objs << obj
      @width += width
    end
  end

  class Breakable
    def initialize(sep, width, q)
      @obj = sep
      @width = width
      @pp = q
      @indent = q.indent
      @group = q.current_group
      @group.breakables.push self
    end
    attr_reader :obj, :width, :indent

    def output(out, output_width)
      @group.breakables.shift
      if @group.break?
        out << @pp.newline
        out << @pp.genspace.call(@indent)
        @indent
      else
        @pp.group_queue.delete @group if @group.breakables.empty?
        out << @obj
        output_width + @width
      end
    end
  end

  class Group
    def initialize(depth)
      @depth = depth
      @breakables = []
      @break = false
    end
    attr_reader :depth, :breakables

    def break
      @break = true
    end

    def break?
      @break
    end

    def first?
      if defined? @first
        false
      else
        @first = false
        true
      end
    end
  end

  class GroupQueue
    def initialize(*groups)
      @queue = []
      groups.each {|g| enq g}
    end

    def enq(group)
      depth = group.depth
      @queue << [] until depth < @queue.length
      @queue[depth] << group
    end

    def deq
      @queue.each {|gs|
        (gs.length-1).downto(0) {|i|
          unless gs[i].breakables.empty?
            group = gs.slice!(i, 1).first
            group.break
            return group
          end
        }
        gs.each {|group| group.break}
        gs.clear
      }
      return nil
    end

    def delete(group)
      @queue[group.depth].delete(group)
    end
  end

  class SingleLine
    def initialize(output, maxwidth=nil, newline=nil)
      @output = output
      @first = [true]
    end

    def text(obj, width=nil)
      @output << obj
    end

    def breakable(sep=' ', width=nil)
      @output << sep
    end

    def nest(indent)
      yield
    end

    def group(indent=nil, open_obj='', close_obj='', open_width=nil, close_width=nil)
      @first.push true
      @output << open_obj
      yield
      @output << close_obj
      @first.pop
    end

    def flush
    end

    def first?
      result = @first[-1]
      @first[-1] = false
      result
    end
  end
end

module Kernel
  # returns a pretty printed object as a string.
  def pretty_inspect
    PP.pp(self, '')
  end

  private
  # prints arguments in pretty form.
  #
  # pp returns nil.
  def pp(*objs) # :doc:
    objs.each {|obj|
      PP.pp(obj)
    }
    nil
  end
  module_function :pp
end

class PP < PrettyPrint
  # Outputs +obj+ to +out+ in pretty printed format of
  # +width+ columns in width.
  #
  # If +out+ is omitted, +$>+ is assumed.
  # If +width+ is omitted, 79 is assumed.
  #
  # PP.pp returns +out+.
  def PP.pp(obj, out=$>, width=79)
    q = PP.new(out, width)
    q.guard_inspect_key {q.pp obj}
    q.flush
    #$pp = q
    out << "\n"
  end

  # Outputs +obj+ to +out+ like PP.pp but with no indent and
  # newline.
  #
  # PP.singleline_pp returns +out+.
  def PP.singleline_pp(obj, out=$>)
    q = SingleLine.new(out)
    q.guard_inspect_key {q.pp obj}
    q.flush
    out
  end

  # :stopdoc:
  def PP.mcall(obj, mod, meth, *args, &block)
    mod.instance_method(meth).bind(obj).call(*args, &block)
  end
  # :startdoc:

  @sharing_detection = false
  class << self
    # Returns the sharing detection flag as a boolean value.
    # It is false by default.
    attr_accessor :sharing_detection
  end

  module PPMethods
    def guard_inspect_key
      if Thread.current[:__recursive_key__] == nil
        Thread.current[:__recursive_key__] = {}
      end

      if Thread.current[:__recursive_key__][:inspect] == nil
        Thread.current[:__recursive_key__][:inspect] = {}
      end

      save = Thread.current[:__recursive_key__][:inspect]

      begin
        Thread.current[:__recursive_key__][:inspect] = {}
        yield
      ensure
        Thread.current[:__recursive_key__][:inspect] = save
      end
    end

    def check_inspect_key(id)
      Thread.current[:__recursive_key__] &&
      Thread.current[:__recursive_key__][:inspect] &&
      Thread.current[:__recursive_key__][:inspect].include?(id)
    end
    def push_inspect_key(id)
      Thread.current[:__recursive_key__][:inspect][id] = true
    end
    def pop_inspect_key(id)
      Thread.current[:__recursive_key__][:inspect].delete id
    end

    # Adds +obj+ to the pretty printing buffer
    # using Object#pretty_print or Object#pretty_print_cycle.
    #
    # Object#pretty_print_cycle is used when +obj+ is already
    # printed, a.k.a the object reference chain has a cycle.
    def pp(obj)
      id = obj.object_id

      if check_inspect_key(id)
        group {obj.pretty_print_cycle self}
        return
      end

      begin
        push_inspect_key(id)
        group {obj.pretty_print self}
      ensure
        pop_inspect_key(id) unless PP.sharing_detection
      end
    end

    # A convenience method which is same as follows:
    #
    #   group(1, '#<' + obj.class.name, '>') { ... }
    def object_group(obj, &block) # :yield:
      group(1, '#<' + obj.class.name, '>', &block)
    end

    if 0x100000000.class == Bignum
      # 32bit
      PointerMask = 0xffffffff
    else
      # 64bit
      PointerMask = 0xffffffffffffffff
    end

    case Object.new.inspect
    when /\A\#<Object:0x([0-9a-f]+)>\z/
      PointerFormat = "%0#{$1.length}x"
    else
      PointerFormat = "%x"
    end

    def object_address_group(obj, &block)
      id = PointerFormat % (obj.object_id * 2 & PointerMask)
      group(1, "\#<#{obj.class}:0x#{id}", '>', &block)
    end

    # A convenience method which is same as follows:
    #
    #   text ','
    #   breakable
    def comma_breakable
      text ','
      breakable
    end

    # Adds a separated list.
    # The list is separated by comma with breakable space, by default.
    #
    # #seplist iterates the +list+ using +iter_method+.
    # It yields each object to the block given for #seplist.
    # The procedure +separator_proc+ is called between each yields.
    #
    # If the iteration is zero times, +separator_proc+ is not called at all.
    #
    # If +separator_proc+ is nil or not given,
    # +lambda { comma_breakable }+ is used.
    # If +iter_method+ is not given, :each is used.
    #
    # For example, following 3 code fragments has similar effect.
    #
    #   q.seplist([1,2,3]) {|v| xxx v }
    #
    #   q.seplist([1,2,3], lambda { q.comma_breakable }, :each) {|v| xxx v }
    #
    #   xxx 1
    #   q.comma_breakable
    #   xxx 2
    #   q.comma_breakable
    #   xxx 3
    def seplist(list, sep=nil, iter_method=:each) # :yield: element
      sep ||= lambda { comma_breakable }
      first = true
      list.__send__(iter_method) {|*v|
        if first
          first = false
        else
          sep.call
        end
        yield(*v)
      }
    end

    def pp_object(obj)
      object_address_group(obj) {
        seplist(obj.pretty_print_instance_variables, lambda { text ',' }) {|v|
          breakable
          v = v.to_s if Symbol === v
          text v
          text '='
          group(1) {
            breakable ''
            pp(obj.instance_eval(v))
          }
        }
      }
    end

    def pp_hash(obj)
      group(1, '{', '}') {
        seplist(obj, nil, :each_pair) {|k, v|
          group {
            pp k
            text '=>'
            group(1) {
              breakable ''
              pp v
            }
          }
        }
      }
    end
  end

  include PPMethods

  class SingleLine < PrettyPrint::SingleLine
    include PPMethods
  end

  module ObjectMixin
    # 1. specific pretty_print
    # 2. specific inspect
    # 3. specific to_s if instance variable is empty
    # 4. generic pretty_print

    # A default pretty printing method for general objects.
    # It calls #pretty_print_instance_variables to list instance variables.
    #
    # If +self+ has a customized (redefined) #inspect method,
    # the result of self.inspect is used but it obviously has no
    # line break hints.
    #
    # This module provides predefined #pretty_print methods for some of
    # the most commonly used built-in classes for convenience.
    def pretty_print(q)
      if /\(Kernel\)#/ !~ Object.instance_method(:method).bind(self).call(:inspect).inspect
        q.text self.inspect
      elsif /\(Kernel\)#/ !~ Object.instance_method(:method).bind(self).call(:to_s).inspect && instance_variables.empty?
        q.text self.to_s
      else
        q.pp_object(self)
      end
    end

    # A default pretty printing method for general objects that are
    # detected as part of a cycle.
    def pretty_print_cycle(q)
      q.object_address_group(self) {
        q.breakable
        q.text '...'
      }
    end

    # Returns a sorted array of instance variable names.
    #
    # This method should return an array of names of instance variables as symbols or strings as:
    # +[:@a, :@b]+.
    def pretty_print_instance_variables
      instance_variables.sort
    end

    # Is #inspect implementation using #pretty_print.
    # If you implement #pretty_print, it can be used as follows.
    #
    #   alias inspect pretty_print_inspect
    #
    # However, doing this requires that every class that #inspect is called on
    # implement #pretty_print, or a RuntimeError will be raised.
    def pretty_print_inspect
      if /\(PP::ObjectMixin\)#/ =~ Object.instance_method(:method).bind(self).call(:pretty_print).inspect
        raise "pretty_print is not overridden for #{self.class}"
      end
      PP.singleline_pp(self, '')
    end
  end
end

class Array
  def pretty_print(q)
    q.group(1, '[', ']') {
      q.seplist(self) {|v|
        q.pp v
      }
    }
  end

  def pretty_print_cycle(q)
    q.text(empty? ? '[]' : '[...]')
  end
end

class Hash
  def pretty_print(q)
    q.pp_hash self
  end

  def pretty_print_cycle(q)
    q.text(empty? ? '{}' : '{...}')
  end
end

class << ENV
  def pretty_print(q)
    h = {}
    ENV.keys.sort.each {|k|
      h[k] = ENV[k]
    }
    q.pp_hash h
  end
end

class Struct
  def pretty_print(q)
    q.group(1, '#<struct ' + PP.mcall(self, Kernel, :class).name, '>') {
      q.seplist(PP.mcall(self, Struct, :members), lambda { q.text "," }) {|member|
        q.breakable
        q.text member.to_s
        q.text '='
        q.group(1) {
          q.breakable ''
          q.pp self[member]
        }
      }
    }
  end

  def pretty_print_cycle(q)
    q.text sprintf("#<struct %s:...>", PP.mcall(self, Kernel, :class).name)
  end
end

class Range
  def pretty_print(q)
    q.pp self.begin
    q.breakable ''
    q.text(self.exclude_end? ? '...' : '..')
    q.breakable ''
    q.pp self.end
  end
end

class File
  class Stat
    def pretty_print(q)
      require 'etc.so'
      q.object_group(self) {
        q.breakable
        q.text sprintf("dev=0x%x", self.dev); q.comma_breakable
        q.text "ino="; q.pp self.ino; q.comma_breakable
        q.group {
          m = self.mode
          q.text sprintf("mode=0%o", m)
          q.breakable
          q.text sprintf("(%s %c%c%c%c%c%c%c%c%c)",
            self.ftype,
            (m & 0400 == 0 ? ?- : ?r),
            (m & 0200 == 0 ? ?- : ?w),
            (m & 0100 == 0 ? (m & 04000 == 0 ? ?- : ?S) :
                             (m & 04000 == 0 ? ?x : ?s)),
            (m & 0040 == 0 ? ?- : ?r),
            (m & 0020 == 0 ? ?- : ?w),
            (m & 0010 == 0 ? (m & 02000 == 0 ? ?- : ?S) :
                             (m & 02000 == 0 ? ?x : ?s)),
            (m & 0004 == 0 ? ?- : ?r),
            (m & 0002 == 0 ? ?- : ?w),
            (m & 0001 == 0 ? (m & 01000 == 0 ? ?- : ?T) :
                             (m & 01000 == 0 ? ?x : ?t)))
        }
        q.comma_breakable
        q.text "nlink="; q.pp self.nlink; q.comma_breakable
        q.group {
          q.text "uid="; q.pp self.uid
          begin
            pw = Etc.getpwuid(self.uid)
          rescue ArgumentError
          end
          if pw
            q.breakable; q.text "(#{pw.name})"
          end
        }
        q.comma_breakable
        q.group {
          q.text "gid="; q.pp self.gid
          begin
            gr = Etc.getgrgid(self.gid)
          rescue ArgumentError
          end
          if gr
            q.breakable; q.text "(#{gr.name})"
          end
        }
        q.comma_breakable
        q.group {
          q.text sprintf("rdev=0x%x", self.rdev)
          q.breakable
          q.text sprintf('(%d, %d)', self.rdev_major, self.rdev_minor)
        }
        q.comma_breakable
        q.text "size="; q.pp self.size; q.comma_breakable
        q.text "blksize="; q.pp self.blksize; q.comma_breakable
        q.text "blocks="; q.pp self.blocks; q.comma_breakable
        q.group {
          t = self.atime
          q.text "atime="; q.pp t
          q.breakable; q.text "(#{t.tv_sec})"
        }
        q.comma_breakable
        q.group {
          t = self.mtime
          q.text "mtime="; q.pp t
          q.breakable; q.text "(#{t.tv_sec})"
        }
        q.comma_breakable
        q.group {
          t = self.ctime
          q.text "ctime="; q.pp t
          q.breakable; q.text "(#{t.tv_sec})"
        }
      }
    end
  end
end

class MatchData
  def pretty_print(q)
    nc = []
    self.regexp.named_captures.each {|name, indexes|
      indexes.each {|i| nc[i] = name }
    }
    q.object_group(self) {
      q.breakable
      q.seplist(0...self.size, lambda { q.breakable }) {|i|
        if i == 0
          q.pp self[i]
        else
          if nc[i]
            q.text nc[i]
          else
            q.pp i
          end
          q.text ':'
          q.pp self[i]
        end
      }
    }
  end
end

class Object
  include PP::ObjectMixin
end

[Numeric, Symbol, FalseClass, TrueClass, NilClass, Module].each {|c|
#  c.class_eval {
#    def pretty_print_cycle(q)
#      q.text inspect
#    end
#  }
}

[Numeric, FalseClass, TrueClass, Module].each {|c|
#  c.class_eval {
#    def pretty_print(q)
#      q.text inspect
#    end
#  }
}
