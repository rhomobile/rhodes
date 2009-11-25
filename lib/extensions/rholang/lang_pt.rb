# lang_pt.rb
# Tradução das mensagens de erro, A completar ou corrigir.
# Translation by Carlos Afonso ( aindaapensar.blogspot.com )


module LocalizationSimplified
  About = {
    :lang => "pt",
    :updated => "2007-08-19"
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
      :inclusion           => "não está incluído na lista",
      :exclusion           => "é reservado",
      :invalid             => "é inválido",
      :confirmation        => "não corresponde à confirmação",
      :accepted            => "deve ser aceite",
      :empty               => "não pode estar vazio",
      :blank               => "não pode estar em branco",# alternate, formulation: "is required"
      :too_long            => "é demasiado longo (%d caracteres no máximo)",
      :too_short           => "é demasiado curto(%d caracteres no mínimo)",
      :wrong_length        => "não tem o comprimento correcto (deveria ter %d caracteres)",
      :taken               => "já existente",
      :not_a_number        => "não é um número",
      #Jespers additions:
      :error_translation   => "erro",
      :error_header        => "%s proíbe %s de ser guardado",
      :error_subheader     => "Há problemas nos seguintes campos:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "menos de %d segundos",
      :half_a_minute       => "meio minuto",
      :less_than_a_minute  => "menos de um minuto",
      :one_minute          => "1 minuto",
      :x_minutes           => "%d minutos",
      :one_hour            => "perto de 1 hora",
      :x_hours             => "perto de %d horas",
      :one_day             => "1 dia",
      :x_days              => "%d dias",
      :one_month           => "1 mês",
      :x_months            => "%d meses",
      :one_year            => "1 ano",
      :x_years             => "%d anos"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Janeiro Fevereiro Março Abril Maio Junho Julho Agosto Setembro Outubro Novembro Dezembro}
    AbbrMonthnames = [nil] + %w{Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez}
    Daynames       = %w{Dimanche Lundi Mardi Mercredi Jeudi Vendredi Samedi}
    AbbrDaynames   = %w{Dom Seg Ter Qua Qui Sex Sab}
    
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
      :unit      => "€",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'e',
      :skip_last_comma => true
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
