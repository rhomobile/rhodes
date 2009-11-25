# lang_en.rb
# English baseline translation file. Comes in handy for testing purposes



module LocalizationSimplified
  About = {
    :lang => "en",
    :updated => "2007-08-02"
  }

  class ActiveRecord
    # ErrorMessages to override default messages in 
    # +ActiveRecord::Errors::@@default_error_messages+
    # This plugin also replaces hardcoded 3 text messages 
    # :error_translation is inflected using the Rails 
    # inflector. 
    #
    # Remember to modify the Inflector with your localized translation
    # of "error" and "errors" in the bottom of this file
    # 
    ErrorMessages = {
      :inclusion           => "is not included in the list",
      :exclusion           => "is reserved",
      :invalid             => "is invalid",
      :confirmation        => "doesn't match confirmation",
      :accepted            => "must be accepted",
      :empty               => "can't be empty",
      :blank               => "can't be blank",# alternate formulation: "is required"
      :too_long            => "is too long (maximum is %d characters)",
      :too_short           => "is too short (minimum is %d characters)",
      :wrong_length        => "is the wrong length (should be %d characters)",
      :taken               => "has already been taken",
      :not_a_number        => "is not a number",
      #Jespers additions:
      :error_translation   => "error",
      :error_header        => "%s prohibited this %s from being saved",
      :error_subheader     => "There were problems with the following fields:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "less than %d seconds",
      :half_a_minute       => "half a minute",
      :less_than_a_minute  => "less than a minute",
      :one_minute          => "1 minute",
      :x_minutes           => "%d minutes",
      :one_hour            => "about 1 hour",
      :x_hours             => "about %d hours",
      :one_day             => "1 day",
      :x_days              => "%d days",
      :one_month           => "1 month",
      :x_months            => "%d months",
      :one_year            => "1 year",
      :x_years             => "%d years"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{January February March April May June July August September October November December}
    AbbrMonthnames = [nil] + %w{Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec}
    Daynames       = %w{Sunday Monday Tuesday Wednesday Thursday Friday Saturday}
    AbbrDaynames   = %w{Sun Mon Tue Wed Thu Fri Sat}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%Y-%m-%d",
      :short    => "%b %e",
      :long     => "%B %e, %Y"
    }

    TimeFormats = {
      :default  => "%a, %d %b %Y %H:%M:%S %z",
      :short    => "%d %b %H:%M",
      :long     => "%B %d, %Y %H:%M"
    }
    # Set the order of +date_select+ and +datetime_select+ boxes
    # Note that at present, the current Rails version only supports ordering of date_select boxes
    DateSelectOrder = {
      :order    => [:year, :month, :day] #default Rails is US ordered: :order => [:year, :month, :day]
    }
  end

  class NumberHelper
    # CurrencyOptions are used as default for +Number#to_currency()+
    # http://api.rubyonrails.org/classes/ActionView/Helpers/NumberHelper.html#M000449
    CurrencyOptions = {
      :unit      => "$",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'and',
      :skip_last_comma => false
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
# Inflector.inflections do |inflect|
#   inflect.plural /^(ox)$/i, '\1en'
#   inflect.singular /^(ox)en/i, '\1'
#   inflect.irregular 'person people'
#   inflect.uncountable %w( information )
# end
