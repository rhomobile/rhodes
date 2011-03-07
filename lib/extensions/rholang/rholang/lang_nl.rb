# lang_nl.rb
# Dutch translation by Jeroen Houben



module LocalizationSimplified
  About = {
    :lang => "nl",
    :updated => "2006-08-23"
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
      :inclusion           => "zit niet in de lijst",
      :exclusion           => "is gereserveerd",
      :invalid             => "is ongeldig",
      :confirmation        => "is niet hetzelfde als de verificatie",
      :accepted            => "moet worden geaccepteerd",
      :empty               => "mag niet leeg zijn",
      :blank               => "mag niet blanko zijn",# alternate formulation: "is required"
      :too_long            => "is te lang (maximum is %d karakters)",
      :too_short           => "is te kort (minimum is %d karakters)",
      :wrong_length        => "is de verkeerde lengte (dient %d karakters te zijn)",
      :taken               => "is reeds in gebruik",
      :not_a_number        => "is geen nummer",
      #Jespers additions:
      :error_translation   => "fout",
      :error_header        => "%s zorgen ervoor dat %s niet kan worden opgeslagen",
      :error_subheader     => "Er zijn problemen met de volgende velden:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "minder dan %d seconden",
      :half_a_minute       => "een halve minuut",
      :less_than_a_minute  => "minder dan een halve minuut",
      :one_minute          => "1 minuut",
      :x_minutes           => "%d minuten",
      :one_hour            => "ongeveer 1 uur",
      :x_hours             => "ongeveer %d uur",
      :one_day             => "1 dag",
      :x_days              => "%d dagen",
      :one_month           => "1 maand",
      :x_months            => "%d maanden",
      :one_year            => "1 jaar",
      :x_years             => "%d jaar"
    }
    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Januari Februari Maart April Mei Juni Juli Augustus September Oktober November December}
    AbbrMonthnames = [nil] + %w{Jan Feb Maa Apr Mei Jun Jul Aug Sep Okt Nov Dec}
    Daynames       = %w{Zondag Maandag Dinsdag Woensdag Donderdag Vrijdag Zaterdag}
    AbbrDaynames   = %w{Zo Ma Di Wo Do Vr Za}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%d-%m-%Y",
      :short    => "%d %b",
      :long     => "%d %B %Y"
    }

    TimeFormats = {
      :default  => "%a, %d %b %Y %H:%M:%S %z",
      :short    => "%d %b %H:%M",
      :long     => "%d %B %Y %H:%M"
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
      :unit      => "€",
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
      :connector => 'en',
      :skip_last_comma => false
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
Inflector.inflections do |inflect|
   inflect.irregular 'fout', 'fouten'
end
