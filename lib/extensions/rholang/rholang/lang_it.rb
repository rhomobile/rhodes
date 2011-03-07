# Traduzione italiana.
# Translation by Michele Franzin ( http://www.seesaw.it/ )



module LocalizationSimplified
  About = {
    :lang => "it",
    :updated => "2007-08-10"
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
      :inclusion           => "non è incluso nella lista",
      :exclusion           => "è riservato",
      :invalid             => "non è valido",
      :confirmation        => "non corrisponde alla conferma",
      :accepted            => "deve essere accettato",
      :empty               => "non può essere vuoto",
      :blank               => "è richiesto",# alternate formulation: "is required"
      :too_long            => "è troppo lungo (massimo %d caratteri)",
      :too_short           => "è troppo corto (minimo %d caratteri)",
      :wrong_length        => "è della lunghezza sbagliata (dovrebbe essere di %d caratteri)",
      :taken               => "è già stato assegnato",
      :not_a_number        => "non è un numero",
      #Jespers additions:
      :error_translation   => "errore",
      :error_header        => "%s impedisce a %s di essere salvato",
      :error_subheader     => "Ci sono dei problemi con i seguenti campi:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "meno di %d secondi",
      :half_a_minute       => "mezzo minuto",
      :less_than_a_minute  => "meno di un minuto",
      :one_minute          => "un minuto",
      :x_minutes           => "%d minuti",
      :one_hour            => "circa un'ora",
      :x_hours             => "circa %d ore",
      :one_day             => "un giorno",
      :x_days              => "%d giorni",
      :one_month           => "un mese",
      :x_months            => "%d mesi",
      :one_year            => "un anno",
      :x_years             => "%d anni"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames = [nil] + %w{ Gennaio Febbraio Marzo Aprile Maggio Giugno Luglio Agosto Settembre Ottobre Novembre Dicembre }
    Daynames = %w{ Domenica Lunedì Martedì Mercoledì Giovedì Venerdì Sabato }
    AbbrMonthnames = [nil] + %w{ Gen Feb Mar Apr Mag Giu Lug Ago Set Ott Nov Dic }
    AbbrDaynames = %w{ Dom Lun Mar Mer Gio Ven Sab }

    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%d-%m-%Y",
      :short    => "%e %b",
      :long     => "%e %B, %Y"
    }

    TimeFormats = {
      :default  => "%a, %d %b %Y %H:%M:%S %z",
      :short    => "%d %b %H:%M",
      :long     => "%d %B, %Y %H:%M"
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
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'e',
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
