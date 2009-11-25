# lang_da.rb
# Danish translation file
# Translation by Jesper Rønn-Jensen ( http://justaddwater.dk/ ) 


module LocalizationSimplified
  About = {
    :lang => "da",
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
      :inclusion           => "er ikke med på listen",
      :exclusion           => "er et reserveret ord",
      :invalid             => "er ugyldig",
      :confirmation        => "matcher ikke med bekræftelsen",
      :accepted            => "skal accepteres",
      :empty               => "kan ikke være tom",
      :blank               => "skal udfyldes",# alternate formulation: "is required"
      :too_long            => "er for langt (max er %d tegn)",
      :too_short           => "er for kort (minimum er %d tegn)",
      :wrong_length        => "har forkert længde (skal være %d tegn)",
      :taken               => "er allerede taget",
      :not_a_number        => "er ikke et tal",
      #Jespers additions:
      :error_translation   => "fejl",
      :error_header        => "%s forhindrede %s i at blive gemt",
      :error_subheader     => "Problemer med følgende felter:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "under %d sekunder",
      :half_a_minute       => "et halvt minut",
      :less_than_a_minute  => "under et minut",
      :one_minute          => "1 minut",
      :x_minutes           => "%d minutter",
      :one_hour            => "omkring en time",
      :x_hours             => "omkring %d timer",
      :one_day             => "1 dag",
      :x_days              => "%d dage",
      :one_month           => "1 måned",
      :x_months            => "%d måneder",
      :one_year            => "1 år",
      :x_years             => "%d år"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{januar februar marts april maj juni juli august september oktober november december}
    AbbrMonthnames = [nil] + %w{jan feb mar apr maj jun jul aug sep okt nov dec}
    Daynames       = %w{søndag mandag tirsdag onsdag torsdag fredag lørdag}
    AbbrDaynames   = %w{søn man tir ons tors fre lør}

    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%Y-%m-%d",
      :short    => "%e. %b",
      :long     => "%e. %B, %Y"
    }

    TimeFormats = {
      :default  => "%A d. %d %B %Y %H:%M", #no timezone
      :short    => "%d. %b %H:%M",
      :long     => "%d. %B %Y %H:%M"
    }
    # Set the order of +date_select+ and +datetime_select+ boxes
    # Note that at present, the current Rails version only supports ordering of date_select boxes
    DateSelectOrder = {
      :order    => [:day, :month, :year] #default Rails is US ordered: :order => [:year, :month, :day]
    }
  end

  class NumberHelper
    # CurrencyOptions are used as default for +Number#to_currency()+
    # http://api.rubyonrails.org/classes/ActionView/Helpers/NumberHelper.html#M000449
    CurrencyOptions = {
      :unit      => "kr.",
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => ".",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number, :unit] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'og', 
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
 Inflector.inflections do |inflect|
   inflect.uncountable %w( fejl )
 end
