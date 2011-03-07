# lang_ru.rb
# Russian translation. Created by Denis Pankratov (denis@crime-research.org)



module LocalizationSimplified
  About = {
    :lang => "ru",
    :updated => "2007-12-25"
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
      :inclusion           => "не входит в список допустимых",
      :exclusion           => "недопустимое значение",
      :invalid             => "неверное значение",
      :confirmation        => "не совпадает с подтверждением",
      :accepted            => "необходимо принять",
      :empty               => "не может быть пустым",
      :blank               => "обязательный параметр",# alternate, formulation: "is required"
      :too_long            => "длиннее, чем нужно (максимум %d символов)",
      :too_short           => "короче, чем нужно (минимум %d символов)",
      :wrong_length        => "неверной длины (нужно %d символов)",
      :taken               => "уже используется",
      :not_a_number        => "не является числом",
      #Jespers additions:
      :error_translation   => "",
      :error_header        => "%s ошибок произошло при сохранении %s ",
      :error_subheader     => "Проблемы касаются следующих полей:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "меньше чем %d секунд",
      :half_a_minute       => "половина минуты",
      :less_than_a_minute  => "меньше минуты",
      :one_minute          => "1 минуты",
      :x_minutes           => "%d минут",
      :one_hour            => "около часа",
      :x_hours             => "около %d часов",
      :one_day             => "1 день",
      :x_days              => "%d дней",
      :one_month           => "1 месяц",
      :x_months            => "%d месяцев",
      :one_year            => "1 год",
      :x_years             => "%d лет"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{Январь Февраль Март Апрель Май Июнь Июль Август Сентябрь Октябрь Ноябрь Декабрь}
    AbbrMonthnames = [nil] + %w{Янв Фев Мар Апр Май Июн Июл Авг Сен Окт Ноя Дек}
    Daynames       = %w{Воскресенье Понедельник Вторник Среда Четверг Пятница Суббота}
    AbbrDaynames   = %w{Вск Пнд Втр Сре Чет Пят Суб}
    
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
      :unit      => "р.",
      :separator => ",",             #unit separator (between integer part and fraction part)
      :delimiter => " ",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:number,:unit] #order is at present unsupported in Rails
      #to support for instance Danish format, the order is different: Unit comes last (ex. "1.234,00 dkr.")
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => 'и',
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
