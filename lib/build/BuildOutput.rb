path = File.dirname(__FILE__)

files = [
  "ExtendedString",
];

files.each do |f|
  require File.join(path, f)
end

class BuildOutput
  NOTE = 0
  WARNING = 1
  ERROR = 2
  FATAL = 3

  MESSAGES = ['NOTE', 'WARNING', 'ERROR', 'FATAL']
  LEVELS = [NOTE, WARNING, ERROR, FATAL]

  @@mqueue = []

  def initialize()
  end

  class << self
    def prepare_message(message)
      if message.kind_of?(Array)
        out = []
        message.flatten(1).each do |line|
          out.concat( line.split($/) )
        end
      else
        out = message.split($/) 
      end

      out
    end

    def decorate_message( message, title )
      message_content = []

      message_content << ' **** ' + title + '*' * (105 - title.length)

      message_content.concat( prepare_message(message).map{|el| ' ' * 3 + el })

      message_content << ' ' + '*' * (110)

      message_content
    end

    def col_message(level, message)
      out = case level
      when FATAL
        message.bold.red
      when ERROR
        message.red
      when WARNING
        message.cyan
      when NOTE
        message.blue
      end

      out
    end

    def to_merge(s1, s2)
      i = 0
      loop do
        e1 = s1.next rescue :eof
        e2 = s2.next rescue :eof
        if e1 == :eof || e2 == :eof || e1 != e2
          break
        else
          i += 1 
        end
      end
      return i
    end

    def merge_head_tail(src, msg)  
      begining = []
      ending = []
      result = []

      # find same headers
      b_len = to_merge(src.each, msg.each)
      if (b_len > 0)
        begining.concat(src.take(b_len))
        src = src.drop(b_len)
        msg = msg.drop(b_len)
      end

      # find same footers
      e_len = to_merge(src.reverse_each, msg.reverse_each)
      if (e_len > 0)
        ending = src.drop(src.length - e_len)
        src = src.take(src.length - e_len)
        msg = msg.take(msg.length - e_len)
      end

      # special case about joining lines

      if src.length == 1 && msg.length == 1
        if msg.first.length > src.first.length && msg.first.start_with?(src.first)
          # warning(["Build message '#{msg} appended from ", " * " + caller.join($/ + " ^ "),'instead of being added to error string'], 'Error in build script')
          src = []
        end
      end

      begining.concat(src).concat(msg).concat(ending)
    end

    def log(level, message, title)
      if !LEVELS.include?(level)
        return
      end

      title = MESSAGES[level] + (title.nil? ? " " : (": " + title + " "))

      message = {
        :level => level,
        :title => title,
        :content => prepare_message(message)
      }

      last = @@mqueue.last

      if !last.nil? && (last[:level] == level) && (last[:title] == title)
        last[:content] =  merge_head_tail(last[:content], message[:content])
      else
        @@mqueue << message
      end
    end

    def put_log(level, message, title = nil)
      if !LEVELS.include?(level)
        return
      end

      msg = decorate_message(message, MESSAGES[level] + (title.nil? ? " " : (": " + title + " ")))
      col = col_message(level, msg.join($/))

      puts col
    end

    def note(message, title = nil)
      log(NOTE, message, title)
    end

    def warning(message, title = nil)
      log(WARNING, message, title)
    end

    def error(message, title = nil)
      log(ERROR, message, title)
    end

    def fatal(message, title = nil)
      log(ERROR, message, title)
    end

    def getLogText
      txt = []
      @@mqueue.sort_by { |hsh| hsh[:level] }
      @@mqueue.each do |message|
        out = decorate_message( message[:content], message[:title] ).join($/)
        col = col_message(message[:level], out)
        message[:output] = out
        message[:color] = col
        txt << col
      end
      txt << '' unless txt.empty?
      txt.join($/)
    end
  end

end
