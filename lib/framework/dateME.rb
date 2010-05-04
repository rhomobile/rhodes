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
    
    DateTime.new(self.localtime).strftime(fmt)
  end

end

class Date

  def self.today
    Date.new( Time.now )
  end

  def self.strptime(str='-4712-01-01', fmt='%F')
    Date.new( Time.strptime(str,fmt) )
  end

  def to_s() format('%.4d-%02d-%02d', year(), mon(), mday()) end # 4p
  
  def self.civil(y=-4712, m=1, d=1, sg=2299161)
    Date.new( Time.new(y,m,d) )
  end
  
  def _get_date
    @m_date      
  end
  
  def - (x)
    ((@m_date-x._get_date()).to_i)/(60*60*24)
  end

  def wday() @m_date.wday end
  def mon() @m_date.mon end
  def year() @m_date.year end
  def mday() @m_date.mday end
  def offset() 
    of = Rational(@m_date.gmt_offset() || 0, 86400)
  end
  def hour() @m_date.hour end
  def min() @m_date.min end
  def sec() @m_date.sec end
  
  def initialize( time )
    @m_date = time
  end
  
  def self.now()
    Date.new( Time.now )  
  end

  private_class_method :now
  
end

class DateTime < Date

  def wday() @m_time.wday end
  def mon() @m_time.mon end
  def year() @m_time.year end
  def mday() @m_time.mday end
  def hour() @m_time.hour end
  def min() @m_time.min end
  def sec() @m_time.sec end
  def offset() 
    of = Rational(@m_time.gmt_offset() || 0, 86400)
  end
  
  def initialize( time )
    @m_time = time
  end

  def to_datetime() self end

  private_class_method :today
  public_class_method  :now  
end
