# lang_ca.rb
# Catalan translation file. 
# Translation by Andrés Cirugeda andres.cirugeda(at)gmail.com


module LocalizationSimplified
  About = {
    :lang => "ca",
    :updated => "2007-02-01"
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
      :inclusion           => "no està inclós a la llista",
      :exclusion           => "està reservat",
      :invalid             => "no és vàlid",
      :confirmation        => "no coincideix amb la confirmació",
      :accepted            => "ha de ser acceptat",
      :empty               => "no pot ser buit",
      :blank               => "no pot estar en blanc",# alternate formulation: "is required"
      :too_long            => "és massa llarg (el màxim és %d caracters)",
      :too_short           => "és massa curt (el mínim és %d caracters)",
      :wrong_length        => "no té la llàrgaria correcta (hauria de ser de %d caracters)",
      :taken               => "ja està ocupat",
      :not_a_number        => "no és un nombre",
      #Jespers additions:
      :error_translation   => "error",
      :error_header        => "%s no permet guardar %s",
      :error_subheader     => "Hi ha hagut problemes amb els següents camp:"
    }
  end

  # Texts to override +distance_of_time_in_words()+
  class DateHelper
    Texts = {
      :less_than_x_seconds => "menys de %d segons",
      :half_a_minute       => "mig minut",
      :less_than_a_minute  => "menys d'un minut",
      :one_minute          => "1 minut",
      :x_minutes           => "%d minuts",
      :one_hour            => "al voltant d'una hora",
      :x_hours             => "al voltant de %d hores",
      :one_day             => "un dia",
      :x_days              => "%d dies",
      :one_month           => "1 mes",
      :x_months            => "%d mesos",
      :one_year            => "1 any",
      :x_years             => "%d anys"
    }

    # Rails uses Month names in Date and time select boxes 
    # (+date_select+ and +datetime_select+ )
    # Currently (as of version 1.1.6), Rails doesn't use daynames
    Monthnames     = [nil] + %w{gener febrer març abril maig juny juliol agost setembre octubre novembre desembre}
    AbbrMonthnames = [nil] + %w{gen feb mar abr mai jun jul ago set oct nov des}
    Daynames       = %w{diumenge dilluns dimarts dimecres dijous divendres dissabte}
    AbbrDaynames   = %w{dmg dll dmt dmc djs dvn dsb}
    
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
      :connector => 'i',
      :skip_last_comma => true
    }
  end
end


# Use the inflector below to pluralize "error" from
# @@default_error_messages[:error_translation] above (if necessary)
# Inflector.inflections do |inflect|
#   inflect.plural /^(error)$/i, '\1s'
# end
