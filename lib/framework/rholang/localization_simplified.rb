# LocalizationSimplified (L10n-simplified)
# Really simple localization for Rails
# By Jesper Rønn-Jensen ( http://justaddwater.dk/ )
# Plugin available at http://rubyforge.org/projects/l10n-simplified/
#
module LocalizationSimplified
  @@ignore = "\xFF\xFF\xFF\xFF" # %% == Literal "%" character
  # substitute all daynames and monthnames with localized names
  # from RUtils plugin
  def self.localize_strftime(date='%d.%m.%Y', time='')
    date.gsub!(/%%/, @@ignore)
    date.gsub!(/%a/, LocalizationSimplified::DateHelper::AbbrDaynames[time.wday])
    date.gsub!(/%A/, LocalizationSimplified::DateHelper::Daynames[time.wday])
    date.gsub!(/%b/, LocalizationSimplified::DateHelper::AbbrMonthnames[time.mon])
    date.gsub!(/%B/, LocalizationSimplified::DateHelper::Monthnames[time.mon])
    date.gsub!(@@ignore, '%%')
  end
end

# Modification of ruby constants
class Date
  #FIXME as these are defined as Ruby constants, they can not be overwritten
  MONTHNAMES         = LocalizationSimplified::DateHelper::Monthnames
  ABBR_MONTHNAMES    = LocalizationSimplified::DateHelper::AbbrMonthnames
  DAYNAMES          = LocalizationSimplified::DateHelper::Daynames        #not in use by Rails
  ABBR_DAYNAMES     = LocalizationSimplified::DateHelper::AbbrDaynames    #not in use by Rails
  
  DATE_FORMATS	=	{ :short => "%e %b", :long => "%B %e, %Y", :db => "%Y-%m-%d", :number => "%Y%m%d" }
  #, :long_ordinal => lambda { |date| date.strftime("%B #{date.day.ordinalize}, %Y") }, # => "April 25th, 2007" :rfc822 => "%e %b %Y"
  DATE_FORMATS.merge!(LocalizationSimplified::DateHelper::DateFormats)

  def to_formatted_s(type)
    strftime(DATE_FORMATS[type])
  end
end

# Modification of default Time format using Time.to_formatted_s(:default)
# Localizes the hash with the formats  :default, :short, :long
# Usage:
# <% t = Time.parse('2006-12-25 13:55') %>
# <%= t.to_formatted_s(:short) #=> outputs time in localized format %>
# <%= t                        #=> outputs time in localized format %>
class Time
  alias_method :old_strftime, :strftime
  # Pre-translate format of Time before the time string is translated by strftime.
  # The <tt>:default</tt> time format is changed by localizing month and daynames.
  # Also Rails ActiveSupport allow us to modify the <tt>:default</tt> timeformatting string.
  # Originally, its <tt>:default  => "%a, %d %b %Y %H:%M:%S %z"</tt> (RFC2822 names), but as it can be
  # modified in this plugin, and we can end up with a different file format in logfiles, etc
  def strftime(date)
    tmpdate=date.dup
    LocalizationSimplified::localize_strftime(tmpdate, self)
    old_strftime(tmpdate)
  end

  DATE_FORMATS	=	{ :db => "%Y-%m-%d %H:%M:%S", :number => "%Y%m%d%H%M%S", :time => "%H:%M", :short => "%d %b %H:%M", :long => "%B %d, %Y %H:%M"  }
  #, :long_ordinal => lambda { |time| time.strftime("%B #{time.day.ordinalize}, %Y %H:%M") }, :rfc822 => lambda { |time| time.strftime("%a, %d %b %Y %H:%M:%S #{time.formatted_offset(false)}") }  
  DATE_FORMATS.merge!(LocalizationSimplified::DateHelper::TimeFormats)  
  
  def to_formatted_s(type)
    strftime(DATE_FORMATS[type])
  end
end

