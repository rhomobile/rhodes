class String

  # Returns true if string contains numeric value
  def numeric?
    Float(self) != nil rescue false
  end

  # Returns true if string contains integer value
  def int?
    Integer(self) != nil rescue false
  end

  def capitalize_first
    str = to_s
    empty? ? str : str[0..0].upcase << str[1..-1]
  end

  # Converts a string to camelcase. This method leaves the first character
  # as given. This allows other methods to be used first, such as #uppercase
  # and #lowercase.
  def camel_case(*separators)
    case separators.first
      when Symbol, TrueClass, FalseClass, NilClass
        first_letter = separators.shift
    end

    separators = ['_', '\s'] if separators.empty?

    str = self.dup

    separators.each do |s|
      str = str.gsub(/(?:#{s}+)([a-z])/){ $1.upcase }
    end

    case first_letter
      when :upper, true
        str = str.gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
      when :lower, false
        str = str.gsub(/(\A|\s)([A-Z])/){ $1 + $2.downcase }
    end

    str
  end

  # Converts a string to module name representation.
  #
  # This is essentially #camelcase, but it also converts
  # '/' to '::' which is useful for converting paths to
  # namespaces.
  def modulize
    #gsub('__','/'). # why was this ever here?
    gsub(/__(.?)/){ "::#{$1.upcase}" }.
        gsub(/\/(.?)/){ "::#{$1.upcase}" }.
        gsub(/(?:_+|-+)([a-z])/){ $1.upcase }.
        gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
  end

  def humanize
    gsub(/_/, " ").capitalize
  end

  # Underscore a string such that camelcase, dashes and spaces are
  # replaced by underscores. This is the reverse of {#camelcase},
  # albeit not an exact inverse.
  def underscore
    #gsub(/::/, '/').
    gsub(/([A-Z]+)([A-Z][a-z])/,'\1_\2').
        gsub(/([a-z\d])([A-Z])/,'\1_\2').
        tr('-', '_').
        gsub(/\s/, '_').
        gsub(/__+/, '_').
        downcase
  end

  def titleize
    underscore.humanize.gsub(/\b('?[a-z])/) { $1.capitalize }
  end

  def hard_wrap(width = 80)
    gsub(/(.{1,#{width}})(\s+|$)/, "\\1\n").strip
  end

  # from natcmp gem
  # Natural comaration based on numbers in strings
  def self.natcmp(str1, str2, ignoreCase=true)
    strArrays = [str1, str2].collect do |str|
      str = str.downcase if ignoreCase
      str.tr(" \t\r\n", '').split(/(\d+)/)
    end

    minSize = strArrays.min_by { |arr| arr.size }.size

    1.step(minSize-1, 2) do |i|
      unless strArrays.any? { |arr| arr[i] =~ /^0/ }
        strArrays.each { |arr| arr[i] = arr[i].to_i }
      end
    end

    strArrays[0] <=> strArrays[1]
  end

  def primary
    self.blue
  end

  def success
    self.green
  end

  def info
    self.cyan
  end

  def warning
    self.magenta
  end

  def alarm
    self.red
  end

  def black; self end
  def red; self end
  def green; self end
  def brown; self end
  def blue; self end
  def magenta; self end
  def cyan; self end
  def gray; self end
  def bg_black; self end
  def bg_red; self end
  def bg_green; self end
  def bg_brown; self end
  def bg_blue; self end
  def bg_magenta; self end
  def bg_cyan; self end
  def bg_gray; self end
  def bold; self end
  def underline; self end
  def reverse_color; self end

  # table of helper functions
  {  
    :black           => ["\033[30m","\033[0m"],
    :red             => ["\033[31m","\033[0m"],
    :green           => ["\033[32m","\033[0m"],
    :brown           => ["\033[33m","\033[0m"],
    :blue            => ["\033[34m","\033[0m"],
    :magenta         => ["\033[35m","\033[0m"],
    :cyan            => ["\033[36m","\033[0m"],
    :gray            => ["\033[37m","\033[0m"],
    :bg_black        => ["\033[40m","\0330m"],
    :bg_red          => ["\033[41m","\033[0m"],
    :bg_green        => ["\033[42m","\033[0m"],
    :bg_brown        => ["\033[43m","\033[0m"],
    :bg_blue         => ["\033[44m","\033[0m"],
    :bg_magenta      => ["\033[45m","\033[0m"],
    :bg_cyan         => ["\033[46m","\033[0m"],
    :bg_gray         => ["\033[47m","\033[0m"],
    :bold            => ["\033[1m","\033[22m"],
    :underline       => ["\033[4m","\033[24m"],
    :reverse_color   => ["\033[7m","\033[27m"]
  }.each do |name, format|
    # use colorizer only for TTY for non windows systems
    is_win = (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
    if STDOUT.tty? && !is_win
      send :define_method, name do
        [format[0], self, format[1]].join
      end
    end
  end
end
