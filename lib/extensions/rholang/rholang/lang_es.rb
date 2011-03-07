# lang_es.rb
# Spanish translation file. 
# Translation by Luis Villa del Campo (www.grancomo.com)


module LocalizationSimplified
  About = {
    :lang => "es",
    :updated => "2006-10-03"
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
      :inclusion           => "no está incluido en la lista",
      :exclusion           => "está reservado",
      :invalid             => "no es válido",
      :confirmation        => "no coincide con la confirmación",
      :accepted            => "debe ser aceptado",
      :empty               => "no puede estar vacío",
      :blank               => "no puede estar en blanco",# alternate formulation: "is required"
      :too_long            => "es demasiado largo (el máximo es %d caracteres)",
      :too_short           => "es demasiado corto (el mínimo es %d caracteres)",
      :wrong_length        => "no posee el largo correcto (debería ser de %d caracteres)",
      :taken               => "ya está ocupado",
      :not_a_number        => "no es un número",
      #Jespers additions:
      :error_translation   => "error",
      :error_header        => "%s no permite guardar %s",
      :error_subheader     => "Ha habido problemas con los siguientes campos:"
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
      :one_hour            => "sobre una hora",
      :x_hours             => "sobre %d horas",
      :one_day             => "un día",
      :x_days              => "%d días",
      :one_month           => "1 mes",
      :x_months            => "%d meses",
      :one_year            => "1 año",
      :x_years             => "%d años"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{enero febrero marzo abril mayo junio julio agosto septiembre octubre noviembre diciembre}
    AbbrMonthnames = [nil] + %w{ene feb mar abr may jun jul ago sep oct nov dic}
    Daynames       = %w{domingo lunes martes miércoles jueves viernes sábado}
    AbbrDaynames   = %w{dom lun mar mié jue vie sáb}
    
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
      :connector => 'y',
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
 Inflector.inflections do |inflect|
   inflect.plural /^(error)$/i, '\1es'
 end
