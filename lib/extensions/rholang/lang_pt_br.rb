# lang_pt_br.rb
# Brazilian Portuguese translation file. 
# Translation by Frederico de Souza Araujo (www.frederico-araujo.com)
# (based on lang_es.rb by Luis Villa del Campo)

module LocalizationSimplified
  About = {
    :lang => "pt_br",
    :updated => "2007-04-04"
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
      :inclusion           => "não está incluido na lista",
      :exclusion           => "ja esta reservado",
      :invalid             => "não é válido",
      :confirmation        => "não coincide com a confirmação",
      :accepted            => "deve ser aceitado",
      :empty               => "não pode estar vazio",
      :blank               => "não pode estar em branco",# alternate formulation: "is required"
      :too_long            => "é largo demais (o máximo é de %d carateres)",
      :too_short           => "é curto demais (o mínimo é de %d carateres)",
      :wrong_length        => "não possui o tamanho correto (deve ser de %d carateres)",
      :taken               => "já está ocupado",
      :not_a_number        => "não é um número",
      #Jespers additions:
      :error_translation   => "erro",
      :error_header        => "%s não permitiu de salvar %s",
      :error_subheader     => "Houve problemas com os siguientes campos:"
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
      :one_hour            => "uma hora",
      :x_hours             => "%d horas",
      :one_day             => "um dia",
      :x_days              => "%d dias",
      :one_month           => "1 mês",
      :x_months            => "%d meses",
      :one_year            => "1 ano",
      :x_years             => "%d anos"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{janeiro fevereiro março abril maio junho julho agosto setembro outubro novembro dezembro}
    AbbrMonthnames = [nil] + %w{jan fev mar abr mai jun jul ago set out nov dez}
    Daynames       = %w{domingo segunda terça quarta quinta sexta sábado}
    AbbrDaynames   = %w{dom seg ter qua qui sex sáb}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%d-%m-%Y",
      :short    => "%b %e",
      :long     => "%e %B, %Y"
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
      :unit      => "R$",
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
 Inflector.inflections do |inflect|
   #   inflect.plural /^(ox)$/i, '\1en'
   #   inflect.singular /^(ox)en/i, '\1'
   #   inflect.irregular 'person people'
   #   inflect.uncountable %w( information )
   inflect.plural /^(error ocorreu)$/i, 'erros ocorreram'
   inflect.plural /^(error)$/i, 'erros'
 end
