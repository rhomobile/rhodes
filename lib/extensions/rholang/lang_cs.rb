# lang_cs.rb
# Czech translation file
# Translation by Karel Minařík (karmi@karmi.cz) 

module LocalizationSimplified
  About = {
    :lang => "cs",
    :updated => "2007-07-16"
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
      :inclusion           => "není v seznamu povolených hodnot",
      :exclusion           => "je vyhrazeno pro jiný účel",
      :invalid             => "není platná hodnota",
      :confirmation        => "nebylo potvrzeno",
      :accepted            => "musí být potvrzeno",
      :empty               => "nesmí být prázdný/é",
      :blank               => "je povinná položka",# alternate formulation: "is required"
      :too_long            => "je příliš dlouhá/ý (max. %d znaků)",
      :too_short           => "je příliš krátký/á (min. %d znaků)",
      :wrong_length        => "nemá správnou délku (očekáváno %d znaků)",
      :taken               => "již databáze obsahuje",
      :not_a_number        => "není číslo",
      #Jespers additions:
      :error_translation   => "chyba",
      :error_header        => "%s nebylo možné %s uložit", #FIX
      :error_subheader     => "Tato pole obsahují chyby:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "asi %d sekundami",
      :half_a_minute       => "asi před půl minutou",
      :less_than_a_minute  => "necelou minutou",
      :one_minute          => "minutou",
      :x_minutes           => "%d minutami",
      :one_hour            => "hodinou",
      :x_hours             => "%d hodinami",
      :one_day             => "24 hodinami", # Nejde dát 'včera' -- srov. "před 3 minutami" x "před včera/jedním dnem" (angl. "one day ago")
      :x_days              => "%d dny",
      :one_month           => "měsícem",
      :x_months            => "%d měsíci", # Nevyřešeno pro budoucnost -- srov. "před 2 měsíci" x "za 2 měsíce" (ang. "2 months ago" - "in 2 months")
      :one_year            => "rokem",
      :x_years             => "%d roky"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Leden Únor Březen Duben Květen Červen Červenec Srpen Září Říjen Listopad Prosinec}
    AbbrMonthnames = [nil] + %w{Led Úno Bře Dub Kvě Čvn Čvc Srp Zář Říj Lis Pro}
    Daynames       = %w{Neděle Pondělí Úterý Středa Čtvrtek Pátek Sobota}
    AbbrDaynames   = %w{Ne Po Út St Čt Pá So}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%d. %m. %Y",
      :short    => "%e. %b.",
      :long     => "%e. %B %Y"
    }

    TimeFormats = {
      :default  => "%d. %m. %Y, %H:%M:",
      :short    => "%e. %m. %H:%M",
      :long     => "%e. %B %Y, %H:%M"
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
      :unit      => "Kč",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => " ",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'a',
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
Inflector.inflections do |inflect|
  inflect.irregular 'chyba', 'chyby'
end