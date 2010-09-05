require "rexml/child"

module REXML
  class Text < Child
    include Comparable

    attr_accessor :raw    
    
    def initialize(arg, respect_whitespace=false, parent=nil, raw=nil, entity_filter=nil )

      @raw = false

      if parent
        super( parent )
        @raw = parent.raw 
      else
        @parent = nil
      end

      @raw = raw unless raw.nil?
      @entity_filter = entity_filter
      @normalized = @unnormalized = nil

      if arg.kind_of? String
        @string = arg.clone
        @string.squeeze!(" \n\t") unless respect_whitespace
      else  
          if arg.kind_of? Text
            @string = arg.to_s
            @raw = arg.raw
          else
            raise "Illegal argument of type #{arg.type} for Text constructor (#{arg})"
          end
      end
      
      @string.gsub!( /\r\n?/, "\n" )
    end

    def node_type
      :text
    end

    def empty?
      @string.size==0
    end


    def clone
      return Text.new(self)
    end


    # Appends text to this text node.  The text is appended in the +raw+ mode
    # of this text node.
    def <<( to_append )
      @string << to_append.gsub( /\r\n?/, "\n" )
    end


    # +other+ a String or a Text
    # +returns+ the result of (to_s <=> arg.to_s)
    def <=>( other )
      to_s() <=> other.to_s
    end

    def doctype
      if @parent
        doc = @parent.document
        doc.doctype if doc
      end
    end

    def to_s
      @string
    end

    def inspect
      @string.inspect
    end

    def value
      #TODO: 
      return @unnormalized if @unnormalized
      @unnormalized = Text::unnormalize( @string, doctype )
      
      @unnormalized
    end

    #TODO optimize it - regular expressions is slow on BB
    def Text::unnormalize( string, doctype=nil, filter=nil, illegal=nil )
        string.to_s.gsub(/\&amp;/, "&").gsub(/\&quot;/, "\"").gsub(/\&gt;/, ">").gsub(/\&lt;/, "<")
    end
=begin    
    def Text::unnormalize( string, doctype=nil, filter=nil, illegal=nil )
      string.gsub( /\r\n?/, "\n" ).gsub( REFERENCE ) {
        ref = $&
        if ref[1] == ?#
          if ref[2] == ?x
            [ref[3...-1].to_i(16)].pack('U*')
          else
            [ref[2...-1].to_i].pack('U*')
          end
        elsif ref == '&amp;'
          '&'
        elsif filter and filter.include?( ref[1...-1] )
          ref
        elsif doctype
          doctype.entity( ref[1...-1] ) or ref
        else
          entity_value = DocType::DEFAULT_ENTITIES[ ref[1...-1] ]
          entity_value ? entity_value.value : ref
        end
      }
    end
=end
  end
end
