# lang_no.rb
# Norwegian translation file
# Translation by Fredrik Bach ( http://www.nidi.no )

module LocalizationSimplified
  About = {
    :lang => "no",
    :updated => "2007-02-12"
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
      :inclusion           => "finnes ikke i listen",
      :exclusion           => "er reservert",
      :invalid             => "er ikke gyldig",
      :confirmation        => "stemmer ikke",
      :accepted            => "må være acceptert",
      :empty               => "kan ikke være tom",
      :blank               => "kan ikke være tom",# alternate formulation: "is required"
      :too_long            => "er for lang (maximum er %d tegn)",
      :too_short           => "er for kort (minimum er %d tegn)",
      :wrong_length        => "har feil lengde (skal være %d tegn)",
      :taken               => "er allerede brukt",
      :not_a_number        => "er ikke et nummer",
      #Jespers additions:
      :error_translation   => "feil",
      :error_header        => "%s hindret %s fra å bli lagret",
      :error_subheader     => "Følgende problemer ble funnet:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "mindre en %d sekunder",
      :half_a_minute       => "et halvt minutt",
      :less_than_a_minute  => "mindre en et minutt",
      :one_minute          => "1 minutt",
      :x_minutes           => "%d minutter",
      :one_hour            => "cirka 1 timma",
      :x_hours             => "cirka %d timmar",
      :one_day             => "1 døgn",
      :x_days              => "%d døgn",
      :one_month           => "1 måned",
      :x_months            => "%d måneder",
      :one_year            => "1 år",
      :x_years             => "%d år"
    }
    
    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{januar februar mars april mai juni juli august september oktober november desember}
    AbbrMonthnames = [nil] + %w{jan feb mar apr mai jun jul aug sep okt nov des}
    Daynames       = %w{søndag mandag tirsdag onsdag torsdag fredag lørdag}
    AbbrDaynames   = %w{søn man tir ons tors fre lør}

    
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
      :order    => [:day, :month, :year] #default Rails is US ordered: :order => [:year, :month, :day]
    }
  end

  class NumberHelper
    # CurrencyOptions are used as default for +Number#to_currency()+
    # http://api.rubyonrails.org/classes/ActionView/Helpers/NumberHelper.html#M000449
    CurrencyOptions = {
      :unit      => "",
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => " ",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number, :unit] #order is at present unsupported in Rails
      #to support for instance Swedish format, the order is different: Unit comes last (ex. "1.234,00 kr.")
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
   inflect.uncountable %w( feil )
 end 