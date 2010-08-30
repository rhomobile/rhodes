# lang_en.rb
# English baseline translation file. Comes in handy for testing purposes
# translated by : Raul Mantilla Assia



module LocalizationSimplified
  About = {
    :lang => "es",
    :updated => "2010-08-30"
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
      :inclusion           => "no esta incluido en la lista",
      :exclusion           => "es reservada",
      :invalid             => "es invalida",
      :confirmation        => "no coincide con la confirmación",
      :accepted            => "debe ser aceptada",
      :empty               => "no puede estar vacío",
      :blank               => "no puede estar en blanco",# alternate formulation: "is required"
      :too_long            => "es demasiado largo (el máximo es de %d caracteres)",
      :too_short           => "es demasiado corto (el mínimo es de %d caracteres)",
      :wrong_length        => "es de longitud incorrecta (debe ser de %d caracteres)",
      :taken               => "ya ha sido tomada",
      :not_a_number        => "no es un número",
      #Jespers additions:
      :error_translation   => "error",
      :error_header        => "%s prohibido %s para ser guardado",
      :error_subheader     => "Hubo problemas con los siguientes campos:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "menos de %d segundos",
      :half_a_minute       => "medio minuto",
      :less_than_a_minute  => "menos de un minuto",
      :one_minute          => "1 minuto",
      :x_minutes           => "%d minutos",
      :one_hour            => "alrededor de 1 hora",
      :x_hours             => "alrededor de %d horas",
      :one_day             => "1 día",
      :x_days              => "%d días",
      :one_month           => "1 mes",
      :x_months            => "%d meses",
      :one_year            => "1 año",
      :x_years             => "%d años"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Enero Febrero Marzo Abril Mayo Junio Julio Agosto Septiembre Octubre Noviembre Diciembre}
    AbbrMonthnames = [nil] + %w{Ene Feb Mar Abr May Jun Jul Ago Sep Oct Nov Dic}
    Daynames       = %w{Domingo Lunes Martes Miércoles Jueves Viernes Sábado}
    AbbrDaynames   = %w{Dom Lun Mar Mié Jue Vie Sab}
    
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
      :connector => 'y',
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
