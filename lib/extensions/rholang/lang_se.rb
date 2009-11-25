# lang_se.rb
# Swedish translation file. 
# Translation from plugin swe_rails by Ola Bini ( http://ola-bini.blogspot.com/ ) and Olle Jonsson ( http://olleolleolle.dk )


module LocalizationSimplified
  About = {
    :lang => "se",
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
      :inclusion           => "finns inte i listan",
      :exclusion           => "Är reserverat",
      :invalid             => "Är ogiltigt",
      :confirmation        => "stämmer inte övererens",
      :accepted            => "måste vara accepterad",
      :empty               => "får ej vara tom",
      :blank               => "får ej vara blank",# alternate formulation: "is required"
      :too_long            => "Är för lång (maximum är %d tecken)",
      :too_short           => "Är för kort (minimum är %d tecken)",
      :wrong_length        => "har fel längd (ska vara %d tecken)",
      :taken               => "har redan tagits",
      :not_a_number        => "Är ej ett nummer",
      #Jespers additions:
      :error_translation   => "fel",
      :error_header        => "%s förhindrade %s från at sparse",
      :error_subheader     => "Problem med dessa fält:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "mindre än %d sekunder",
      :half_a_minute       => "en halv minut",
      :less_than_a_minute  => "mindre än en minut",
      :one_minute          => "1 minut",
      :x_minutes           => "%d minutter",
      :one_hour            => "ungefär 1 timma",
      :x_hours             => "ungefär %d timmar",
      :one_day             => "1 dygn",
      :x_days              => "%d dygn",
      :one_month           => "1 month",
      :x_months            => "%d months",
      :one_year            => "1 year",
      :x_years             => "%d years"
    }
    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{januari februari mars april maj juni juli augusti september oktober november december}
    AbbrMonthnames = [nil] + %w{jan feb mar apr maj jun jul aug sep okt nov dec}
    Daynames       = %w{söndag måndag tisdag onsdag torsdag fredag lördag}
    AbbrDaynames   = %w{sön mån tis ons tors fre lör}

    
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
      :unit      => "kr.",
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => ".",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number, :unit] #order is at present unsupported in Rails
      #to support for instance Swedish format, the order is different: Unit comes last (ex. "1.234,00 kr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'och', 
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
 Inflector.inflections do |inflect|
   inflect.uncountable %w( fel )
 end
