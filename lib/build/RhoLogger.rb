class Logger
  alias :original_add :add

  def add(severity, message = nil, progname = nil)
    if (self.level == Logger::DEBUG)
      begin
        #try to get rake task name
        taskName = Rake.application.current_task
      rescue Exception => e
      end

      if message
        message = "#{taskName}|\t#{message}"
      else
        progname = "#{taskName}|\t#{progname}"
      end
    end
    original_add( severity, message, progname )
  end
end

def puts( s )
  if $logger
    $logger.info( s )
  else
    Kernel::puts( s )
  end
end
