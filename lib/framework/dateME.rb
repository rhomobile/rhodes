require 'date/format'

class Time

  def __makeTwoDigit(num)
    str = num.to_s
    if str.length < 2
        str = '0' + str
    end
    
    str    
  end
  
  def strftime(fmt='%F')

    if fmt == "%m/%d/%Y, %I:%M%p"
        strRes = "" 
        strPM = 'AM'
        nHour = hour();
        if nHour >= 12
            strPM = 'PM'
            nHour = nHour-12 if nHour > 12
        end
        
        strRes += __makeTwoDigit(mon()) + '/' + __makeTwoDigit(mday()) + '/' + year().to_s + ', ' + __makeTwoDigit(nHour) + ':' + __makeTwoDigit(min()) + strPM;
        return strRes 
    end
    
    DateTimeME.new(self.localtime).strftime(fmt)
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
