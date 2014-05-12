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
    def decorate_message( message, title )
      message_content = []

      message_content << '  **** ' + title +'*'*(105-title.length)

      if message.kind_of?(Array)
        message_content.concat( message.map{|el| ' '*4 + el })
      else
        message_content << ' '*4 + message
      end

      message_content << '  *'*(110)

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

    def log(level, message, title)
      if !LEVELS.include?(level)
        return
      end

      message = {
        :level => level,
        :content => decorate_message(message, MESSAGES[level] + (title.nil? ? " " : (": " + title + " ")))
      }

      @@mqueue << message
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
        out = message[:content].join($/)
        col = col_message(message[:level], out)
        message[:output] = out
        message[:color] = col
        txt << col
      end

      txt.join($/)
    end
  end

end