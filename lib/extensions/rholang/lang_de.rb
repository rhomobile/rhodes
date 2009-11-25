# lang_de.rb
# German translation file
# Translation by Benedikt Huber
# Additions by Matthias Tarasiewicz - parasew (at) gmail

module LocalizationSimplified
  About = {
    :lang => "de",
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
      :inclusion           => "ist nicht in der Liste gültiger Optionen enthalten",
      :exclusion           => "ist reserviert",
      :invalid             => "ist ungültig",
      :confirmation        => "entspricht nicht der Bestätigung",
      :accepted            => "muss akzeptiert werden",
      :empty               => "darf nicht leer sein",
      :blank               => "wird benötigt",# alternate formulation: "is required"
      :too_long            => "ist zu lang (höchstens %d Zeichen)",
      :too_short           => "ist zu kurz (mindestens %d Zeichen)",
      :wrong_length        => "hat eine falsche Länge (sollte %d Zeichen sein)",
      :taken               => "ist schon vergeben",
      :not_a_number        => "ist keine Zahl",
      #Jespers additions:
      :error_translation   => "Fehler",
      :error_header        => "%s hinderte %s daran, gespeichert zu werden",
      :error_subheader     => "Es gab folgende Probleme: "
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "weniger als %d Sekunden",
      :half_a_minute       => "eine halbe Minute",
      :less_than_a_minute  => "weniger als eine Minute",
      :one_minute          => "1 Minute",
      :x_minutes           => "%d Minuten",
      :one_hour            => "ungefähr 1 Stunde",
      :x_hours             => "ungefähr %d Stunden",
      :one_day             => "1 Tag",
      :x_days              => "%d Tage",
      :one_month           => "1 Monat",
      :x_months            => "%d Monate",
      :one_year            => "1 Jahr",
      :x_years             => "%d Jahre"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Januar Februar März April Mai Juni Juli August September Oktober November Dezember}
    AbbrMonthnames = [nil] + %w{Jan Feb Mrz Apr Mai Jun Jul Aug Sep Okt Nov Dez}
    Daynames       = %w{Sonntag Montag Dienstag Mittwoch Donnerstag Freitag Samstag}
    AbbrDaynames   = %w{So Mo Di Mi Do Fr Sa}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%Y-%m-%d",
      :short    => "%d. %b.",
      :long     => "%d. %B %Y"
    }

    TimeFormats = {
      :default  => "%A, %d. %B %Y %H:%M %Z",
      :short    => "%d. %b. %H:%M",
      :long     => "%d. %B %Y, %H:%M"
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
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => ".",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'und',
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
 Inflector.inflections do |inflect|
   inflect.uncountable %w( Fehler )
 end
