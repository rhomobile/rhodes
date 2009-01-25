require 'rationalME'

class Time

  def to_time() getlocal end

  def to_datetime
    DateTime.new(self)
  end

  def strftime(fmt='%F')
    to_datetime().strftime(fmt);
  end
end

class Date
  # Full month names, in English.  Months count from 1 to 12; a
  # month's numerical representation indexed into this array
  # gives the name of that month (hence the first element is nil).
  MONTHNAMES = [nil] + %w(January February March April May June July
			  August September October November December)

end

class DateTime < Date

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
