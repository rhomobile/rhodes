require 'date/format'

class Time

  def strftime(fmt='%F')
    DateTimeME.new(self).strftime(fmt);
  end
end

class DateTimeME < Date

  def wday() @m_time.wday end
  def mon() @m_time.mon end
  def year() @m_time.year end
  def mday() @m_time.mday end
  def hour() @m_time.hour end
  def min() @m_time.min end
  def sec() @m_time.sec end
  
  def initialize( time )
    @m_time = time
  end
  
end
