class String

  def capitalize_first
    self.slice(0, 1).capitalize + self.slice(1..-1)
  end

  def underscore
    self.gsub(/::/, '/').
        gsub(/([A-Z]+)([A-Z][a-z])/, '\1_\2').
        gsub(/([a-z\d])([A-Z])/, '\1_\2').
        tr('- ', '_').
        downcase
  end

  # from natcmp gem
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
    else
      send :define_method, name do
        self
      end
    end
  end
end
