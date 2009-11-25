# lang_ko.rb
# Translation by Jeong Mok, Cho ( http://niceview.egloos.com )


module LocalizationSimplified
  About = {
    :lang => "ko",
    :updated => "2006-09-29"
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
      :inclusion           => "값은 목록에 없습니다.",
      :exclusion           => "값은 이미 사용중입니다.",
      :invalid             => "값이 잘못되었습니다.",
      :confirmation        => "가 일치하지 않습니다.",
      :accepted            => "항목이 채크되어야 합니다.",
      :empty               => "값은 꼭 입력하셔야 합니다.",
      :blank               => "값은 꼭 입력하셔야 합니다.",# alternate, formulation: "is required"
      :too_long            => "값이 너무 깁니다. (최대 %d자 이내)",
      :too_short           => "값이 너무 짧습니다. (최소 %d자 이상)",
      :wrong_length        => "값은 길이가 잘못되었습니다. (%d자로 입력하세요)",
      :taken               => "값은 이미 사용중입니다.",
      :not_a_number        => "값은 숫자가 아닙니다.",
      #Jespers additions:
      :error_translation   => "개의 애러",
      :error_header        => "%s가 발생하였습니다. 이 %s는(은) 저장되지 않았습니다.",
      :error_subheader     => "다음의 항목에 대한 입력값들이 잘못되었습니다."
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "%d초 이내",
      :half_a_minute       => "30초",
      :less_than_a_minute  => "1분 이내",
      :one_minute          => "1분",
      :x_minutes           => "%d분",
      :one_hour            => "약 1시간",
      :x_hours             => "약 %d시간",
      :one_day             => "1일",
      :x_days              => "%d일",
      :one_month           => "1개월",
      :x_months            => "%d개월",
      :one_year            => "1년",
      :x_years             => "%d년"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{1월 2월 3월 4월 5월 6월 7월 8월 9월 10월 11월 12월}
    AbbrMonthnames = [nil] + %w{1 2 3 4 5 6 7 8 9 10 11 12}
    Daynames       = %w{일요일 월요일 화요일 수요일 목요일 금요일 토요일}
    AbbrDaynames   = %w{일 월 화 수 목 금 토}
    
    # Date and time format syntax explained in http://www.rubycentral.com/ref/ref_c_time.html#strftime
    # These are sent to strftime that Ruby's date and time handlers use internally
    # Same options as php (that has a better list: http://www.php.net/strftime )
    DateFormats = {
      :default  => "%Y/%m/%d",
      :short    => "%m/%d",
      :long     => "%Y년 %b월 %e일 %A"
    }

    TimeFormats = {
      #:default  => "%a, %d %b %Y %H:%M:%S %z",
      :default  => "%Y/%m/%d (%a) %p %H:%M:%S",
      :short    => "%H:%M",
      :long     => "%Y년 %b월 %e일 %A %p %I시 %M분 %S초"
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
      :unit      => "￦",
      :separator => ".",             #unit separator (between integer part and fraction part)
      :delimiter => ",",             #delimiter between each group of thousands. Example: 1.234.567 
      :order     => [:unit, :number] #order is at present unsupported in Rails
    }
  end

  class ArrayHelper
    # Modifies +Array#to_sentence()+
    # http://api.rubyonrails.org/classes/ActiveSupport/CoreExtensions/Array/Conversions.html#M000274
    ToSentenceTexts = {
      :connector => '그리고',
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
  inflect.uncountable '개의 애러'
end

