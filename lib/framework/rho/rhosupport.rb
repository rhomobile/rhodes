require 'indifferent_access'

module Rho
  module RhoSupport

    class << self
      
      def url_encode(s)
#        s.to_s.dup.force_encoding("ASCII-8BIT").gsub(/[^a-zA-Z0-9_\-.]/n) {
#          sprintf("%%%02X", $&.unpack("C")[0])
#        }
        s.to_s.gsub(/[^a-zA-Z0-9_\-.]/n) do
            us = $&
            tmp = ''
            us.each_byte do |uc|
              tmp << sprintf('%%%02X', uc)
            end
            tmp
        end.force_encoding("ASCII-8BIT")

      end
      #def url_decode(s)
      #  s.tr('+',' ').gsub(/((?:%[0-9a-fA-f]{2})+)/n) do
      #    [$1.delete('%')].pack('H*')  
      #  end
      #end
      
     # def _unescape(str, regex) str.gsub(regex){ $1.hex.chr } end
      def url_decode(str)
        _url_decode(str, /((?:%[0-9a-fA-f]{2})+)/n)
      end
      
      def _url_decode(str, regex)
        return str if str.nil? || str.length() == 0
        
        isEncoded = false
        res = str.tr('+',' ').gsub(regex) { 
          isEncoded = true
          #[$&[1, 2].hex].pack('C') 
          [$1.delete('%')].pack('H*')
        }
        if isEncoded
            res.force_encoding("ASCII-8BIT") #need for BB since Java string is UTF16
            res.force_encoding('UTF-8')
        end
        
        res    
      end

      #ESCAPED = /%([0-9a-fA-F]{2})/
	
      #def unescape_form(str)
      #  _unescape(str.gsub(/\+/, " "), ESCAPED)
      #end
      def form_decode(str)
        _url_decode(str, /%([0-9a-fA-F]{2})/)
      end

      def parse_query_parameters(query_string)
        return {} if query_string.nil?

        pairs = query_string.split('&').collect do |chunk|
          next if !chunk || chunk.empty?
          key, value = chunk.split('=', 2)
          next if !key || key.empty?
          value = value.nil? ? nil : form_decode(value)
          [ form_decode(key), value ]
        end.compact

        UrlEncodedPairParser.new(pairs).result
      end
		
      def query_params(req)
        params = {}
        unless req['id'].nil?
          params['id'] = req['id']
        end
        unless req['request-query'].nil? or req['request-query'].length == 0
          params.merge!(parse_query_parameters(req['request-query']))				
        end
        unless req['headers'].nil? or req['headers']['Content-Type'].nil?
          if 'application/x-www-form-urlencoded'.eql? req['headers']['Content-Type']
            params.merge!(parse_query_parameters(req['request-body']))
          end	
        end
        puts "Params: " + params.to_s unless params.empty?
        params
      end
    end
	
    class UrlEncodedPairParser < StringScanner #:nodoc:
      attr_reader :top, :parent, :result

      def initialize(pairs = [])
        super('')
        @result = {}
        pairs.each { |key, value| parse(key, value) }
      end

      KEY_REGEXP = %r{([^\[\]=&]+)}
      BRACKETED_KEY_REGEXP = %r{\[([^\[\]=&]+)\]}

      # Parse the query string
      def parse(key, value)
        self.string = key
        @top, @parent = result, nil

        # First scan the bare key
        key = scan(KEY_REGEXP) or return
        key = post_key_check(key)

        # Then scan as many nestings as present
        until eos?
          r = scan(BRACKETED_KEY_REGEXP) or return
          key = self[1]
          key = post_key_check(key)
        end

        bind(key, value)
      end

      private
      # After we see a key, we must look ahead to determine our next action. Cases:
      #
      #   [] follows the key. Then the value must be an array.
      #   = follows the key. (A value comes next)
      #   & or the end of string follows the key. Then the key is a flag.
      #   otherwise, a hash follows the key.
      def post_key_check(key)
        if scan(/\[\]/) # a[b][] indicates that b is an array
          container(key, Array)
          nil
        elsif check(/\[[^\]]/) # a[b] indicates that a is a hash
          container(key, Hash)
          nil
        else # End of key? We do nothing.
          key
        end
      end

      # Add a container to the stack.
      def container(key, klass)
        type_conflict! klass, top[key] if top.is_a?(Hash) && top.key?(key) && ! top[key].is_a?(klass)
        value = bind(key, klass.new)
        type_conflict! klass, value unless value.is_a?(klass)
        push(value)
      end

      # Push a value onto the 'stack', which is actually only the top 2 items.
      def push(value)
        @parent, @top = @top, value
      end

      # Bind a key (which may be nil for items in an array) to the provided value.
      def bind(key, value)
        if top.is_a? Array
          if key
            if top[-1].is_a?(Hash) && ! top[-1].key?(key)
              top[-1][key] = value
            else
              top << {key => value}.with_indifferent_access
              push top.last
              value = top[key]
            end
          else
            top << value
          end
        elsif top.is_a? Hash
          #key = CGI.unescape(key)
          parent << (@top = {}) if top.key?(key) && parent.is_a?(Array)
          top[key] ||= value
          return top[key]
        else
          raise ArgumentError, "Don't know what to do: top is #{top.inspect}"
        end

        return value
      end

      def type_conflict!(klass, value)
        raise TypeError, "Conflicting types for parameter containers. Expected an instance of #{klass} but found an instance of #{value.class}. This can be caused by colliding Array and Hash parameters like qs[]=value&qs[key]=value. (The parameters received were #{value.inspect}.)"
      end
    end
  end # RhoSupport
end # Rho